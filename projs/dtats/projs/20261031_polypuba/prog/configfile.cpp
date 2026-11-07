//--------------------------------------------------------------------------------------------------
//Polypuba configuration file parser
// 
//Parses the configuration file, stores the content, and makes it available
//to the rest of the program.
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
//software to the public domain.We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors.We intend this dedication to be an overt act of
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
#define MODULE_CONFIGFILE_CPP

#include "configfile.hpp"

//For simplicity, the configuration file is parsed into a one-dimensional array of
//records.  It is single-purpose, and inelegant.  There is a maximum depth to
//the scheme, set at compile time.  Essentially, each record is a leaf node,
//and includes information about all of the parents.  The top-level parent
//is a dictionary.

/*!
 * \brief                       Default constructor.  Constructs a new object.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
ConfigFile::ConfigFile() noexcept : x{0}
{}


/*!
 * \brief                       Destructor.  Destroys an object.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
ConfigFile::~ConfigFile() noexcept
{
   x = 0;
}

void ConfigFile::MarkAll(void) noexcept
{}

void ConfigFile::UnmarkAll(void) noexcept
{}

void ConfigFile::MarkOne(size_t in_el_to_mark) noexcept
{}

void ConfigFile::UnmarkOne(size_t in_el_to_mark) noexcept
{}

void ConfigFile::MarkRange(size_t in_el_0, size_t in_el_n_minus_one) noexcept
{}

void ConfigFile::UnmarkRange(size_t in_el_0, size_t in_el_n_minus_one) noexcept
{}

bool ConfigFile::IsMarked(size_t in_el) const noexcept
{
   return true;
}

bool ConfigFile::Parse(const LgBufUint8& in_fc) noexcept
{
   return true;
}

size_t ConfigFile::Nelem(void) const noexcept
{
    return 0;
}

void ConfigFile::Nelem(size_t in_el, tConfigFileObjRecord& out_rec) const noexcept
{}
