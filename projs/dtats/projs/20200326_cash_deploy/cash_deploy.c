//-------------------------------------------------------------------------------------------------
//Copyright (c) 2020, David T. Ashley
//
//This file is the source code for the program "cash_deploy", a quick-and-dirty program
//for determining how to place limit orders for dollar cost averaging during the COVID-19
//epidemic in 2020.
//
//This source code and any program in which it is compiled/used is licensed under the MIT License,
//reproduced below.
//
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
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//-------------------------------------------------------------------------------------------------
//Note:  Did not find a good solution to the problems of:
//       a)Buying a consistent number of shares at both endpoints.
//       b)Finding the next buy point without iterating through the price, penny by penny.
//       I'm sure an algebraic/mathematical solution exists for both problems, but I didn't have
//       enough time to invest (as of this writing).
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Returns the amount of cash that should be available as a function of the price point.
static unsigned cash_desired(unsigned price_bottom, unsigned price_top, unsigned price_in, unsigned cash)
{
   if (price_in < price_bottom)
   {
      return 0;
   }
   else if (price_in >= price_top)
   {
      return cash;
   }
   else
   {
      unsigned long long span = price_top - price_bottom;
      unsigned long long penetration = price_in - price_bottom;
      unsigned long long rv;

      rv = (penetration * cash) / span;

      return (unsigned)rv;
   }
}

static void hline(void)
{
  printf("----------------------------------------------------------------------\n");
}


static void evaluate(char *desc, unsigned price_bottom, unsigned price_top, unsigned cash, unsigned increment)
{
   int expenditures;
   unsigned price;

   unsigned count;
   expenditures = 0;

   hline();
   printf("%s\n", desc);
   hline();

   for (price=price_bottom; price<=price_top; price+=increment)
   {
      if (
            ((expenditures + (int)(cash_desired(price_bottom, price_top, price, cash))) >= 0)
         )
      {
         //printf("Expenditures: %d DJIA: %u Cash desired: %u\n", expenditures, djia, cash_desired(djia));
         count = 0;
         while ((expenditures + (int)(cash_desired(price_bottom, price_top, price, cash))) >= 0)
         {
            count++;
            expenditures -= price;
         }
         printf("Buy %3u share(s) at %3u.%02u (Expenditures: %6d.%02d)\n", count, price/100, price%100, (-expenditures)/100, (-expenditures)%100);
      }
   }
}


int main(void)
{
  //evaluate("DIA", 12000, 29500, 2000000,   1);
  evaluate("USO", 300,   500,   500000,    2);
  hline();
  return 0;
}
