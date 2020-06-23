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



#ifndef E_ETN_H
#define E_ETN_H

#include <libetn/e_err.h>

typedef struct e_etn_s e_etn_t;

__BEGIN_DECLS

E_EXPORT e_etn_t *e_etn_new(const char *filename) E_GNUC_WARN_UNUSED_RESULT E_GNUC_MALLOC E_NONNULL(1);
E_EXPORT void e_etn_free(e_etn_t *etn);
E_EXPORT e_etn_t *e_etn_ref(e_etn_t *etn) E_NONNULL(1);
E_EXPORT void e_etn_unref(e_etn_t *etn);

E_EXPORT void e_etn_public_suffix(e_etn_t * __restrict etn, const char * __restrict domain, const char ** __restrict ps, bool * __restrict icann) E_NONNULL(1, 2, 3, 4);
E_EXPORT void e_etn_eTLD_plus_one(e_etn_t * __restrict etn, const char * __restrict domain, const char ** __restrict eTLD) E_NONNULL(1, 2, 3);

__END_DECLS

#endif /* E_ETN_H */
