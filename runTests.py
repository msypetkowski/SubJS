#!/bin/env python3

from subprocess import Popen, PIPE, call

executable = './SubJs'
unitTestsExecutable = './test'

# for veryfing answers
jsExecutable = 'js'

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

    ('print([1]);', b'1'),

    ('print([1,2,3]);', b'1,2,3'),

    ('print([1,2,[1,2]]);', b'1,2,1,2'),

    ('print([,,,,]);', b',,,'),

    ('''
a=[,,1,,2,];
b=[,,1,,2];
print(a);
print(b);
''', b',,1,,2\n,,1,,2'),

    ('print([9,8,7][1]);', b'8'),

    ('''
print([,,,][1]);
print([1,2,3][1,2]);
print([1,22,3,4][0,2,'test',1]);
''', b'undefined\n3\n22'),

    ('print([1,2,3,[9,8,7]][3][1]);', b'8'),

    ('print([1,2,3][1][0]);', b'undefined'),

    ('''
a=[1,2,3];
b=[1,,a,6];
print(b[2][2]);
''', b'3'),

    ('''
a = []["constructor"];
print(a);
''', b'function Array() {\n    [native code]\n}'),

    ('''
b = "constr"+"uctor";
a = []["constructo" +  'r'][b];
print(a);
''', b'function Function() {\n    [native code]\n}'),

    ('''
a=1,2,3;
b=(1,2,3);
c=6, d=7,8;
print(a,b,c,d);
''', b'1 3 6 7'),

    ('''
a="asd";
b=a+"qwe";
a="xyz";
c=a+b;
print(a,b,c);
''', b'xyz asdqwe xyzasdqwe'),

    ('''
a="asd";
a+="qwe";
a+=" xy";
print(a);
''', b'asdqwe xy'),

    ('''
var a="a";
b = a + "b";
b += b;
c = "x";
b += c;
b += c;
c = a;
print(b, c);
''', b'ababxx a'),

    ('''
a="\x61\x3d\x64";
b=a;
print(a, b);
''', b'a=d a=d'),

    ('''
print(1>2);
print(100+5>102);
print(100-60 < 50);
print(100-49 < 50);
''', b'false\ntrue\ntrue\nfalse'),

    ('''
if (1>2)
    print(1);
if (1>0)
    print(2);
if (1<100)
    print(3);
''', b'2\n3'),

    ('''
if ((1+5) > 10)
    print(1);
else
    if (1>2)
        print(2);
    else print(3);
''', b'3'),

    ('''
a=1;
b=5;
if (a+b >= a+1)
    print(b);
if (a+b <= a)
    print(b);
else
    print(a);
''', b'5\n1'),

    ('''
if ([]["constructor"])
    print(1);
else
    print(2);
if ([]["constructo"])
    print(1);
else
    print(2);
''', b'1\n2'),

    ('''
a="qwe";
print(a["charAt"](1));
''', b'w'),

    ('''
a="qwe";
b="char" + "At";
print(a[b](1));
''', b'w'),

    ('''
a="qwe";
b="char" + "At";
print(a[b](1)[b]);
print(a[b](1)[b]());
''', b'function charAt() {\n    [native code]\n}\nw'),

    ('''
a=1; var b=2;
if(a>b) {
    print(a);
} else {
    print(a);
    print(b);
}
''', b'1\n2'),

    ('''
a=1; var b=2;
if(a>b - 10) {
    print(a);
    if (3>2) {
        print("abc");
    }
} else {
    print(a);
    print(b);
}
''', b'1\nabc'),

    ('''
{
print(1);
{ print(2); print(3);
}
}
''', b'1\n2\n3'),

    ('''
a="qwe";
b="split";
print(a[b]);
print(a["split"]());
''', b'function split() {\n    [native code]\n}\nqwe'),

    ('''
print([1,2,"qwe"]);
''', b'1,2,qwe'),

    ('''
print("qwe-asd-zxc"["split"]("-"));
''', b'qwe,asd,zxc'),

    ('''
a=[1,2,3];
print(a["reverse"]);
''', b'function reverse() {\n    [native code]\n}'),

    ('''
a=[1,2,3];
print(a["join"]);
''', b'function join() {\n    [native code]\n}'),

    ('''
print([1,2,3]["reverse"]());
''', b'3,2,1'),

    ('''
    a="reverse";
print([1,2,3]["reverse"]()[a]());
''', b'1,2,3'),

    ('''
    a="join";
print([1,2,3]["reverse"]()[a]());
''', b'3,2,1'),

    ('''
    a="join";
print([1,2,3]["reverse"]()[a]("-=-"));
''', b'3-=-2-=-1'),

    ('''
    b="abc";
    print(b["split"](''));
''', b'a,b,c'),

    ('''
    b="abcdef";
    var a=b["split"]('')["reverse"]()["join"]('');
    print(a);
''', b'fedcba'),


    ('''
function qwe(x) {
    return x;
}
print(qwe(123));
''', b'123'),

    ('''
function foo(a) {
    return a;
}
a=1;
print(foo(a), foo(10));
''', b'1 10'),

    (r'''
print("a\nb\rc", 1);
''', b'a\nb\rc 1'),

    (r'''
a=-10;
print(-1, -1+2, -1*2, a);
''', b'-1 1 -2 -10'),


    (r'''
i=5;
while(i<10) {
    i+=1;
    print(i);
}
''', b'6\n7\n8\n9\n10'),

    (r'''
i=1;
i+=5;
j=2;
i*=j;
print(i,j);
''', b'12 2'),

    (r'''
i=0;
j=2;
while((i+=1)<10)
    j*=2;
print(j);
''', b'1024'),

]

anyFail = False
for code, stdout in finalTests:
    p = Popen([executable,'-c',code], stdin=PIPE, stdout=PIPE, stderr=PIPE)
    stdout1 = p.communicate()[0].strip()

    p2 = Popen([jsExecutable], stdin=PIPE, stdout=PIPE, stderr=PIPE)
    stdout2 = p2.communicate(code.encode())[0].strip()
    if (stdout != stdout2):
        anyFail = True
        print("Invalid test case")
        print("\nReal JavaScript output:",stdout2)
        print("Testcase out is:",stdout)
        continue

    if (stdout1 != stdout):
        anyFail = True
        print("Wrong answer for code:")
        print(code)
        print("\nExpected:",stdout)
        print("Got:",stdout1)
        print("-----------------------------")
if not anyFail:
    print("All {} final tests passed.".format(len(finalTests)))

print( '''
-----------------------\\
Running big final tests|
-----------------------/
''')

# from bigTests import bigTests
# 
# anyFail = False
# for code in bigTests:
#     p = Popen([executable,'-c',code], stdin=PIPE, stdout=PIPE, stderr=PIPE)
#     stdout1 = p.communicate()[0].strip()
# 
#     p2 = Popen([jsExecutable], stdin=PIPE, stdout=PIPE, stderr=PIPE)
#     stdout2 = p2.communicate(code.encode())[0].strip()
# 
#     if (stdout1 != stdout2):
#         anyFail = True
#         print("Wrong answer for code:")
#         print(code)
#         print("\nExpected:",stdout2)
#         print("Got:     ",stdout1)
#         print("-----------------------------")
# if not anyFail:
#     print("All {} big final tests passed.".format(len(bigTests)))
