//-------------------------------------------------------------------------------
//$Header: /home/dashley/cvsrep/uculib01/uculib01/src/stm8/cosmic/modx/bas16absdiffboundedrxx/unittest/ut_project/stm8/cosmic/modx/utfu_bas16absdiffboundedrxx.c,v 1.3 2010/03/31 17:20:48 dashley Exp $
//-------------------------------------------------------------------------------
//Copyright (c)2010 David T. Ashley
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software source code and associated documentation files (the
//"Software"), to deal in the Software without restriction, including without
//limitation the rights to use, copy, modify, merge, publish, distribute,
//sublicense, and/or sell copies of the Software, and to permit persons to whom
//the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.
//-------------------------------------------------------------------------------
#define MODULE_UTFU_BAS16ABSDIFFBOUNDEDRXX

#include "\uculib01\include\stm8\cosmic\c\uculib.h"
#include "utfu_bas16absdiffboundedrxx.h"


typedef struct
   {
   UCU_SINT16  arg1;
   UCU_SINT16  arg2;
   UCU_SINT16  expected_result;
   } BAS16ABSDIFFBOUNDEDRXX_TV;


//This is the table of test vectors.
//
const BAS16ABSDIFFBOUNDEDRXX_TV BaS16AbsDiffBoundedRxx_Tvs[] =
   {
      //Calling convention check.
      {  0x0505,   0x0707,   0x0202},  //Test to be sure that 0x0505 in X and 0x0707 on stack.
      //Branch path checks.
      {    1000,      333,      667},  //No overflow, no need to negate the result.
      {     333,     1000,      667},  //No overflow, need to negate the result.
      {     -10,    32758,    32767},  //No overflow, tests equal to -32768.
      {   32758,      -10,    32767},  //Overflow, tests equal to -32768.
      //Should not overflow.
      {       0,        0,        0},
      {       0,    32767,    32767},
      {   32767,        0,    32767},
      {   32000,     -767,    32767},
      {    -767,    32000,    32767},
      {  -32000,      768,    32767},
      //Should overflow.
      {      -2,    32767,    32767},
      {  -32767,        2,    32767},
      {   32000,     -768,    32767},
      {    -767,    32002,    32767},
      {  -32000,      769,    32767},
   };


//Returns the number of test vectors that may be used for a speed test.
//
UCU_UINT32 UtsnBaS16AbsDiffBoundedRxx(void)
   {
   return(sizeof(BaS16AbsDiffBoundedRxx_Tvs)/sizeof(BaS16AbsDiffBoundedRxx_Tvs[0]));
   }


//Runs a selected test vector.  This is used for automated speed testing.
//
void UtspBaS16AbsDiffBoundedRxx(UCU_UINT32 test_no, void (*start_pointer)(void), void (*end_pointer)(void))
   {
   UCU_SINT16 arg1, arg2;
   UCU_SINT16 result;

   //Error out if the index is invalid.
   //
   if (test_no >= (sizeof(BaS16AbsDiffBoundedRxx_Tvs)/sizeof(BaS16AbsDiffBoundedRxx_Tvs[0])))
      {
      while(UCU_TRUE)  //Can't allow this scenario and dereference an invalid vector.
         ;
      }

   //Fetch the parameters.  This removes all of the array indexing and so on from the
   //elapsed time calculation.
   arg1           = BaS16AbsDiffBoundedRxx_Tvs[test_no].arg1;
   arg2           = BaS16AbsDiffBoundedRxx_Tvs[test_no].arg2;

   //We are ready to make the call.  Start the caller's timer.
   (*start_pointer)();

   //Call the function.
   result = UcuBaS16AbsDiffBoundedRxx(arg1, arg2);

   //The call is finished.  Stop the caller's timer.
   (*end_pointer)();
   }


//Runs all test vectors.  Returns TRUE if OK, FALSE if any failed.
//
UCU_BOOLEAN UtfuBaS16AbsDiffBoundedRxx(void)
   {
   UCU_BOOLEAN rv;
   UCU_UINT16  i;
   UCU_SINT16  arg1, arg2, expected_result, actual_result;
	 UCU_SINT32  base1, base2, longcalc;

   rv = UCU_TRUE;

   for (i=0; i<(sizeof(BaS16AbsDiffBoundedRxx_Tvs)/sizeof(BaS16AbsDiffBoundedRxx_Tvs[0])); i++)
      {
      arg1            = BaS16AbsDiffBoundedRxx_Tvs[i].arg1;
      arg2            = BaS16AbsDiffBoundedRxx_Tvs[i].arg2;
      expected_result = BaS16AbsDiffBoundedRxx_Tvs[i].expected_result;

      actual_result   = UcuBaS16AbsDiffBoundedRxx(arg1, arg2);

      if (actual_result != expected_result)
         rv = UCU_FALSE;
      }

	 for (base1 = -32768; base1 <= 32767; base1++)
	    {
			for (base2 = -32768; base2 <= 32767; base2++)
			   {
				 longcalc = base1 - base2;
				 if (longcalc <= -32768)
				    {
					  expected_result = 32767;
				    }
				 else if (longcalc > 32767)
				    {
						expected_result = 32767;
				    }
				 else if (longcalc < 0)
				    {
					  expected_result = -longcalc;
						}
				 else
				    {
					  expected_result = longcalc;
				    }
						
				 arg1 = base1;
				 arg2 = base2;
				
         actual_result   = UcuBaS16AbsDiffBoundedRxx(arg1, arg2);
 
         if (actual_result != expected_result)
            rv = UCU_FALSE;
			   }
	    }

   return(rv);
   }


//-------------------------------------------------------------------------------
//$Log: utfu_bas16absdiffboundedrxx.c,v $
//Revision 1.3  2010/03/31 17:20:48  dashley
//Unit test enhanced.
//
//Revision 1.2  2010/03/21 02:03:35  dashley
//Initial checkin.
//
//Revision 1.1  2010/03/21 02:02:52  dashley
//Initial checkin.
//-------------------------------------------------------------------------------
//End of $RCSfile: utfu_bas16absdiffboundedrxx.c,v $
//-------------------------------------------------------------------------------

