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


#ifndef E_TIMER_H
#define E_TIMER_H

#include <libetn/e_err.h>
#include <stdbool.h>

typedef struct e_timer_s e_timer_t;

__BEGIN_DECLS

E_EXPORT e_timer_t *e_timer_new(void) E_GNUC_WARN_UNUSED_RESULT E_GNUC_MALLOC;
E_EXPORT void e_timer_free(e_timer_t *timer);
E_EXPORT e_errno_t e_timer_start(e_timer_t *timer) E_NONNULL(1);
E_EXPORT e_errno_t e_timer_stop(e_timer_t *timer) E_NONNULL(1);
E_EXPORT e_errno_t e_timer_reset(e_timer_t *timer) E_NONNULL(1);
E_EXPORT e_errno_t e_timer_continue(e_timer_t *timer) E_NONNULL(1);

/* elapsed is in seconds */
E_EXPORT e_errno_t e_timer_elapsed(e_timer_t * __restrict timer, double * __restrict elapsed, uint64_t * __restrict microseconds) E_NONNULL(1, 2);

/* return remaining time in microseconds when it is interruptible */
E_EXPORT uint64_t e_usleep(uint64_t microseconds, bool interruptible);

__END_DECLS

#endif /* E_TIMER_H */
