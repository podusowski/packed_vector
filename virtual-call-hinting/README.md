Why
===
Usually, virtual function call in C++ looks like this:

```
mov    (%rdi),%rax
callq  *(%rax)
```

This is problematic because it is impossible for CPU to even guess where the destination is going to be. Modern platform tries to overcome this by implementing _branch prediction_ algorithms which maintains some sort of history of the branch destination (it can `jmp`, `ret` or anything else that takes computated address). This works pretty well but it depends on how good is the algorithm and how big is the history buffer so it's safe to assume that it will behave better on some modern `CORE i7` than on say, `ARMv7`.
