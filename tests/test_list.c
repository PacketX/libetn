/**
 * Copyright 2020 PacketX Technology
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <libetn.h>

static int a[] = { 2, 5, 3, 1, 6, 7, 0, 9, 8, 4 };
#define ARR_LEN E_N_ELEMENTS(a)
static int b[] = { 4, 8, 9, 0, 7, 6, 1, 3, 5, 2, 2, 5, 3, 1, 6, 7, 0, 9, 8, 4 };
static int c = 9999;
static int compare_cb(const void *a, const void *b);
static int d[] = { 4, 8, 9, 0, 7, 6, 1, 3, 5, 2, 2, 5, 3, 1, 6, 7, 0, 9, 8, 4, 9999 };
static int e = 1234;
static int f[] = { 4, 8, 9, 0, 7, 6, 1, 3, 5, 2, 2, 5, 3, 1, 6, 7, 0, 9, 8, 4, 1234, 9999 };
static int g[] = { 4, 8, 9, 0, 7, 6, 1, 3, 5, 2, 2, 5, 3, 1, 6, 7, 0, 9, 8, 4, 1234, 9999, 1234 };
static int h[] = { 4, 8, 9, 0, 7, 6, 1, 3, 5, 2, 2, 5, 3, 1, 6, 7, 0, 9, 8, 4, 1234, 1234 };
static int j[] = { 8, 9, 0, 7, 6, 1, 3, 5, 2, 2, 5, 3, 1, 6, 7, 0, 9, 8, 4 };
static int k[] = { 4, 8, 9, 0, 7, 6, 1, 3, 5, 2, 2, 5, 3, 1, 6, 7, 0, 9, 8 };
static int m[] = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9 };
static void *copy_cb(void *data, void *arg);
static void free_cb(void *data);

int main(int argc, char *argv[]) {
    size_t      i;
    e_list_t    *list = NULL, *l, *tmp = NULL, *t;

    for(i = 0 ; i < ARR_LEN ; i++) {
        e_assert_true(list = e_list_append(list, a + i));
    }//end for

    for(i = 0 ; i < ARR_LEN ; i++) {
        e_assert_true(list = e_list_prepend(list, a + i));
    }//end for

    l = list;
    i = 0;
    while(l) {
        e_assert_true(*((int *)(l->data)) == b[i++]);
        l = e_list_next(l);
    }//end while

    e_assert_true(list = e_list_insert_sorted(list, &c, compare_cb));
    l = list;
    i = 0;
    while(l) {
        e_assert_true(*((int *)(l->data)) == d[i++]);
        l = e_list_next(l);
    }//end while

    e_assert_true(list = e_list_insert_before(list, e_list_last(list), &e));
    l = list;
    i = 0;
    while(l) {
        e_assert_true(*((int *)(l->data)) == f[i++]);
        l = e_list_next(l);
    }//end while

    e_assert_true(tmp = e_list_append(tmp, &e));
    e_assert_true(list = e_list_concat(list, tmp));
    l = list;
    i = 0;
    while(l) {
        e_assert_true(*((int *)(l->data)) == g[i++]);
        l = e_list_next(l);
    }//end while

    e_assert_true(list = e_list_remove(list, &c, NULL));
    l = list;
    i = 0;
    while(l) {
        e_assert_true(*((int *)(l->data)) == h[i++]);
        l = e_list_next(l);
    }//end while

    e_assert_true(list = e_list_remove_all(list, &e, NULL));
    l = list;
    i = 0;
    while(l) {
        e_assert_true(*((int *)(l->data)) == b[i++]);
        l = e_list_next(l);
    }//end while

    tmp = e_list_first(list);
    e_assert_true(list = e_list_remove_link(list, tmp));
    l = list;
    i = 0;
    while(l) {
        e_assert_true(*((int *)(l->data)) == j[i++]);
        l = e_list_next(l);
    }//end while
    e_list_free(tmp);

    tmp = e_list_copy_deep(list, copy_cb, NULL, free_cb);
    e_assert_true(tmp);
    l = list;
    t = tmp;
    i = 0;
    while(l) {
        e_assert_true(*((int *)(l->data)) == *((int *)(t->data)));
        l = e_list_next(l);
        t = e_list_next(t);
    }//end while
    e_list_free_full(tmp, free_cb);

    e_assert_true(list = e_list_reverse(list));
    l = list;
    i = 0;
    while(l) {
        e_assert_true(*((int *)(l->data)) == k[i++]);
        l = e_list_next(l);
    }//end while

    e_assert_true(tmp = e_list_nth(list, 4));
    e_assert_true(*((int *)tmp->data) == 7);

    e_assert_true(tmp = e_list_nth_prev(e_list_last(list), 2));
    e_assert_true(*((int *)tmp->data) == 0);

    e_assert_true(tmp = e_list_find(list, (void *)(a + 2)));
    e_assert_true(e_list_position(list, tmp) == 7);
    e_assert_true(e_list_index(list, tmp->data) == 7);

    e_assert_true(e_list_count(list) == 19);

    list = e_list_sort(list, compare_cb);
    l = list;
    i = 0;
    while(l) {
        e_assert_true(*((int *)(l->data)) == m[i++]);
        l = e_list_next(l);
    }//end while

    e_list_free(list);
    return 0;
}//end main


/* ===== private function ===== */
static int compare_cb(const void *a, const void *b) {
    return *((const int *)a) - *((const int *)b);
}//end compare_cb

static void *copy_cb(void *data, void *arg) {
    int *ptr;

    ptr = e_malloc(sizeof(int));
    e_assert_true(ptr);
    *ptr = *((int *)data);
    return ptr;
}//end copy_cb

static void free_cb(void *data) {
    e_free(data);
}//end free_cb
