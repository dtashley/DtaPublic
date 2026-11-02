#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>

class Sha1
{
public:
    Sha1() noexcept;

    void Update(std::span<const std::uint8_t> data);
    void Update(std::string_view data);

    [[nodiscard]] std::string Finalize();

    [[nodiscard]] static std::string Hash(std::span<const std::uint8_t> data);
    [[nodiscard]] static std::string Hash(std::string_view data);

private:
    void ProcessBlock(const std::uint8_t* block) noexcept;

    std::uint32_t state_[5];
    std::uint64_t bitCount_;
    std::uint8_t buffer_[64];
    std::size_t bufferSize_;
    bool finalized_;
};
