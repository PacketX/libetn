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


#include <libetn/e_timer.h>
#include <libetn/e_time.h>
#include <libetn/e_mem.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>

struct e_timer_s {
    uint64_t start;
    uint64_t end;

    bool active;
};

static inline int64_t e_timer_get_monotonic_time(void);

e_timer_t *e_timer_new(void) {
    e_timer_t *timer;

    timer = e_calloc(1, sizeof(e_timer_t));
    if(E_UNLIKELY(!timer)) {
        return NULL;
    }//end if
    timer->active = true;
    timer->start = e_timer_get_monotonic_time();

    return timer;
}//end e_timer_new

void e_timer_free(e_timer_t *timer) {
    if(E_LIKELY(timer)) {
        e_free(timer);
    }//end if
}//end e_timer_free

e_errno_t e_timer_start(e_timer_t *timer) {
    int64_t t;

    t = e_timer_get_monotonic_time();
    if(E_UNLIKELY(t == -1)) {
        return E_ERR_C_ERR;
    }//end if
    timer->active = true;
    timer->start = t;
    return E_OK;
}//end e_timer_start

e_errno_t e_timer_stop(e_timer_t *timer) {
    int64_t t;

    t = e_timer_get_monotonic_time();
    if(E_UNLIKELY(t == -1)) {
        return E_ERR_C_ERR;
    }//end if
    timer->active = false;
    timer->end = t;
    return E_OK;
}//end e_timer_stop

e_errno_t e_timer_reset(e_timer_t *timer) {
    int64_t t;

    t = e_timer_get_monotonic_time();
    if(E_UNLIKELY(t == -1)) {
        return E_ERR_C_ERR;
    }//end if
    timer->start = t;
    return E_OK;
}//end e_timer_reset

e_errno_t e_timer_continue(e_timer_t *timer) {
    int64_t  t;
    uint64_t elapsed;

    if(timer->active) {
        return E_ERR_DEACT;
    }//end if

    elapsed = timer->end - timer->start;
    t = e_timer_get_monotonic_time();
    if(E_UNLIKELY(t == -1)) {
        return E_ERR_C_ERR;
    }//end if

    timer->start = t;
    timer->start -= elapsed;
    timer->active = true;
    return E_OK;
}//end e_timer_continue

e_errno_t e_timer_elapsed(e_timer_t *timer, double *elapsed, uint64_t *microseconds) {
    double  total;
    int64_t t, tmp;

    if(timer->active) {
        t = e_timer_get_monotonic_time();
        if(E_UNLIKELY(t == -1)) {
            return E_ERR_C_ERR;
        }//end if
        timer->end = t;
    }//end if

    tmp = timer->end - timer->start;
    total = tmp / 1e6;

    if(microseconds) {
        *microseconds = tmp % 1000000;
    }//end if

    *elapsed = total;
    return E_OK;
}//end e_timer_elapsed

uint64_t e_usleep(uint64_t microseconds, bool interruptible) {
    struct timeval  tv;
    struct timespec request, remaining;

    memset(&remaining, 0, sizeof(struct timespec));
    request.tv_sec = microseconds / E_USEC_PER_SEC;
    request.tv_nsec = 1000 * (microseconds % E_USEC_PER_SEC);

    if(interruptible) {
        nanosleep(&request, &remaining);
    }//end if
    else {
        while(nanosleep(&request, &remaining) == -1 && errno == EINTR) {
            request = remaining;
        }//end while
    }//end else

    E_TIMESPEC_TO_TIMEVAL(&remaining, &tv);
    return tv.tv_sec * E_USEC_PER_SEC + tv.tv_usec;
}//end e_usleep


/* ===== private function ===== */
static inline int64_t e_timer_get_monotonic_time(void) {
    int             ret;
    struct timespec ts;

    ret = clock_gettime(CLOCK_MONOTONIC, &ts);
    if(E_UNLIKELY(ret != 0)) {
        return (int64_t)-1;
    }//end if

    return (((int64_t)ts.tv_sec) * 1000000) + (ts.tv_nsec / 1000);
}//end e_timer_get_monotonic_time
