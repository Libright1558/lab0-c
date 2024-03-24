#include "queue.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sort_impl.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head_node = malloc(1 * sizeof(struct list_head));
    if (!head_node)
        return NULL;
    INIT_LIST_HEAD(head_node);
    return head_node;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (head) {
        struct list_head *node = NULL, *safe = NULL;
        list_for_each_safe (node, safe, head) {
            q_release_element(container_of(node, element_t, list));
        }
        free(head);
    }
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!s || !*s || !head)
        return false;
    char *value = strdup(s);
    if (!value)
        return false;
    element_t *ele = malloc(sizeof(element_t));
    if (!ele) {
        free(value);
        return false;
    }
    ele->value = value;
    list_add(&ele->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!s || !*s || !head)
        return false;
    char *value = strdup(s);
    if (!value)
        return false;
    element_t *ele = malloc(sizeof(element_t));
    if (!ele) {
        free(value);
        return false;
    }
    ele->value = value;
    list_add_tail(&ele->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *ele = list_first_entry(head, element_t, list);
    if (!(ele->value))
        return NULL;
    if (sp) {
        sp = strncpy(sp, ele->value, bufsize - 1);
        *(sp + bufsize - 1) = '\0';
    }
    list_del(&ele->list);
    return ele;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *ele = list_last_entry(head, element_t, list);
    if (!(ele->value))
        return NULL;
    if (sp) {
        sp = strncpy(sp, ele->value, bufsize - 1);
        *(sp + bufsize - 1) = '\0';
    }
    list_del(&ele->list);
    return ele;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;
    struct list_head *iter = head->next;
    int num = 0;
    while (iter != head) {
        num++;
        iter = iter->next;
    }
    return num;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;
    struct list_head *node = NULL, *safe = NULL;
    int target = q_size(head) / 2;
    list_for_each_safe (node, safe, head) {
        if (target == 0) {
            list_del(node);
            q_release_element(container_of(node, element_t, list));
            break;
        }
        target--;
    }
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    struct list_head *prev = NULL, *curr = NULL;
    for (prev = head->next, curr = prev->next; prev != head && curr != head;
         prev = prev->next->next, curr = curr->next->next) {
        struct list_head *p = prev->prev, *l = curr->next;
        curr->prev = p;
        prev->next = l;
        curr->next = prev;
        prev->prev = curr;
        p->next = curr;
        l->prev = prev;

        // prev and curr pointer switch
        curr = curr->next;
        prev = prev->prev;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    struct list_head *tail_node = head->prev, *head_node = head->next;
    for (; tail_node != head_node;
         head_node = head_node->next, tail_node = tail_node->prev) {
        struct list_head *tl = tail_node->next, *tp = tail_node->prev,
                         *hl = head_node->next, *hp = head_node->prev;

        if (tail_node->prev == head_node && head_node->next == tail_node) {
            tail_node->prev = hp;
            head_node->next = tl;
            tl->prev = head_node;
            hp->next = tail_node;
            tail_node->next = head_node;
            head_node->prev = tail_node;
            break;
        }
        tail_node->next = hl;
        head_node->prev = tp;
        tail_node->prev = hp;
        head_node->next = tl;
        tp->next = head_node;
        tl->prev = head_node;
        hp->next = tail_node;
        hl->prev = tail_node;

        // tail_node and head_node pointer switch
        struct list_head *tmp = head_node;
        head_node = tail_node;
        tail_node = tmp;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head) || list_is_singular(head) || k == 1)
        return;

    struct list_head *l = head->next, *rfix = NULL, *lfix = l,
                     *tail = head->prev, *head_node = head->next;
    int count = 1;
    for (; l != head; l = l->next, count++) {
        if (count != k)
            continue;
        rfix = l;
        struct list_head *lp = lfix->prev, *rp = rfix->next;

        // Cut the list
        lp->next = NULL;
        lfix->prev = head;
        rp->prev = NULL;
        rfix->next = head;
        head->next = lfix;
        head->prev = rfix;

        // Reverse the list
        q_reverse(head);

        // Reposition the pointer
        lfix = head->next;
        rfix = head->prev;
        l = rfix;
        lp->next = lfix;
        lfix->prev = lp;
        rfix->next = rp;
        rp->prev = rfix;

        if (lp == head) {
            head_node = lfix;
        }

        if (rp == head) {
            tail = rfix;
        }
        head->next = head_node;
        head->prev = tail;
        lfix = rp;
        count = 0;
    }
}

int compare(void *priv, const struct list_head *a, const struct list_head *b)
{
    if (a == b) {
        return 0;
    }

    bool is_descend = NULL;

    if (priv) {
        is_descend = *((bool *) priv);
    }

    int res;
    if (!is_descend) {
        res = strcmp(list_entry(a, element_t, list)->value,
                     list_entry(b, element_t, list)->value);
    } else {
        res = strcmp(list_entry(b, element_t, list)->value,
                     list_entry(a, element_t, list)->value);
    }
    return res;
}


/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    timsort(&descend, head, compare);
}
/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head) || list_is_singular(head))
        return 0;
    struct list_head *prev = head->prev, *curr = prev->prev,
                     *delete_node = NULL;
    int count = 1;
    for (; curr != head;) {
        element_t *ele1 = container_of(prev, element_t, list),
                  *ele2 = container_of(curr, element_t, list);
        if (strcmp(ele1->value, ele2->value) > 0) {
            delete_node = curr;
            curr = curr->prev;
            list_del(delete_node);
            q_release_element(container_of(delete_node, element_t, list));
        } else {
            count++;
            prev = curr;
            curr = curr->prev;
        }
    }
    return count;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head) || list_is_singular(head))
        return 0;
    struct list_head *prev = head->prev, *curr = prev->prev,
                     *delete_node = NULL;
    int count = 1;
    for (; curr != head;) {
        element_t *ele1 = container_of(prev, element_t, list),
                  *ele2 = container_of(curr, element_t, list);
        if (strcmp(ele1->value, ele2->value) < 0) {
            delete_node = curr;
            curr = curr->prev;
            list_del(delete_node);
            q_release_element(container_of(delete_node, element_t, list));
        } else {
            count++;
            prev = curr;
            curr = curr->prev;
        }
    }
    return count;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
