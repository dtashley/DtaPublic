#!/usr/local/bin/python
#----------------------------------------------------------------------------------------------------
#$Header: svn://localhost/dtapublic/projs/dtats/trunk/projs/2018/20181111_learning_python/lutz_5th_ed/chapter_04/chapter_04.py 253 2018-11-12 02:11:09Z dashley $
#----------------------------------------------------------------------------------------------------
#David T. Ashley, 11/2018
#----------------------------------------------------------------------------------------------------
#This file contains notes and experiments as I worked through Mark Lutz's book, "Learning
#Python: Powerful Object-Oriented Programming 5th Edition, Kindle Edition", Chapter 4.
#----------------------------------------------------------------------------------------------------
import sys
import math   #Many math functions available in Python libraries.

#Math functions built in.
x = math.pi
print(x)
y = math.sqrt(x)
print(y)

#String assignment.
s = 'Now is the time for all good men to come to the aid of their country.'

#Reference to single character.
print(s[3])

#Reference to character range, called slices.  Second
#limit is < limit, i.e. open interval on the right.
print(s[3:7])

#Can use one greater on the right to get full string.
print(s[0:len(s)])

#Can use negative indices to go backwards in a string.
print(s[3:-3])

#String concatenation.
t = s+s
print(t)

#String repetition.
u = s*10
print(u)

#Strings are immutable (can't be changed in place).  Illegal assignment below:
#s[0] = 'q'
#Must instead construct new string and assign.

#list:  expands to list.
l=list(s)
print(l)

#Can omit left or right limits, in which case defaults to first and last elements.
print(s[:3])
print(s[3:])
print(s[:])

#Byte arrays.  Literals are b'chars'.  Created with bytearray().
ba=bytearray(b'spam')
ba.extend(b'eggs')
print(ba)
bad=ba.decode()
print(bad)

#Determining which system one is running on.
print(sys.platform)

#Arbitrary-precision arithmetic:  integers grow automatically.
print(2**100)

#String repetition.
x = 'Spam! '
print(x*8)

#Pausing to accept the <ENTER> key--useful when using an icon to launch
#a script.
#input()

#String concatenation, comment.
print('The bright side of' + ' Life ...') #Comment.

#Built in methods.  Conversion of strings to lists.
s='Spam'
print(s.find('pa'))
print(s.replace('pa', 'XYZ'))
line='aaa,bbb,ccccc,dd'
print(line.split(','))
print(s.upper())
print(s.isalpha())
print(s.isdigit())

#Standard string operations that may be useful.
line='aaa,bbb,ccccc,dd'
print(line.rstrip())

#Combine two operations.  Note that rstrip() runs first, because Python does
#this from left to right.  This is counterintuitive, at least to me.
print(line.rstrip().split(','))

#Formatting.  This seems to behave essentially like sprintf() in C.
print('%s, eggs, and %s' % ('spam', 'SPAM!'))
print('{0}, eggs, and {1}'.format('spam', 'SPAM!'))
print('{1}, eggs, and {0}'.format('spam', 'SPAM!'))
print('{1}, eggs, and {1}'.format('spam', 'SPAM!'))
print('{:,.2f}'.format(296999.2567))
print('%.2f | %+05d' % (3.14159, -42))

#Find out what methods are available for strings.  The names with
#double underscores are for operator overloading, discussed later.
#In general, the names with leading or trailing double underscores
#have to do with implementation of Python internals.
q='Now is the time'
print(dir(q))

#Python has built-in help for methods.
print(help(q.replace))

#Can also get help for a data type (string).
#print(help(str))

#Can encode string characters as hex constants.
s='A\nB\tC'
print(len(s))

#ord() returns the decimal value of a character.
print(ord('\n'))

#Double quotes allowed in string constants.
#Triple quotes(single or double) allowed.  This allows multi-line
#text to be included.  Line breaks in a constant occur as well.
triple_quoted_string = '''Now is the time
for all good men to come to the aid of their
country, and the line break should be next
line break shold be before'''
print(triple_quoted_string)
triple_quoted_string2 = """Now is the time
for all good men to come to the aid of their
country, and the line break should be next
line break shold be before"""
print(triple_quoted_string2)

#Can add triple quotes on both sides to disable a block of code.

#Raw string literals prefixed with "r":  turns off backslash escape mechanism.
raw_literal=r'C:\text\new'
print(raw_literal)

#Unicode is supported, and literals prefixed with "u".  Normal string functions
#handle Unicode as well as ASCII.  Need to review that later.
unicode_string=u'sp\u00c4m'
print(unicode_string)

#Book made the statement that ASCII is a special case of Unicode--need to review that statement
#and Unicode encoding and be sure I understand it.

#There is a distinct "bytes" string type for binary values (like raw file contents,
#I'd guess).  Prefix is "b".
binary_string=b'a\x01c'
print(binary_string)
print(len(binary_string))

#"re" is the module for pattern matching.  Can do regular expression searching
#and so on.

#Lists
#-----
#Same sequence operations for strings apply.
#   len()
#   indexing and slicing
#   repetition
#Can freely mix types.
#Can add and remove elements (append, pop, other operations).
#Can sort and reverse.
#Can index off the end or assign off the end.  (To assign off the end, must
#use append method or similar.  Can't just assign.)
#Python allows nesting.  (A list of lists, or a list of dictionaries, each of which
#contain other lists.)
#A list of lists can be double-indexed, i.e. list[1][2].
#
#One way to think about matrix[m][n] is that matrix[m] grabs the entire row "m"
#(a list itself), then [n] indexes into that list.
matrix=[[1,2,3],[4,5,6],[7,8,9]]
print(matrix)
print('Element[0][0] is: ' + str(matrix[0][0]))
print('Element[1][2] is: ' + str(matrix[1][2]))

#Comprehensions
#--------------
#List comprehension / list comprehension expression:
#   o Derive from set notation.
#   o Build a new list by running an expression on each item in a sequence, one at a time, from
#     left to right.
#   o Consist of an expression and a looping construct that share a variable name.
#In the code below, each element of matrix is obtained (a row), then for each row column[1]
#is extracted, and this is placed in a list.
col2 = [row[1] for row in matrix]
print(col2)
diagonal = [matrix[i][i] for i in [0, 1, 2]]
print(diagonal)

#Dictionaries
#------------
#Mappings from key to value.
#Mutable:  can be changed in place and grow and shrink on demand.
#Assignments with new keys create the keys (unlike lists, where this is forbidden).
d={'food': 'Spam', 'quantity': 4, 'color': 'pink'}
print(d)
#zip function can be used to combine parallel arrays into key:value dictionary.
#Dictionary entries can be accessed as dict_name[key].
#The nested types in Python seem to be nearly as flexible as a tree in C (formed with malloc() and
#friends.  Dictionaries and lists have variable numbers of elements and can be nested arbitrarily
#unevenly ... effectively a tree.
#.keys() method:  gets the keys of a dictionary.

#For loops
#---------
#for loop is an iterator.  Will work on any iterable object.

#Tuples
#------
# o Immutable sequences.
# o Coded in parenthesis.
# o Can be arbitrarily nested.
t=(1, 2, 3, 4)
print(t)
print(len(t))
print(t.index(4))
print(t.count(4))
#Tuples cannot be changed once created (immutable).

#Files
#-----
#Created using open() and similar functions.
#Functions:
#   f=open()
#   f.read()
#   f.write()
#   f.close()
#File's contents are always a string.
#   for line in open('fname'): print(line)

#Binary Files
#------------
#Python "struct" module:  can both create and unpack packed binary data.
#Can open files with "b" option and so use binary strings.

#Unicode in Files
#----------------
#Can pass in an encoding name.  Will read and write unicode as text.

#Other Objects
#-------------
#Python can deal with pipes, FIFO's, sockets, etc. as well.

#Sets
#----
#Now supported in Python.
#Include intersection, union, etc.

#Other Numerical Types
#---------------------
#Decimal numbers.
#Fractions (i.e. rational numbers).

#Booleans
#--------
#Can only be False or True.

#None
#----
#Placeholder, commonly used to initialize names and objects.

#type()
#------
#Gives the type of another object.

#User-Defined Classes
#--------------------
#Supported.
#----------------------------------------------------------------------------------------------------

