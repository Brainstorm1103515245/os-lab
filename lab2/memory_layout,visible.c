#include <stdio.h>
#include <stdlib.h>

int global_var = 42;
int uninit_global;

int main(void) {
    int local_var = 7;

    int *heap_var = malloc(sizeof(int));
    *heap_var = 99;

    printf("Code (main) at:                %p\n", (void *)main);
    printf("Global (initialized):          %p\n", (void *)&global_var);
    printf("Global (uninitialized, BSS):   %p\n", (void *)&uninit_global);
    printf("Heap (malloc):                 %p\n", (void *)heap_var);
    printf("Stack (local variable):        %p\n", (void *)&local_var);

    free(heap_var);

    return 0;
}
