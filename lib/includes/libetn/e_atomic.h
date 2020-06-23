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


#ifndef E_ATOMIC_H
#define E_ATOMIC_H

#include <libetn/e_err.h>
#include <inttypes.h>
#include <signal.h>
#include <stdint.h>

typedef volatile sig_atomic_t   e_sig_atomic_t;
typedef volatile int64_t        e_atomic_int_t;
typedef volatile uint64_t       e_atomic_uint_t;
typedef e_atomic_int_t          e_atomic_t;

/* *printf() e_atomic_int_t and e_atomic_uint_t modifier */
#define E_PRIdATOMIC PRId64
#define E_PRIiATOMIC PRIi64
#define E_PRIoATOMIC PRIo64
#define E_PRIuATOMIC PRIu64
#define E_PRIxATOMIC PRIx64
#define E_PRIXATOMIC PRIX64

#define e_atomic_get(atomic)                        (*(atomic))
#define e_atomic_set(atomic, newval)                (*(atomic) = newval)
#define e_atomic_cmp_set(atomic, oldval, newval)    __sync_bool_compare_and_swap(atomic, oldval, newval)
#define e_atomic_add(atomic, val)                   __sync_fetch_and_add(atomic, val)
#define e_atomic_sub(atomic, val)                   __sync_fetch_and_sub(atomic, val)
#define e_atomic_inc(atomic)                        __sync_fetch_and_add(atomic, 1)
#define e_atomic_dec(atomic)                        __sync_fetch_and_sub(atomic, 1)
#define e_atomic_dec_and_test(atomic)               (__sync_fetch_and_sub(atomic, 1) == 1)
#define e_atomic_memory_barrier()                   __sync_synchronize()
#define e_atomic_trylock(lock)                      (*(lock) == 0 && e_atomic_cmp_set(lock, 0, 1))
#define e_atomic_unlock(lock)                       (*(lock) = 0)
#define e_atomic_cpu_pause()                        __asm__("pause")

#endif /* E_ATOMIC_H */
