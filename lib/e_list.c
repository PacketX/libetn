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


#include <libetn/e_list.h>
#include <libetn/e_err.h>
#include <libetn/e_mem.h>

static inline e_list_t *e_list_sort_real(e_list_t *list, e_compare_cb_t *compare_cb);
static inline e_list_t *e_list_sort_merge(e_list_t *l1, e_list_t *l2, e_compare_cb_t *compare_cb);

e_list_t *e_list_new(void) {
    e_list_t *list;

    list = e_calloc(1, sizeof(e_list_t));
    if(E_UNLIKELY(!list)) {
        return NULL;
    }//end if

    return list;
}//end e_list_new

void e_list_free(e_list_t *list) {
    return e_list_free_full(list, NULL);
}//end e_list_free

void e_list_free_full(e_list_t *list, e_free_cb_t *free_cb) {
    e_list_t *l;

    l = list;
    while(E_LIKELY(l)) {
        if(free_cb) {
            free_cb(l->data);
        }//end if
        list = l->next;
        e_free(l);
        l = list;
    }//end while
}//end e_list_free_full

void e_list_free_single(e_list_t *list) {
    return e_list_free_single_full(list, NULL);
}//end e_list_free_single

void e_list_free_single_full(e_list_t *list, e_free_cb_t *free_cb) {
    if(E_LIKELY(list)) {
        if(free_cb) {
            free_cb(list->data);
        }//end if
        e_free(list);
    }//end if
}//end e_list_free_single_full

e_list_t *e_list_append(e_list_t *list, void *data) {
    e_list_t *new_list;
    e_list_t *last;

    if(E_UNLIKELY(!(new_list = e_list_new()))) {
        return NULL;
    }//end if
    new_list->data = data;
    new_list->next = NULL;

    if(list) {
        last = e_list_last(list);
        last->next = new_list;
        new_list->prev = last;

        return list;
    }//end if
    else {
        new_list->prev = NULL;
        return new_list;
    }//end else
}//end e_list_append

e_list_t *e_list_prepend(e_list_t *list, void *data) {
    e_list_t *new_list;

    if(E_UNLIKELY(!(new_list = e_list_new()))) {
        return NULL;
    }//end if
    new_list->data = data;
    new_list->next = list;

    if(list) {
        new_list->prev = list->prev;
        if(list->prev) {
            list->prev->next = new_list;
        }//end if
        list->prev = new_list;
    }//end if
    else {
        new_list->prev = NULL;
    }//end else
  
    return new_list;
}//end e_list_prepend

e_list_t *e_list_insert(e_list_t *list, void *data, int position) {
    e_list_t *new_list;
    e_list_t *tmp_list;

    if(position < 0) {
        return e_list_append(list, data);
    }//end if
    else if(position == 0) {
        return e_list_prepend(list, data);
    }//end if

    tmp_list = e_list_nth(list, position);
    if(!tmp_list) {
        return e_list_append(list, data);
    }//end if

    if(E_UNLIKELY(!(new_list = e_list_new()))) {
        return NULL;
    }//end if
    new_list->data = data;
    new_list->prev = tmp_list->prev;
    tmp_list->prev->next = new_list;
    new_list->next = tmp_list;
    tmp_list->prev = new_list;

    return list;
}//end e_list_insert

e_list_t *e_list_insert_sorted(e_list_t *list, void *data, e_compare_cb_t *compare_cb) {
    int         cmp;
    e_list_t    *tmp_list = list;
    e_list_t    *new_list;

    if(!list) {
        if(E_UNLIKELY(!(new_list = e_list_new()))) {
            return NULL;
        }//end if
        new_list->data = data;
        return new_list;
    }//end if

    cmp = compare_cb(data, tmp_list->data);
    while((tmp_list->next) && (cmp > 0)) {
        tmp_list = tmp_list->next;
        cmp = compare_cb(data, tmp_list->data);
    }//end while

    if(E_UNLIKELY(!(new_list = e_list_new()))) {
        return NULL;
    }//end if
    new_list->data = data;

    if((!tmp_list->next) && (cmp > 0)) {
        tmp_list->next = new_list;
        new_list->prev = tmp_list;
        return list;
    }//end if
   
    if(tmp_list->prev) {
        tmp_list->prev->next = new_list;
        new_list->prev = tmp_list->prev;
    }//end if
    new_list->next = tmp_list;
    tmp_list->prev = new_list;
 
    return (tmp_list == list) ? new_list : list;
}//end e_list_insert_sorted

e_list_t *e_list_insert_before(e_list_t *list, e_list_t *sibling, void *data) {
    e_list_t *node, *last;

    if(!list) {
        if(E_UNLIKELY(!(list = e_list_new()))) {
            return NULL;
        }//end if
        list->data = data;
        return list;
    }//end if
    else if(sibling) {
        if(E_UNLIKELY(!(node = e_list_new()))) {
            return NULL;
        }//end if

        node->data = data;
        node->prev = sibling->prev;
        node->next = sibling;
        sibling->prev = node;
        if(node->prev) {
            node->prev->next = node;
            return list;
        }//end if
        else {
            return node;
        }//end else
    }//end if
    else {
        last = list;
        while(last->next) {
            last = last->next;
        }//end while

        if(E_UNLIKELY(!(last->next = e_list_new()))) {
            return NULL;
        }//end if
        last->next->data = data;
        last->next->prev = last;
        last->next->next = NULL;

        return list;
    }//end else
}//end e_list_insert_before

e_list_t *e_list_concat(e_list_t *list1, e_list_t *list2) {
    e_list_t *tmp_list;
  
    if(list2) {
        tmp_list = e_list_last(list1);
        if(tmp_list) {
            tmp_list->next = list2;
        }//end if
        else {
            list1 = list2;
        }//end else
        list2->prev = tmp_list;
    }//end if

    return list1;
}//end e_list_concat

e_list_t *e_list_remove(e_list_t *list, const void *data, e_free_cb_t *free_cb) {
    e_list_t *tmp;

    tmp = list;
    while(tmp) {
        if(tmp->data != data) {
            tmp = tmp->next;
        }//end if
        else {
            list = e_list_remove_link(list, tmp);
            e_list_free_full(tmp, free_cb);

            break;
        }//end else
    }//end while
    return list;
}//end e_list_remove

e_list_t *e_list_remove_all(e_list_t *list, const void *data, e_free_cb_t *free_cb) {
    e_list_t *tmp = list, *next;

    while(tmp) {
        if(tmp->data != data) {
            tmp = tmp->next;
        }//end if
        else {
            next = tmp->next;

            if(tmp->prev) {
                tmp->prev->next = next;
            }//end if
            else {
                list = next;
            }//end else
            if(next) {
                next->prev = tmp->prev;
            }//end if

            if(free_cb) {
                free_cb(tmp->data);
            }//end if
            e_free(tmp);
            tmp = next;
        }//end else
    }//end while

    return list;
}//end e_list_remove_all

e_list_t *e_list_remove_link(e_list_t *list, e_list_t *link) {

    if(link == NULL) {
        return list;
    }//end if

    if(link->prev && (link->prev->next == link)) {
        link->prev->next = link->next;
    }//end if
    if(link->next && (link->next->prev == link)) {
        link->next->prev = link->prev;
    }//end if

    if(link == list) {
        list = list->next;
    }//end if

    link->next = NULL;
    link->prev = NULL;

    return list;
}//end e_list_remove_link

e_list_t *e_list_delete_link(e_list_t *list, e_list_t *link, e_free_cb_t *free_cb) {
    list = e_list_remove_link(list, link);
    e_list_free_full(link, free_cb);
    return list;
}//end e_list_delete_link

e_list_t *e_list_copy_deep(e_list_t *list, e_list_copy_cb_t *copy_cb, void *arg, e_free_cb_t *free_cb) {
    e_list_t *new_list = NULL, *last;

    if(list) {
        if(E_UNLIKELY(!(new_list = e_list_new()))) {
            return NULL;
        }//end if
        if(copy_cb) {
            new_list->data = copy_cb(list->data, arg);
        }//end if
        else {
            new_list->data = list->data;
        }//end else
        new_list->prev = NULL;
        last = new_list;
        list = list->next;
        while(list) {
            if(E_UNLIKELY(!(last->next = e_list_new()))) {
                e_list_free_full(new_list, free_cb);
                return NULL;
            }//end if
            last->next->prev = last;
            last = last->next;
            if(copy_cb) {
                last->data = copy_cb(list->data, arg);
            }//end if
            else {
                last->data = list->data;
            }//end else
            list = list->next;
        }//end while
        last->next = NULL;
    }//end if

  return new_list;
}//end e_list_copy_deep

e_list_t *e_list_reverse(e_list_t *list) {
    e_list_t *last;
  
    last = NULL;
    while(list) {
        last = list;
        list = last->next;
        last->next = last->prev;
        last->prev = list;
    }//end while

    return last;
}//end e_list_reverse

e_list_t *e_list_nth(e_list_t *list, uint n) {
    while((n-- > 0) && list) {
        list = list->next;
    }//end while
  
    return list;
}//end e_list_nth

e_list_t *e_list_nth_prev(e_list_t *list, uint n) {
    while((n-- > 0) && list) {
        list = list->prev;
    }//end while
  
    return list;
}//end e_list_nth_prev

void *e_list_nth_data(e_list_t *list, uint n) {
    list = e_list_nth(list, n);
    return list ? list->data : NULL;
}//end e_list_nth_data

void *e_list_nth_data_prev(e_list_t *list, uint n) {
    list = e_list_nth_prev(list, n);
    return list ? list->data : NULL;
}//end e_list_nth_data_prev

e_list_t *e_list_find(e_list_t *list, const void *data) {
    while(list) {
        if(list->data == data) {
            return list;
        }//end if
        list = list->next;
    }//end while

  return NULL;
}//end e_list_find

int e_list_position(e_list_t *list, e_list_t *llink) {
    int i;

    i = 0;
    while(list) {
        if(list == llink) {
            return i;
        }//end if
        i++;
        list = list->next;
    }//end while

    return -1;
}//end e_list_position

int e_list_index(e_list_t *list, const void *data) {
    int i;

    i = 0;
    while(list) {
        if(list->data == data) {
            return i;
        }//end if
        i++;
        list = list->next;
    }//end while

    return -1;
}//end e_list_index

e_list_t *e_list_first(e_list_t *list) {
    if(list) {
        while(list->prev) {
            list = list->prev;
        }//end while
    }//end if
    return list;
}//end e_list_first

e_list_t *e_list_last(e_list_t *list) {
    if(list) {
        while(list->next) {
            list = list->next;
        }//end while
    }//end if
    return list;
}//end e_list_last

size_t e_list_count(e_list_t *list) {
    size_t count;
  
    count = 0;
    while(list) {
        count++;
        list = list->next;
    }//end while
  
    return count;
}//end e_list_count

int e_list_foreach(e_list_t *list, e_list_foreach_cb_t *foreach_cb, void *arg) {
    int         ret;
    e_list_t    *next;

    while(list) {
        next = list->next;
        ret = foreach_cb(list->data, arg);
        if(E_UNLIKELY(ret != 0)) {
            return ret;
        }//end if
        list = next;
    }//end while

    return 0;
}//end e_list_foreach

e_list_t *e_list_sort(e_list_t *list, e_compare_cb_t *compare_cb) {
    return e_list_sort_real(list, compare_cb);
}//end e_list_sort


/* ===== private function ===== */
static inline e_list_t *e_list_sort_real(e_list_t *list, e_compare_cb_t *compare_cb) {
    e_list_t *l1, *l2;

    if(E_UNLIKELY(!list || !list->next)) {
        return list;
    }//end if

    l1 = list;
    l2 = list->next;

    while((l2 = l2->next) != NULL) {
        if((l2 = l2->next) == NULL) {
            break;
        }//end if
        l1 = l1->next;
    }//end while
    l2 = l1->next; 
    l1->next = NULL;

    return e_list_sort_merge(
        e_list_sort_real(list, compare_cb),
        e_list_sort_real(l2, compare_cb),
        compare_cb);
}//end e_list_sort_real

static inline e_list_t *e_list_sort_merge(e_list_t *l1, e_list_t *l2, e_compare_cb_t *compare_cb) {
    int         cmp;
    e_list_t    list, *l, *lprev;

    l = &list; 
    lprev = NULL;

    while(l1 && l2) {
        cmp = compare_cb(l1->data, l2->data);

        if(cmp <= 0) {
            l->next = l1;
            l1 = l1->next;
        }//end if
        else  {
          l->next = l2;
          l2 = l2->next;
        }//end else
        l = l->next;
        l->prev = lprev; 
        lprev = l;
    }//end while
    l->next = l1 ? l1 : l2;
    l->next->prev = l;

    return list.next;
}//end e_list_sort_merge
