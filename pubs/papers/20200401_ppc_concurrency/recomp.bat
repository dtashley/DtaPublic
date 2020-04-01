rem Batch file to make LaTeX document for PPC concurrency whitepaper.
rem Because of the way LaTeX works, it may be necessary to run this file up to three or four
rem times, because early portions of the document which are formed by collecting entries
rem from later portions of the document may grow and thereby shift page numbering.  Running
rem this four times should be enough.
latex ppc_conc
makeindex ppc_conc
dvips -T 8.5in,11in ppc_conc
