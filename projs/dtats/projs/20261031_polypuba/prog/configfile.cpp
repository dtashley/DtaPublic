//--------------------------------------------------------------------------------------------------
//Polypuba configuration file parser.
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
//binary, for any purpose, commercial or non-commercial, and by any
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

#include <iostream>

#include "configfile.hpp"

#include "lg_cpp_dd_parse_rdp_buf_uint8.hpp"
#include "lg_cpp_dd_parse_lex_cust_a.hpp"


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
   for (auto& element : m_value_recs)
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
   for (auto& element : m_value_recs)
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
   if (in_el_to_mark < m_value_recs.size())
   {
      m_value_recs.at(in_el_to_mark).marked = true;
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
   if (in_el_to_unmark < m_value_recs.size())
   {
      m_value_recs.at(in_el_to_unmark).marked = false;
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

   nelem = m_value_recs.size();

   if (in_el_0 < nelem)  //Can't proceed if starting element is too large.
   {
      //Trim down the end of the range if it too large.  There is no point
      //in iterating when the elements don't exist.
      if (in_el_n_plus_one > nelem)
        in_el_n_plus_one = nelem;

      //Mark the range.
      for (i = in_el_0; i < in_el_n_plus_one; i++)
      {
         m_value_recs.at(i).marked = true;
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

    nelem = m_value_recs.size();

    if (in_el_0 < nelem)  //Can't proceed if starting element is too large.
    {
        //Trim down the end of the range if it too large.  There is no point
        //in iterating when the elements don't exist.
        if (in_el_n_plus_one > nelem)
            in_el_n_plus_one = nelem;

        //Mark the range.
        for (i = in_el_0; i < in_el_n_plus_one; i++)
        {
           m_value_recs.at(i).marked = false;
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

    if (in_el < m_value_recs.size())
    {
        rv = m_value_recs.at(in_el).marked;
    }
    else
    {
        rv = false;
    }

    return rv;
}


/*!
 * \brief                                Parses a list of configuration file tokens to an object-internal data
 *                                       structure consisting of an array of nesting information (enclosing
 *                                       key names, dictionary names, array names, and array subscripts) and
 *                                       the values.  The client can then retrieve this information via
 *                                       separate method calls.
 *
 * \param[in]     in_lex_token_list      The list of tokens to parse.
 * \param[in,out] out_parse_warn_n_errs  The caller provides a list of warnings and errors that this
 *                                       function will append to with any detected warnings and errors from
 *                                       the parse.  The most normal case would be for the caller to
 *                                       provide an empty list.  If any warnings or errors are appended,
 *                                       the success code (both the function's return value and *out_success)
 *                                       will be false.  At the present time, there are no warnings, and
 *                                       every detected anomaly will prevent parsing.
 * \param[out]    out_success            A pointer to return a bool that will be the same as the function's
 *                                       return value.  This pointer may legitimately be nullptr, in which
 *                                       case the pointer won't be dereferenced.
 *
 * \returns                              true if the parse was successful, or false otherwise.
 *
 * \reentrancyandthreadsafety            Not evaluated.
 *
 * \errorsandexceptions                  Errors or exceptions due to out of memory are possible.
 */
bool ConfigFile::Parse(const LgBufUint8& in_buf, const LgCppDdParseLexCustA& in_lex_token_list, LgCppDdErrlistCdA& out_parse_warn_n_errs, bool* out_success)
{
   constexpr bool debug = true;
      //true if more information should be written.
   bool rv_success = true;
      //Return value, true means success.
   enum 
   {
      PsInit,
         //Initial state, waiting for dictionary entry.
      PsDictEntered,
         //A dictionary was entered.
      PsNameParsed,
         //The name of a name:value pair has been parsed.
      PsColonParsed,
         //The colon of name:value pair has been parsed.
   } parse_state = PsInit;
                     //The major state of the parsing state machine.
   bool completion_exit_flag = false;
      //Set to true if there is a normal completion and the while() loop should
      //exit.
   bool error_exit_flag = false;
      //Set to true if an error is encountered.  All errors will result in
      //deleting the vector stack of values parsed, so that there is no possibility
      //of using an incomplete result.
   size_t cur_token_idx = 0;
      //The index of the lexical analysis token currently being examined.
   tLgCppDdParseLexCustAStruct cur_token;
      //The token currently being examined.
   size_t n_tokens = 0;
      //Number of tokens.

   if (debug)
      std::cout << "ConfigFile::Parse() entry." << std::endl;

   n_tokens = in_lex_token_list.Ntokens();

   while ((!completion_exit_flag) && (!error_exit_flag))
   {
      //Load a token.  Member function correctly handles invalid index.
      in_lex_token_list.GetTokenCopy(cur_token_idx, cur_token);

      switch (parse_state)
      {
         case PsInit:
            if (debug)
               std::cout << "ConfigFile::Parse(), in state PsInit." << std::endl;

            if (cur_token_idx >= n_tokens)
            {
               //Ran out of tokens.  Not good.
               out_parse_warn_n_errs.AppendFull(
                                                  in_buf,
                                                  "",
                                                  "",
                                                  0,
                                                  0,
                                                  0,
                                                  LgCppDdErrlistCdASeverity::High,
                                                  "",
                                                  "",
                                                  "Prematurely exhausted tokens."
                                               );
               error_exit_flag = true;
            }
            else if (cur_token.tokentype == LgCppDdParseCustLexATokenType::BraceLeft)
            {
               //Token as expected.
               cur_token_idx++;
               parse_state = PsDictEntered;
            }
            else
            {
               //In this state, there is only one type of token to progress.  It was not found.
               out_parse_warn_n_errs.AppendFull(
                                                  in_buf,
                                                  "",
                                                  "",
                                                  0,
                                                  0,
                                                  0,
                                                  LgCppDdErrlistCdASeverity::High,
                                                  "",
                                                  "",
                                                  "Unable to progress."
                                               );
            }
            break;
         case PsDictEntered:
            if (debug)
               std::cout << "ConfigFile::Parse(), in state PsDictEntered." << std::endl;
            completion_exit_flag = true;
            break;
         case PsNameParsed:
            if (debug)
               std::cout << "ConfigFile::Parse(), in state PsNameParsed." << std::endl;
            completion_exit_flag = true;
            break;
         case PsColonParsed:
            if (debug)
               std::cout << "ConfigFile::Parse(), in state PsColonParsed." << std::endl;
            completion_exit_flag = true;
            break;
         default:
            completion_exit_flag = true;
            break;
      }
   }

   //Assign the pass-by-reference value.
   if (out_success)
      *out_success = rv_success;
 
   return rv_success;

#if 0
   bool rv_success = false;
      //Return value, true means success.
   LgCppDdParseLexCustA cust_lexer;
      //Custom lexographic analyzer.
   bool lex_success;
      //Success or failure from the lexographic analyzer.

   cust_lexer.Parse(in_buf, &lex_success, out_lex_warnings_and_errors);

#if 0
   size_t rv_err_line = 0;
      //Error line that will be returned.
   std::string rv_err_msg = "";
      //Error message, if any, that will be returned.
   bool exit_flag = false;
      //true if should exit while() loop.
   size_t cur_line = 1;
   size_t cur_parse_pos = 0;
   size_t next_parse_pos = 0;
   std::string name_buffered;
      //A name (standard identifier) buffered.
   tConfigFileObjRecord pushable_record{};

   while (!exit_flag)
   {
      switch (parse_state)
      {
         case PsInit:
         {
            if (LgPrdpBu8ParseSpcTbNoEols(in_fc, cur_parse_pos, next_parse_pos, nullptr))
            {
               if (debug)
               {
                  std::cout << "PsInit: consume whitespace, start: " << cur_parse_pos << " next:" << next_parse_pos << std::endl;
               }
               cur_parse_pos = next_parse_pos;
            }
            else if (LgPrdpBu8ParseCppComment(in_fc, cur_parse_pos, next_parse_pos, nullptr))
            {
               if (debug)
               {
                  std::cout << "PsInit: consume C++ comment, start: " << cur_parse_pos << " next:" << next_parse_pos << std::endl;
               }
               cur_parse_pos = next_parse_pos;
            }
            else if (LgPrdpBu8ParseEolAnyPfm(in_fc, cur_parse_pos, next_parse_pos, nullptr))
            {
               if (debug)
               {
                  std::cout << "PsInit: consume eol, start: " << cur_parse_pos << " next:" << next_parse_pos << std::endl;
               }
               cur_line++;
               cur_parse_pos = next_parse_pos;
            }
            else if (LgPrdpBu8ParseCurlyBraceLeft(in_fc, cur_parse_pos, next_parse_pos, nullptr))
            {
               if (debug)
               {
                  std::cout << "PsInit: consume '{', start: " << cur_parse_pos << " next:" << next_parse_pos << std::endl;
               }
               cur_parse_pos = next_parse_pos;
               parse_state = PsDictEntered;
            }
            else
            {
               if (debug)
               {
                  std::cout << "PsInit: unable to consume: line_no: " << cur_line << " cur_parse_pos: " << cur_parse_pos << std::endl;
               }

               exit_flag = true;
            }
         }
         break;
         case PsDictEntered:
         {
            if (LgPrdpBu8ParseSpcTbNoEols(in_fc, cur_parse_pos, next_parse_pos, nullptr))
            {
               if (debug)
               {
                  std::cout << "PsDictEntered: consume whitespace, start: " << cur_parse_pos << " next:" << next_parse_pos << std::endl;
               }
               cur_parse_pos = next_parse_pos;
            }
            else if (LgPrdpBu8ParseCppComment(in_fc, cur_parse_pos, next_parse_pos, nullptr))
            {
               if (debug)
               {
                  std::cout << "PsDictEntered: consume C++ comment, start: " << cur_parse_pos << " next:" << next_parse_pos << std::endl;
               }
               cur_parse_pos = next_parse_pos;
            }
            else if (LgPrdpBu8ParseEolAnyPfm(in_fc, cur_parse_pos, next_parse_pos, nullptr))
            {
               if (debug)
               {
                  std::cout << "PsDictEntered: consume eol, start: " << cur_parse_pos << " next:" << next_parse_pos << std::endl;
               }
               cur_line++;
               cur_parse_pos = next_parse_pos;
            }
            else if (LgPrdpBu8ParseStdIdentifier(in_fc, cur_parse_pos, next_parse_pos, nullptr))
            {
               if (debug)
               {
                  std::cout << "PsDictEntered: consume standard identifier, start: " << cur_parse_pos << " next:" << next_parse_pos << std::endl;
               }

               name_buffered.assign((char *)(in_fc.GetBufPtr() + cur_parse_pos), next_parse_pos - cur_parse_pos);

               parse_state = PsNameParsed;
               cur_parse_pos = next_parse_pos;
            }
            else
            {
               if (debug)
               {
                  std::cout << "PsDictEntered: unable to consume: line_no: " << cur_line << " cur_parse_pos: " << cur_parse_pos << std::endl;
               }

               exit_flag = true;
            }
         }
         break;
         case PsNameParsed:
         {
            if (LgPrdpBu8ParseSpcTbNoEols(in_fc, cur_parse_pos, next_parse_pos, nullptr))
            {
               if (debug)
               {
                  std::cout << "PsNameParsed: consume whitespace, start: " << cur_parse_pos << " next:" << next_parse_pos << std::endl;
               }
               cur_parse_pos = next_parse_pos;
            }
            else if (LgPrdpBu8ParseCppComment(in_fc, cur_parse_pos, next_parse_pos, nullptr))
            {
               if (debug)
               {
                  std::cout << "PsNameParsed: consume C++ comment, start: " << cur_parse_pos << " next:" << next_parse_pos << std::endl;
               }
               cur_parse_pos = next_parse_pos;
            }
            else if (LgPrdpBu8ParseEolAnyPfm(in_fc, cur_parse_pos, next_parse_pos, nullptr))
            {
               if (debug)
               {
                  std::cout << "PsNameParsed: consume eol, start: " << cur_parse_pos << " next:" << next_parse_pos << std::endl;
               }
               cur_line++;
               cur_parse_pos = next_parse_pos;
            }
            else if (LgPrdpBu8ParseColon(in_fc, cur_parse_pos, next_parse_pos, nullptr))
            {
               if (debug)
               {
                  std::cout << "PsNameParsed: consume colon, start: " << cur_parse_pos << " next:" << next_parse_pos << std::endl;
               }

               //name_buffered.assign((char*)(in_fc.GetBufPtr() + cur_parse_pos), next_parse_pos - cur_parse_pos);

               parse_state = PsColonParsed;
               cur_parse_pos = next_parse_pos;
            }
            else
            {
               if (debug)
               {
                  std::cout << "PsNameParsed: unable to consume: line_no: " << cur_line << " cur_parse_pos: " << cur_parse_pos << std::endl;
               }

               exit_flag = true;
            }
         }
         break;
         case PsColonParsed:
         {
            exit_flag = true;

         }
         break;
         default:
         {

         }
         break;
      }
   }

   out_success  = rv_success;
   out_err_line = rv_err_line;
   out_err_msg  = rv_err_msg;
#endif

   return rv_success;
#endif
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

   rv = m_value_recs.size();

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

    for (const auto& element : m_value_recs)
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

    if (in_el < m_value_recs.size())
   {
      //Grab a reference.
      const auto& element = m_value_recs.at(in_el);

      //Copy to the caller's area.
      out_rec.marked                  = element.marked;
      out_rec.n_nesting_level_records = element.n_nesting_level_records;

      //Assigning manually because unsure of C++ behavior, either for arrays or for
      //copies of objects within records.
      for (i = 0; i < MODULE_CONFIGFILE_CFGFILE_DEPTH_MAX; i++)
      {
         //out_rec.paths[i].enclosure_type    = element.paths[i].enclosure_type;
         //out_rec.paths[i].key             = element.paths[i].key;
         //out_rec.paths[i].value_string    = element.paths[i].value_string;
         //out_rec.paths[i].value_int32     = element.paths[i].value_int32;
      }
   }
   else
   {
      //Zero out the caller's area.
      out_rec.marked                  = false;
      out_rec.n_nesting_level_records = 0;

      for (i = 0; i < MODULE_CONFIGFILE_CFGFILE_DEPTH_MAX; i++)
      {
         //out_rec.paths[i].m_type       = ConfigFileNestLevelEnclosureType::Undefined;
         //out_rec.paths[i].key          = "";
         // out_rec.paths[i].value_string = "";
         //out_rec.paths[i].value_int32  = 0;
      }
   }
}
