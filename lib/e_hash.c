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


#include <libetn/e_hash.h>
#include <libetn/e_strfuncs.h>
#include <string.h>

bool e_hash_str_equal(const void *v1, const void *v2) {
    return strcmp((const char *)v1, (const char *)v2) == 0;
}//end e_hash_str_equal

uint e_hash_str(const void *v) {
    uint32_t            h;
    const signed char  *p;

    h = 5381;
    for(p = v; *p != '\0'; p++) {
        h = (h << 5) + h + *p;
    }//end for

    return (uint)h;
}//end e_hash_str

bool e_hash_str_case_equal(const void *v1, const void *v2) {
    return strcasecmp((const char *)v1, (const char *)v2) == 0;
}//end e_hash_str_case_equal

uint e_hash_str_case(const void *v) {
    uint32_t            h;
    const signed char  *p;

    h = 5381;
    for(p = v; *p != '\0'; p++) {
        h = (h << 5) + h + e_ascii_tolower(*p);
    }//end for

    return (uint)h;
}//end e_hash_str_case

bool e_hash_direct_equal(const void *v1, const void *v2) {
    return v1 == v2;
}//end e_hash_direct_equal

uint e_hash_direct(const void *v) {
    return (uint)(uintptr_t)v;
}//end e_hash_direct

bool e_hash_int_equal(const void *v1, const void *v2) {
    return *((const int *)v1) == *((const int *)v2);
}//end e_hash_int_equal

uint e_hash_int(const void *v) {
    return (uint)(*((const int *)v));
}//end e_hash_int

bool e_hash_int64_equal(const void *v1, const void *v2) {
    return *((const int64_t *)v1) == *((const int64_t *)v2);
}//end e_hash_int64_equal

uint e_hash_int64(const void *v) {
    return (uint)(*((const int64_t *)v));
}//end e_hash_int64

bool e_hash_double_equal(const void *v1, const void *v2) {
    return *((const double *)v1) == *((const double *)v2);
}//end e_hash_double_equal

uint e_hash_double(const void *v) {
    return (uint)(*((const double *)v));
}//end e_hash_double
