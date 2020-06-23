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


#ifndef E_HASH_H
#define E_HASH_H

#include <libetn/e_err.h>
#include <stdbool.h>

__BEGIN_DECLS

E_EXPORT bool e_hash_str_equal(const void *v1, const void *v2) E_GNUC_PURE E_HOT;
E_EXPORT uint e_hash_str(const void *v) E_GNUC_PURE E_HOT;

E_EXPORT bool e_hash_str_case_equal(const void *v1, const void *v2) E_GNUC_PURE E_HOT;
E_EXPORT uint e_hash_str_case(const void *v) E_GNUC_PURE E_HOT;

E_EXPORT bool e_hash_direct_equal(const void *v1, const void *v2) E_GNUC_PURE E_HOT;
E_EXPORT uint e_hash_direct(const void *v) E_GNUC_PURE E_HOT;

E_EXPORT bool e_hash_int_equal(const void *v1, const void *v2) E_GNUC_PURE E_HOT;
E_EXPORT uint e_hash_int(const void *v) E_GNUC_PURE E_HOT;

E_EXPORT bool e_hash_int64_equal(const void *v1, const void *v2) E_GNUC_PURE E_HOT;
E_EXPORT uint e_hash_int64(const void *v) E_GNUC_PURE E_HOT;

E_EXPORT bool e_hash_double_equal(const void *v1, const void *v2) E_GNUC_PURE E_HOT;
E_EXPORT uint e_hash_double(const void *v) E_GNUC_PURE E_HOT;

__END_DECLS

#endif /* E_HASH_H */
