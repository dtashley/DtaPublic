//$Header: svn://localhost/dtapublic/projs/dtats/trunk/projs/2018/20180707_cgi_web_tools_aux_exe/dtats_cgi_aux_arith_large/auxfuncs.h 207 2018-07-15 21:50:56Z dashley $
//********************************************************************************
//Copyright (c) 2003, 2018 David T. Ashley.
//********************************************************************************
//This file is part of "arith_large_cgi", a program that is designed to be
//invoked by a PHP script as part of serving a web page that performs
//calculations involving large integers.  (A secondary compiled program is
//used because a compiled program can perform certain  calculation-intensive
//tasks far more efficiently than a PHP script.)  This program is provided by
//David T. Ashley (dashley@gmail.com) under the MIT License (reproduced
//immediately below).
//********************************************************************************
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//********************************************************************************
//
#ifndef AUXFUNCS_H_INCLUDED
   #define AUXFUNCS_H_INCLUDED

   #ifdef MODULE_AUXFUNCS
      #define DECMOD_AUXFUNCS
   #else
      #define DECMOD_AUXFUNCS extern
   #endif

   //CHARACTER CLASSIFICATION FUNCTIONS.
   DECMOD_AUXFUNCS int AUXFUNCS_is_digit(char c);
   DECMOD_AUXFUNCS int AUXFUNCS_digit_to_val(char digit);

   //SIZE_T INTEGER FUNCTIONS.
   DECMOD_AUXFUNCS size_t AUXFUNCS_size_t_min(size_t a, size_t b);
   DECMOD_AUXFUNCS size_t AUXFUNCS_size_t_max(size_t a, size_t b);

   //CONTRACTING STRING FUNCTIONS.
   DECMOD_AUXFUNCS void AUXFUNCS_remove_non_digits(char *s);
   DECMOD_AUXFUNCS void AUXFUNCS_remove_leading_zeros(char *s);

   /****************************************************************/
   /***   VERSION CONTROL REPORTING FUNCTIONS   ********************/
   /****************************************************************/
   DECMOD_AUXFUNCS const char *AUXFUNCS_cvcinfo(void);
   DECMOD_AUXFUNCS const char *AUXFUNCS_hvcinfo(void);

   /* Preprocessor string to allow the H-file version to be
   ** compiled into the C-file.
   */
   #define AUXFUNCS_H_VERSION ("$Header: svn://localhost/dtapublic/projs/dtats/trunk/projs/2018/20180707_cgi_web_tools_aux_exe/dtats_cgi_aux_arith_large/auxfuncs.h 207 2018-07-15 21:50:56Z dashley $")

#endif /* AUXFUNCS_H_INCLUDED */

//********************************************************************************
// End of AUXFUNCS.H.
//********************************************************************************
