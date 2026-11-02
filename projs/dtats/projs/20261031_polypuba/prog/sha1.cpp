#include "sha1.h"

#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace
{
constexpr std::uint32_t RotateLeft(std::uint32_t value, unsigned int bits) noexcept
{
    return (value << bits) | (value >> (32U - bits));
}

std::string ToHex(const std::uint32_t* state)
{
    constexpr char digits[] = "0123456789abcdef";
    std::string result;
    result.reserve(40);

    for (int i = 0; i < 5; ++i)
    {
        for (int shift = 28; shift >= 0; shift -= 4)
        {
            result.push_back(digits[(state[i] >> shift) & 0x0fU]);
        }
    }

    return result;
}
}

Sha1::Sha1() noexcept
    : state_{0x67452301U, 0xefcdab89U, 0x98badcfeU, 0x10325476U, 0xc3d2e1f0U},
      bitCount_(0),
      buffer_{},
      bufferSize_(0),
      finalized_(false)
{
}

void Sha1::Update(std::span<const std::uint8_t> data)
{
    if (finalized_)
    {
        throw std::logic_error("Cannot update a finalized SHA-1 hash.");
    }

    bitCount_ += static_cast<std::uint64_t>(data.size()) * 8U;

    while (!data.empty())
    {
        const std::size_t bytesToCopy = (std::min)(data.size(), sizeof(buffer_) - bufferSize_);
        std::memcpy(buffer_ + bufferSize_, data.data(), bytesToCopy);
        bufferSize_ += bytesToCopy;
        data = data.subspan(bytesToCopy);

        if (bufferSize_ == sizeof(buffer_))
        {
            ProcessBlock(buffer_);
            bufferSize_ = 0;
        }
    }
}

void Sha1::Update(std::string_view data)
{
    Update(std::span<const std::uint8_t>(
        reinterpret_cast<const std::uint8_t*>(data.data()), data.size()));
}

std::string Sha1::Finalize()
{
    if (finalized_)
    {
        throw std::logic_error("SHA-1 hash has already been finalized.");
    }

    const std::uint64_t originalBitCount = bitCount_;
    buffer_[bufferSize_++] = 0x80U;

    if (bufferSize_ > 56)
    {
        std::memset(buffer_ + bufferSize_, 0, sizeof(buffer_) - bufferSize_);
        ProcessBlock(buffer_);
        bufferSize_ = 0;
    }

    std::memset(buffer_ + bufferSize_, 0, 56 - bufferSize_);
    bufferSize_ = 56;

    for (int i = 0; i < 8; ++i)
    {
        buffer_[bufferSize_ + i] = static_cast<std::uint8_t>(originalBitCount >> (56 - i * 8));
    }
    bufferSize_ = sizeof(buffer_);
    ProcessBlock(buffer_);
    bufferSize_ = 0;
    finalized_ = true;

    return ToHex(state_);
}

std::string Sha1::Hash(std::span<const std::uint8_t> data)
{
    Sha1 hash;
    hash.Update(data);
    return hash.Finalize();
}

std::string Sha1::Hash(std::string_view data)
{
    Sha1 hash;
    hash.Update(data);
    return hash.Finalize();
}

void Sha1::ProcessBlock(const std::uint8_t* block) noexcept
{
    std::uint32_t words[80]{};

    for (int i = 0; i < 16; ++i)
    {
        words[i] = (static_cast<std::uint32_t>(block[i * 4]) << 24) |
                   (static_cast<std::uint32_t>(block[i * 4 + 1]) << 16) |
                   (static_cast<std::uint32_t>(block[i * 4 + 2]) << 8) |
                   static_cast<std::uint32_t>(block[i * 4 + 3]);
    }

    for (int i = 16; i < 80; ++i)
    {
        words[i] = RotateLeft(words[i - 3] ^ words[i - 8] ^ words[i - 14] ^ words[i - 16], 1);
    }

    std::uint32_t a = state_[0];
    std::uint32_t b = state_[1];
    std::uint32_t c = state_[2];
    std::uint32_t d = state_[3];
    std::uint32_t e = state_[4];

    for (int i = 0; i < 80; ++i)
    {
        std::uint32_t function;
        std::uint32_t constant;

        if (i < 20)
        {
            function = (b & c) | ((~b) & d);
            constant = 0x5a827999U;
        }
        else if (i < 40)
        {
            function = b ^ c ^ d;
            constant = 0x6ed9eba1U;
        }
        else if (i < 60)
        {
            function = (b & c) | (b & d) | (c & d);
            constant = 0x8f1bbcdcU;
        }
        else
        {
            function = b ^ c ^ d;
            constant = 0xca62c1d6U;
        }

        const std::uint32_t temporary = RotateLeft(a, 5) + function + e + constant + words[i];
        e = d;
        d = c;
        c = RotateLeft(b, 30);
        b = a;
        a = temporary;
    }

    state_[0] += a;
    state_[1] += b;
    state_[2] += c;
    state_[3] += d;
    state_[4] += e;
}
