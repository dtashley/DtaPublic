# _polypuba_

## General Description

The program contained in this directory, *polypuba*:
* Translates from a text-processor-agnostic input format (augmented LaTeX, described for brevity
as *agnostic markup*) into 3 formats:
  * EPUB.
  * LaTeX.
  * HTML.
* Performs certain other utility operations useful in publication authorhing.

The mnemonic of the program name is:
* *poly* for many, *pub* for publication; produces many [types of] publication.
* *a* (the suffix) indicates it is the first version of the program.  Subsequent
  versions would be suffixed *b*, *c*, etc.

*polypuba* is a compiled program, written in C++ for Visual Studio, for *Windows* only.  It could likely easily
be ported to *nix, but this has not been attempted.

The general paradigm of the program is:
* A YAML configuration file provides information about the project, such as:
  * Conversion options.
  * File mappings.
* Options on the program command line indicate what is to be done using the
  information in the YAML configuration file (convert, cleanup, parse for anomalies, etc.).
* All errors and anomalies result in a diagnostic message and program termination.

## Licensing

The content used in *polypuba* is provided under the following licenses:
* *polypuba* itself is provided under *The Unlicense*.
* [LibGen](https://github.com/dtashley/LibGen) is provided under *The Unlicense*.
* [LibNum](https://github.com/dtashley/LibNum) is provided under *The Unlicense*.
* [yaml-cpp](https://github.com/jbeder/yaml-cpp/tree/master/src) is provided under *The MIT License*.

## Building and Debugging the Program

*polypuba* was built under *Visual Studio 2026, Community Edition*.  It is a statically-linked
single-threaded console-mode C++ program; and building it, debugging it, or modifying it should be
easy.

Detailed instructions will be provided.

## Invocation of the Program

The program is always invoked as:

```polypuba <options> <command> <path_to_yaml_configuration_file>```

### Options

None defined yet.  Each option will begin with a hyphen, although
there is no need because the required parameters make it
unambiguous which parameters are options.

### Commands

#### *translate*

The program will read the configuration file, and undertake full translation
from agnostic markup into all formats as specified in the configuration file.

#### epubtocindexadd

The program will read the configuration file, and adjust the table of contents in the EPUB version of the
output to include the index.

## Configuration File Format and Options

The configuration file is a YAML file, and specifies:
* The general options to use for the conversion from the 
  agnostic markup to EPUB, LaTeX, or HTML.
* The file binding from the agnostic markup to the EPUB,
  LaTeX, or HTML files.
* The fundamental unit for all 3 targets is the *chapter*.
  * One agnostic markup file, one EPUB file, or one
    HTML file usually corresponds to one chapter.
  * The configuration file format in almost all cases requires
    a 1:1 correspondence between agnostic markup files
    and EPUB, LaTeX, or HTML output files.

### General Options

General options are at the top level of the YAML file (not part of a list or array).

####	*debug_level* (Optional)

Specifies the amount of extra information printed to the console output to
aid in debugging.  If not provided, the default of 0 is used.

| Numerical Value  | Behavior        |
| ------------- | -------------      |
| 0 (default)   | Terse / General    |
| 1             | Semi-detailed      |
| 2             | Detailed           |
| 3             | Extra-detailed     |
| 4             | Maximally-detailed |

Example: ```debug_level: 3```

### *epub_options*, *latex_options*, *html_options* (Optional)

*epub_options*, *latex_options*, and *html_options* are
each lists that specify the options to use for EPUB
conversion, LaTeX conversion, and HTML conversion.
If any of these lists are omitted, the program
will not attempt conversion to the respective
format. If none of these lists are present, the
program will not attempt any conversion.
The elements in the list are divided into two
categories:
- Options that are common to more than one
  conversion format (typically, all).
- Options that are specific to only one conversion
  formats.

#### Common Conversion Options

##### *base_output_directory* (Optional)

The base directory where output of the specific
type should be placed. This path should not include
a trailing path separator.  This may be an absolute or
relative path. If it is relative, it is relative
to the current working directory.

If this option is not included, the output directory
is taken to be the current working directory.

##### *base_input_directory* (Optional)

The base directory where input files are read.
This path should not include
a trailing path separator.  This may be an absolute or
relative path. If it is relative, it is relative
to the working directory from which the program
is launched.

If this option is not included, the input directory
is taken to be the current working directory.

##### *cover_graphic* (Optional)

For EPUB generation, path to the file that should
be used as the cover graphic.
For HTML generation, path to the file that should
be used at the start of the document.

If this option is not provided, 

##### 2.2.1.4 title

The title of the work. A newline in the title is
interpreted as a title in larger font and a
subtitle in smaller font.

##### 2.2.1.5 content_list

A list of the content in the work. Each item of
content should have the following fields.
- `type`
  - `P` = preface
  - `A` = acknowledgements
  - `F` = list of figures
  - `T` = list of tables
  - `C` = chapter
  - `R` = part separator
  - `F` = list of references
  - `I` = index
- `input_file`
  - The file from which to read the input,
    agnostic markup.
- `output_file`
  - The file into which to place the
    corresponding output.

#### 2.2.2 epub-Specific Conversion Options

##### 2.2.2.1 equation_graphics_input_directory

The directory, relative to base_input_directory,
where equation graphics are contained.

##### 2.2.2.2 equation_graphics_output_directory

The directory, relative to base_input_directory,
where equation graphics are stored.

##### 2.2.2.3 figure_graphics_input_directory

##### 2.2.2.4 figure_graphics_output_directory

##### 2.2.2.5 max_output_line_len

#### 2.2.3 LaTeX-Specific Conversion Options

TBD.

#### 2.2.4 HTML-Specific Conversion Options

TBD.

## 3 Agnostic Markup Language Definition

### 3.1 Overview of Agnostic Markup

The agnostic markup language closely mirrors
LaTeX, with these differences.
1. Additional commands are added that have no
analogue in LaTeX.
2. Certain identically-named commands are
different or take a different number or type of
parameters.
The SHA1 cryptographic hash is used throughout for
indexing, especially equations.

### 3.2 Character Sets and Special Characters

### 3.3 Lines and Line Breaks

### 3.4 Channel-Directed Content

The \directedcontent[channels]{} command will
produce corresponding output only to the specified
channels.
\directedcontent fills essentially the same role
as the C preprocessor. It is the first command
processed, before all others.

### 3.5 Labels and References

The LaTeX \label{}, \ref{}, and \pageref{} are
processed.
It is assumed that labels are all of the form
“cint0:spcx9”, where colons are used to separate
identifiers that begin with a letter and may end
with digit(s).
The author is responsible for assuring uniqueness.
\pageref{} is only relevant for a document that
lays out on physical pages.
\secnumlink{}
\secnumtitlelin{}

### 3.6 Math and Equation Typesetting

TBD.

#### 3.6.1 Inline Equations

#### 3.6.2 Explicit Equations

#### 3.6.3 Equation Arrays

### 3.7 Figures

TBD.

### 3.8 Tables

TBD.

### 3.9 Index Entries

TBD.

## Left-Over Content

The content below is yet to be filed and
reorganized.

* Parses the source files of an _ePub_ project.
  * The source files must be annotated with
  special HTML comments
    that this tool interprets.
  * The special HTML comments are similar in
  spirit to the way
    _LaTeX_ works:
      * Sections, equations, figures, tables, and
      bibliography references
        receive text labels.
      * References to sections, equations,
      figures, tables, and
        bibliography references include those
        labels.
      * A mechanism is also provided for index
      entries.
  * This tool:
    * Parses the _ePub_ source code and flags any
    logical anomalies.
    * Neatens the _ePub_ source code (paragraph
    word wrapping,
      indentation).
    * Renumbers sections, equations, figures,
    tables, and bibliography
      references.
    * Updates references to sections, equations,
    figures, tables, and
      bibliography references.
    * Updates the table of contents based on
    collected information.
    * Updates the list of figures based on
    collected information.
    * Updates the List of tables based on
    collected information.
    * Updates the index based on collected
    information.

_epub_ref_maintain_ reads from a command file
(described below)
and performs
the actions described in the command file
sequentially. This
requires the user to know in detail how the
program works, as it would be
possible to create bizarre results with an
incorrect command file.

_epub_ref_maintain_ is at the present time built
only for
_Windows_, and only project files for _Visual
Studio_ are
included in _Git_. However, since the program is
standard C++,
it should port to _Unix_ platforms easily.

## Program Build Requirements

The program is a standard _Visual Studio_ project,
and the project
files are included in _Git_. The program should
build successfully
using the included _Visual Studio_ project files.

However, in order to build, other repositories
must be cloned to specific
relative locations. The following repositories
must be cloned at the
same directory level.

* _DtaPublic_,
https://github.com/dtashley/DtaPublic. (This
repository
  contains the _pub_mft_gen_ program and its
  _Visual Studio_ project
  files. When building _epub_ref_maintain_, the
  repository containing
  it, _DtaPublic_, must be cloned at the same
  level as the two
  additional repositories
  cited below.)
* _LibGen_, https://github.com/dtashley/LibGen.
* _LibNum_, https://github.com/dtashley/LibNum.

## Invocation and Command-Line Options

The program should be invoked using a command line
should be of the form:

*_epub_ref_maintain_ [-v] epub_path
command_file_path*

* The only supported option is *-v*, which
commands extra verbosity.
* *epub_path* is the path to the _ePub_ directory,
including the
  terminating directory path separator. The _ePub_
  directory
  is the top-level directory containing the
  _mimetype_
  file. This string is prepended to each filename
  contained
  in command file commands the specify a file.
  *epub_path* can be either a relative (more
  common) or absolute
  (less common) path. In the case of a relative
  path, it should be
  relative to the working directory at the time
  this program is run.
* *command_file_path* is a mandatory argument, and
is the path to the
  command file (format described below).

## Program Memory Requirements and Error
Terminations

This program buffers all source content except
graphics in memory
and operates on it there. The actual memory
requirements may be
several times the combined size of all source
content, due to the
storage of lexical analysis tokens and parsing
data structures,
in addition to storage of the original file.

In practice, this should not be problematic. A
modern personal
computer should have at least 8 gigabytes of
storage available
for the heap; and a modern personal computer
should also employ
virtual memory.

It is possible, however, for this program to run
out of memory.
If this occurs, the program will terminate and the
error will be
plainly announced.

This program may terminate in the following 4
ways, with the
return codes listed below.

* *Normal termination (return code 0)*
  Program executed cleanly, no errors detected.
* *Out of memory (return code 1)*
  The heap was exhausted.
* *Processing error (return code 2)*
  Undefined references, ill-formatted input, etc.
  The program makes no attempt at error recovery,
  as it is assumed
  that the author using this program wishes to
  keep the source
  material in correct form at all times.  Only
  unused references are warnings; all other
  anomalies
  are errors.
* *Operating system termination (return code
unknown)*
  The program may experience a segmentation fault
  or
  otherwise be forcibly terminated by the
  operating
  system.  (Hopefully, this would never occur.)

## Command File Format

The command file must be a conventionally encoded
ASCII text file (no
multi-byte encodings or Unicode). The command file
may contain
only printable characters, spaces, tabs, carriage
returns, and line
feeds. The command file may not contains
characters with the leading
bit set (those above 0x7F).

The command file is parsed in the following
phases.
* Conditioning of end-of-line sequences.
  * End-of-line sequences are modified to an
  internal canonical
    form.
  * Any carriage return (decimal value 13) or
  linefeed (decimal
    value 10) character is interpreted as part of
    an
    end-of-line sequence.
  * The program tolerates both properly-formatted
  Windows
    and properly-formatted Unix end-of-line
    sequences, even
    mixed.
  * The program will tolerate ill-formed
  end-of-line
    sequences. The linefeed character (decimal
    value 10) is used to
    decide how many end-of-line tokens exist. So,
    the sequence
    13-10-13-13-13-10 (ill-formed) would be
    treated as two
    end-of-line tokens.
* Tokenization
  * The program reduces the command file to a
  series of tokens.
  * As part of the process, comments, quoted
  strings,
    escape sequences, and continuation characters
    are processed.
  * Comments
    * A hash character ("#") that is not part of a
    quoted string
      causes the hash character and the remainder
      of the line
      to be discarded. However, if the last
      character of the line
      is the continuation character (described
      below), the
      characters up to the continuation character
      (i.e. the
      intended comment) is discarded, but the line
      is still
      continued as described below.
    * This means that comments may be:
      * Placed on a line alone, preceded by a hash
      character.
      * Placed at the end of a line after
      non-comment material.
      * Placed at the end of a continued line, so
      that a continued
        line may have several comments, one on
        each physical
        line of the continued line.
    * A hash character that is within a quoted
    string is ignored.
  * Special characters
    * These characters are parsed as special
    characters, and cannot
      appear as part of an unquoted string. They
      may, however,
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
    * Special characters may be tokens by
    themselves, or may be
      aggregated into tokens involving multiple
      special characters.
      However, the list of tokens is a parsing
      rather than
      a lexical analysis matter. The special
      characters may not appear
      as part of an unquoted string.
    * The list of special characters was chosen
    mostly to avoid
      characters that commonly occur in filenames.
      Note that:
      * Hash characters cannot occur within an
      unquoted string unless
        a comment is intended. A quoted string
        must be used instead.
      * The forward slash character ("/"),
        the backslash character ("\\"), the
        underscore character
        ("_"), the hyphen character ("-"), and
        parenthesis are
        not special characters and don't need to
        be escaped when
        used in an unquoted string. However,
        within a quoted
        string, the backslash character must be
        escaped as
        described below.
      * Spaces or tabs in a file or path name
      always require placement
        in a quoted string.
  * Escape sequences
    * Escape sequences can occur only within
    quoted strings.
      Escape sequences cannot occur outside quoted
      strings or in
      comments.
    * The defined escape sequences are:
      * "\\\\" to specify a backslash character
      that
        occurs within a quoted string.  (The
        backslash character is escaped in this way
        to
        provide for backwards compatibility in the
        event
        that the list of escape sequences is
        expanded.)
      * ' \\" ' to specify a double quote
      character that
        occurs inside a quoted string. This escape
        sequence
        may not occur outside of a quoted string.
      * "\\x*hh*" to specify the byte whose
      numerical value is given
        by _hh_ interpreted as a hexadecimal
        number. Any
        letters within _hh_ may be either lower-
        or upper-case.
      * "\\u*hhhh*" to specify a non-ASCII Unicode
      code point
        below 10000 hexadecimal. Any letters
        within _hhhh_ may be
        either lower- or upper-case.
      * "\U*hhhhhhhh* to specify a non-ASCII
      Unicode code point
        where h is a hexadecimal digit. Any
        letters withi
         _hhhhhhhh_ may be either lower- or
         upper-case.
    * At present, there are no non-numeric forms
    for characters
      (for example, "\\t"). This capability may be
      added later.
  * Unquoted strings
    * Any sequence of these characters, not
    delimited by double
      quotes, with no whitespace, is
      treated as an unquoted string:
      * Letters, both lower- and upper-case.
      * Digits.
      * Underscores.
      * Hyphens.
      * Forward slashes.
      * Backslashes.
    * Note that the notion of unquoted string will
    cover
      most (but not all) paths and filenames.
    * An unquoted string may not cross an
    end-of-line boundary,
      even when a continuation character is used.
  * Quoted strings
    * A quoted string is delimited by double
    quotes, and is necessary
      to hold a string that doesn't meet the
      requirements to be
      an unquoted string.
    * Within a quoted string, any double quotes
    must be
      escaped as ' \\" '
    * An unquoted string may not cross an
    end-of-line boundary.
  * Processing of continuation characters
    * A backslash as the last character of a line
    is treated as a
      continuation character. It causes the
      backslash to be removed
      and the next line to be appended.
    * As many lines as desired may be combined in
    this way.
  * Combination of adjacent strings
    * Strings, both unquoted and quoted, that are
    adjacent with
      no intervening special characters or tokens,
      are combined.

## Command File Commands

The command file is a collection of commands and
arguments, with
one command per line (although continuation
characters may be used
to spread a command and its arguments out over
more than one physical
line).

The general flow of commands within a command file
is:
* Clear state (optional if only one book is being
processed).
* Set parameters and options (modifies subsequent
steps).
* Read files.
  * Buffers entire file in computer memory.
  * Lexically analyzes and parses file.
    * Certain formatting and logical structure
    errors will be detected.
  * Calculates numbering of sections, figures,
  tables, and equations and
    assigns value to alphanumeric labels.
  * Records index markings.
  * Note that certain types of files are processed
  differently
    than others. A preface, for example, does not
    cause
    the chapter number to be incremented; whereas
    a chapter
    does cause the chapter number to be
    incremented.
  * Note that some "read" commands corresponding
  to fully generated
    parts of the book do not actually read a file.
    They simply
    reserve space in numbering and in the table of
    contents.
* Check references, resolve references.
  * Internal (computer memory) data structures are
  examined, and
    references are checked and resolved. Errors
    involving duplicate
    definitions or undefined references
    may be detected at this point.
  * Unused items in the bibliography may be
  flagged, but a human
    must remove those if desired.
* Calculate and write files.
  * The following files are calculated and written
  by commands
    in the command file.
    * File manifest.
    * Table of contents (optional).
    * List of figures (optional).
    * List of tables (optional).
    * List of equations (optional).
    * Preface (optional).
    * Chapter files.
    * Index (optional).

The supported commands are listed below.

<table align="center" border="1">
<tr>
   <td>
      <b>Read Command</b>
   </td>
   <td>
      <b>Write Command</b>
   </td>
   <td>
      <b>Attribute</b>
   </td>
   <td>
      <b>Value</b>
   </td>
   <td>
      <b>Description</b>
   </td>
</tr>
<tr>
   <td rowspan="6">
      <i>read_manifest_file manifest_file</i>
   </td>
   <td rowspan="6">
      <i>write_manifest_file</i>
   </td>
   <td>
      <b>Multiple Instances Allowed</b>
   </td>
   <td>
      Y
   </td>
   <td>
      <b>Description</b>
   </td>
</tr>
<tr>
   <td>
      <b>Path from Read Command Used for Write</b>
   </td>
   <td>
      Y
   </td>
   <td>
      <b>Description</b>
   </td>
</tr>
<tr>
   <td>
      <b>Path Matching</b>
   </td>
   <td>
      N
   </td>
   <td>
      <b>Description</b>
   </td>
</tr>
<tr>
   <td>
      <b>Read File Ignored</b>
   </td>
   <td>
      N
   </td>
   <td>
      <b>Description</b>
   </td>
</tr>
  <tr>
     <td>
        <b>Formatting Cleanup</b>
     </td>
     <td>
        Y
     </td>
     <td>
        <b>Description</b>
     </td>
  </tr>
  <tr>
     <td>
        <b>Special HTML Comment Processing</b>
     </td>
     <td>
        N
     </td>
     <td>
        <b>Description</b>
     </td>
  </tr>
</table>

* *setparameter parametername parametervalue*
  Used to set parameters, to customize the output
  and behavior.
* *setoption optionname optionvalue*
  Used to set option values, to customize the
  output and behavior.
* *clear_all_state*
  Clears all internally held data structures, and
  resets all options
  and parameters to their default values.
  This command is provided in case multiple
  overlapping operations
  need to be performed using the same command file
  (processing
  multiple books, for example).
  All internal data structures are cleared and all
  options and
  parameters are at their default values when the
  program is
  invoked, so this command generally does not need
  to be used
  if only one book is being processed.
* *read_manifest_file manifest_file*
  Reads the manifest file.  This program does not
* *read_frontmatter*
* *read_mainmatter*
* *read_backmatter*
  These commands are provided for compatibility
  with the *LaTeX*
  way of thinking. Currently they are accepted but
  unimplemented.
  In *LaTeX*, these commands make the switch
  between the
  numbering system used for things like the
  preface, the numbering
  system used for chapters, and the numbering
  system used
  for things like the any appendices, index, etc.
  If this program is refined to work
  satisfactorily and
  the paradigm seems useful, it may be
  generalized.
  Note that these commands would not read files.
  They would simply
  cause this program to adjust internal state and
  data
  structures.
* *read_part partfile*
  Reads an HTML file containing a part separator
  page. I'm not sure yet how a part should appear
  in the
  table of contents.
* *read_cover_page cover_page_file*
  Reads the cover page file. Canonically, this
  must be
  first in the commands that specify viewable book
  content.
* *read_title_page_file title_page_file*
  Reads the title page file. Canonically, this
  must be
  second in the commands that specify viewable
  book
  content.
* *read_title_page_back_file title_page_back_file*
  Reads the title page back file.  The "title page
  back" (probably the wrong publishing
  nomenclature) contains
  the copyright information, ISBN, etc.
  Canonically, this must
  be third in the commands that specify viewable
  book content.
* *read_table_of_contents table_of_contents_file*
  The table of contents is fully automatically
  generated,
  so the file is not read. However, in the future,
  if the paradigm of embedding information in
  comments proves viable,the paradigm may be
  expanded to allow some customization, in which
  case
  the file read might contain a placeholder for
  the
  automatically-generated content. The place in
  which the
  table of contents appears in _ePub_ is governed
  by
  where this command appears in the command file.
* *read_preface_like_file prefacelikefile*
  Reads a preface-like file (preface,
  acknowledgement, foreword, etc.)
  Multiple commands of this type may appear
  consecutively. The title
  of the preface-like section is specified by HTML
  comments in the file. The ordering within the
  book
  is governed by where this file appears in the
  command file.
* *read_list_of_figures list_of_figures_file*
  *read_list_of_tables list_of_tables_file*
  *read_list_of_equations list_of_equations_file*
  Generation of these files is fully automated,,
  so the files are not read. However, in the
  future,
  if the paradigm of embedding information in
  comments proves viable,the paradigm may be
  expanded to allow some customization, in which
  case
  the files read might contain a placeholders for
  the
  automatically-generated content. The place in
  which the
  these tables appears in _ePub_ is governed by
  where these command appears in the command file.
* *read_source_chapter chapterfile*
  Reads the HTML file as if it is a source
  chapter,
  and builds internal data structures. The
  internal data structures
  include the association between section numbers,
  figures,
  tables, and equations and their alphanumeric
  labels.
  Multiple commands of this type may appear
  consecutively.
  The order in which files are specified is
  significant,
  as the numbering of sections, figures, tables,
  and equations
  depends on the order in which these files are
  read. Generally,
  the order of the files specified mirrors the
  order of chapters
  in the book.
* *read_bibliography bibfile*
  Reads the bibliography (list of references) to
  determine
  what alphanumeric labels exist and what their
  numbering
  should be.
  The logical content of this file is created
  exclusively by the author,
  and is not generated or modified by automation.
  However, the write may correct formatting
  issues.
* *write_manifest*
  Writes the manifest file based on the
  information
  in internal data structures. The manifest file
  path
  is taken from the *read_manifest* command.
* *write_cover_page*
  Writes the cover page file. The cover page file
  path
  is taken from the *read_cover_page* command.
  Any special HTML comments that were in the
  original file are processed, but the file is
  unlikely
  to contain any such comments.
* *write_title_page_file*
  Writes the title page file. The title page file
  path
  is taken from the *read_cover_page* command.
  Any special HTML comments that were in the
  original file are processed, but the file is
  unlikely
  to contain any such comments.
* *write_title_page_back_file
title_page_back_file*
  Write the title page back file. The title page
  back file path
  is taken from the *read_cover_page_back_file*
  command.
  Any special HTML comments that were in the
  original file are processed, but the file is
  unlikely
  to contain any such comments.
* *write_table_of_contents*
  Write the table of contents. The table of
  contents file path
  is taken from the *read_table_of_contents*
  command.
  The table of contents is fully automatically
  generated,
  so this file is written without respect to the
  original
  file contents. This may be changed in the
  future.
* *write_preface_like_file prefacelikefile*
  Writes a preface-like file (preface,
  acknowledgement, foreword, etc.)
  The path must precisely match a path provided in
  a
  *read_preface_like_file* command.
  Any special HTML comments that were in the
  original file are processed.
* *write_list_of_figures*
  *write_list_of_tables*
  *write_list_of_equations*
   Writes these files. Generation is fully
   automated,
   so the input file is not considered. This may
   change
   in the future.
* *write_source_chapter chapterfile*
   Writes chapter file. The path must exactly
   match
   a path in a *read_source_chapter* command.
* *write_bibliography bibfile*
  Writes the bibliography (list of references)
  file.
  The written version will contain only cleanup.
* *write_manifest manifest_file*
  Writes the manifest file based on the
  information
  in internal data structures.

## List of Named Parameters

TBD.

## List of Named Options

TBD.

## _pub_mft_gen_ Markup Language

### General Structure

The structure generally resembles _LaTeX_, with
the following
similarities and differences.

Similarities:

* ASCII input, with everything non-ASCII (umlauts,
special
  characters, etc. ASCII-encoded via escape
  sequences).
* Backslash-escaped commands, with same general
syntax.
* Labels and references handled in the same way.
* Equations specified in the same way.

Differences:

* Only a subset of _LaTeX_ commands supported
(subset).
* Expanded set of _LaTeX_ commands (superset).
* Some expanded parameter specification on _LaTeX_
commands.
* Equations are specified and hashed to find the
right graphic.

### Non-Equation Image Handling

Non-equation images are stored all in the same
directory
(this decision may be amended later).

Images filenames are suffixed with _hhhxwww
(example, mypet_400x600.jpg),
and this may be used by the program when
generating HTML.

### Equation Handling

Three sorts of equations are handled:

* Inline.
  * Handled via a single graphic.
  * Hopefully these are short so that they can be
  typeset effectively.
* Classic equation
  * Typeset via a table with no borders.
    * Image comes reservoir of images.
    * Equation number is text generated by
    program.
* Equation array
  * Implemented by breaking it into sub-equations,
  and using a table
    to combine the images.
  * Equation  number(s) generated by program.

The system for handling equations is:
* For any sort of equation, the _LaTeX_ source is
put into
  an internal canonical form, which might mean
  removing all
  the whitespace or making some other adjustment.
  The canonical
  form is designed to ensure that two functionally
  identical
  equations use the same graphic files.
  The SHA256 hash of the canonical form
  is calculated and characterizes the equation.
* For now, equation graphics are stored in a flat
directory
  in both the source and destination file trees.
  For the
  destination, this may later be changed to a
  hashed system with another
  directory level of two added.
* Equation base filenames in the source tree
consist of:
  * The entire SHA256 of the canonical form of the
  equation
    (64 characters), with any letters being lower
    case.
  * An underscore.
  * The type of equation for which the graphic is
  intended.
    i = inline, e = classic equation, a = equation
    array.
  * An underscore.
  * A size indicator. 0 = footnote, 5 = normal
  text, other sizes
    t0 be determined.
  * An underscore.
  * The dimensions of the equation, width first,
  example
    100x20.
* There are three possible systems for dealing
with the graphic
  files, depending on what an e-reader will
  tolerate.
  * The files are used with no name change. The
  base name
    may be in the neighborhood of 85 characters.
  * The hashes are truncated to the minimum length
  required for
    uniqueness.
  * The files are numbered sequentially.

### Index Handling

The _LaTeX_ system seems fine.

### Table of Contents Handling

The standard _LaTeX_ mechanisms for markup seem
fine.

### List of Figures Handling

The standard _LaTeX_ mechanisms for markup seem
fine.

### List of Tables Handling

The standard _LaTeX_ mechanisms for markup seem
fine.

### References Handling

The standard _LaTeX_ mechanisms for markup seem
fine.
