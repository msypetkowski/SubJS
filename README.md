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
  * [Differences from JavaScript](#differences-from-javascript)
    * [Not supported JS syntax examples](#not-supported-js-syntax-examples)
    * [Not supported keywords list](#not-supported-keywords-list)


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
| 27  | return | |
| 28  | if | |
| 29  | else | |
| 30  | continue | |
| 31  | break | |
| 32  | do | |
| 33  | while | |
| 34  | for | |
| 35  | '=' | |
| 36  | '+=' | |
| 37  | '-=' | |
| 38  | '*=' | |
| 39  | '/=' | |
| 40  | '^=' | |
| 41  | '&=' | |
| 42  | '|=' | |

Syntax
------

#### Starting symbol
Starting symbol is Program.
```C
Program
    = Declaration DeclarationSeparator Program
    | OneLineCommaOperator OneLineCommaOperatorSeparator Program
    | Separator Program
    | Program

    | FunctionExpression Program
    | Symbol ExpressionRest DeclarationSeparator
    | Array ExpressionRest DeclarationSeparator
    | CommaOperator ExpressionRest DeclarationSeparator

    | IfStatement Program
    | While Program
    | DoWhile Program
    | For Program

    | continue Separator
    | break Separator
    | return Expression Separator
Separator
    = ';'
    | '\n'
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

#### OneLineCommaOperator
Should contain at least one ','
```C
OneLineCommaOperatorSeparator
    = '\n'
    | ';'
    | EOF
OneLineCommaOperator
    = Expression ',' OneLineCommaOperatorNext
OneLineCommaOperatorNext
    = Expression ',' OneLineCommaOperatorNext
    | Expression
```

#### Expression

```C
Symbol
    = ident
    | numberConst
    | strConst
Expression
    = Symbol ExpressionRest
    | Array ExpressionRest
    | CommaOperator ExpressionRest
    | FunctionExpression ExpressionRest
ExpressionRest
    = Get
    | Get2
    | Call
    | ArithmeticOp
    | AssignmentOp
    | epsilon

// Expression beginnings (Array or CommaOperator in brackets)
Array
    = '[' ArrayElem
ArrayElem
    = Expression ',' ArrayElem
    | Expression ']'
    | ',' ArrayElem
    | ']'
CommaOperator
    = '(' CommaOperatorNext
CommaOperatorNext
    = Expression ',' CommaOperatorNext
    | Expression ')'

// particular Expression rests
Get
    = '[' Expression ']'
Get2
    = '.' ident
Call
    = '(' CallNextArg
    | '(' ')'
CallNextArg
    = Expression ')'
    | Expression ',' CallNextArg
ArithmeticOp
    = '+' Expression
    | '-' Expression
    | '*' Expression
    | '/' Expression
    | '^' Expression
    | '&' Expression
    | '|' Expression
AssignmentOp
    = '=' Expression
    | '+=' Expression
    | '-=' Expression
    | '*=' Expression
    | '/=' Expression
    | '^=' Expression
    | '&=' Expression
    | '|=' Expression
```

### FunctionExpression
Function expression and definition
```C
FunctionExpression
    = FunctionHead '{' Program '}'
FunctionHead
    = function ident '(' FunctionNextParam
    | function ident '(' ')'
    | function '(' FunctionNextParam
    | function '(' ')'
FunctionNextParam
    = ident ',' FunctionNextParam
    = ident ')'
```

### IfStatement
```C
IfStatement
    = if CommaOperator '{' Program '}'
    | if CommaOperator '{' Program '}' else '{' Program '}'
```

### While
```C
DoWhile
    = do '{' Program '}' while CommaOperator
While
    = while CommaOperator '{' Program '}'
```

### For
```C
For
    = for '(' Declaration ';' Expression ';' Expression ')' '{' program '}'
    | for '(' Expression  ';' Expression ';' Expression ')' '{' program '}'
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
Range of possibilities will be defined in later state of the project.


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

### Examples - OneLineCommaOperator:
Prints 3
```javascript
abc=2, abc++, print(abc)
```

### Examples - Get:
Prints 22
```javascript
print([1,22,3,4][0,2,'test',1])
```

### Examples - Get2:
Prints "test-join"
```javascript
a = ["test", "join"]
a.join("-")
print(a)
```

### Examples - FunctionExpression:
Prints "abc"
```javascript
function fun(){
    return 'abc'
}
fun['call']()
```
Prints "abc"
```javascript
(1,2,3, (function fun(){print('abc')}))()
```

Syntax Error
```javascript
function a(){print("abc")} .abc
```

Syntax Ok
```javascript
1, function a(){print("abc")} .abc
```

### Examples - IfStatement
Prints 2
```javascript
if (1,2,3,0) {print(1)} else {print(2)}
```
Prints 1
```javascript
if (1,2,3,1) {print(1)} else {print(2)}
```


### Complex examples
Prints "abc"
```javascript
aaa = "aaa"
ccc = "ccc"
function abc(param){
    print("abc")
}
abc["aaaccc"] = abc
abc[aaa + (function fff(){return "ccc";}())](123);
```

Differences from JavaScript
===========================

continue, break and return statements not inside loop/function are not an syntax error, but this will cause error during interpretation.

Not supported JS syntax examples
--------------------------------

Defining maps like:
```javascript
a = {'abc':1, 'def':2}
```

Function expressions without '{' '}'
```javascript
function fun()print('abc');
```

If statements without '{' '}'
```javascript
if(1)print(1);
```

Not supported keywords list:
----------------------------
 - case
 - catch
 - class
 - debugger
 - default
 - delete
 - export
 - extends
 - finally
 - for
 - import
 - in
 - instanceof
 - new
 - super
 - switch
 - this
 - throw
 - try
 - typeof
 - void
 - with
 - yield
