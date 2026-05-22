#include <stdio.h>

int sum_array(int *arr, int n) {
    int sum = 0;

    for (int i = 0; i <= n; i++) {
        sum += arr[i];
    }

    return sum;
}

/* Off-by-one bug */

int main(void) {
    int data[5] = {10, 20, 30, 40, 50};

    printf("Sum = %d\n", sum_array(data, 5));

    return 0;
}
