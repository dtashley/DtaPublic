//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_cmode/fcmiof.c 244 2018-08-05 19:05:46Z dashley $
//{723c099f-de0c-46b4-93a9-ca89bc796de7}
//-------------------------------------------------------------------------------------------------
//This file is part of "Embedded Tool Set", a tool set designed to facilitate embedded system
//software and hardware development.
//-------------------------------------------------------------------------------------------------
//This source code and any program in which it is compiled/used is provided under the MIT License,
//reproduced below.
//-------------------------------------------------------------------------------------------------
//Permission is hereby granted, free of charge, to any person obtaining a copy of
//this software and associated documentation files(the "Software"), to deal in the
//Software without restriction, including without limitation the rights to use,
//copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the
//Software, and to permit persons to whom the Software is furnished to do so,
//subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//-------------------------------------------------------------------------------------------------
#define MODULE_FCMIOF

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "fcmiof.h"

#define FCMIOF_HORIZONTAL_BAR_SEP_CHAR    ('-')


int FCMIOF_stream_repchar(FILE *s, char c, size_t n)
{
   int rv = 0;

   assert(s != NULL);

   while ((n--) && (rv >= 0))
   {
      rv = fprintf(s, "%c", c);
   }

   return(rv);
}


int FCMIOF_repchar(char c, size_t n)
{
   int rv = 0;

   while ((n--) && (rv >= 0))
   {
      rv = printf("%c", c);
   }

   return(rv);
}


int FCMIOF_stream_hline(FILE *s, size_t line_len)
{
   int rv;

   assert(s != NULL);

   rv = FCMIOF_stream_repchar(s, FCMIOF_HORIZONTAL_BAR_SEP_CHAR, line_len);

   if (rv >= 0)
      rv = fprintf(s, "\n");

   return(rv);
}


int FCMIOF_hline(size_t line_len)
{
   int rv;

   rv = FCMIOF_repchar(FCMIOF_HORIZONTAL_BAR_SEP_CHAR, line_len);

   if (rv >= 0)           //If no error from previous output attempts.
      rv = printf("\n");  //Do the final printf().

   return(rv);
}


int FCMIOF_stream_bannerheading(FILE *f,
                                char *s,
                                size_t line_len,
                                size_t n_extra_lines)
{
   size_t lr_padding = 4;
      //The number of spaces on each side of what is printed.
   size_t i;
      //General iteration variable.
   size_t length_limit;
      //The maximum length-related value that will be used for
      //calculation.  This is to prevent overflow and incorrect
      //results in logical expressions.
   size_t input_arg_len;
      //Length of the banner heading provided by the caller.  May
      //be clipped to prevent logical errors.
   size_t total_padding_chars, total_padding_chars_left, total_padding_chars_right;
      //The total number of characters available for padding, and the
      //number that will be used on the left, and on the right.  This
      //includes asterisks, and spaces.
   size_t n_left_asterisks, n_right_asterisks;
      //The number of asterisks that will be printed on the left and right, respectively.

   int rv = 0;

   //Check the file pointer, string pointer, and other parameters.
   assert(f != NULL);
   assert(s != NULL);

   //Figure out how many asterisks to print on each side of the
   //argument, and how many spaces.  We also need to figure out
   //how many characters of the input argument to print--if there
   //are too many characters, we need to truncate.

   //Figure out the length of the string passed by the caller.
   input_arg_len = strlen(s);

   //We must trim the parameters involving length to a sane value, otherwise
   //the arithemtic expressions below will break down.  There are other ways
   //to handle this, with more branches and less clarity.  The C standard
   //specifies that SIZE_MAX must be at least 65535, so SIZE_MAX/32 >= 2047.
   length_limit = SIZE_MAX / 32;
   if (line_len > length_limit)
      line_len = length_limit;
   if (lr_padding > length_limit)
      lr_padding = length_limit;
   if (input_arg_len > length_limit)
      input_arg_len = length_limit;

   //Figure out the number of characters available for padding on the left
   //and right.  If the total number of characters available is odd, we want
   //few characters on the right.
   if (line_len > input_arg_len)
      total_padding_chars = line_len - input_arg_len;
   else
      total_padding_chars = 0;

   if ((total_padding_chars % 2) == 0)
   {
      //Even.
      total_padding_chars_left = total_padding_chars_right = total_padding_chars / 2;
   }
   else
   {
      //Odd.
      total_padding_chars_left = total_padding_chars / 2;
      total_padding_chars_right = total_padding_chars_left + 1;
   }

   //Figure out the number of asterisks on the left.
   if (total_padding_chars_left > lr_padding)
      n_left_asterisks = total_padding_chars_left - lr_padding;
   else
      n_left_asterisks = 0;

   //Figure out the number of asterisks on the right.
   if (total_padding_chars_right > lr_padding)
      n_right_asterisks = total_padding_chars_right - lr_padding;
   else
      n_right_asterisks = 0;

   // Print the correct number of solid lines of asterisks to the
   // standard output.
   for (i = 0; (i < n_extra_lines) && (rv >= 0); i++)
   {
      rv = FCMIOF_stream_repchar(f, '*', line_len);
      if (rv >= 0)
         rv = fprintf(f, "\n");
   }

   //return(0);

   //If there has been an error in the printing, return.
   if (rv < 0)
      return(rv);

   //Left asterisks of banner.
   rv = FCMIOF_stream_repchar(f, '*', n_left_asterisks);
   if (rv < 0)
      return(rv);

   //return(0);

   //Left spaces of banner.
   rv = FCMIOF_stream_repchar(f, ' ', lr_padding);
   if (rv < 0)
      return(rv);

   //The banner text itself.
   for (i = 0; (i < input_arg_len) && (rv >= 0); i++)
   {
      rv = fprintf(f, "%c", s[i]);
   }

   if (rv < 0)
      return(rv);

   //Right spaces of banner.
   rv = FCMIOF_stream_repchar(f, ' ', lr_padding);
   if (rv < 0)
      return(rv);

   //Right asterisks of banner.
   rv = FCMIOF_stream_repchar(f, '*', n_right_asterisks);
   if (rv < 0)
      return(rv);

   //Terminating newline.
   rv = fprintf(f, "\n");
   if (rv < 0)
      return(rv);

   /* Print the right number of solid lines of asterisks to the
   ** standard output.
   */
   for (i = 0; (i < n_extra_lines) && (rv >= 0); i++)
   {
      rv = FCMIOF_stream_repchar(f, '*', line_len);
      if (rv >= 0)
         rv = fprintf(f, "\n");
   }

   return(rv);
}

int FCMIOF_bannerheading(char *s, size_t line_len, size_t n_extra_lines)
   {
   int rv;

   assert(s != NULL);

   rv = FCMIOF_stream_bannerheading(stdout, s, line_len, n_extra_lines);

   return(rv);
   }

void FCMIOF_time_stream(FILE *s, time_t ltime)
   {
   char *p;

   assert(s != NULL);

   time(&ltime);

   p = ctime(&ltime);

   if (p)
      {
      int i;

      for (i=11; i<19; i++)
         fprintf(s, "%c", p[i]);
      fprintf(s, " ");
      for (i=0; i<10; i++)
         fprintf(s, "%c", p[i]);
      fprintf(s, " ");
      for (i=20; i<24; i++)
         fprintf(s, "%c", p[i]);
      }
   else
      {
      fprintf(s, "??? ??? ?? ??:??:?? ????");
      }
   }



//08/16/01:  Visual inspection OK.
const char *FCMIOF_cvcinfo(void)
   {
   return("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_cmode/fcmiof.c 244 2018-08-05 19:05:46Z dashley $");
   }


//08/16/01:  Visual inspection OK.
const char *FCMIOF_hvcinfo(void)
   {
   return(FCMIOF_H_VERSION);
   }

//End of fcmiof.c.
