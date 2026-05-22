#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int *p = malloc(sizeof(int));

    *p = 42;

    free(p);

    printf("%d\n", *p);   /* Bug: use-after-free */

    return 0;
}
