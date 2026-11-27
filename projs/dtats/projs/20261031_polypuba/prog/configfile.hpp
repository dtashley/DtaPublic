//--------------------------------------------------------------------------------------------------
//Header file for Polypuba configuration file parsing.
//--------------------------------------------------------------------------------------------------
//This file is part of LibGen, https://github.com/dtashley/DtaPublic, and is provided
//under The Unlicense, reproduced below.
//--------------------------------------------------------------------------------------------------
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non-commercial, and by any
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

#include "lg_cpp_dd_parse_lex_cust_a.hpp"
#include "lg_cpp_dd_rpt_errlst_cd_a.hpp"

#define MODULE_CONFIGFILE_CFGFILE_DEPTH_MAX (5)
  //!< The maximum depth of the configuration file nesting.  Each dictionary
  //!< or array is a new level.


//! As the configuration file tokens are parsed, each recorded value is associated with
//! enclosing named dictionaries, named arrays, and keys.  This is the enclosure type
//! for a value.
enum class ConfigFileNestLevelEnclosureType : unsigned
{
   Undefined,
      //!< Unused or undefined.  This is the binary zero initialization value.
   KeyName,
      //!< The enclosing item is a key, which has a name.
   DictionaryName,
      //!< The enclosing item is a named dictionary.
   ArrayName,
      //!< The enclosing item is a named array.
   ArraySubscript,
      //!< A dictionary.
   EnumLimit
      //!< Not a valid value.  Defines the upper limit of the enumeration.
};

//!< The fundamental record representing a nesting level.
struct ConfigFileNestingLevelRecord
{
   ConfigFileNestLevelEnclosureType enclosure_type{};
      //!< The type of element.
   std::string enclosure_name;
      //!< The name of the enclosing key name, dictionary name, or array name.  If the enclosure type is
      //! ArraySubscript, this will be the empty string.
   size_t enclosure_array_subscript{};
      //! If the enclosure type is ArraySubscript, this will be the array subscript, starting with [0].
      //! For other enclosure types, this value will be 0.
};
typedef struct ConfigFileNestingLevelRecord tConfigFileNestingLevelRecord;

//!< The fundamental record representing a value.
struct ConfigFileValueRecord
{
   tLgCppDdParseLexCustAStruct parse_token_record{};
      //!< The parse token.
};
typedef struct ConfigFileValueRecord tConfigFileValueRecord;

//!< The fundamental record representing a leaf node from the configuration file.
struct ConfigFileObjRecord
{
    bool marked{};
       //!< A boolean indicating whether the record is marked or not,
       //!< true indicates marked.
    size_t n_nesting_level_records{};
      //!< The number of nesting level records that are used.
    tConfigFileNestingLevelRecord paths[MODULE_CONFIGFILE_CFGFILE_DEPTH_MAX]{};
      //!< An array specifying the components to the leaf node.
    tConfigFileValueRecord value{};
      //!< The value.
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
    std::vector<tConfigFileObjRecord> m_leaf_node_recs;
       //!< The array of records, one per leaf node.
public:
   ConfigFile() noexcept;
   ~ConfigFile() noexcept;
   void MarkAll(void) noexcept;
   void UnmarkAll(void) noexcept;
   void MarkOne(size_t in_el_to_mark) noexcept;
   void UnmarkOne(size_t in_el_to_mark) noexcept;
   void MarkRange(size_t in_el_0, size_t in_el_n_plus_one) noexcept;
   void UnmarkRange(size_t in_el_0, size_t in_el_n_plus_one) noexcept;
   bool IsMarked(size_t in_el) const noexcept;
   bool Parse(const LgBufUint8& in_buf, bool* out_success, LgCppDdErrlistCdA& out_lex_warnings_and_errors, LgCppDdErrlistCdA& out_parse_warnings_and_errors);
   size_t Nelem(void) const noexcept;
   size_t Nmarked(void) const noexcept;
   void GetRecByIdx(size_t in_el, tConfigFileObjRecord &out_rec) const noexcept;
};
#endif
