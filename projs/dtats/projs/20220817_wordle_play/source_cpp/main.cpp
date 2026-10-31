// 20220817_wordle_play.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// Note to self:
//    When I consider the algorithms, need to consider relationships that extend beyond simple letter
//    exclusions and rely on the fact that there are only 5 letters.  For example, let x (a lower-case
//    letter) denote a failed letter guess, _x denote an out-of-position guess, and X denote an
//    in-position guess.  Then let's assume the play sequence:
//  
//             p   O   U  _t   y
// 
//            _t   O   U  _c   h
//
// Then "t" has to be in the last position because of the O and U being in the correct position and
// the failed guess in the first column.  Similarly, C has to be first position, because t is
// proved in the last position and there is the failed guess for c in column 4.
//
// The algorithms need to take this calculus into account.
//
#include <cstddef>
#include <stdio.h>

#include "dictionary.h"
#include "main.h"

void emit_help(void)
{
    static const char* m[] =
    {
        "------------------------------------------------------------------------------",
        "This program (\"20220817_wordle_play\"), is a strategy assistance program for",
        "Wordle (the popular web-based game provided by The New York Times).  Although",
        "it can be used as a standalone console-mode program, it is designed to be",
        "invoked by a PHP-based web page script.  [When invoked by a PHP-based web",
        "page script, the command-line parameters (which are a bit awkward) can be",
        "formed by the PHP script, and the output can be parsed by the PHP script.",
        "",
        "This program is novel in that it uses combinatorial algorithms to determine",
        "what might be the best Wordle guesses.  Although a human may be satisfied",
        "with the guesses this program provides, the algorithm leading to the guesses",
        "is not an intuitive one for a human.",
        "",
        "COMMAND-LINE INVOCATION FORMAT",
        "------------------------------",
        "   20220817_wordle_play major_mode [options] [parameters]",
        "",
        "where major_mode is described below, and which options and parameters are",
        "valid is dependent on the major mode.",
        "",
        "The program return code will be 0 if no errors or anomalies were detected, or",
        "4 if error or anomalies are detected.",
        "",
        "MAJOR MODE:  HELP [-h, --h, -help, --help, or unrecognized options/parameters]",
        "------------------------------------------------------------------------------",
        "This complete help text is emitted.",
        "",
        "MAJOR MODE:  TRAIN [-train]",
        "------------------------------------------------------------------------------",
        "This complete help text is emitted.",
        "",
        "------------------------------------------------------------------------------",
    };

    size_t ui;

    for (ui = 0; ui < (sizeof(m) / sizeof(m[0])); ui++)
    {
        printf("%s\n", m[ui]);
    }
}

int main()
{
    Dictionary dictionary;

    emit_help();

//    for (size_t i = 0; i < dictionary.Nelem(); i++)
//    {
//        printf("[%6zu] %s\n", i, dictionary.GetStr(i));
//    }
}
