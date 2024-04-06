#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

size_t MSTR(size_t nj, const size_t *uj, size_t u, int k)
{
    size_t sum = 0;

    for (int j = 1; j <= k; j++) {
        size_t diff = (uj[j - 1] - u) * (uj[j - 1] - u);
        sum = sum + (nj * diff);
    }

    size_t result = sum / (k - 1);
    return result;
}

size_t MSE(size_t nj, const size_t *uj, size_t u, int k)
{
    size_t sum = 0, divisor = 1;

    for (int j = 1; j <= k; j++) {
        size_t diff = (uj[j - 1] - u) * (uj[j - 1] - u);
        sum = sum + (nj - 1) * diff;
        divisor = divisor + (nj - k);
    }

    size_t result = sum / divisor;
    return result;
}

int main()
{
    FILE *f1 = fopen("listsort.dat", "r");
    FILE *f2 = fopen("timsort.dat", "r");

    size_t listsort_ele = 0, listsort_sum = 0, uj[2];
    size_t timsort_ele = 0, timsort_sum = 0;
    int count1 = 0, count2 = 0;

    while (fscanf(f1, "%zu\n", &listsort_ele) != EOF) {
        listsort_sum += listsort_ele;
        count1++;
    }

    if (count1 != 0)
        uj[0] = listsort_sum / count1;

    while (fscanf(f2, "%zu\n", &timsort_ele) != EOF) {
        timsort_sum += timsort_ele;
        count2++;
    }

    if (count2 != 0)
        uj[1] = timsort_sum / count2;

    size_t u = (listsort_sum + timsort_sum) / (count1 + count2);
    size_t mstr = MSTR(49999, uj, u, 2);
    size_t mse = MSE(49999, uj, u, 2);

    size_t r = mstr / mse;

    fclose(f2);
    fclose(f1);

    printf("F value: %zu, listsort avg time: %zu, timsort avg time: %zu\n", r,
           uj[0], uj[1]);

    return 0;
}
