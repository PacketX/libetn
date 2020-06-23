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


#ifndef E_MACROS_H
#define E_MACROS_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/cdefs.h>
#include <inttypes.h>
#include <stdbool.h>

/*
 * Here we provide E_GNUC_EXTENSION as an alias for __extension__,
 * where this is valid. This allows for warningless compilation of
 * "long long" types even in the presence of '-ansi -pedantic'. 
 */
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 8)
#define E_GNUC_EXTENSION __extension__
#else
#define E_GNUC_EXTENSION
#endif

/* provide a string identifying the current function, non-concatenatable */
#if defined(__GNUC__) && defined(__cplusplus)
#define E_STRFUNC     ((const char *)(__PRETTY_FUNCTION__))
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define E_STRFUNC     ((const char *)(__func__))
#elif defined(__GNUC__) || (defined(_MSC_VER) && (_MSC_VER > 1300))
#define E_STRFUNC     ((const char *)(__FUNCTION__))
#else
#define E_STRFUNC     ((const char *)("???"))
#endif

/* provide definitions for some commonly used macros */
#define E_MAX(a, b)             (((a) > (b)) ? (a) : (b))
#define E_MIN(a, b)             (((a) < (b)) ? (a) : (b))
#define E_ABS(a)                (((a) < 0) ? -(a) : (a))
#define E_CLAMP(x, low, high)   (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

/*
 * The E_LIKELY and E_UNLIKELY macros let the programmer give hints to 
 * the compiler about the expected result of an expression. Some compilers
 * can use this information for optimizations.
 */
#if defined(__GNUC__) && (__GNUC__ > 2)

#if defined(__OPTIMIZE__)

#define _E_BOOLEAN_EXPR(expr)                           \
    E_GNUC_EXTENSION({                                  \
        bool _e_boolean_var_ = (expr) ? true : false;   \
        _e_boolean_var_;                                \
    })

#else

#define _E_BOOLEAN_EXPR(expr) (!!(expr))

#endif /* __OPTIMIZE__ */

#define E_LIKELY(expr)      (__builtin_expect(_E_BOOLEAN_EXPR((expr)), true))
#define E_UNLIKELY(expr)    (__builtin_expect(_E_BOOLEAN_EXPR((expr)), false))

#else

#define E_LIKELY(expr)      (!!(expr))
#define E_UNLIKELY(expr)    (!!(expr))

#endif

/* we can only use __typeof__ on GCC >= 4.8, and not when compiling C++ */
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)) && !defined(__cplusplus)
#define E_HAS_TYPEOF
#endif /* E_HAS_TYPEOF */

#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#define E_GNUC_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define E_GNUC_WARN_UNUSED_RESULT
#endif /* __GNUC__ */

#define E_NONNULL(args...)      __attribute__((nonnull(args)))
#define E_GNUC_MALLOC           __attribute__((malloc))
#define E_NO_RETURN             __attribute__((noreturn))
#define E_HOT                   __attribute__((hot))
#define E_GNUC_PURE             __attribute__((pure))

/* *printf() size_t modifier */
#ifndef PRIdSIZE
#define PRIdSIZE "zd"
#endif /* PRIdSIZE */

#ifndef PRIiSIZE
#define PRIiSIZE "zi"
#endif /* PRIiSIZE */

#ifndef PRIoSIZE
#define PRIoSIZE "zo"
#endif /* PRIoSIZE */

#ifndef PRIuSIZE
#define PRIuSIZE "zu"
#endif /* PRIuSIZE*/

#ifndef PRIxSIZE
#define PRIxSIZE "zx"
#endif /* PRIxSIZE */

#ifndef PRIXSIZE
#define PRIXSIZE "zX"
#endif /* PRIXSIZE */

/* pointer conversion macros */
#define E_POINTER_TO_TYPE(p, type) ((type)(uintptr_t)(p))
#define E_TYPE_TO_POINTER(v)       ((void *)(uintptr_t)(v))

/* count the number of elements in an array */
#define E_N_ELEMENTS(arr) (sizeof(arr) / sizeof((arr)[0]))

/* for macro */
#define E_STR_HELPER(x) #x
#define E_MACRO_STR(x) E_STR_HELPER(x)

/*
 * GCC 2.95 and later have "__restrict"; C99 compilers have
 * "restrict", and "configure" may have defined "restrict"
 */
#ifndef __restrict
#if !(2 < __GNUC__ || (2 == __GNUC__ && 95 <= __GNUC_MINOR__))
#if defined(restrict) || 199901L <= __STDC_VERSION__
#define __restrict restrict
#else
#define __restrict
#endif
#endif
#endif

typedef void e_free_cb_t(void *data);
typedef int  e_compare_cb_t(const void *a, const void *b);

#endif /* E_MACROS_H */
