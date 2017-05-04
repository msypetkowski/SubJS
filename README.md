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

#### No keyword atoms:

| Symbol        | Description   |
| :------------ |:-------------:|
| ident         | variable, class of function name |
| constant      | constants like "string" or 100 or 123.0021 |

#### Keyword atoms:

| no  | Symbol        |
|:--- | :------------ |
| 0   | '++' |
| 1   | '--' |
| 2   | '.' |
| 3   | ',' |
| 4   | ';' |
| 5   | '__unused_symbol' |
| 6   | '$' |
| 7   | function |
| 8   | class |
| 9   | '(' |
| 10  | ')' |
| 11  | '[' |
| 12  | ']' |
| 13  | '}' |
| 14  | '{' |
| 15  | var |
| 16  | let |
| 17  | const |
| 18  | new |
| 19  | '!' |
| 20  | '^' |
| 21  | '&' |
| 22  | '*' |
| 23  | '-' |
| 24  | '+' |
| 25  | '/' |
| 26  | '\|' |
| 27  | return |
| 28  | if |
| 29  | else |
| 30  | continue |
| 31  | break |
| 32  | do |
| 33  | while |
| 34  | for |
| 35  | '=' |
| 36  | '+=' |
| 37  | '-=' |
| 38  | '*=' |
| 39  | '/=' |
| 40  | '^=' |
| 41  | '&=' |
| 42  | '\|=' |
| 43  | try |
| 44  | catch |
| 45  | finally |
| 46  | '==' |
| 47  | '!=' |
| 48  | '&&' |
| 49  | '||' |
| 50  | '<' |
| 51  | '<=' |
| 52  | '>' |
| 53  | '>=' |
| 54  | '%' |
| 55  | '%=' |
| 56  | '?' |
| 57  | ':' |
| 58  | '<<' |
| 59  | '>>' |
| 60  | '>>>' |

Syntax
------

#### Starting symbol
Starting symbol is Program.
```C
Program
    = { Element }
    | epsilon

Element
    = Declaration
    | OneLineCommaOperator OneLineCommaOperatorSeparator

    | Symbol ExpressionRest
    | Array ExpressionRest
    | CommaOperator ExpressionRest
    | new Expression
    | FunctionExpression

    | IfStatement
    | While
    | DoWhile
    | For
    | Try

    | continue
    | break
    | return Expression

    | ';'
```

#### Declaration
```C
DeclarationType
    = var
    | let
    | const
Declaration
    = DeclarationType DeclarationElem { ',' DeclarationElem }
DeclarationElem
    = ident '=' AssignmentExpression
    | ident
```

#### OneLineCommaOperator
Should contain at least one ','
```C
OneLineCommaOperatorSeparator
    = epsilon
    | ';'
    | '$'
OneLineCommaOperator
    = Expression ',' OneLineCommaOperatorNext
OneLineCommaOperatorNext
    = Expression ',' OneLineCommaOperatorNext
    | Expression
```

#### Expression

```C
Expression
    = AssignmentExpression
    | AssignmentExpression "," Expression

AssignmentExpression
    = ConditionalExpression
    | ConditionalExpression AssignmentOperator AssignmentExpression

ConditionalExpression
    = OrExpression
    | OrExpression "?" AssignmentExpression ":" AssignmentExpression

OrExpression
    = AndExpression
    | AndExpression "||" OrExpression

AndExpression
    = BitwiseOrExpression
    | BitwiseOrExpression "&&" AndExpression

BitwiseOrExpression
    = BitwiseXorExpression
    | BitwiseXorExpression "|" BitwiseOrExpression

BitwiseXorExpression
    = BitwiseAndExpression
    | BitwiseAndExpression "^" BitwiseXorExpression

BitwiseAndExpression
    = EqualityExpression
    | EqualityExpression "&" BitwiseAndExpression

EqualityExpression
    = RelationalExpression
    | RelationalExpression EqualityOperator EqualityExpression

RelationalExpression
    = ShiftExpression
    | RelationalExpression RelationalOperator ShiftExpression

ShiftExpression
    = AdditiveExpression
    | AdditiveExpression ShiftOperator ShiftExpression

AdditiveExpression
    = MultiplicativeExpression
    | MultiplicativeExpression AdditiveOperator AdditiveExpression

MultiplicativeExpression
    = UnaryExpression
    | UnaryExpression MultiplicativeOperator MultiplicativeExpression

UnaryExpression
    = MemberExpression
    | UnaryOperator UnaryExpression
    | '-' UnaryExpression
    | IncrementOperator MemberExpression // TODO
    | MemberExpression IncrementOperator
    | new Constructor // TODO
    | delete MemberExpression

// TODO
Constructor
    = this . ConstructorCall
    | ConstructorCall

ConstructorCall
    = ident
    | ident ( ArgumentListOpt )
    | ident . ConstructorCall

MemberExpression
    = PrimaryExpression
    | PrimaryExpression '.' MemberExpression
    | PrimaryExpression '[' Expression ']'
    | PrimaryExpression '(' ArgumentListOpt ')'

ArgumentListOpt
    = epsilon
    | ArgumentList

ArgumentList
    = AssignmentExpression
    | AssignmentExpression ',' ArgumentList

PrimaryExpression
    = '(' Expression ')'
    | ident
    | constant
    | false
    | true
    | null
    | this

AssignmentOperator
    = '='
    | '+='
    | '-='
    | '*='
    | '/='
    | '^='
    | '&='
    | '|='
    | '%='

EqualityOperator
    = '==' Expression
    | '!=' Expression

RelationalOperator
    = '<' Expression
    | '<=' Expression
    | '>' Expression
    | '>=' Expression

ShiftOperator
    = '<<'
    | '>>'
    | '>>>'

AdditiveOperator
    = '+'
    | '-'

MultiplicativeOperator
    = '*'
    | '/'

UnaryOperator
    = '--'
    = '++'
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

### Try, Catch, Finally

```C
Try
    = try '{' Program '}' Catch
Catch
    = catch '(' ident ')' '{' Program '}' Catch
    | Finally
    | epsilon
Finally
    = finally '{' Program '}'

```

Building
========
Compilation:
```bash
scons
```
Clean:
```bash
scons -c
```

Running
=======
See:
```bash
./SubJs -h
```

Architecture
============

There are 3 most important classes:
1. Lexer
2. Parser
3. Interpreter

Lexer
-----

Lexer is constructed using string object (code).

Lexer generates 3 types of tokens:
- symbol (like variable name)
- constant (like 123 or "text")
- keyword (like "for", "if", "+=" ...)

Parser
------

Parser is constructed using Lexer object.
Parser provides methods to access generated tree.
Parser is LL (left-left) type.

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

continue, break and return statements not inside loop/function are not an syntax error, but will cause error during interpretation.

Unary operators '--' and '++' works only postfix.

Functions can be used in OneLineCommaOperator (see syntax).

There is no endline token. For example this code:
```javascript
a = 1 b = 1
```
Will work in this interpreter, but it doesn't run in JavaScript.
This difference simplifies the syntax. Let's look at these examples:
```javascript
a
=
1
```
```javascript
a
[0]
```
These are valid JavaScript examples.
In the second example [0] there is actually get(0) operator, not an list.

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
 - class
 - debugger
 - default
 - delete
 - export
 - extends
 - import
 - in
 - instanceof
 - super
 - switch
 - this
 - throw
 - typeof
 - void
 - with
 - yield
