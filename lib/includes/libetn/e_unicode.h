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


#ifndef E_UNICODE_H
#define E_UNICODE_H

#include <libetn/e_err.h>

typedef uint32_t    e_unicode32_t;
typedef uint16_t    e_unicode16_t;
typedef uint8_t     e_unicode8_t;

typedef enum {
    E_UNICODE_TYPE_CONTROL,
    E_UNICODE_TYPE_FORMAT,
    E_UNICODE_TYPE_UNASSIGNED,
    E_UNICODE_TYPE_PRIVATE_USE,
    E_UNICODE_TYPE_SURROGATE,
    E_UNICODE_TYPE_LOWERCASE_LETTER,
    E_UNICODE_TYPE_MODIFIER_LETTER,
    E_UNICODE_TYPE_OTHER_LETTER,
    E_UNICODE_TYPE_TITLECASE_LETTER,
    E_UNICODE_TYPE_UPPERCASE_LETTER,
    E_UNICODE_TYPE_SPACING_MARK,
    E_UNICODE_TYPE_ENCLOSING_MARK,
    E_UNICODE_TYPE_NON_SPACING_MARK,
    E_UNICODE_TYPE_DECIMAL_NUMBER,
    E_UNICODE_TYPE_LETTER_NUMBER,
    E_UNICODE_TYPE_OTHER_NUMBER,
    E_UNICODE_TYPE_CONNECT_PUNCTUATION,
    E_UNICODE_TYPE_DASH_PUNCTUATION,
    E_UNICODE_TYPE_CLOSE_PUNCTUATION,
    E_UNICODE_TYPE_FINAL_PUNCTUATION,
    E_UNICODE_TYPE_INITIAL_PUNCTUATION,
    E_UNICODE_TYPE_OTHER_PUNCTUATION,
    E_UNICODE_TYPE_OPEN_PUNCTUATION,
    E_UNICODE_TYPE_CURRENCY_SYMBOL,
    E_UNICODE_TYPE_MODIFIER_SYMBOL,
    E_UNICODE_TYPE_MATH_SYMBOL,
    E_UNICODE_TYPE_OTHER_SYMBOL,
    E_UNICODE_TYPE_LINE_SEPARATOR,
    E_UNICODE_TYPE_PARAGRAPH_SEPARATOR,
    E_UNICODE_TYPE_SPACE_SEPARATOR
} e_unicode_type_t;

E_IMPORT const char *const e_utf8_skip;

__BEGIN_DECLS

E_EXPORT bool e_unicode_validate(e_unicode32_t ch);

E_EXPORT bool e_unicode_isalnum(e_unicode32_t ch);
E_EXPORT bool e_unicode_iscntrl(e_unicode32_t ch);
E_EXPORT bool e_unicode_isdefined(e_unicode32_t ch);
E_EXPORT bool e_unicode_isdigit(e_unicode32_t ch);
E_EXPORT bool e_unicode_isgraph(e_unicode32_t ch);
E_EXPORT bool e_unicode_islower(e_unicode32_t ch);
E_EXPORT bool e_unicode_ismark(e_unicode32_t ch);
E_EXPORT bool e_unicode_isprint(e_unicode32_t ch);
E_EXPORT bool e_unicode_ispunct(e_unicode32_t ch);
E_EXPORT bool e_unicode_isspace(e_unicode32_t ch);
E_EXPORT bool e_unicode_istitle(e_unicode32_t ch);
E_EXPORT bool e_unicode_isupper(e_unicode32_t ch);
E_EXPORT bool e_unicode_isxdigit(e_unicode32_t ch);
E_EXPORT bool e_unicode_iswide(e_unicode32_t ch);
E_EXPORT bool e_unicode_iswide_cjk(e_unicode32_t ch);
E_EXPORT bool e_unicode_iszerowidth(e_unicode32_t ch);

E_EXPORT e_unicode32_t e_unicode_toupper(e_unicode32_t ch);
E_EXPORT e_unicode32_t e_unicode_tolower(e_unicode32_t ch);
E_EXPORT e_unicode32_t e_unicode_totitle(e_unicode32_t ch);

E_EXPORT int e_unicode_digit_value(e_unicode32_t ch);
E_EXPORT int e_unicode_xdigit_value(e_unicode32_t ch);

E_EXPORT e_unicode32_t e_utf8_get_char(const char *p);
#define e_utf8_next_char(p) (char *)((p) + e_utf8_skip[*(const uint8_t *)(p)])
E_EXPORT e_unicode32_t e_utf8_get_char_validated(const char *p, ssize_t max_len);
E_EXPORT char *e_utf8_offset_to_pointer(const char *str, off_t offset) E_NONNULL(1);
E_EXPORT off_t e_utf8_pointer_to_offset(const char *str, const char *pos) E_NONNULL(1, 2);

E_EXPORT e_unicode_type_t e_unicode_type(e_unicode32_t ch);

E_EXPORT size_t e_utf8_strlen(const char *p, ssize_t max) E_NONNULL(1);
E_EXPORT char *e_utf8_strncpy(char *dest, const char *src, size_t n) E_NONNULL(1, 2);
E_EXPORT char *e_utf8_strchr(const char *p, ssize_t len, e_unicode32_t c) E_NONNULL(1);
E_EXPORT char *e_utf8_strrchr(const char *p, ssize_t len, e_unicode32_t c) E_NONNULL(1);
E_EXPORT char *e_utf8_strreverse(const char *str, ssize_t len) E_GNUC_WARN_UNUSED_RESULT E_GNUC_MALLOC E_NONNULL(1);
E_EXPORT char *e_utf8_substring(const char *str, off_t start_pos, off_t end_pos) E_GNUC_WARN_UNUSED_RESULT E_GNUC_MALLOC E_NONNULL(1);

E_EXPORT bool e_utf8_validate(const char * __restrict str, ssize_t max_len, const char ** __restrict end) E_NONNULL(1);
E_EXPORT bool e_utf8_validate_len(const char * __restrict str, size_t max_len, const char ** __restrict end) E_NONNULL(1);

E_EXPORT size_t e_unicode32_to_utf8(e_unicode32_t c, char *outbuf);

E_EXPORT e_errno_t e_utf8_to_utf16(const e_unicode8_t * __restrict str, ssize_t str_len, ssize_t *nread, e_unicode16_t ** __restrict ret, ssize_t *nwrite) E_NONNULL(1, 4);
E_EXPORT e_errno_t e_utf8_to_ucs4(const e_unicode8_t * __restrict str, ssize_t str_len, ssize_t *nread, e_unicode32_t ** __restrict ret, ssize_t *nwrite) E_NONNULL(1, 4);
E_EXPORT e_errno_t e_utf8_to_ucs4_fast(const e_unicode8_t * __restrict str, ssize_t str_len, e_unicode32_t ** __restrict ret, ssize_t *nwrite) E_NONNULL(1, 3);
E_EXPORT e_errno_t e_utf16_to_ucs4(const e_unicode16_t * __restrict str, ssize_t str_len, ssize_t *nread, e_unicode32_t ** __restrict ret, ssize_t *nwrite) E_NONNULL(1, 4);
E_EXPORT e_errno_t e_utf16_to_utf8(const e_unicode16_t * __restrict str, ssize_t str_len, ssize_t *nread, e_unicode8_t ** __restrict ret, ssize_t *nwrite) E_NONNULL(1, 4);
E_EXPORT e_errno_t e_ucs4_to_utf16(const e_unicode32_t * __restrict str, ssize_t str_len, ssize_t *nread, e_unicode16_t ** __restrict ret, ssize_t *nwrite) E_NONNULL(1, 4);
E_EXPORT e_errno_t e_ucs4_to_utf8(const e_unicode32_t * __restrict str, ssize_t str_len, ssize_t *nread, e_unicode8_t ** __restrict ret, ssize_t *nwrite) E_NONNULL(1, 4);

__END_DECLS

#endif /* E_UNICODE_H */
