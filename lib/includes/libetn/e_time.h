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


#ifndef E_TIME_H
#define E_TIME_H

#define E_USEC_PER_SEC 1000000
#define E_NSEC_PER_SEC 1000000000

/* Macros for converting between `struct timeval' and `struct timespec'.  */
#define E_TIMEVAL_TO_TIMESPEC(tv, ts)           \
    do {                                        \
        (ts)->tv_sec = (tv)->tv_sec;            \
        (ts)->tv_nsec = (tv)->tv_usec * 1000;   \
    } while(0)
#define E_TIMESPEC_TO_TIMEVAL(ts, tv)           \
    do {                                        \
        (tv)->tv_sec = (ts)->tv_sec;            \
        (tv)->tv_usec = (ts)->tv_nsec / 1000;   \
    } while(0)

#define E_TIMEVAL_CLEAR(tvp)        ((tvp)->tv_sec = (tvp)->tv_usec = 0)
#define E_TIMESPEC_CLEAR(tsp)       ((tsp)->tv_sec = (tsp)->tv_nsec = 0)

#define E_TIMEVAL_SET(tvp, sec, usec)   ((tvp)->tv_sec = sec, (tvp)->tv_usec = usec)
#define E_TIMESPEC_SET(tsp, sec, usec)  ((tsp)->tv_sec = sec, (tsp)->tv_nsec = nsec)

#define E_TIMEVAL_CMP(a, b, CMP1, CMP2)    (((a)->tv_sec CMP1 (b)->tv_sec) && ((a)->tv_usec CMP2 (b)->tv_usec))
#define E_TIMESPEC_CMP(a, b, CMP1, CMP2)   (((a)->tv_sec CMP1 (b)->tv_sec) && ((a)->tv_nsec CMP2 (b)->tv_nsec))

#define E_TIMEVAL_USEC(a)           (((a)->tv_sec * E_USEC_PER_SEC) + (a)->tv_usec)
#define E_TIMESPEC_NSEC(a)          (((a)->tv_sec * E_NSEC_PER_SEC) + (a)->tv_usec)

#define E_TIMEVAL_ADD(a, b, result)                         \
    do {                                                    \
        (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;       \
        (result)->tv_usec = (a)->tv_usec + (b)->tv_usec;    \
        if((result)->tv_usec >= E_USEC_PER_SEC) {           \
            ++(result)->tv_sec;                             \
            (result)->tv_usec -= E_USEC_PER_SEC;            \
        }                                                   \
    } while(0)

#define E_TIMESPEC_ADD(a, b, result)                        \
    do {                                                    \
        (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;       \
        (result)->tv_nsec = (a)->tv_nsec + (b)->tv_nsec;    \
        if((result)->tv_nsec >= E_NSEC_PER_SEC) {           \
            ++(result)->tv_sec;                             \
            (result)->tv_nsec -= E_NSEC_PER_SEC;            \
        }                                                   \
    } while(0)

#define E_TIMEVAL_SUB(a, b, result)                         \
    do {                                                    \
        (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;       \
        (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;    \
        if((result)->tv_usec < 0) {                         \
            --(result)->tv_sec;                             \
            (result)->tv_usec += E_NSEC_PER_SEC;            \
        }                                                   \
    } while(0)

#define E_TIMESPEC_SUB(a, b, result)                        \
    do {                                                    \
        (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;       \
        (result)->tv_nsec = (a)->tv_nsec - (b)->tv_nsec;    \
        if((result)->tv_nsec < 0) {                         \
            --(result)->tv_sec;                             \
            (result)->tv_nsec += E_NSEC_PER_SEC;            \
        }                                                   \
    } while(0)

#endif /* E_TIME_H */
