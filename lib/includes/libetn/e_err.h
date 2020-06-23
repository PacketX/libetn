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


#ifndef E_ERR_H
#define E_ERR_H

#include <libetn/e_visibility.h>
#include <libetn/e_macros.h>

typedef enum {
    E_OK = 0,
    E_ERR_NOFOUND,      /* No found */
    E_ERR_NOBUFS,       /* No buffer space available */
    E_ERR_INVAL,        /* Invalid argument */
    E_ERR_INVAL_IP,     /* Invalid IP address */
    E_ERR_INVAL_PORT,   /* Invalid Port number */
    E_ERR_EMPTY,        /* Empty */
    E_ERR_EXIST,        /* Exist */
    E_ERR_FAMEM,        /* Allocate memory failed */
    E_ERR_EXPR,         /* expr2match compile error */
    E_ERR_RANGE,        /* Range relate error */
    E_ERR_ACT,          /* Activated */
    E_ERR_DEACT,        /* Deactivated */
    E_ERR_UNRGNZ,       /* Unrecognized */
    E_ERR_AGAIN,        /* Again */
    E_ERR_NOTSUP,       /* Not supported */
    E_ERR_OVERFLOW,     /* Overflow */
    E_ERR_ACCES,        /* Permission denied */
    E_ERR_CHILD,        /* No child processes */
    E_ERR_C_ERR,        /* errno type error */
    E_ERR_MAX
} e_errno_t;

__BEGIN_DECLS

E_EXPORT const char *e_err_errno_string(e_errno_t err);

__END_DECLS

#endif /* E_ERR_H */
