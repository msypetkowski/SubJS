#!/bin/env python3

from subprocess import Popen, PIPE, call

executable = './SubJs'
unitTestsExecutable = './test'

print( '''
--------------------\\
Building executables|
--------------------/
''')
call(['scons', 'test'])

print( '''
-------------------\\
Running boost tests|
-------------------/
''')
call(unitTestsExecutable)

print( '''
-------------------\\
Running final tests|
-------------------/
''')

finalTests = [
    ('print(1);', b'1'),
    ('print("a" + "bc");', b'abc'),
    ('print(2+2*2, (1+2)*5, (6*6)*(1*1));', b'6 15 36'),
    ('''
var a=1;
b=a+5;
c=a+b;
a=2;
print(a,b,c);
''', b'2 6 7'),
    ('a=1,2,3; print(a);', b'1'),
    ('a=(1,2,3); print(a);', b'3'),
    ('print((1,2,3)*3);', b'9'),
    ('a=4,b=5,print(a+b);', b'9'),
]

anyFail = False
for code, stdout in finalTests:
    p = Popen([executable,'-c',code], stdin=PIPE, stdout=PIPE, stderr=PIPE)
    stdout1 = p.communicate()[0].strip()
    if (stdout1 != stdout):
        anyFail = True
        print("Wrong answer for code:")
        print(code)
        print("\nExpected:",stdout)
        print("Got:",stdout1)
        print("-----------------------------")
if not anyFail:
    print("All {} final tests passed.".format(len(finalTests)))
