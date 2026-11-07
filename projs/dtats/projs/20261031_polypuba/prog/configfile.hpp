//--------------------------------------------------------------------------------------------------
//Header file for Polypuba configuration file parsing.
//--------------------------------------------------------------------------------------------------
//This file is part of LibGen, https://github.com/dtashley/LibGen, and is provided
//under The Unlicense, reproduced below.
//--------------------------------------------------------------------------------------------------
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non - commercial, and by any
//means.
//
//In jurisdictions that recognize copyright laws, the author or authors
//of this software dedicate any and all copyright interest in the
//software to the public domain. We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors. We intend this dedication to be an overt act of
//relinquishment in perpetuity of all present and future rights to this
//software under copyright law.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.
//
//For more information, please refer to <https://unlicense.org>
//--------------------------------------------------------------------------------------------------
#ifndef CONFIGFILE_HPP_INCLUDED
#define CONFIGFILE_HPP_INCLUDED

#include <cstdint>
#include <string>

enum class ConfigFileObjType
{
   Invalid,
   Key,
   KeyValPair,
   Dictionary,
   Array
};

struct ConfigFileObjRecord
{
   enum class ConfigFileObjType m_type;
   char *key;
   char *value_string;
   int32_t value_int32;
   size_t numChildren;
   struct ConfigFileObjRecord *children;
};
typedef struct ConfigFileObjRecord tConfigFileObjRecord;

//!< \class ConfigFile
//!< Facilitate the calculation of the SHA1 hash, as described in NIST FIPS 180-1.
//!< The general paradigm is that the hash state is initialized (often by construction),
//!< data is added to the hash calculation (always in a byte buffer, all at once or
//!< incrementally), the hash is finalized, and then the hash value can be extracted.
class ConfigFile
{
#ifndef UT_GTEST
private:
#else
public:
#endif
   tConfigFileObjRecord *m_root;
   //!< The number of bytes presently in the buffer.  These are being held in order to reach
   //!< finalization or enough bytes added to fill the buffer.
public:
   ConfigFile() noexcept;
   ~ConfigFile() noexcept;
   void MarkAll(void) noexcept;
   void ClearAll(void) noexcept;
};
#endif
