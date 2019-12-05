Quick start
===========
You use `cmake` to build the examples:

```
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
```

Then you can compare the two versions:

```
$ perf stat -e branches,branch-misses ./build/vanilla
     3 004 419 337      branches
       987 260 519      branch-misses             #   32,86% of all branches
      14,594517772 seconds time elapsed
```

```
$ perf stat -e branches,branch-misses ./build/niceapi
     4 013 043 523      branches
           481 089      branch-misses             #    0,01% of all branches
      11,844777995 seconds time elapsed
```


Why
===
Usually, virtual function call in C++ looks like this:

```
mov    (%rdi),%rax
callq  *(%rax)
```

This is problematic because it is impossible for CPU to even guess where the destination is going to be. Modern platform tries to overcome this by implementing _branch prediction_ algorithms which maintains some sort of history of the branch destination (it does `jmp`, `ret` or anything else that takes computated address). This works pretty well but it depends on how good is the algorithm and how big is the history buffer so it's safe to assume that it will behave better on some modern `CORE i7` than on say, `ARMv7`.

Dynamic branch predictor will do nothing for indirect calls when there is no history. _Static predictor_ is a different thing though. Consider this branch:

```
cmp    %rdi,%rax
je     1188 <foo+0x28>
```

There are only two options so CPU can at least try to guess which one will be taken. Usually it's assumed that branch is not taken, i.e. the jump will not occur. We can use that in cases where interfaces are introduced only to simplify writing unit tests and they have only two derived classes - implementation and the mock. The idea can be simply expressed in the pseudo-code:

```
class implementation
class mock

if class of an object == implementation:
    implementation(object).foo()
else
    object.foo()
```

This helps with two things:
 - static prediction will usually be sufficient so we don't depend on branch history
 - compiler can do inlining (which will then enable other imptimizations)

The caveat is that the code will probably be bigger which will cause higher load of the `icache` so as always, **you should measure** your particular system before deploying this solution.
