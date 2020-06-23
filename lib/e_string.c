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


#include <libetn/e_string.h>
#include <libetn/e_refcount.h>
#include <libetn/e_mem.h>
#include <libetn/e_strfuncs.h>
#include <libetn/e_hash.h>

/* https://github.com/GNOME/glib/blob/master/glib/gstring.h */
/* https://github.com/GNOME/glib/blob/master/glib/gstring.c */

struct e_string_s {
    char                *str;
    size_t              len;
    size_t              allocated_len;
    e_atomic_refcount_t ref_count;
};

static inline e_errno_t e_string_maybe_expand(e_string_t *string, size_t len);
static inline size_t e_nearest_power(size_t base, size_t num);

e_string_t *e_string_new(const char *s) {
    size_t      len;
    e_errno_t   err;
    e_string_t  *string;

    if(s == NULL || *s == '\0') {
        string = e_string_sized_new(2);
    }//end if
    else {
        len = strlen(s);
        string = e_string_sized_new(len + 2);
        if(E_UNLIKELY(!string)) {
            return NULL;
        }//end if
        err = e_string_append_len(string, s, len);
        if(E_UNLIKELY(err != E_OK)) {
            e_string_free(string);
            return NULL;
        }//end if
    }//end else

    return string;
}//end e_string_new

e_string_t *e_string_new_len(const char *s, ssize_t len) {
    e_errno_t   err;
    e_string_t  *string;

    if(len < 0) {
        return e_string_new(s);
    }//end if
    else {
        string = e_string_sized_new(len);
        if(E_UNLIKELY(!string)) {
            return NULL;
        }//end if

        if(s) {
            err = e_string_append_len(string, s, len);
            if(E_UNLIKELY(err != E_OK)) {
                e_string_free(string);
                return NULL;
            }//end if
        }//end if

        return string;
    }//end else
}//end e_string_new_len

e_string_t *e_string_sized_new(size_t size) {
    e_errno_t   err;
    e_string_t  *string;

    string = e_calloc(1, sizeof(e_string_t));
    if(E_UNLIKELY(!string)) {
        return NULL;
    }//end if
    e_atomic_refcount_init(&(string->ref_count));

    err = e_string_maybe_expand(string, E_MAX(size, 2));
    if(E_UNLIKELY(err != E_OK)) {
        e_string_free(string);
        return NULL;
    }//end if
    string->str[0] = 0;

    return string;
}//end e_string_sized_new

void e_string_free(e_string_t *string) {
    e_string_unref(string);
}//end e_string_free

e_string_t *e_string_ref(e_string_t *string) {
    e_atomic_refcount_inc(&(string->ref_count));
    return string;
}//end e_string_ref

void e_string_unref(e_string_t *string) {
    if(E_LIKELY(string) && e_atomic_refcount_dec(&(string->ref_count))) {
        if(E_LIKELY(string->str)) {
            e_free(string->str);
        }//end if
        e_free(string);
    }//end if
}//end e_string_unref

void e_string_get_c_string(e_string_t *string, char **s, size_t *len) {
    if(s) {
        *s = string->str;
    }//end if
    if(len) {
        *len = string->len;
    }//end if
}//end e_string_get_c_string

e_errno_t e_string_get_c_string_dup(e_string_t *string, char **s, size_t *len) {

    *s = e_strdup(string->str);
    if(E_UNLIKELY(!*s)) {
        return E_ERR_C_ERR;
    }//end if

    if(len) {
        *len = string->len;
    }//end if

    return E_OK;
}//end e_string_get_c_string_dup

bool e_string_equal(const e_string_t *s1, const e_string_t *s2) {
    char    *p, *q;
    size_t  i = s1->len;

    if(i != s2->len) {
        return false;
    }//end if

    p = s1->str;
    q = s2->str;
    while(i) {
        if(*p != *q) {
            return false;
        }//end if
        p++;
        q++;
        i--;
    }//end while
    return true;
}//end e_string_equal

uint e_string_hash(const e_string_t *string) {
    uint        h;
    size_t      n;
    const char *p;

    h = 0;
    n = string->len;
    p = string->str;

    /* 31 bit hash function */
    while(n--) {
        h = (h << 5) - h + *p;
        p++;
    }//end while

    return h;
}//end e_string_hash

e_errno_t e_string_assign(e_string_t *string, const char *s) {
    /* Make sure assigning to itself doesn't corrupt the string. */
    if(string->str != s) {
      /* Assigning from substring should be ok, since
       * e_string_truncate() does not reallocate.
       */
        e_string_truncate(string, 0);
        return e_string_append(string, s);
    }//end if

    return E_OK;
}//end e_string_assign

void e_string_truncate(e_string_t *string, size_t len) {
    string->len = E_MIN(len, string->len);
    string->str[string->len] = 0;
}//end e_string_truncate

e_errno_t e_string_insert_len(e_string_t *string, ssize_t pos, const char *s, ssize_t len) {
    size_t      offset, precount;
    e_errno_t   err;

    if(len == 0) {
        return E_OK;
    }//end if

    if(len < 0) {
        len = strlen(s);
    }//end if

    if(pos < 0) {
        pos = string->len;
    }//end if
    else if(E_UNLIKELY(pos > string->len)) {
        return E_ERR_RANGE;
    }//end if

    /* Check whether val represents a substring of string.
   * This test probably violates chapter and verse of the C standards,
   * since ">=" and "<=" are only valid when val really is a substring.
   * In practice, it will work on modern archs.
   */
    if(E_UNLIKELY(s >= string->str && s <= string->str + string->len)) {
        offset = s - string->str;
        precount = 0;

        err = e_string_maybe_expand(string, len);
        if(E_UNLIKELY(err != E_OK)) {
            return err;
        }//end if
        s = string->str + offset;
        /* At this point, val is valid again.  */

        /* Open up space where we are going to insert.  */
        if(pos < string->len) {
            memmove(string->str + pos + len, string->str + pos, string->len - pos);
        }//end if

        /* Move the source part before the gap, if any.  */
        if(offset < pos) {
            precount = E_MIN(len, pos - offset);
            memcpy(string->str + pos, s, precount);
        }//end if

        /* Move the source part after the gap, if any.  */
        if(len > precount) {
            memcpy(string->str + pos + precount,
                    s + /* Already moved: */ precount + /* Space opened up: */ len,
                    len - precount);
        }//end if
    }//end if
  else {
        err = e_string_maybe_expand(string, len);
        if(E_UNLIKELY(err != E_OK)) {
            return err;
        }//end if

        /* If we aren't appending at the end, move a hunk
         * of the old string to the end, opening up space
         */
        if(pos < string->len) {
            memmove(string->str + pos + len, string->str + pos, string->len - pos);
        }//end if

        /* insert the new string */
        if(len == 1) {
            string->str[pos] = *s;
        }//end if
        else {
            memcpy(string->str + pos, s, len);
        }//end else
    }//end else

    string->len += len;
    string->str[string->len] = 0;

    return E_OK;
}//end e_string_insert_len

e_errno_t e_string_append(e_string_t *string, const char *s) {
    return e_string_insert_len(string, -1, s, -1);
}//end e_string_append

e_errno_t e_string_append_len(e_string_t *string, const char *s, ssize_t len) {
    return e_string_insert_len(string, -1, s, len);
}//end e_string_append_len

e_errno_t e_string_append_c(e_string_t *string, char c) {
    return e_string_insert_c(string, -1, c);
}//end e_string_append_c

e_errno_t e_string_append_unichar(e_string_t *string, wchar_t wc) {
    return e_string_insert_unichar(string, -1, wc);
}//end e_string_append_unichar

e_errno_t e_string_prepend(e_string_t *string, const char *s) {
    return e_string_insert_len(string, 0, s, -1);
}//end e_string_prepend

e_errno_t e_string_prepend_c(e_string_t *string, char c) {
    return e_string_insert_c(string, 0, c);
}//end e_string_prepend_c

e_errno_t e_string_prepend_unichar(e_string_t *string, wchar_t wc) {
    return e_string_insert_unichar(string, 0, wc);
}//end e_string_prepend_unichar

e_errno_t e_string_prepend_len(e_string_t *string, const char *s, ssize_t len) {
    return e_string_insert_len(string, 0, s, len);
}//end e_string_prepend_len

e_errno_t e_string_insert(e_string_t *string, ssize_t pos, const char *s) {
    return e_string_insert_len(string, pos, s, -1);
}//end e_string_insert

e_errno_t e_string_insert_c(e_string_t *string, ssize_t pos, char c) {
    e_errno_t err;

    err = e_string_maybe_expand(string, 1);
    if(E_UNLIKELY(err != E_OK)) {
        return err;
    }//end if

    if(pos < 0) {
        pos = string->len;
    }//end if
    else if(E_UNLIKELY(pos > string->len)) {
        return E_ERR_RANGE;
    }//end if

    /* If not just an append, move the old stuff */
    if(pos < string->len) {
        memmove(string->str + pos + 1, string->str + pos, string->len - pos);
    }//end if

    string->str[pos] = c;
    string->len += 1;
    string->str[string->len] = 0;

    return E_OK;
}//end e_string_insert_c

e_errno_t e_string_insert_unichar(e_string_t *string, ssize_t pos, wchar_t wc) {
    int         first;
    char        *dest;
    size_t      charlen, i;
    e_errno_t   err;

    /* Code copied from e_unichar_to_utf() */
    if(wc < 0x80) {
        first = 0;
        charlen = 1;
    }//end if
    else if(wc < 0x800) {
        first = 0xc0;
        charlen = 2;
    }//end if
    else if(wc < 0x10000) {
        first = 0xe0;
        charlen = 3;
    }//end if
    else if(wc < 0x200000) {
        first = 0xf0;
        charlen = 4;
    }//end if
    else if(wc < 0x4000000) {
        first = 0xf8;
        charlen = 5;
    }//end if
    else {
        first = 0xfc;
        charlen = 6;
    }//end else
    /* End of copied code */

    err = e_string_maybe_expand(string, charlen);
    if(E_UNLIKELY(err != E_OK)) {
        return err;
    }//end if

    if(pos < 0) {
        pos = string->len;
    }//end if
    else if(E_UNLIKELY(pos > string->len)) {
        return E_ERR_RANGE;
    }//end if

    /* If not just an append, move the old stuff */
    if(pos < string->len) {
        memmove(string->str + pos + charlen, string->str + pos, string->len - pos);
    }//end if

    dest = string->str + pos;
    /* Code copied from e_unichar_to_utf() */
    for(i = charlen - 1; i > 0; --i) {
        dest[i] = (wc & 0x3f) | 0x80;
        wc >>= 6;
    }//end for
    dest[0] = wc | first;
    /* End of copied code */

    string->len += charlen;
    string->str[string->len] = 0;

    return E_OK;
}//end e_string_insert_unichar

e_errno_t e_string_overwrite(e_string_t *string, size_t pos, const char *s) {
    return e_string_overwrite_len(string, pos, s, strlen(s));
}//end e_string_overwrite

e_errno_t e_string_overwrite_len(e_string_t *string, size_t pos, const char *s, ssize_t len) {
    size_t      end;
    e_errno_t   err;

    if(E_UNLIKELY(!len)) {
        return E_OK;
    }//end if

    if(E_UNLIKELY(pos > string->len)) {
        return E_ERR_RANGE;
    }//end if

    if(len < 0) {
        len = strlen(s);
    }//end if

    end = pos + len;
    if(end > string->len) {
        err = e_string_maybe_expand(string, end - string->len);
        if(E_UNLIKELY(err != E_OK)) {
            return err;
        }//end if
    }//end if

    memcpy(string->str + pos, s, len);

    if(end > string->len) {
        string->str[end] = '\0';
        string->len = end;
    }//end if

    return E_OK;
}//end e_string_overwrite_len

e_errno_t e_string_erase(e_string_t *string, ssize_t pos, ssize_t len) {
    if(E_UNLIKELY(pos > string->len)) {
        return E_ERR_RANGE;
    }//end if

    if(len < 0) {
        len = string->len - pos;
    }//end if
    else {
        if(E_UNLIKELY(pos + len > string->len)) {
            return E_ERR_RANGE;
        }//end if

        if(pos + len < string->len) {
            memmove(string->str + pos, string->str + pos + len, string->len - (pos + len));
        }//end if
    }//end else

    string->len -= len;
    string->str[string->len] = 0;

    return E_OK;
}//end e_string_erase

void e_string_ascii_down(e_string_t *string) {
    char    *s;
    size_t  n;

    n = string->len;
    s = string->str;

    while(n) {
        *s = e_ascii_tolower(*s);
        s++;
        n--;
    }//end while
}//end e_string_ascii_down

void e_string_ascii_up(e_string_t *string) {
    char    *s;
    size_t  n;

    n = string->len;
    s = string->str;

    while(n) {
        *s = e_ascii_toupper(*s);
        s++;
        n--;
    }//end while
}//end e_string_ascii_up

e_errno_t e_string_vprintf(e_string_t *string, size_t *length, const char *fmt, va_list ap) {
    e_string_truncate(string, 0);
    return e_string_append_vprintf(string, length, fmt, ap);
}//end e_string_vprintf

e_errno_t e_string_printf(e_string_t *string, size_t *length, const char *fmt, ...) {
    va_list     ap;
    e_errno_t   err;

    e_string_truncate(string, 0);

    va_start(ap, fmt);
    err = e_string_append_vprintf(string, length, fmt, ap);
    va_end(ap);

    return err;
}//end e_string_printf

e_errno_t e_string_append_vprintf(e_string_t *string, size_t *length, const char *fmt, va_list ap) {
    char        *buf = NULL;
    size_t      len;
    e_errno_t   err;

    len = vasprintf(&buf, fmt, ap);
    if(E_UNLIKELY(len < 0)) {
        return E_ERR_C_ERR;
    }//end if

    if(len >= 0) {
        err = e_string_maybe_expand(string, len);
        if(E_UNLIKELY(err != E_OK)) {
            e_free(buf);
            return err;
        }//end if
        memcpy(string->str + string->len, buf, len + 1);
        string->len += len;
        e_free(buf);
    }//end if

    if(length) {
        *length = len;
    }//end if

    return E_OK;
}//end e_string_append_vprintf

e_errno_t e_string_append_printf(e_string_t *string, size_t *length, const char *fmt, ...) {
    va_list     ap;
    e_errno_t   err;

    va_start(ap, fmt);
    err = e_string_append_vprintf(string, length, fmt, ap);
    va_end(ap);

    return err;
}//end e_string_append_printf


/* ===== private function ===== */
static inline e_errno_t e_string_maybe_expand(e_string_t *string, size_t len) {
    void    *tmp;
    size_t  new_len;
    if(string->len + len >= string->allocated_len) {
        new_len = e_nearest_power(1, string->len + len + 1);
        tmp = e_realloc(string->str, new_len);
        if(E_UNLIKELY(!tmp)) {
            return E_ERR_FAMEM;
        }//end if
        string->str = tmp;
        string->allocated_len = new_len;
    }//end if
    return E_OK;
}//end e_string_maybe_expand

static inline size_t e_nearest_power(size_t base, size_t num) {
    size_t n;
    if(num > ((size_t)-1) / 2) {
        return ((size_t)-1);
    }//end if
    else {
        n = base;
        while(n < num) {
            n <<= 1;
        }//end while
        return n;
    }//end else
}//end e_nearest_power
