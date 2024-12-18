# _pub_mft_gen_
The program contained in this directory, _pub_mft_gen_ (mnemonic:
**pub**lication **m**ulti**f**orma**t** **gen**erator):
* Accepts input as marked up text (using markup conventions
  similar to *LaTeX*), and produces:
  * _ePub_ output.
  * (_Unimplemented_) Structured HTML output, suitable for online of offline publications
    or help.
  * (_Unimplemented_) *LaTeX* output.
* Handles section numbering and references.
* Handles equation numbering and references (but not generation of equation
  graphics).
* Handles table of contents generation.
* Handles bibliography references.
* Handles index generation.
* Neatens the template source files.

_pub_mft_gen_ reads from a command file (described below) and performs
the actions described in the command file sequentially. This
requires the user to know in detail how the program works, as it would be
possible to create bizarre results with an incorrect command file.

_pub_mft_gen_ is at the present time built only for _Windows_, and only
project files for _Visual Studio_ are included in _Git_. However,
since the program is standard C++, it should port to _Unix_ platforms
easily.

## Program Build Requirements
The program is a standard _Visual Studio_ project, and the project
files are included in _Git_.  The program should build successfully
using the included _Visual Studio_ project files.

However, in order to build, other repositories must be cloned to specific
relative locations.  The following repositories must be cloned at the
same directory level.

* _DtaPublic_, https://github.com/dtashley/DtaPublic. (This repository
  contains the _pub_mft_gen_ program and its _Visual Studio_ project
  files.  When building _pub_mft_gen_, the repository containing
  it, _DtaPublic_, must be cloned at the same level as the two
  additional repositories
  cited below.)
* _LibGen_, https://github.com/dtashley/LibGen.
* _LibNum_, https://github.com/dtashley/LibNum.

## Invocation and Command-Line Options
The program should be invoked using a command line should be of the form:

*pub_mft_gen [-v] command_file_path*

* The only supported option is *-v*, which commands extra verbosity.
* *command_file_path* is a mandatory argument, and is the path to the
  command file (format described below).

## Command File Format
The command file must be a conventionally encoded ASCII text file (no
multi-byte encodings or Unicode).  The command file may contain
only printable characters, spaces, tabs, carriage returns, and line
feeds.  The command file may not contains characters with the leading
bit set (those above 0x7F).

The command file is parsed in the following phases.
* Conditioning of end-of-line sequences.
  * End-of-line sequences are modified to an internal canonical
    form.
  * Any carriage return (decimal value 13) or linefeed (decimal
    value 10) character is interpreted as part of an
    end-of-line sequence.
  * The program tolerates both properly-formatted Windows
    and properly-formatted Unix end-of-line sequences, even
    mixed.
  * The program will tolerate ill-formed end-of-line
    sequences. The linefeed character (decimal value 10) is used to
    decide how many end-of-line tokens exist.  So, the sequence
    13-10-13-13-13-10 (ill-formed) would be treated as two
    end-of-line tokens.
* Tokenization
  * The program reduces the command file to a series of tokens.
  * As part of the process, comments, quoted strings,
    escape sequences, and continuation characters are processed.
  * Comments
    * A hash character ("#") that is not part of a quoted string
      causes the hash character and the remainder of the line
      to be discarded.  However, if the last character of the line
      is the continuation character (described below), the
      characters up to the continuation character (i.e. the
      intended comment) is discarded, but the line is still
      continued as described below.
    * This means that comments may be:
      * Placed on a line alone, preceded by a hash character.
      * Placed at the end of a line after non-comment material.
      * Placed at the end of a continued line, so that a continued
        line may have several comments, one on each physical
        line of the continued line.
    * A hash character that is within a quoted string is ignored.
  * Special characters
    * These characters are parsed as special characters, and cannot
      appear as part of an unquoted string.  They may, however,
      appear as part of a quoted string.
      * ":"
      * "="
      * ","
      * ";"
      * "+"
      * "*"
      * "^"
      * "&"
      * "|"
      * "("
      * ")"
      * "{"
      * "}"
      * "["
      * "]"
    * Special characters may be tokens by themselves, or may be
      aggregated into tokens involving multiple special characters.
      However, the list of tokens is a parsing rather than
      a lexical analysis matter.  The special characters may not appear
      as part of an unquoted string.
    * The list of special characters was chosen mostly to avoid
      characters that commonly occur in filenames.  Note that:
      * Hash characters cannot occur within an unquoted string unless
        a comment is intended.  A quoted string must be used instead.
      * The forward slash character ("/"),
        the backslash character ("\\"), the underscore character
        ("_"), the hyphen character ("-"), and parenthesis are
        not special characters and don't need to be escaped when
        used in an unquoted string.  However, within a quoted
        string, the backslash character must be escaped as
        described below.
      * Spaces or tabs in a file or path name always require placement
        in a quoted string.
  * Escape sequences
    * Escape sequences can occur only within quoted strings.
      Escape sequences cannot occur outside quoted strings or in
      comments.
    * The defined escape sequences are:
      * "\\\\" to specify a backslash character that
        occurs within a quoted string.  (The
        backslash character is escaped in this way to
        provide for backwards compatibility in the event
        that the list of escape sequences is expanded.)
      * ' \\" ' to specify a double quote character that
        occurs inside a quoted string.  This escape sequence
        may not occur outside of a quoted string.
      * "\\x*hh*"	to specify the byte whose numerical value is given
        by _hh_ interpreted as a hexadecimal number. Any
        letters within _hh_ may be either lower- or upper-case.
      * "\\u*hhhh*" to specify a non-ASCII Unicode code point
        below 10000 hexadecimal.  Any letters within _hhhh_ may be
        either lower- or upper-case.
      * "\U*hhhhhhhh* to specify a non-ASCII Unicode code point
        where h is a hexadecimal digit.  Any letters withi
         _hhhhhhhh_ may be either lower- or upper-case.
    * At present, there are no non-numeric forms for characters
      (for example, "\\t").  This capability may be added later.
  * Unquoted strings
    * Any sequence of these characters, not delimited by double
      quotes, with no whitespace, is
      treated as an unquoted string:
      * Letters, both lower- and upper-case.
      * Digits.
      * Underscores.
      * Hyphens.
      * Forward slashes.
      * Backslashes.
    * Note that the notion of unquoted string will cover
      most (but not all) paths and filenames.
    * An unquoted string may not cross an end-of-line boundary,
      even when a continuation character is used.
  * Quoted strings
    * A quoted string is delimited by double quotes, and is necessary
      to hold a string that doesn't meet the requirements to be
      an unquoted string.
    * Within a quoted string, any double quotes must be
      escaped as ' \\" '
    * An unquoted string may not cross an end-of-line boundary.
  * Processing of continuation characters
    * A backslash as the last character of a line is treated as a
      continuation character.  It causes the backslash to be removed
      and the next line to be appended.
    * As many lines as desired may be combined in this way.
  * Combination of adjacent strings
    * Strings, both unquoted and quoted, that are adjacent with
      no intervening special characters or tokens, are combined.

## Command File Commands

The command file is a collection of commands and arguments, with
one command per line (although continuation characters may be used
to spread a command and its arguments out over more than one physical
line).

The supported commands are listed below.

* *source_template_file_neaten file1, file2, ..., fileN*<br><br>
  Neatens each of the specified source files.  Neatening consists
  of adjusting whitespace and formatting, and doesn't require any
  knowledge of the higher level structure of the files, although
  certain errors may be detected.<br><br>
  Neatening of output files isn't required, as these are automatically
  generated in a neat form.<br><br>
* *filecopy source destination*<br><br>
  Copies from the source file to the destination file.  Any existing
  destination file is overwritten.<br><br>
* *filedelete filename*<br><br>
  Deletes the specified file, if it exists.  If it does not exist,
  no error is generated.<br><br>
* *setparameter parametername parametervalue*<br><br>
  Used to set parameters, to customize the output and behavior.<br><br>
* *setoption optionname optionvalue*<br><br>
  Used to set option values, to customize the output and behavior.<br><br>
* *process_epub special_files sourcefiles*<br><br>
  Processes the files including chapter headings, index, etc.
  This needs to be refined.  The special files (of which there are
  a known number) need to come first, then the variable-length
  list of actual source files.

## List of Named Parameters

TBD.

## List of Named Options

TBD.

## _pub_mft_gen_ Markup Language

### General Structure

The structure generally resembles _LaTeX_, with the following
similarities and differences.

Similarities:

* ASCII input, with everything non-ASCII (umlauts, special
  characters, etc. ASCII-encoded via escape sequences).
* Backslash-escaped commands, with same general syntax.
* Labels and references handled in the same way.
* Equations specified in the same way.

Differences:

* Only a subset of _LaTeX_ commands supported (subset).
* Expanded set of _LaTeX_ commands (superset).
* Some expanded parameter specification on _LaTeX_ commands.
* Equations are specified and hashed to find the right graphic.


### Non-Equation Image Handling

Non-equation images are stored all in the same directory
(this decision may be amended later).

Images filenames are suffixed with _hhhxwww (example, mypet_400x600.jpg),
and this may be used by the program when generating HTML.

### Equation Handling

Three sorts of equations are handled:

* Inline.
  * Handled via a single graphic.
  * Hopefully these are short so that they can be typeset effectively.
* Classic equation
  * Typeset via a table with no borders.
    * Image comes reservoir of images.
    * Equation number is text generated by program.
* Equation array
  * Implemented by breaking it into sub-equations, and using a table
    to combine the images.
  * Equation  number(s) generated by program.

The system for handling equations is:
* For any sort of equation, the _LaTeX_ source is put into
  an internal canonical form, which might mean removing all
  the whitespace or making some other adjustment.  The canonical
  form is designed to ensure that two functionally identical
  equations use the same graphic files.
  The SHA256 hash of the canonical form
  is calculated and characterizes the equation.
* For now, equation graphics are stored in a flat directory
  in both the source and destination file trees.  For the
  destination, this may later be changed to a hashed system with another
  directory level of two added.
* Equation base filenames in the source tree consist of:
  * The entire SHA256 of the canonical form of the equation
    (64 characters), with any letters being lower case.
  * An underscore.
  * The type of equation for which the graphic is intended.
    i = inline, e = classic equation, a = equation array.
  * An underscore.
  * A size indicator.  0 = footnote, 5 = normal text, other sizes
    t0 be determined.
  * An underscore.
  * The dimensions of the equation, width first, example
    100x20.
* There are three possible systems for dealing with the graphic
  files, depending on what an e-reader will tolerate.
  * The files are used with no name change.  The base name
    may be in the neighborhood of 85 characters.
  * The hashes are truncated to the minimum length required for
    uniqueness.
  * The files are numbered sequentially.

### Index Handling

The _LaTeX_ system seems fine.

### Table of Contents Handling

The standard _LaTeX_ mechanisms for markup seem fine.

### List of Figures Handling

The standard _LaTeX_ mechanisms for markup seem fine.

### List of Tables Handling

The standard _LaTeX_ mechanisms for markup seem fine.

### References Handling

The standard _LaTeX_ mechanisms for markup seem fine.
