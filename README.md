# SubJS interpreter
----

Project made for TKOM classes.
Its general functionality is to support analysis of obfuscated (probably malware) JavaScript code, by parsing and interpreting a language that is subset of JavaScript.

## Interpretable language specification
----

###  Some of missing JS syntax
Defining maps like:
```javascript
a = {'abc':1, 'def':2}
```

###  Lexical units
- ident
- strConst
- numberConst

Simple atoms:

- ','
- ';'
- '\n'
- EOF
- function
- class
- '('
- ')'
- '['
- ']'
- '}'
- '{'
- var
- let
- const
- new
- '!'
- '^'
- '&'
- '*'
- '-'
- '+'
- '/'
- '|'

### Syntax

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


## Functionality
----
Program can detect execution of a code from string object.
When this situation occur, code (argument of this evaluation) will be saved, and interpretation stopped.
User can resume interpretation.

## Building
----
TODO

## Running
----
TODO

## Architecture
----

There are 3 most important classes:
1. Lexer
2. Parser
3. Interpreter

### Lexer

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

### Parser

Parser is constructed using Lexer object.
Parser has also run() method, that returns the same structure as Lexer.
Only difference is that integer is number of a token (not position in the text).

Parser has methods to access generated tree.

### Interpreter

Interpreter is constructed using Parser object.
TODO



## Interpretable code examples
---

TODO

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
