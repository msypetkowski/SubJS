SubJS interpreter
=================

Its general functionality is to support analysis of obfuscated (probably malware) JavaScript code, by:
 - parsing and interpreting a language that is subset of JavaScript
 - providing mechanism for execution interrupting when there occur next level of obfuscated code execution

Program can detect execution of a code from string object.
When this situation occur, code (argument of this evaluation) will be saved, and interpretation stopped.
User can resume interpretation.


Table of contents
=================

  * [SubJs interpreter](#subjs-interpreter)
  * [Table of contents](#table-of-contents)
  * [Interpretable language specification](#interpretable-language-specification)
    * [Lexical units](#lexical-units)
    * [Syntax](#syntax)
  * [Building](#building)
  * [Running](#running)
  * [Architecture](#architecture)
    * [Lexer](#lexer)
    * [Parser](#parser)
    * [Interpreter](#interpreter)
  * [Examples](#examples)
    * [Interpretable code examples](#interpretable-code-examples)
    * [Missing JS syntax examples](#missing-js-syntax-examples)


Interpretable language specification
====================================

Lexical units
-------------

| no  | Symbol        | Description   |
|:--- | :------------ |:-------------:|
| 0   | ident         | variable, class of function name |
| 1   | strConst      | constants like "string" |
| 2   | numberConst   | number |
| 3   | ',' | |
| 4   | ';' | |
| 5   | '\n' | |
| 6   | EOF | |
| 7   | function | |
| 8   | class | |
| 9   | '(' | |
| 10  | ')' | |
| 11  | '[' | |
| 12  | ']' | |
| 13  | '}' | |
| 14   | '{' | |
| 15  | var | |
| 16  | let | |
| 17  | const | |
| 18  | new | |
| 19  | '!' | |
| 20  | '^' | |
| 21  | '&' | |
| 22  | '*' | |
| 23  | '-' | |
| 24  | '+' | |
| 25  | '/' | |
| 26  | '|' | |

Syntax
------

#### Starting symbol
```C
Program
    = Declaration DeclarationSeparator Program
```

#### Declaration
```C
DeclarationSeparator
    = '\n'
    | ';'
    | EOF

DeclarationType
    = var
    | let
    | const
Declaration
    = DeclarationType DeclarationElem DeclarationNext
DeclarationElem
    = ident '=' Expression
    | ident
DeclarationNext
    = ',' DeclarationElem | epsilon
```

#### Expression

```C
Expression
    = ident ExpressionRest
    | Array ExpressionRest
    | CommaOperator ExpressionRest
ExpressionRest
    = Get
    | Get2
    | Assignment
    | Call
    | Add
    | Sub
    | Mul
    | Div
    | Xor
    | And
    | Or
    | epsilon

// Expression beginnings (Array or CommaOperator in brackets)
Array
    = '[' ArrayElem
ArrayElem
    = Expression ',' ArrayElem
    | Expression ']'
    | ',' ArrayElem
    | ']'
// TODO:
// CommaOperator

// particular Expression rests
Get
    = '[' Expression ']'
Get2
    = '.' ident
Assignment
    = '=' Expression
Call
    = '(' CallNextArg
CallNextArg
    = Expression ')'
    | Expression ',' CallNextArg
    | ')'
Add
    = '+' Expression
Sub
    = '-' Expression
Mul
    = '*' Expression
Div
    = '/' Expression
Xor
    = '^' Expression
And
    = '&' Expression
Or
    = '|' Expression
```

Building
========
TODO

Running
=======
TODO

Architecture
============

There are 3 most important classes:
1. Lexer
2. Parser
3. Interpreter

Lexer
-----

Lexer is constructed using string object (code).
It has run() method, that returns
```C++
pair<bool, vector<pair<int,string> > >
```
bool is True if analysis succeed, else False.
If analysis succeed vector is empty.
Integer in pair from vector, points position in text where error occured.
String is error message associated with given position.

Lexer has methods that allow access to generated tokens.
Token is integer number (numbers all possible tokens - see Interpretable languate specification)
    or pair of integer numbers (second integer is index in symbol table)

Lexer generates 4 symbol tables:
- for identificators
- for constant strings
- for constant integers
- for constant floating-point numbers

Parser
------

Parser is constructed using Lexer object.
Parser has also run() method, that returns the same structure as Lexer.
Only difference is that integer is number of a token (not position in the text).

Parser has methods to access generated tree.

Interpreter
-----------

Interpreter is constructed using Parser object.
TODO


Examples
========

Interpretable code examples
---------------------------

### Examples - Declaration:
```javascript
var a
```
```javascript
var a,b,c,d
```
```javascript
var a=1,b,c='test',d
```

### Examples - Get:
Returns 22
```javascript
[1,22,3,4][0,2,'test',1]
```

Missing JS syntax examples
--------------------------

Defining maps like:
```javascript
a = {'abc':1, 'def':2}
```
