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


#include <libetn/e_punycode.h>
#include <string.h>

/* https://github.com/libidn/libidn2/blob/master/lib/puny_encode.c */
/* https://github.com/libidn/libidn2/blob/master/lib/puny_decode.c */

#define e_punycode_encode_basic(cp) ((e_unicode32_t)(cp) < 0x80)
#define e_punycode_decode_basic(cp) ((cp >= 'a' && cp <= 'z') || (cp >= '0' && cp <='9') || (cp >= 'A' && cp <='Z') || cp == '-' || cp == '_')

const int base = 36;
const int tmin = 1;
const int tmax = 26;
const int skew = 38;
const int damp = 700;
const int initial_bias = 72;
const int initial_n = 0x80;
const int delimiter = 0x2D;

static inline char e_punycode_encode_digit(e_unicode32_t d, bool flag);
static inline unsigned e_punycode_decode_digit(int cp);
static inline e_unicode32_t e_punycode_adapt(e_unicode32_t delta, e_unicode32_t numpoints, bool firsttime);

e_errno_t e_punycode_encode(const e_unicode32_t *src, size_t src_len, char *dst, size_t dst_len, size_t *out_len) {
    size_t          out, max_out;
    e_unicode32_t   input_length, n, delta, h, b, bias, j, m, q, k, t;

    /* The Punycode spec assumes that the src length is the same type */
    /* of integer as a code point, so we need to convert the size_t to  */
    /* a punycode_uint, which could overflow.                           */

    if(E_UNLIKELY(src_len > UINT32_MAX)) {
        return E_ERR_OVERFLOW;
    }//end if
    input_length = (e_unicode32_t)src_len;

    /* Initialize the state: */
    n = initial_n;
    delta = 0;
    out = 0;
    max_out = dst_len;
    bias = initial_bias;

    /* Handle the basic code points: */
    for(j = 0; j < input_length; j++) {
        if(e_punycode_encode_basic(src[j])) {
            if(E_UNLIKELY(max_out - out < 2)) {
                return E_ERR_NOBUFS;
            }//end if
            dst[out++] = (char)src[j];
        }//end if
        else if(src[j] > 0x10FFFF || (src[j] >= 0xD800 && src[j] <= 0xDBFF)) {
            return E_ERR_INVAL;
        }//end if
    }//end for

    h = b = (e_unicode32_t)out;
    /* cannot overflow because out <= input_length <= UINT32_MAX */

    if(b > 0) {
        dst[out++] = delimiter;
    }//end if

    /* Main encoding loop: */
    while(h < input_length) {
        /* All non-basic code points < n have been     */
        /* handled already.  Find the next larger one: */

        for(m = UINT32_MAX, j = 0; j < input_length; j++) {
            /* if (e_punycode_encode_basic(src[j])) continue; */
            /* (not needed for Punycode) */
            if(src[j] >= n && src[j] < m) {
                m = src[j];
            }//end if
        }//end for

        /* Increase delta enough to advance the decoder's    */
        /* <n,i> state to <m,0>, but guard against overflow: */

        if(E_UNLIKELY(m - n > (UINT32_MAX - delta) / (h + 1))) {
            return E_ERR_OVERFLOW;
        }//end if
        delta += (m - n) * (h + 1);
        n = m;

        for(j = 0; j < input_length; j++) {
            /* Punycode does not need to check whether src[j] is basic: */
            if(src[j] < n /* || e_punycode_encode_basic(src[j]) */) {
                if(E_UNLIKELY(++delta == 0)) {
                    return E_ERR_OVERFLOW;
                }//end if
            }//end for

            if(src[j] == n) {
                /* Represent delta as a generalized variable-length integer: */

                for(q = delta, k = base;; k += base) {
                    if(E_UNLIKELY(out >= max_out)) {
                        return E_ERR_NOBUFS;
                    }//end if
                    t = k <= bias /* + tmin */ ? tmin :     /* +tmin not needed */
                    k >= bias + tmax ? tmax : k - bias;
                    if(q < t) {
                        break;
                    }//end if
                    dst[out++] = e_punycode_encode_digit(t + (q - t) % (base - t), false);
                    q = (q - t) / (base - t);
                }//end for

                dst[out++] = e_punycode_encode_digit(q, false);
                bias = e_punycode_adapt(delta, h + 1, h == b);
                delta = 0;
                ++h;
            }//end if
        }//end for
        ++delta, ++n;
    }//end while

    if(out_len) {
        *out_len = out;
    }//end if
    dst[out] = '\0';

    return E_OK;
}//end e_punycode_encode

e_errno_t e_punycode_decode(const char *src, size_t src_len, e_unicode32_t *dst, size_t dst_len, size_t *out_len) {
    size_t          b = 0, j, in;
    e_unicode32_t   n, out = 0, i, max_out, bias, oldi, w, k, digit, t;

    if(E_UNLIKELY(src_len == 0)) {
        return E_ERR_INVAL;
    }//end if

    /* Check that all chars are basic */
    for(j = 0; j < src_len; j++) {
        if(!e_punycode_decode_basic(src[j])) {
            return E_ERR_INVAL;
        }//end if

        if(src[j] == delimiter) {
            b = j;
        }//end if
    }//end for

    max_out = dst_len > UINT32_MAX ? UINT32_MAX : (e_unicode32_t)dst_len;

    if(src[b] == delimiter) {
        /* do not accept leading or trailing delimiter
         *   - leading delim must be omitted if there is no ASCII char in u-label
         *   - trailing delim means there where no non-ASCII chars in u-label
         */
        if(!b || b == src_len - 1) {
            return E_ERR_INVAL;
        }//end if

        if(E_UNLIKELY(b >= max_out)) {
            return E_ERR_NOBUFS;
        }//end if

        /* Check that all chars before last delimiter are basic chars */
        /* and copy the first b code points to the dst. */
        for(j = 0; j < b; j++) {
            dst[out++] = src[j];
        }//end for

        b += 1; /* advance to non-basic char encoding */
    }//end if

    /* Initialize the state: */
    n = initial_n;
    i = 0;
    bias = initial_bias;

    /* Main decoding loop:  Start just after the last delimiter if any  */
    /* basic code points were copied; start at the beginning otherwise. */
    for(in = b; in < src_len; out++) {
        /* in is the index of the next ASCII code point to be consumed, */
        /* and out is the number of code points in the output array.    */

        /* Decode a generalized variable-length integer into delta,  */
        /* which gets added to i.  The overflow checking is easier   */
        /* if we increase i as we go, then subtract off its starting */
        /* value at the end to obtain delta.                         */

        for(oldi = i, w = 1, k = base; ; k += base) {
            if(E_UNLIKELY(in >= src_len)) {
                return E_ERR_INVAL;
            }//end if
            digit = e_punycode_decode_digit(src[in++]);
            if(digit >= base) {
                return E_ERR_INVAL;
            }//end if
            if(E_UNLIKELY(digit > (UINT32_MAX - i) / w)) {
                return E_ERR_OVERFLOW;
            }//end if
            i += digit * w;
            t = k <= bias /* + tmin */ ? tmin :     /* +tmin not needed */
            k >= bias + tmax ? tmax : k - bias;
            if(digit < t) {
                break;
            }//end if
            if(E_UNLIKELY(w > UINT32_MAX / (base - t))) {
                return E_ERR_OVERFLOW;
            }//end if
            w *= (base - t);
        }//end for

        bias = e_punycode_adapt(i - oldi, out + 1, oldi == 0);

        /* i was supposed to wrap around from out+1 to 0,   */
        /* incrementing n each time, so we'll fix that now: */
        if(E_UNLIKELY(i / (out + 1) > UINT32_MAX - n)) {
            return E_ERR_OVERFLOW;
        }//end if
        n += i / (out + 1);
        if(n > 0x10FFFF || (n >= 0xD800 && n <= 0xDBFF)) {
            return E_ERR_INVAL;
        }//end if
        i %= (out + 1);

        /* Insert n at position i of the dst: */

        /* not needed for Punycode: */
        /* if (e_punycode_decode_digit(n)) return punycode_bad_input; */
        if(E_UNLIKELY(out >= UINT32_MAX)) {
            return E_ERR_OVERFLOW;
        }//end if

        memmove(dst + i + 1, dst + i, (out - i) * sizeof(e_unicode32_t));
        dst[i++] = n;
    }//end for

    if(out_len) {
        *out_len = out;
    }//end if
    dst[out] = '\0';

    return E_OK;
}//end e_punycode_decode


/* ===== private function ===== */
static inline char e_punycode_encode_digit(e_unicode32_t d, bool flag) {
    return d + 22 + 75 * (d < 26) - ((flag != false) << 5);
    /*  0..25 map to ASCII a..z or A..Z */
    /* 26..35 map to ASCII 0..9         */
}//end e_punycode_encode_digit

static inline unsigned e_punycode_decode_digit(int cp) {
    if(cp >= 'a' && cp <= 'z') {
        return cp - 'a';
    }//end if
    if(cp >= '0' && cp <= '9') {
        return cp - '0' + 26;
    }//end if
    if(cp >= 'A' && cp <= 'Z') {
        return cp - 'A';
    }//end if

    return 0;
}//end e_punycode_decode_digit

static inline e_unicode32_t e_punycode_adapt(e_unicode32_t delta, e_unicode32_t numpoints, bool firsttime) {
    e_unicode32_t k;

    delta = firsttime ? delta / damp : delta >> 1;
    /* delta >> 1 is a faster way of doing delta / 2 */
    delta += delta / numpoints;

    for(k = 0; delta > ((base - tmin) * tmax) / 2; k += base) {
        delta /= base - tmin;
    }//end for

    return k + (base - tmin + 1) * delta / (delta + skew);
}//end e_punycode_adapt
