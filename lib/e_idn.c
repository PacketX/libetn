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


#include <libetn/e_idn.h>
#include <libetn/e_unicode.h>
#include <libetn/e_punycode.h>
#include <libetn/e_string.h>
#include <libetn/e_strfuncs.h>
#include <libetn/e_mem.h>

e_errno_t e_idn_encode(const char *fqdn, size_t fqdn_len, char **encoded_fqdn, size_t *encoded_fqdn_len) {
    char            **domainv, buf[E_STRBUF * 2];
    bool            dont_punycode;
    size_t          i, ntoken, len, punycode_len, k;
    ssize_t         j, nread, nwrite;
    e_errno_t       err;
    e_string_t      *encoded_str;
    e_unicode32_t   *unicode;

    if(E_UNLIKELY(strlen(fqdn) == 0)) {
        return E_ERR_INVAL;
    }//end if

    encoded_str = e_string_sized_new(E_STRBUF * 2);
    if(E_UNLIKELY(!encoded_str)) {
        return E_ERR_C_ERR;
    }//end if

    domainv = e_strsplit_set(fqdn, ".", &ntoken);
    if(E_UNLIKELY(!domainv)) {
        e_string_free(encoded_str);
        return E_ERR_C_ERR;
    }//end if

    if(E_UNLIKELY(ntoken == 0)) {
        e_string_free(encoded_str);
        e_strfreev(domainv);
        return E_ERR_INVAL;
    }//end if

    for(i = 0 ; i < ntoken ; i++) {
        if(E_UNLIKELY(!domainv[i])) {
            e_string_free(encoded_str);
            e_strfreev(domainv);
            return E_ERR_INVAL;
        }//end if
        len = strlen(domainv[i]);

        err = e_utf8_to_ucs4((const e_unicode8_t *)domainv[i], len, &nread, &unicode, &nwrite);
        if(E_UNLIKELY(err != E_OK)) {
            e_strfreev(domainv);
            e_string_free(encoded_str);
            return err;
        }//end if
        if(E_UNLIKELY(len != nread)) {
            e_strfreev(domainv);
            e_free(unicode);
            e_string_free(encoded_str);
            return E_ERR_INVAL;
        }//end if

        /* verify unicode */
        for(j = 0 ; j < nwrite ; j++) {
            if(!e_unicode_isgraph(unicode[j])) {
                e_strfreev(domainv);
                e_free(unicode);
                e_string_free(encoded_str);
                return E_ERR_INVAL;
            }//end if
        }//end for

        /* check if need to punycode */
        dont_punycode = true;
        if(nread != nwrite) {
            for(j = 0 ; j < nwrite ; j++) {
                if(domainv[i][j] != (char)unicode[j]) {
                    dont_punycode = false;
                    break;
                }//end if
            }//end for
        }//end if

        if(dont_punycode) {
            for(k = 0 ; k < len ; k++) {
                domainv[i][k] = e_ascii_tolower(domainv[i][k]);
            }//end for
            if(i + 1 != ntoken) {
                err = e_string_append_printf(encoded_str, NULL, "%s.", domainv[i]);
            }//end if
            else {
                err = e_string_append(encoded_str, domainv[i]);
            }//end else
            if(E_UNLIKELY(err != E_OK)) {
                e_strfreev(domainv);
                e_free(unicode);
                e_string_free(encoded_str);
                return err;
            }//end if
            e_free(unicode);
            continue;
        }//end if

        err = e_punycode_encode(unicode, (size_t)nwrite, buf, sizeof(buf), &punycode_len);
        if(E_UNLIKELY(err != E_OK)) {
            e_strfreev(domainv);
            e_free(unicode);
            e_string_free(encoded_str);
            return err;
        }//end if

        for(k = 0 ; k < punycode_len ; k++) {
            buf[k] = e_ascii_tolower(buf[k]);
        }//end for

        if(i + 1 != ntoken) {
            err = e_string_append_printf(encoded_str, NULL, "xn--%s.", buf);
        }//end if
        else {
            err = e_string_append_printf(encoded_str, NULL, "xn--%s", buf);
        }//end else
        if(E_UNLIKELY(err != E_OK)) {
            e_strfreev(domainv);
            e_free(unicode);
            e_string_free(encoded_str);
            return err;
        }//eend if

        e_free(unicode);
    }//end for

    e_strfreev(domainv);

    err = e_string_get_c_string_dup(encoded_str, encoded_fqdn, encoded_fqdn_len);
    if(E_UNLIKELY(err != E_OK)) {
        e_string_free(encoded_str);
        return err;
    }//end if

    e_string_free(encoded_str);
    return E_OK;
}//end e_idn_encode

e_errno_t e_idn_decode(const char *encoded_fqdn, size_t encoded_fqdn_len, char **decoded_fqdn, size_t *decoded_fqdn_len) {
    char            **domainv, *domain;
    bool            dont_punycode;
    size_t          i, ntoken, len, punycode_len;
    ssize_t         nread, nwrite;
    e_errno_t       err;
    e_string_t      *decoded_str;
    e_unicode32_t   unicode[E_STRBUF * 2];

    /* verify encoding */
    for(i = 0 ; i < encoded_fqdn_len ; i++) {
        if(!e_ascii_isgraph(encoded_fqdn[i])) {
            return E_ERR_INVAL;
        }//end if
    }//end for

    decoded_str = e_string_sized_new(E_STRBUF * 2);
    if(E_UNLIKELY(!decoded_str)) {
        return E_ERR_C_ERR;
    }//end if

    domainv = e_strsplit_set(encoded_fqdn, ".", &ntoken);
    if(E_UNLIKELY(!domainv)) {
        e_string_free(decoded_str);
        return E_ERR_C_ERR;
    }//end if

    for(i = 0 ; i < ntoken ; i++) {
        if(E_UNLIKELY(!domainv[i])) {
            e_string_free(decoded_str);
            e_strfreev(domainv);
            return E_ERR_INVAL;
        }//end if
        len = strlen(domainv[i]);

        dont_punycode = true;
        if(e_str_has_prefix(domainv[i], "xn--")) {
            dont_punycode = false;
        }//end if

        if(dont_punycode) {
            if(i + 1 != ntoken) {
                err = e_string_append_printf(decoded_str, NULL, "%s.", domainv[i]);
            }//end if
            else {
                err = e_string_append(decoded_str, domainv[i]);
            }//end else
            if(E_UNLIKELY(err != E_OK)) {
                e_strfreev(domainv);
                e_string_free(decoded_str);
                return err;
            }//end if
            continue;
        }//end if

        if(E_UNLIKELY(len < 4)) {
            e_strfreev(domainv);
            e_string_free(decoded_str);
            return E_ERR_INVAL;
        }//end if

        err = e_punycode_decode(domainv[i] + 4, len - 4, unicode, E_N_ELEMENTS(unicode), &punycode_len);
        if(E_UNLIKELY(err != E_OK)) {
            e_strfreev(domainv);
            e_string_free(decoded_str);
            return err;
        }//end if

        err = e_ucs4_to_utf8(unicode, (ssize_t)punycode_len, &nread, (e_unicode8_t **)&domain, &nwrite);
        if(E_UNLIKELY(err != E_OK)) {
            e_strfreev(domainv);
            e_string_free(decoded_str);
            return err;
        }//end if

        if(i + 1 != ntoken) {
            err = e_string_append_printf(decoded_str, NULL, "%s.", domain);
        }//end if
        else {
            err = e_string_append_printf(decoded_str, NULL, "%s", domain);
        }//end else
        if(E_UNLIKELY(err != E_OK)) {
            e_strfreev(domainv);
            e_string_free(decoded_str);
            e_free(domain);
            return err;
        }//end if

        e_free(domain);
    }//end for

    e_strfreev(domainv);

    err = e_string_get_c_string_dup(decoded_str, decoded_fqdn, decoded_fqdn_len);
    if(E_UNLIKELY(err != E_OK)) {
        e_string_free(decoded_str);
        return err;
    }//end if

    e_string_free(decoded_str);
    return E_OK;
}//end e_idn_decode
