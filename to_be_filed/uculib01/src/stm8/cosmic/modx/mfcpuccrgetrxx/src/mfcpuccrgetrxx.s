;-------------------------------------------------------------------------------
;$Header: /home/dashley/cvsrep/uculib01/uculib01/src/stm8/cosmic/modx/mfcpuccrgetrxx/src/mfcpuccrgetrxx.s,v 1.5 2010/03/21 05:08:29 dashley Exp $
;-------------------------------------------------------------------------------
;Copyright (c)2010 David T. Ashley
;
;Permission is hereby granted, free of charge, to any person obtaining a copy
;of this software source code and associated documentation files (the
;"Software"), to deal in the Software without restriction, including without
;limitation the rights to use, copy, modify, merge, publish, distribute,
;sublicense, and/or sell copies of the Software, and to permit persons to whom
;the Software is furnished to do so, subject to the following conditions:
;
;The above copyright notice and this permission notice shall be included in
;all copies or substantial portions of the Software.
;
;THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
;THE SOFTWARE.
;-------------------------------------------------------------------------------
;This function doesn't use any static storage, so it is OK for either mods or
;modsl memory models.  However, it does assume callf/retf (rather than call/ret)
;which affects both instructions used, stack offsets, and numerical values in
;the .dcall directive.  For this reason, must error out if being assembled
;for the wrong memory model.
#ifndef UCU_BD_MMBP
   #error "Program memory model define not provided on command line (UCU_BD_MMBP)."
#endif
#if (UCU_BD_MMBP != 2)
   #error "Attempt to assemble module for wrong program memory model (UCU_BD_MMBP != 2)."
#endif
;
            switch .text
            xdef   f_UcuMfCpuCcrGetRxx
;
;Per discussion with Cosmic, the first integer is the stack space used by the
;call instruction plus any automatic storage used by the function.  The second integer
;is the number of bytes stacked by the caller.  Haven't yet discussed the scenario
;of one assembly-language function called by another.  I've noticed that some
;assembly-language functions have a larger first integer than the stack they
;used, so there may be a special convention if C calls .S which then calls .S,
;as maybe the tools don't detect the .S calling the .S.  Will need to investigate
;further with Cosmic.
;
;3 bytes for the return address stacked by CALLF, 1 byte stacked by this function because 
;of the mechanism used to obtain the CCR, 0 bytes stacked by the caller.
            .dcall  "4,0,f_UcuMfCpuCcrGetRxx"
;
f_UcuMfCpuCcrGetRxx:
        push  cc              ;Only way to copy out the CC register.
        pop   a               ;Get it into A.
        retf                  ;Return value set up.  Can return.
;
        end
;
;-------------------------------------------------------------------------------
;End of $Id: mfcpuccrgetrxx.s,v 1.5 2010/03/21 05:08:29 dashley Exp $
;-------------------------------------------------------------------------------
;$Log: mfcpuccrgetrxx.s,v $
;Revision 1.5  2010/03/21 05:08:29  dashley
;Comments corrected.
;
;Revision 1.4  2010/02/20 16:07:22  dashley
;Missing switch directive added.
;
;Revision 1.3  2010/02/18 14:41:23  dashley
;Documentation corrected.
;
;Revision 1.2  2010/02/14 03:12:14  dashley
;Extra line removed.
;
;Revision 1.1  2010/02/14 03:11:49  dashley
;Initial checkin.
;-------------------------------------------------------------------------------

