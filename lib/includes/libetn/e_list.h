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


#ifndef E_LIST_H
#define E_LIST_H

#include <libetn/e_visibility.h>
#include <libetn/e_macros.h>
#include <sys/types.h>

typedef struct e_list_s e_list_t;

typedef void *e_list_copy_cb_t(void *data, void *arg);
typedef int e_list_foreach_cb_t(void *data, void *arg);

struct e_list_s {
    void        *data;
    e_list_t    *next;
    e_list_t    *prev;
};

__BEGIN_DECLS

E_EXPORT e_list_t *e_list_new(void) E_GNUC_WARN_UNUSED_RESULT E_GNUC_MALLOC;
E_EXPORT void e_list_free(e_list_t *list);
E_EXPORT void e_list_free_full(e_list_t *list, e_free_cb_t *free_cb);
E_EXPORT void e_list_free_single(e_list_t *list);
E_EXPORT void e_list_free_single_full(e_list_t *list, e_free_cb_t free_cb);

E_EXPORT e_list_t *e_list_append(e_list_t *list, void *data) E_GNUC_WARN_UNUSED_RESULT;
E_EXPORT e_list_t *e_list_prepend(e_list_t *list, void *data) E_GNUC_WARN_UNUSED_RESULT;
E_EXPORT e_list_t *e_list_insert(e_list_t *list, void *data, int position) E_GNUC_WARN_UNUSED_RESULT;
E_EXPORT e_list_t *e_list_insert_sorted(e_list_t *list, void *data, e_compare_cb_t *compare_cb) E_GNUC_WARN_UNUSED_RESULT E_NONNULL(3);
E_EXPORT e_list_t *e_list_insert_before(e_list_t *list, e_list_t *sibling, void *data) E_GNUC_WARN_UNUSED_RESULT;
E_EXPORT e_list_t *e_list_concat(e_list_t *list1, e_list_t *list2) E_GNUC_WARN_UNUSED_RESULT;

E_EXPORT e_list_t *e_list_remove(e_list_t *list, const void *data, e_free_cb_t *free_cb) E_GNUC_WARN_UNUSED_RESULT;
E_EXPORT e_list_t *e_list_remove_all(e_list_t *list, const void *data, e_free_cb_t *free_cb) E_GNUC_WARN_UNUSED_RESULT;
E_EXPORT e_list_t *e_list_remove_link(e_list_t *list, e_list_t *link) E_GNUC_WARN_UNUSED_RESULT;
E_EXPORT e_list_t *e_list_delete_link(e_list_t *list, e_list_t *link, e_free_cb_t *free_cb) E_GNUC_WARN_UNUSED_RESULT;

E_EXPORT e_list_t *e_list_copy_deep(e_list_t *list, e_list_copy_cb_t *copy_cb, void *arg, e_free_cb_t *free_cb) E_GNUC_WARN_UNUSED_RESULT;

E_EXPORT e_list_t *e_list_reverse(e_list_t *list) E_GNUC_WARN_UNUSED_RESULT;

E_EXPORT e_list_t *e_list_nth(e_list_t *list, uint n);
E_EXPORT e_list_t *e_list_nth_prev(e_list_t *list, uint n);
E_EXPORT void *e_list_nth_data(e_list_t *list, uint n);
E_EXPORT void *e_list_nth_data_prev(e_list_t *list, uint n);

E_EXPORT e_list_t *e_list_find(e_list_t *list, const void *data);
E_EXPORT int e_list_position(e_list_t *list, e_list_t *llink);
E_EXPORT int e_list_index(e_list_t *list, const void *data);

E_EXPORT e_list_t *e_list_first(e_list_t *list);
E_EXPORT e_list_t *e_list_last(e_list_t *list);

E_EXPORT size_t e_list_count(e_list_t *list);

E_EXPORT int e_list_foreach(e_list_t *list, e_list_foreach_cb_t *foreach_cb, void *arg) E_NONNULL(2);

E_EXPORT e_list_t *e_list_sort(e_list_t *list, e_compare_cb_t *compare_cb) E_GNUC_WARN_UNUSED_RESULT E_NONNULL(2);

#define e_list_previous(list)   ((list) ? (((e_list_t *)(list))->prev) : NULL)
#define e_list_next(list)       ((list) ? (((e_list_t *)(list))->next) : NULL)

__END_DECLS

#endif /* E_LIST_H */
