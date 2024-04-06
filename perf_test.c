#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "harness.h"
#include "queue.h"
#include "random.h"

void set_cautious_mode(bool cautious);

#define MIN_RANDSTR_LEN 5
#define MAX_RANDSTR_LEN 10
static const char charset[] = "abcdefghijklmnopqrstuvwxyz";

static void fill_rand_string(char *buf, size_t buf_size)
{
    size_t len = 0;
    while (len < MIN_RANDSTR_LEN)
        len = rand() % buf_size;

    uint64_t randstr_buf_64[MAX_RANDSTR_LEN] = {0};
    randombytes((uint8_t *) randstr_buf_64, len * sizeof(uint64_t));
    for (size_t n = 0; n < len; n++)
        buf[n] = charset[randstr_buf_64[n] % (sizeof(charset) - 1)];
    buf[len] = '\0';
}

int main()
{
    for (int round = 0; round < 50000; round++) {
        struct list_head *head1 = q_new(), *head2 = q_new();
        for (int k = 0; k < 50000; k++) {
            char randstr_buf[MAX_RANDSTR_LEN];
            fill_rand_string(randstr_buf, sizeof(randstr_buf));
            q_insert_tail(head1, randstr_buf);
            q_insert_tail(head2, randstr_buf);
        }
        q_listsort(head1, false);
        q_sort(head2, false);
        set_cautious_mode(false);
        q_free(head1);
        q_free(head2);
        set_cautious_mode(true);
    }
    return 0;
}
