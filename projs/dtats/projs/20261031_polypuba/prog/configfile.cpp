//--------------------------------------------------------------------------------------------------
//Polypuba configuration file parser
// 
//Parses the configuration file, stores the content, and makes it available
//to the rest of the program.
//--------------------------------------------------------------------------------------------------
//This file is part of LibGen, https://github.com/dtashley/DtaPublic, and is provided
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
ConfigFile::ConfigFile() noexcept
{
}


/*!
 * \brief                       Destructor.  Destroys an object.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
ConfigFile::~ConfigFile() noexcept
{
}


/*!
 * \brief                       Marks all of the elements in the array of leaf nodes.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
void ConfigFile::MarkAll(void) noexcept
{
   for (auto& element : m_leaf_node_recs)
   {
      element.marked = true;
   }
}


/*!
 * \brief                       Unmarks all of the elements in the array of leaf nodes.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
void ConfigFile::UnmarkAll(void) noexcept
{
   for (auto& element : m_leaf_node_recs)
   {
      element.marked = false;
   }
}


/*!
 * \brief                       Marks a single specific element of the array of leaf nodes.
 *
 * \param[in]  in_el_to_mark    Index (starting from [0]) of element to mark.  Indices that
 *                              are too large will result in no action by this function.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
void ConfigFile::MarkOne(size_t in_el_to_mark) noexcept
{
   if (in_el_to_mark < m_leaf_node_recs.size())
   {
      m_leaf_node_recs.at(in_el_to_mark).marked = true;
   }
   else
   {
      //Ignore attempt if index out of range.
   }
}


/*!
 * \brief                       Unmarks a single specific element of the array of leaf nodes.
 *
 * \param[in]  in_el_to_unmark  Index (starting from [0]) of element to unmark.  Indices that
 *                              are too large will result in no action by this function.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
void ConfigFile::UnmarkOne(size_t in_el_to_unmark) noexcept
{
   if (in_el_to_unmark < m_leaf_node_recs.size())
   {
      m_leaf_node_recs.at(in_el_to_unmark).marked = false;
   }
   else
   {
      //Ignore attempt if index out of range.
   }
}


/*!
 * \brief                         Marks a contingous range of the array of leaf nodes.
 *
 * \param[in]  in_el_0            Index (starting from [0]) of first element to mark.  Indices
 *                                that are too large will result in no action by this function.
 * \param[in]  in_el_n_plus_one   Index (starting from [0]) of the element one beyond the last
 *                                element to mark.  Indices that are too large will result
 *                                in no action by this function.
 *
 * \reentrancyandthreadsafety     Not evaluated.
 *
 * \errorsandexceptions           No errors or exceptions are possible.
 */
void ConfigFile::MarkRange(size_t in_el_0, size_t in_el_n_plus_one) noexcept
{
   size_t nelem;
   size_t i;

   nelem = m_leaf_node_recs.size();

   if (in_el_0 < nelem)  //Can't proceed if starting element is too large.
   {
      //Trim down the end of the range if it too large.  There is no point
      //in iterating when the elements don't exist.
      if (in_el_n_plus_one > nelem)
        in_el_n_plus_one = nelem;

      //Mark the range.
      for (i = in_el_0; i < in_el_n_plus_one; i++)
      {
         m_leaf_node_recs.at(i).marked = true;
      }
   }
}


/*!
 * \brief                         Unmarks a contingous range of the array of leaf nodes.
 *
 * \param[in]  in_el_0            Index (starting from [0]) of first element to unmark.  Indices
 *                                that are too large will result in no action by this function.
 * \param[in]  in_el_n_plus_one   Index (starting from [0]) of the element one beyond the last
 *                                element to mark.  Indices that are too large will result
 *                                in no action by this function.
 *
 * \reentrancyandthreadsafety     Not evaluated.
 *
 * \errorsandexceptions           No errors or exceptions are possible.
 */
void ConfigFile::UnmarkRange(size_t in_el_0, size_t in_el_n_plus_one) noexcept
{
    size_t nelem;
    size_t i;

    nelem = m_leaf_node_recs.size();

    if (in_el_0 < nelem)  //Can't proceed if starting element is too large.
    {
        //Trim down the end of the range if it too large.  There is no point
        //in iterating when the elements don't exist.
        if (in_el_n_plus_one > nelem)
            in_el_n_plus_one = nelem;

        //Mark the range.
        for (i = in_el_0; i < in_el_n_plus_one; i++)
        {
            m_leaf_node_recs.at(i).marked = false;
        }
    }
}


/*!
 * \brief                         Determines whether a specified element of the array of leaf nodes
 *                                is marked.
 *
 * \param[in]  in_el              Index (starting from [0]) of the element to test.  Indices
 *                                that are too large will result in no action by this function.
 * \returns                       true if the element is marked, or false otherwise.
 *
 * \reentrancyandthreadsafety     Not evaluated.
 *
 * \errorsandexceptions           No errors or exceptions are possible.
 */
bool ConfigFile::IsMarked(size_t in_el) const noexcept
{
    bool rv;

    if (in_el < m_leaf_node_recs.size())
    {
        rv = m_leaf_node_recs.at(in_el).marked;
    }
    else
    {
        rv = false;
    }

    return rv;
}

bool ConfigFile::Parse(const LgBufUint8& in_fc) noexcept
{
   return true;
}


/*!
 * \brief                       Returns the number of elements in the list of leaf nodes.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
size_t ConfigFile::Nelem(void) const noexcept
{
   size_t rv;

   rv = m_leaf_node_recs.size();

   return rv;
}


/*!
 * \brief                       Returns the number of elements in the list of leaf nodes
 *                              that are marked.  The number of unmarked elements can be
 *                              inferred from the total number minus the number marked.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
size_t ConfigFile::Nmarked(void) const noexcept
{
    size_t rv = 0;

    for (const auto& element : m_leaf_node_recs)
    {
       if (element.marked)
          rv++;
    }

    return rv;
}


/*!
 * \brief                         Provides the caller a copy of the specified element from the list
 *                                of leaf nodes.
 *
 * \param[in]  in_el              Index (starting from [0]) of the element to provide a copy of.  Indices
 *                                that are too large will result in no action by this function.
 * \param[out] out_rec            A copy provided to the caller.  If in_el is invalid, all fields of the
 *                                copy target will be set to 0, and strings will be set to the empty string.
 * 
 *
 * \reentrancyandthreadsafety     Not evaluated.
 *
 * \errorsandexceptions           No errors or exceptions are possible.
 */
void ConfigFile::GetRecByIdx(size_t in_el, tConfigFileObjRecord& out_rec) const noexcept
{
    size_t i;

    if (in_el < m_leaf_node_recs.size())
   {
      //Grab a reference.
      const auto& element = m_leaf_node_recs.at(in_el);

      //Copy to the caller's area.
      out_rec.marked          = element.marked;
      out_rec.leaf_node_depth = element.leaf_node_depth;

      //Assigning manually because unsure of C++ behavior, either for arrays or for
      //copies of objects within records.
      for (i = 0; i < MODULE_CONFIGFILE_CFGFILE_DEPTH_MAX; i++)
      {
         out_rec.paths[i].m_type       = element.paths[i].m_type;
         out_rec.paths[i].key          = element.paths[i].key;
         out_rec.paths[i].value_string = element.paths[i].value_string;
         out_rec.paths[i].value_int32  = element.paths[i].value_int32;
      }
   }
   else
   {
      //Zero out the caller's area.
      out_rec.marked          = false;
      out_rec.leaf_node_depth = 0;

      for (i = 0; i < MODULE_CONFIGFILE_CFGFILE_DEPTH_MAX; i++)
      {
         out_rec.paths[i].m_type       = ConfigFileObjType::Undefined;
         out_rec.paths[i].key          = "";
         out_rec.paths[i].value_string = "";
         out_rec.paths[i].value_int32  = 0;
      }
   }
}
