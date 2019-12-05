Why
===
Usually, virtual function call in C++ looks like this:

```
mov    (%rdi),%rax
callq  *(%rax)
```

This is problematic because it is impossible for CPU to even guess where the destination is going to be. Modern platform tries to overcome this by implementing _branch prediction_ algorithms which maintains some sort of history of the branch destination (it can `jmp`, `ret` or anything else that takes computated address). This works pretty well but it depends on how good is the algorithm and how big is the history buffer so it's safe to assume that it will behave better on some modern `CORE i7` than on say, `ARMv7`.


Quick start
===========
You use `cmake` to build the examples:

```
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
```

Then you can compare the two version:
```
$ perf stat -e branches,branch-misses ./build/vanilla

 Performance counter stats for './build/vanilla':

     3 004 419 337      branches
       987 260 519      branch-misses             #   32,86% of all branches

      14,594517772 seconds time elapsed

      14,593636000 seconds user
       0,000000000 seconds sys

$ perf stat -e branches,branch-misses ./build/niceapi 

 Performance counter stats for './build/niceapi':

     4 013 043 523      branches
           481 089      branch-misses             #    0,01% of all branches

      11,844777995 seconds time elapsed

      11,843340000 seconds user
       0,000000000 seconds sys
```
