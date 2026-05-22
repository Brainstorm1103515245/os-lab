#include <stdlib.h>
#include <string.h>

int main(void) {
    char *buf = malloc(100);

    strcpy(buf, "hello");

    /* Forgot to free memory */

    return 0;
}
