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


#ifndef E_MEM_H
#define E_MEM_H

#include <libetn/e_macros.h>
#include <libetn/e_visibility.h>
#include <stdlib.h>

#if defined(__GNUC__)

/* GCC does the right thing */
#undef alloca
#define alloca(size) __builtin_alloca(size)

#elif defined(HAVE_ALLOCA_H)

/* a native and working alloca.h is there */
#include <alloca.h>

#endif /* alloca() */

/* allocation */
#define e_malloc    malloc
#define e_calloc    calloc
#define e_realloc   realloc
#define e_alloca    alloca
#define e_free      free

__BEGIN_DECLS

E_EXPORT void *e_memdup(void *mem, size_t byte_size) E_GNUC_WARN_UNUSED_RESULT E_GNUC_MALLOC E_HOT E_NONNULL(1);

static inline void *e_steal_pointer(void *pp) {
    void **ptr = (void **)pp;
    void *ref;

    ref = *ptr;
    *ptr = NULL;

    return ref;
}//end e_steal_pointer

/* type safety */
#ifdef E_HAS_TYPEOF
#define e_steal_pointer(pp) ((__typeof__(*pp))(e_steal_pointer)(pp))
#else  /* __GNUC__ */
/* This version does not depend on gcc extensions, but gcc does not warn
 * about incompatible-pointer-types: */
#define e_steal_pointer(pp) (0 ? (*(pp)) : (e_steal_pointer)(pp))
#endif /* __GNUC__ */

#ifdef E_HAS_TYPEOF

#define e_clear_pointer(pp, destroy)        \
    do {                                    \
        __typeof__((pp))_pp = (pp);         \
        __typeof__(*(pp))_ptr = *_pp;       \
        *_pp = NULL;                        \
        if(_ptr) {                          \
            (destroy)(_ptr);                \
        }                                   \
    } while(0)

#else /* __GNUC__ */

#define e_clear_pointer(pp, destroy)                                \
    do {                                                            \
        /* Only one access, please; work around type aliasing */    \
        union { char *in; void **out; } _pp;                        \
        void * _p;                                                  \
        /* This assignment is needed to avoid a gcc warning */      \
        void (*_destroy)(void *) = (void (*)(void *))(destroy);     \
        _pp.in = (char *)(pp);                                      \
        _p = *_pp.out;                                              \
        if(_p) {                                                    \
            *_pp.out = NULL;                                        \
            _destroy(_p);                                           \
        }                                                           \
    } while(0)

#endif /* __GNUC__ */

__END_DECLS

#endif /* E_MEM_H */
