# BUGS FOUND AND FIXED

## 1. NULL Pointer Dereference — crash.c

Bug:
The program dereferenced a NULL pointer.

How Found:
Used gdb and inspected pointer value using:
print p

Observed:
p = 0x0

Fix:
Avoid dereferencing NULL pointers.


---

## 2. Off-by-One Error — buggy_sum.c

Bug:
Loop used:
i <= n

This accessed memory outside array bounds.

How Found:
Used gdb with breakpoints and printed array indices.

Fix:
Changed:
i <= n

to:
i < n


---

## 3. Memory Leak — leak.c

Bug:
Allocated memory with malloc() but never freed it.

How Found:
Used Valgrind:
valgrind --leak-check=full ./leak

Fix:
Added:
free(buf);


---

## 4. Use-After-Free — use_after_free.c

Bug:
Program accessed memory after calling free().

How Found:
Valgrind reported an invalid read.

Fix:
Removed dereference after free().
