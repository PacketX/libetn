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


#ifndef E_STRING_H
#define E_STRING_H

#include <libetn/e_err.h>
#include <stdbool.h>
#include <wchar.h>
#include <stdarg.h>

typedef struct e_string_s e_string_t;

__BEGIN_DECLS

E_EXPORT e_string_t *e_string_new(const char *s) E_GNUC_WARN_UNUSED_RESULT E_GNUC_MALLOC;
E_EXPORT e_string_t *e_string_new_len(const char *s, ssize_t len) E_GNUC_WARN_UNUSED_RESULT E_GNUC_MALLOC;
E_EXPORT e_string_t *e_string_sized_new(size_t size) E_GNUC_WARN_UNUSED_RESULT E_GNUC_MALLOC;

E_EXPORT void e_string_free(e_string_t *string);
E_EXPORT e_string_t *e_string_ref(e_string_t *string) E_NONNULL(1);
E_EXPORT void e_string_unref(e_string_t *string);

E_EXPORT void e_string_get_c_string(e_string_t * __restrict string, char ** __restrict s, size_t * __restrict len) E_NONNULL(1);
E_EXPORT e_errno_t e_string_get_c_string_dup(e_string_t * __restrict string, char ** __restrict s, size_t * __restrict len) E_NONNULL(1, 2);
E_EXPORT bool e_string_equal(const e_string_t *s1, const e_string_t *s2) E_NONNULL(1, 2);
E_EXPORT uint e_string_hash(const e_string_t *string) E_NONNULL(1);
E_EXPORT e_errno_t e_string_assign(e_string_t * __restrict string, const char * __restrict s) E_NONNULL(1, 2);
E_EXPORT void e_string_truncate(e_string_t *string, size_t len) E_NONNULL(1);
E_EXPORT e_errno_t e_string_insert_len(e_string_t * __restrict string, ssize_t pos, const char * __restrict s, ssize_t len) E_NONNULL(1, 3);
E_EXPORT e_errno_t e_string_append(e_string_t * __restrict string, const char * __restrict s) E_NONNULL(1, 2);
E_EXPORT e_errno_t e_string_append_len(e_string_t * __restrict string, const char * __restrict s, ssize_t len) E_NONNULL(1, 2);
E_EXPORT e_errno_t e_string_append_c(e_string_t *string, char c) E_NONNULL(1);
E_EXPORT e_errno_t e_string_append_unichar(e_string_t *string, wchar_t wc) E_NONNULL(1);
E_EXPORT e_errno_t e_string_prepend(e_string_t * __restrict string, const char * __restrict s) E_NONNULL(1, 2);
E_EXPORT e_errno_t e_string_prepend_c(e_string_t *string, char c) E_NONNULL(1);
E_EXPORT e_errno_t e_string_prepend_unichar(e_string_t *string, wchar_t wc) E_NONNULL(1);
E_EXPORT e_errno_t e_string_prepend_len(e_string_t * __restrict string, const char * __restrict s, ssize_t len) E_NONNULL(1, 2);
E_EXPORT e_errno_t e_string_insert(e_string_t * __restrict string, ssize_t pos, const char * __restrict s) E_NONNULL(1, 3);
E_EXPORT e_errno_t e_string_insert_c(e_string_t *string, ssize_t pos, char c) E_NONNULL(1);
E_EXPORT e_errno_t e_string_insert_unichar(e_string_t *string, ssize_t pos, wchar_t wc) E_NONNULL(1);
E_EXPORT e_errno_t e_string_overwrite(e_string_t * __restrict string, size_t pos, const char * __restrict s) E_NONNULL(1, 3);
E_EXPORT e_errno_t e_string_overwrite_len(e_string_t * __restrict string, size_t pos, const char * __restrict s, ssize_t len) E_NONNULL(1, 3);
E_EXPORT e_errno_t e_string_erase(e_string_t *string, ssize_t pos, ssize_t len) E_NONNULL(1);
E_EXPORT void e_string_ascii_down(e_string_t *string) E_NONNULL(1);
E_EXPORT void e_string_ascii_up(e_string_t *string) E_NONNULL(1);
E_EXPORT e_errno_t e_string_vprintf(e_string_t * __restrict string, size_t * __restrict length, const char * __restrict fmt, va_list ap) E_NONNULL(1, 3);
E_EXPORT e_errno_t e_string_printf(e_string_t * __restrict string, size_t * __restrict length, const char * __restrict fmt, ...) E_NONNULL(1, 3);
E_EXPORT e_errno_t e_string_append_vprintf(e_string_t * __restrict string, size_t * __restrict length, const char * __restrict fmt, va_list ap) E_NONNULL(1, 3);
E_EXPORT e_errno_t e_string_append_printf(e_string_t * __restrict string, size_t * __restrict length, const char * __restrict fmt, ...) E_NONNULL(1, 3);

__END_DECLS

#endif /* E_STRING_H */
