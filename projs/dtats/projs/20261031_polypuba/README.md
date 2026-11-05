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
* A configuration file provides information about the project, such as:
  * Conversion options.
  * File mappings.
* Options on the program command line indicate what is to be done using the
  information in the configuration file (convert, cleanup, parse for anomalies, etc.).
* All errors and anomalies result in a diagnostic message and program termination.

## Licensing

The content used in *polypuba* is provided under the following licenses:
* *polypuba* itself is provided under *The Unlicense*.
* [LibGen](https://github.com/dtashley/LibGen) is provided under *The Unlicense*.
* [LibNum](https://github.com/dtashley/LibNum) is provided under *The Unlicense*.

## Building and Debugging the Program

*polypuba* was built under *Visual Studio 2026, Community Edition*.  It is a statically-linked
single-threaded console-mode C++ program; and building it, debugging it, or modifying it should be
easy.

Detailed instructions will be provided.

## Invocation of the Program

The program is always invoked as:

```polypuba <options> <command> <path_to_configuration_file>```

### Options

None defined yet.  Each option will begin with a hyphen, although
there is no need because the required parameters make it
unambiguous which parameters are options.

### Commands

#### *translate*

The program will read the configuration file, and undertake full translation
from agnostic markup into all formats as specified in the configuration file.

#### *epubtocindexadd*

The program will read the configuration file, and adjust the table of contents in the EPUB version of the
output to include the index.

## Configuration File Format and Options

The configuration file is a text file, and specifies:
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

The configuration file contains several types of objects.
* Values
    * An unquoted string that contains no whitespace, and consists only of
      letters, numbers, and the underscore character.
    * An integer in the range of -2<sup>31</sup>
      to 2<sup>31</sup>-1, inclusive.
    * A double-quoted string.
      * Within a double-quoted string, double quotes may be escaped with the
        backslash character.
      * Backslashes may be placed in a quoted string by doubling them.
      * Only ordinary printable characters are allowed.
      * More defined escape sequences may be introduced later.
* Keys
  * Identifiers that begin with a letter and contain
    only letters, digits, and the underscore character.  Case is
    significant.
* Key-value pairs, where the key is separated from the value by a colon (":").
  * If key is present without a colon, it is treated as a key-value pair with
    an empty value.  (There is no syntax for specifying an empty value explicitly--it can
    only be inferred from a key with no value.)
* Dictionaries, signified with curly braces ("{" and "}"), which are
  ordered collections of key-value pairs, separated by commas.
* Arrays, signified by square brackets ("[" and "]"), which are ordered collections
  of arrays and dictionaries, separated by commas.

Since a dictionary may contain an array (only as a value, not as a key),
and an array may contain a dictionary, dictionaries and arrays may be nested
arbitrarily deeply.

The configuration file follows the conventions one would expect
for a language consisting of tokens.  Whitespace is not significant,
except within a quoted string.  A quoted string may not span multiple lines.

C- and C++-style comments are supported.

The configuration file is syntactically a dictionary, including the
curly braces.  A sample configuration file is below, showing the major
features.

```
/* Sample configuration file */
{
  debug_level : 2,
  base_input_directory : "",
  text_input_directory : "",
  cover_graphic : "",
  title : ""

  epub_options:
    {
      base_output_directory : ""
    },
  latex_options:
    {
      base_output_directory : ""
    },
  html_options:
    {
      base_output_directory : ""
    },

  things_i_like: {bananas: -3, 
                  coconuts, //Key with no value
                  grapes: "yummy yes"},
  content_list:
  [
    {type: P, inputfile: "testfile"},
    {type: Q, inputfile: "testfile"}
  ]
}
```

The configuration file format described above came about because it was
awkward to integrate the public YAML libraries I could find.

### How Paths are Calculated

All path separators specified in the configuration file must
be the forward slash (Unix style).  They are converted
internally to Windows path separators as necessary.

The *working directory* or *current working directory* is 
an idea that exists under both Windows and *nix
(see https://en.wikipedia.org/wiki/Working_directory).
In this README file, it is denoted *current_working_directory*.
If a specified path is relative, it will be interpreted as with respect
to *current_working_directory*.

The assumption is made that the input files will all be arranged
under a single directory (*base_input_directory*), as a tree;
and similarly for the output files (*base_output_directory*).

Within the *base_input_directory* (or *base_output_directory*), it is
assumed that there will be subdirectories containing specific types
of content (*text_input_directory*, for example).

Furthermore, individual files are specified using options like *input_file*.

Using the example of *input_file*, the path used to try to open the file would be:
* If *base_input_directory* is an absolute path:
  * *base_input_directory* + "/" + *text_input_directory* + "/" + *input_file*.
* If *base_input_directory* is a relative path:
  * *current_working_directory* + "/" + *base_input_directory* + "/" + *text_input_directory* + "/" + *input_file*.

Note that because of path concatenation, there is a large amount of
flexibility in how paths are specified.  It would even possible to
to not specify any path except *input_file*.  In all cases,
*polypuba* will write to the console which path it is attempting to use
to read or write a file, so any unexpected path calculations can be diagnosed.

### General Options

General options are at the top level of the configuration file (part of
the top-level dictionary).

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

##### *base_input_directory* (Optional)

The base directory where input agnostic markup
files are read.
This path should not include
a trailing path separator.  This may be an absolute or
relative path. If it is relative, it is relative
to the working directory from which the program
is launched.

If this option is not included, the input directory
is taken to be the current working directory.

##### *base_output_directory* (Optional)

The base directory where text output of the specific
type should be placed. This path should not include
a trailing path separator.  This may be an absolute or
relative path. If it is relative, it is relative
to the current working directory.

If this option is not included, the output directory
is taken to be the current working directory.

##### *text_input_directory* (Optional)

The base directory where input agnostic markup
files are read.
This path should not include
a trailing path separator.  This may be an absolute or
relative path. If it is relative, it is relative
to the working directory from which the program
is launched.

If this option is not included, the input directory
is taken to be the current working directory.

##### *text_output_directory* (Optional)

The base directory where text output of the specific
type should be placed. This path should not include
a trailing path separator.  This may be an absolute or
relative path. If it is relative, it is relative
to the current working directory.

If this option is not included, the output directory
is taken to be the current working directory.

##### *cover_graphic* (Optional)

For EPUB generation, path to the file that should
be used as the cover graphic.
For HTML generation, path to the file that should
be used at the start of the document.

For LaTeX generation, this option is ignored.

If this option is not provided, EPUB and HTML output both
will not incorporate a cover graphic.

##### *title* (Optional)

The title of the work. A newline in the title is
interpreted as a title in larger font and a
subtitle in smaller font.  There may be at most one newline
in the title.

If this option is not provided, the text "Untitled" is used
as a title.

##### *content_list* (Required)

A list of the content in the work. Each item of
content should have the following fields.
- *type*
  - *P* = preface
  - *A* = acknowledgements
  - *F* = list of figures
  - *T* = list of tables
  - *C* = chapter
  - *R* = part separator
  - *F* = list of references
  - *I* = index
- *input_file*
  - The file from which to read the input,
    agnostic markup.
- *output_file*
  - The file into which to place the
    corresponding output.

#### EPUB-Specific Conversion Options

##### *equation_graphics_input_directory* (Optional)

The directory, relative to base_input_directory,
where equation graphics are contained.

##### *equation_graphics_output_directory* (Optional)

The directory, relative to base_input_directory,
where equation graphics are stored.

##### *figure_graphics_input_directory*



##### *figure_graphics_output_directory*

##### *max_output_line_len*


#### LaTeX-Specific Conversion Options

TBD.

#### HTML-Specific Conversion Options

TBD.

## Agnostic Markup Language Definition

### Overview of Agnostic Markup

The agnostic markup language closely mirrors
LaTeX, with these differences.
1. Additional commands are added that have no
analogue in LaTeX.
2. Certain identically-named commands are
different or take a different number or type of
parameters.
The SHA1 cryptographic hash is used throughout for
indexing, especially equations.

### Character Sets and Special Characters

### Lines and Line Breaks

### Channel-Directed Content

The \directedcontent[channels]{} command will
produce corresponding output only to the specified
channels.
\directedcontent fills essentially the same role
as the C preprocessor. It is the first command
processed, before all others.

### Labels and References

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

### Math and Equation Typesetting

TBD.

#### Inline Equations

#### Explicit Equations

#### Equation Arrays

### Figures

TBD.

### Tables

TBD.

### Index Entries

TBD.

