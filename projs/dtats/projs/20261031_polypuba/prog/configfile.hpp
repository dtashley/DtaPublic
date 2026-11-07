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
#include <vector>

#include "lg_cpp_dd_buf_uint8.hpp"

#define MODULE_CONFIGFILE_CFGFILE_DEPTH_MAX (5)
  //!< The maximum depth of the configuration file nesting.  Each dictionary
  //!< or array is a new level.

enum class ConfigFileObjType
{
   Undefined,
      //!< Unused or undefined.  This would normally occur when for unused levels
      //!< of an array holding leaf nodes.
   Key,
      //!< Key only with no value.
   KeyValPairString,
      //!< Key-value pair, with the value represented as a string.  This means
      //!< either that the value was double-quoted, or that the value would not
      //!< parse as an integer.  In this case, the string field is populated
      //!< but the integer is canonically zero.
   KeyValPairInt,
      //!< Key-value pair, with the value represented as an integer.  This means
      //!< the integer was not double-quoted, and parsed successfully as a signed
      //!< integer in the range of -2^31 to 2^31-1, inclusive.  In this case,
      //!< the string field is the textual representation of the integer and the
      //!< integer field is the value of the integer.
   Dictionary,
      //!< A dictionary.
   ArrayDict
      //!< An array of dictionaries.  An array can only contain dictionaries.
};


//!< The fundamental record representing a nesting level in a leaf node.
struct ConfigFileNlRecord
{
   enum class ConfigFileObjType m_type;
      //!< The type of element.
   std::string key;
      //!< The key value.  This will always exist.
   std::string value_string;
      //!< The value.  If this is not populated, it will be the empty string.
   int32_t value_int32;
      //!< The signed 32-bit integer, if the value parses as an integer.
};
typedef struct ConfigFileNlRecord tConfigFileNlRecord;

//!< The fundamental record representing a leaf node from the configuration file.
struct ConfigFileObjRecord
{
    size_t leaf_node_depth;
    tConfigFileNlRecord paths[MODULE_CONFIGFILE_CFGFILE_DEPTH_MAX];
};
typedef struct ConfigFileObjRecord tConfigFileObjRecord;


//! \class ConfigFile
//! The class holding the results of parsing a configuration file.
class ConfigFile
{
#ifndef UT_GTEST
private:
#else
public:
#endif
    int x;
    std::vector<tConfigFileObjRecord> m_recs;
public:
   ConfigFile() noexcept;
   ~ConfigFile() noexcept;
   void MarkAll(void) noexcept;
   void UnmarkAll(void) noexcept;
   void MarkOne(size_t in_el_to_mark) noexcept;
   void UnmarkOne(size_t in_el_to_mark) noexcept;
   void MarkRange(size_t in_el_0, size_t in_el_n_minus_one) noexcept;
   void UnmarkRange(size_t in_el_0, size_t in_el_n_minus_one) noexcept;
   bool IsMarked(size_t in_el) const noexcept;
   bool Parse(const LgBufUint8 &in_fc) noexcept;
   size_t Nelem(void) const noexcept;
   void Nelem(size_t in_el, tConfigFileObjRecord &out_rec) const noexcept;
};
#endif
