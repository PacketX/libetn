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


#ifndef E_STRFUNCS_H
#define E_STRFUNCS_H

#include <libetn/e_visibility.h>
#include <libetn/e_macros.h>
#include <sys/types.h>
#include <inttypes.h>
#include <string.h>

#define E_STRBUF    (256)
#define E_TAB       "    "

/* functions like the ones in <ctype.h> that are not affected by locale */
enum {
    E_ASCII_ALNUM  = 1 << 0,
    E_ASCII_ALPHA  = 1 << 1,
    E_ASCII_CNTRL  = 1 << 2,
    E_ASCII_DIGIT  = 1 << 3,
    E_ASCII_GRAPH  = 1 << 4,
    E_ASCII_LOWER  = 1 << 5,
    E_ASCII_PRINT  = 1 << 6,
    E_ASCII_PUNCT  = 1 << 7,
    E_ASCII_SPACE  = 1 << 8,
    E_ASCII_UPPER  = 1 << 9,
    E_ASCII_XDIGIT = 1 << 10
};

E_IMPORT const uint16_t *const e_ascii_table;

#define e_ascii_isalnum(c)  ((e_ascii_table[(u_char)(c)] & E_ASCII_ALNUM) != 0)
#define e_ascii_isalpha(c)  ((e_ascii_table[(u_char)(c)] & E_ASCII_ALPHA) != 0)
#define e_ascii_iscntrl(c)  ((e_ascii_table[(u_char)(c)] & E_ASCII_CNTRL) != 0)
#define e_ascii_isdigit(c)  ((e_ascii_table[(u_char)(c)] & E_ASCII_DIGIT) != 0)
#define e_ascii_isgraph(c)  ((e_ascii_table[(u_char)(c)] & E_ASCII_GRAPH) != 0)
#define e_ascii_islower(c)  ((e_ascii_table[(u_char)(c)] & E_ASCII_LOWER) != 0)
#define e_ascii_isprint(c)  ((e_ascii_table[(u_char)(c)] & E_ASCII_PRINT) != 0)
#define e_ascii_ispunct(c)  ((e_ascii_table[(u_char)(c)] & E_ASCII_PUNCT) != 0)
#define e_ascii_isspace(c)  ((e_ascii_table[(u_char)(c)] & E_ASCII_SPACE) != 0)
#define e_ascii_isupper(c)  ((e_ascii_table[(u_char)(c)] & E_ASCII_UPPER) != 0)
#define e_ascii_isxdigit(c) ((e_ascii_table[(u_char)(c)] & E_ASCII_XDIGIT) != 0)

#define e_ascii_toupper(c) (e_ascii_islower(c) ? (c) - 'a' + 'A' : (c))
#define e_ascii_tolower(c) (e_ascii_isupper(c) ? (c) - 'A' + 'a' : (c))

__BEGIN_DECLS

E_EXPORT char *e_strdup(const char *str) E_GNUC_WARN_UNUSED_RESULT E_GNUC_MALLOC;
E_EXPORT char *e_strndup(const char *str, size_t n) E_GNUC_WARN_UNUSED_RESULT E_GNUC_MALLOC;
#define e_strdupa(str) strdupa(str)
#define e_strndupa(str, n)                  \
    (E_GNUC_EXTENSION({                     \
        char *nee_str;                      \
        nee_str = e_alloca(n + 1);          \
        strncpy(nee_str, str, n);           \
        nee_str[n] = '\0';                  \
        nee_str;                            \
    }))

E_EXPORT char *e_strrstr(const char *haystack, const char *needle) E_NONNULL(1, 2);
E_EXPORT char *e_strstr_len(const char *haystack, ssize_t haystack_len, const char *needle) E_NONNULL(1, 3);
E_EXPORT char *e_strrstr_len(const char *haystack, ssize_t haystack_len, const char *needle) E_NONNULL(1, 3);

E_EXPORT bool e_str_has_suffix(const char *str, const char *suffix) E_NONNULL(1, 2);
E_EXPORT bool e_str_has_prefix(const char *str, const char *prefix) E_NONNULL(1, 2);

/* string vector */
E_EXPORT char **e_strsplit(const char *str, const char *delimiter, size_t * __restrict ntoken) E_GNUC_WARN_UNUSED_RESULT E_GNUC_MALLOC E_NONNULL(1, 2);
E_EXPORT char **e_strsplit_set(const char *str, const char *delimiters, size_t * __restrict ntoken) E_GNUC_WARN_UNUSED_RESULT E_GNUC_MALLOC E_NONNULL(1, 2);
E_EXPORT void e_strfreev(char **str_array);
E_EXPORT char **e_strsanitisev(const char *const * __restrict str_array, size_t * __restrict ntoken) E_GNUC_WARN_UNUSED_RESULT E_GNUC_MALLOC E_NONNULL(1);
E_EXPORT char **e_strdupv(const char *const *strv1);
E_EXPORT char *e_strjoinv(const char *const *strv1, const char *separator) E_GNUC_WARN_UNUSED_RESULT E_GNUC_MALLOC E_NONNULL(1);
E_EXPORT size_t e_strlenv(const char *const *strv1) E_NONNULL(1);
E_EXPORT bool e_strv_contains(const char *const *strv, const char *str) E_NONNULL(1, 2);
E_EXPORT bool e_strv_equal(const char *const *strv1, const char *const *strv2) E_NONNULL(1, 2);

__END_DECLS

#endif /* E_STRFUNCS_H */
