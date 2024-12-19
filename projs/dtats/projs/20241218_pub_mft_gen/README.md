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

The hash character ('#') is used to delimit comments. When a hash
character appears on a line, the rest of the line is ignored.

The backslash character ('\\') is used as a line continuation
character.  When a backslash character is the last character on a line,
the following line is considered part of the same line.  Multiple
backslash characters can be used to spread a logical
line across as many physical lines as desired.
