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


#include <libetn/e_strfuncs.h>
#include <libetn/e_mem.h>
#include <libetn/e_hash.h>
#include <libetn/e_list.h>

static const uint16_t e_ascii_table_data[256] = {
    0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004,
    0x004, 0x104, 0x104, 0x004, 0x104, 0x104, 0x004, 0x004,
    0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004,
    0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004,
    0x140, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0,
    0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0,
    0x459, 0x459, 0x459, 0x459, 0x459, 0x459, 0x459, 0x459,
    0x459, 0x459, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0,
    0x0d0, 0x653, 0x653, 0x653, 0x653, 0x653, 0x653, 0x253,
    0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253,
    0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253,
    0x253, 0x253, 0x253, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0,
    0x0d0, 0x473, 0x473, 0x473, 0x473, 0x473, 0x473, 0x073,
    0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073,
    0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073,
    0x073, 0x073, 0x073, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x004
    /* the upper 128 are all zeroes */
};

const uint16_t *const e_ascii_table = e_ascii_table_data;

char *e_strdup(const char *str) {
    char    *nee_str;
    size_t  len;

    if(E_LIKELY(str)) {
        len = strlen(str) + 1;
        nee_str = e_malloc(len);
        if(E_UNLIKELY(!nee_str)) {
            return NULL;
        }//end if
        memcpy(nee_str, str, len);
    }//end if
    else {
        nee_str = NULL;
    }//end else

    return nee_str;
}//end e_strdup

char *e_strndup(const char *str, size_t n) {
    char *nee_str;

    if(E_LIKELY(str)) {
        nee_str = e_malloc(n + 1);
        if(E_UNLIKELY(!nee_str)) {
            return NULL;
        }//end if
        strncpy(nee_str, str, n);
        nee_str[n] = '\0';
    }//end if
    else {
        nee_str = NULL;
    }//end else

    return nee_str;
}//end e_strndup

char *e_strrstr(const char *haystack, const char *needle) {
    size_t      i, needle_len, haystack_len;
    const char  *p;

    needle_len = strlen(needle);
    haystack_len = strlen(haystack);

    if(needle_len == 0) {
        return (char *)haystack;
    }//end if

    if(haystack_len < needle_len) {
        return NULL;
    }//end if

    p = haystack + haystack_len - needle_len;

    while(p >= haystack) {
        for(i = 0; i < needle_len; i++) {
            if(p[i] != needle[i]) {
                goto next;
            }//end if
        }//end for

        return (char *)p;
next:
        p--;
    }//end while

    return NULL;
}//end e_strrstr

char *e_strstr_len(const char *haystack, ssize_t haystack_len, const char *needle) {
    size_t      needle_len, haystack_len_unsigned, i;
    const char  *p, *end;

    if(haystack_len < 0) {
        return strstr(haystack, needle);
    }//end if
    else {
        p = haystack;
        needle_len = strlen(needle);
        haystack_len_unsigned = haystack_len;

        if(needle_len == 0) {
            return (char *)haystack;
        }//end if

        if(haystack_len_unsigned < needle_len) {
            return NULL;
        }//end if

        end = haystack + haystack_len - needle_len;

        while(p <= end && *p) {
            for(i = 0; i < needle_len; i++) {
                if(p[i] != needle[i]) {
                    goto next;
                }//end if
            }//end for

            return (char *)p;
next:
            p++;
        }//end while

        return NULL;
    }//end else
}//end e_strstr_len

char *e_strrstr_len(const char *haystack, ssize_t haystack_len, const char *needle) {
    size_t      needle_len, i;
    const char  *haystack_max, *p;

    if(haystack_len < 0) {
        return e_strrstr(haystack, needle);
    }//end if
    else {
        needle_len = strlen(needle);
        haystack_max = haystack + haystack_len;
        p = haystack;

        while(p < haystack_max && *p) {
            p++;
        }//end while

        if(p < haystack + needle_len) {
            return NULL;
        }//end if

        p -= needle_len;

        while(p >= haystack) {
            for(i = 0; i < needle_len; i++) {
                if(p[i] != needle[i]) {
                    goto next;
                }//end if
            }//end for

            return (char *)p;
next:
            p--;
        }//end while

        return NULL;
    }//end else
}//end e_strrstr_len

bool e_str_has_suffix(const char *str, const char *suffix) {
    size_t str_len, suffix_len;

    str_len = strlen(str);
    suffix_len = strlen(suffix);

    if(str_len < suffix_len) {
        return false;
    }//end if

    return strcmp(str + str_len - suffix_len, suffix) == 0 ? true : false;
}//end e_str_has_suffix

bool e_str_has_prefix(const char *str, const char *prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0 ? true : false;
}//end e_str_has_prefix

char **e_strsplit(const char *str, const char *delimiter, size_t *ntoken) {
    char        **str_array, *s;
    void        *data;
    size_t      n, delimiter_len, len;
    e_list_t    *string_list, *slist, *list;
    const char  *remainder;

    string_list = NULL;
    n = 0;
    remainder = str;
    s = strstr(remainder, delimiter);

    if(s) {
        delimiter_len = strlen(delimiter);

        while(s) {
            len = s - remainder;
            data = e_strndup(remainder, len);
            if(E_UNLIKELY(!data)) {
                e_list_free_full(string_list, e_free);
                return NULL;
            }//end if
            list = e_list_prepend(string_list, data);
            if(E_UNLIKELY(!list)) {
                e_free(data);
                e_list_free_full(string_list, e_free);
                return NULL;
            }//end if
            string_list = list;
            n++;
            remainder = s + delimiter_len;
            s = strstr(remainder, delimiter);
        }//end while
    }//end if

    if(*str) {
        n++;
        data = e_strdup(remainder);
        if(E_UNLIKELY(!data)) {
            e_list_free_full(string_list, e_free);
            return NULL;
        }//end if
        list = e_list_prepend(string_list, data);
        if(E_UNLIKELY(!list)) {
            e_free(data);
            e_list_free_full(string_list, e_free);
            return NULL;
        }//end if
        string_list = list;
    }//end if

    str_array = e_malloc((n + 1) * sizeof(char *));
    if(E_UNLIKELY(!str_array)) {
        e_list_free_full(string_list, e_free);
        return NULL;
    }//end if
    if(ntoken) {
        *ntoken = n;
    }//end if
    str_array[n--] = NULL;

    for(slist = string_list; slist; slist = slist->next) {
        str_array[n--] = slist->data;
    }//end for

    e_list_free(string_list);
    return str_array;
}//end e_strsplit

char **e_strsplit_set(const char *str, const char *delimiters, size_t *ntoken) {
    bool        delim_table[256];
    char        *token, **result;
    size_t      i, n_tokens;
    e_list_t    *tokens, *list;
    const char  *s, *current;

    if(*str == '\0') {
        result = e_malloc(sizeof(char *));
        if(E_UNLIKELY(!result)) {
            return NULL;
        }//end if
        result[0] = NULL;
        return result;
    }//end if

    for(i = 0 ; i < E_N_ELEMENTS(delim_table) ; i++) {
        delim_table[i] = false;
    }//end if

    for(s = delimiters; *s != '\0'; s++) {
        delim_table[*(u_char *)s] = true;
    }//end for

    tokens = NULL;
    n_tokens = 0;
    s = current = str;

    while(*s != '\0') {
        if(delim_table[*(u_char *)s]) {
            token = e_strndup(current, s - current);
            if(E_UNLIKELY(!token)) {
                e_list_free_full(tokens, e_free);
                return NULL;
            }//end if
            list = e_list_prepend(tokens, token);
            if(E_UNLIKELY(!list)) {
                e_free(token);
                e_list_free_full(tokens, e_free);
                return NULL;
            }//end if
            tokens = list;
            ++n_tokens;
            current = s + 1;
        }//end if

        ++s;
    }//end while

    token = e_strndup(current, s - current);
    if(E_UNLIKELY(!token)) {
        e_list_free_full(tokens, e_free);
        return NULL;
    }//end if
    list = e_list_prepend(tokens, token);
    if(E_UNLIKELY(!list)) {
        e_free(token);
        e_list_free_full(tokens, e_free);
        return NULL;
    }//end if
    tokens = list;
    ++n_tokens;

    result = e_malloc((n_tokens + 1) * sizeof(char *));
    if(E_UNLIKELY(!result)) {
        e_list_free_full(tokens, e_free);
        return NULL;
    }//end if
    if(ntoken) {
        *ntoken = n_tokens;
    }//end if
    result[n_tokens] = NULL;

    for(list = tokens; list != NULL; list = list->next) {
        result[--n_tokens] = list->data;
    }//end for

    e_list_free(tokens);
    return result;
}//end e_strsplit_set

void e_strfreev(char **str_array) {
    size_t i;

    if(E_LIKELY(str_array)) {
        for(i = 0; str_array[i] != NULL; i++) {
            e_free(str_array[i]);
        }//end for

        e_free(str_array);
    }//end if
}//end e_strfreev

char **e_strsanitisev(const char *const *str_array, size_t *ntoken) {
    char        **result;
    void        *data;
    size_t      i, len, n;
    e_list_t    *slist, *list;

    len = e_strlenv(str_array);
    if(len == 0) {
        return NULL;
    }//end if

    list = NULL;
    n = 0;
    slist = NULL;

    for(i = 0 ; i < len ;i++) {
        if(strlen(str_array[i]) > 0) {
            data = e_strdup(str_array[i]);
            if(E_UNLIKELY(!data)) {
                e_list_free_full(slist, e_free);
                return NULL;
            }//end if
            list = e_list_prepend(slist, data);
            if(E_UNLIKELY(!list)) {
                e_free(data);
                e_list_free_full(slist, e_free);
                return NULL;
            }//end if
            slist = list;
            n++;
        }//end if
    }//end for

    result = e_malloc((n + 1) * sizeof(char *));
    if(E_UNLIKELY(!result)) {
        e_list_free_full(slist, e_free);
        return NULL;
    }//end if
    if(ntoken) {
        *ntoken = n;
    }//end if
    result[n] = NULL;

    for(list = slist; list != NULL; list = list->next) {
        result[--n] = list->data;
    }//end for

    e_list_free(slist);

    return result;
}//end e_strsanitisev

char **e_strdupv(const char *const *str_array) {
    char    **ret, *p;
    size_t  i;

    if(E_LIKELY(str_array)) {
        i = 0;
        while(str_array[i]) {
            i++;
        }//end while

        ret = e_malloc((i + 1) * sizeof(char *));
        if(E_UNLIKELY(!ret)) {
            return NULL;
        }//end if

        i = 0;
        while(str_array[i]) {
            p = e_strdup(str_array[i]);
            if(E_UNLIKELY(!p)) {
                e_strfreev(ret);
                return NULL;
            }//end if
            ret[i] = p;
            i++;
        }//end while
        ret[i] = NULL;

        return ret;
    }//end if
    else {
        return NULL;
    }//end else
}//end e_strdupv

char *e_strjoinv(const char *const *str_array, const char *separator) {
    char    *str, *ptr;
    size_t  i, len, separator_len;

    if(separator == NULL) {
        separator = "";
    }//end if

    if(*str_array) {
        separator_len = strlen(separator);
        /* first part, getting length */
        len = 1 + strlen(str_array[0]);
        for(i = 1; str_array[i] != NULL; i++) {
            len += strlen(str_array[i]);
        }//end for
        len += separator_len * (i - 1);

        /* second part, building string */
        str = e_malloc(len);
        if(E_UNLIKELY(!str)) {
            return NULL;
        }//end if

        ptr = stpcpy(str, *str_array);
        for(i = 1; str_array[i] != NULL; i++) {
            ptr = stpcpy(ptr, separator);
            ptr = stpcpy(ptr, str_array[i]);
        }//end for
    }//end if
    else {
        str = e_strdup("");
        if(E_UNLIKELY(!str)) {
            return NULL;
        }//end if
    }//end else

    return str;
}//end e_strjoinv

size_t e_strlenv(const char *const *str_array) {
    size_t i;

    i = 0;
    while(str_array[i]) {
        i++;
    }//end while
    return i;
}//end e_strlenv

bool e_strv_contains(const char * const *strv, const char *str) {
    for(; *strv != NULL; strv++) {
        if(e_hash_str_equal(str, *strv)) {
            return true;
        }//end if
    }//end for

    return false;
}//end e_strv_contains

bool e_strv_equal(const char *const *strv1, const char *const *strv2) {
    if(strv1 == strv2) {
        return true;
    }//end if

    for(; *strv1 != NULL && *strv2 != NULL; strv1++, strv2++) {
        if(!e_hash_str_equal(*strv1, *strv2)) {
            return false;
        }//end if
    }//end for

    return (*strv1 == NULL && *strv2 == NULL);
}//end e_strv_equal
