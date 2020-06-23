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


#ifndef E_TESTUTILS_H
#define E_TESTUTILS_H

#include <libetn/e_err.h>
#include <stdarg.h>

#define e_assert_true(expr) \
    do { \
        if(E_UNLIKELY(!(expr))) { \
            e_assertion_fatal_message(__FILE__, __LINE__, E_STRFUNC, "'" #expr "' should be 'true'"); \
        } \
    } while(0)

#define e_assert_false(expr) \
    do { \
        if(E_UNLIKELY(expr)) { \
            e_assertion_fatal_message(__FILE__, __LINE__, E_STRFUNC, "'" #expr "' should be 'false'"); \
        } \
    } while(0)

#define e_assert_errno(err_code, expr) \
    do { \
        e_errno_t __err_code = (expr); \
        if(E_UNLIKELY(__err_code != (err_code))) { \
            e_assertion_fatal_message(__FILE__, __LINE__, E_STRFUNC, "'" #expr "' should return '" #err_code "' instead of '%s'", e_err_errno_string(__err_code)); \
        } \
    } while(0)

#define e_assert_not_errno(err_code, expr) \
    do { \
        e_errno_t __err_code = (expr); \
        if(E_UNLIKELY(__err_code == (err_code))) { \
            e_assertion_fatal_message(__FILE__, __LINE__, E_STRFUNC, "'" #expr "' should not return '" #err_code "'"); \
        } \
    } while(0)

__BEGIN_DECLS

E_EXPORT void e_assertion_fatal_message(const char * __restrict file, int line, const char * __restrict func, const char * __restrict message, ...) E_NO_RETURN E_NONNULL(1, 3);

__END_DECLS

#endif /* E_TESTUTILS_H */
