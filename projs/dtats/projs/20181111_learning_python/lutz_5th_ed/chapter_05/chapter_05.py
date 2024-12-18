#!/usr/local/bin/python
#----------------------------------------------------------------------------------------------------
#$Header: svn://localhost/dtapublic/projs/dtats/trunk/projs/2018/20181111_learning_python/lutz_5th_ed/chapter_04/chapter_04.py 253 2018-11-12 02:11:09Z dashley $
#----------------------------------------------------------------------------------------------------
#David T. Ashley, 11/2018
#----------------------------------------------------------------------------------------------------
#This file contains notes and experiments as I worked through Mark Lutz's book, "Learning
#Python: Powerful Object-Oriented Programming 5th Edition, Kindle Edition", Chapter 5.
#----------------------------------------------------------------------------------------------------
import sys
import math   #Many math functions available in Python libraries.
import random

#Numeric Types
#-------------

#Numeric Literals
#   o Integers as expected.
#   o Octal:   0o177
#   o Hex:     0x9ff
#   o Binary:  0b101010
#   o Complex Number:  3+4j, 3j

#Floating points are implemented as C "double"s.

#Integer promotion to arbitrary length is automatic in Python 3.X.
#Normal and long integers have been merged.

#Built-in calls hex(), oct(), and bin() convert an integer to that
#representation.
print(hex(41213))
print(oct(41213))
print(bin(41213))

#Complex numbers represented internally with a pair of floating point numbers.
c = 1j
c2 = c*c
print(c)
print(c2)

#Complex numbers may also be crated with the complex() built-in call.
c3 = complex(-4.29, 3.22)
print(c3)

#x//y floor division.
print(49.0/5.0)
print(49.0//5.0)

#Comparison operators may be chained:  x < y < z is the same as
#x < y and y < z.

#Don't fully understand the slice remarks.  May need to review slicing in detail.

#In expressions, mixed types are converted "up".  Python ranks types by simplicity,
#and promotes to the more complex type, similar to C.

#Python provides operators to convert type.
#   int()
#   float()
#and others.

#All Python operators may be overloaded.

#// is integer division.
a=2
b=3
print(2/3)
print(2//3)
num=1/3.0
print('%e'%num)

#"Comparisons:  Normal and Chained"
#a < b < c is shorthand for (a < b) and (b < c).
print(1.1+2.2==3.3)  #False because of binary floating point imprecision.
print(1.1+2.2)

#Division
#--------
#Classic: '/', truncates remainders for integers and keeping fractions for floating point
#         numbers.  Python 3.X keeps remainders for all types.
#Floor:   '//', truncates fractional remainders.  For negatives, goes lower, ala floor().
#True     '/', 3.X addition, keeps remainders for all types, always returns float.

print(math.floor(2.5))
print(math.floor(-2.5))

print(math.trunc(2.5))
print(math.trunc(-2.5))

#For C programmers, must learn to use // in contexts where truncation is wanted.

#Use of long ints automatic.
for i in range(0,100):
    z = 2**i
    print(z)

#Complex Numbers
#---------------
#Core type.
#Represented as two floating point numbers.
#j suffix.
#General form r+ij, i.e. 2.0+9.3j.

#Eval
#----
#eval() function call, treats string as python code.
#Obviously, should be from trusted source.
#Will run comparatively slowly because must be compiled.
print('')
eval('print(' + '2' + '**' + '64)')

#Bitwise Operations
#------------------
#Python supports most of the operations available in C.
#<<, |, &, ^.
#Integer "bit_length" method, which allows one to query number of bits required
#to represent a number's value in binary.

#Built-in Functions
#------------------
#As expected for an interpreted language.  pow() and abs(), for example.

#Standard Library Modules
#------------------------
#Use "import".

print(random.random())
print(random.random())
print(random.random())
print(random.random())
print(random.randint(1, 10))
print(random.randint(1, 10))
print(random.randint(1, 10))
print(random.randint(1, 10))
print(random.randint(1, 10))
print(random.randint(1, 10))
print(random.randint(1, 10))
print(random.randint(1, 10))
print(random.randint(1, 10))
print(random.randint(1, 10))

#Decimal Type
#------------
#'Decimal'
#Like floating-point numbers, but have a fixed number of decimal points.
#Fixed-precision floating-point values.
from decimal import Decimal
print(Decimal('0.1')+Decimal('0.1'))

#Fraction Type
#-------------
#Rational number.  Keeps numerator and denominator explicitly.
#Resides in a module.
from fractions import Fraction
x=Fraction(1,3)
y=Fraction(2,3)
print(x)
print(y)

#Sets
#----
#Unordered collection of unique and immutable objects.
x=set('abcde')
print(x)
for item in x:
   print(item)
#Don't support sequence operations like indexing and slicing.
#Set literals.
y={'dog', 'cat', 'chimpanzee'}
print(y)
#Only immutable types object types may be stored in sets.
#Tuples can be stored in sets.
#Sets are mutable, and so can't be stored in other sets.

#Left off at set comprehensions.


#----------------------------------------------------------------------------------------------------

