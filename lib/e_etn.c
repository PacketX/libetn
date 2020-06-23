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


#include <libetn/e_etn.h>
#include <libetn/e_refcount.h>
#include <libetn/e_mem.h>
#include <libetn/e_strfuncs.h>
#include <arpa/inet.h>
#include <string.h>

#define E_ETN_MAGIC 0x9601042d

/**
 * RFC 1035: 2.3.4
 * http://www.freesoft.org/CIE/RFC/1035/9.htm
 * labels          63 octets or less
 * names           255 octets or less
 * UDP messages    512 octets or less
 */
#define E_ETN_DOMAIN_MAX 255

#define E_ETN_GET_UINT32(buf, off)              \
    E_GNUC_EXTENSION({                          \
        uint32_t v;                             \
        v = htonl(*((uint32_t *)(buf + off)));  \
        off += sizeof(uint32_t);                \
        (uint32_t)v;                            \
    })

#define E_ETN_NOT_FOUND 0xFFFFFFFF

struct e_etn_s {
    uint32_t            nodes_bits_children;
    uint32_t            nodes_bits_ICANN;
    uint32_t            nodes_bits_text_offset;
    uint32_t            nodes_bits_text_length;
    uint32_t            children_bits_wildcard;
    uint32_t            children_bits_node_type;
    uint32_t            children_bits_hi;
    uint32_t            children_bits_lo;
    uint32_t            node_type_normal;
    uint32_t            node_type_exception;
    uint32_t            node_type_parent_only;
    uint32_t            num_TLD;
    uint32_t            text_length;
    char                *text;
    uint32_t            nodes_length;
    uint32_t            *nodes;
    uint32_t            children_length;
    uint32_t            *children;
    e_atomic_refcount_t ref_count;
};

static inline e_errno_t e_etn_load_file(e_etn_t *etn, const char *filename);
static inline int e_etn_strncmp(const char *s1, size_t s1_len, const char *s2, size_t s2_len);
static inline uint32_t e_etn_find(e_etn_t *etn, const char *label, uint32_t lo, uint32_t hi);
static inline const char *e_etn_node_label(e_etn_t *etn, uint32_t i, size_t *len);

e_etn_t *e_etn_new(const char *filename) {
    e_etn_t     *etn;
    e_errno_t   err;

    etn = e_calloc(1, sizeof(e_etn_t));
    if(E_UNLIKELY(!etn)) {
        return NULL;
    }//end if
    e_atomic_refcount_init(&(etn->ref_count));

    err = e_etn_load_file(etn, filename);
    if(E_UNLIKELY(err != E_OK)) {
        e_etn_free(etn);
        return NULL;
    }//end if

    return etn;
}//end e_etn_new

void e_etn_free(e_etn_t *etn) {
    e_etn_unref(etn);
}//end e_etn_free

e_etn_t *e_etn_ref(e_etn_t *etn) {
    e_atomic_refcount_inc(&(etn->ref_count));
    return etn;
}//end e_etn_ref

void e_etn_unref(e_etn_t *etn) {
    if(E_LIKELY(etn) && e_atomic_refcount_dec(&(etn->ref_count))) {
        if(E_LIKELY(etn->children)) {
            e_free(etn->children);
        }//end if
        if(E_LIKELY(etn->nodes)) {
            e_free(etn->nodes);
        }//end if
        if(E_LIKELY(etn->text)) {
            e_free(etn->text);
        }//end if
        e_free(etn);
    }//end if
}//end e_etn_unref

void e_etn_public_suffix(e_etn_t *etn, const char *domain, const char **ps, bool *icann) {
    char        *s, *tmp;
    bool        wildcard;
    size_t      suffix, len;
    ssize_t     dot;
    uint32_t    lo, hi, f, u, type;

    suffix = len = strlen(domain);
    if(E_UNLIKELY(len > E_ETN_DOMAIN_MAX)) {
        return;
    }//end if

    s = e_strndupa(domain, len);
    lo = 0;
    hi = etn->num_TLD;
    wildcard = false;

    while(true) {
        tmp = strrchr(s, '.');
        dot = tmp ? tmp - s : -1;

        if(wildcard) {
            suffix = 1 + dot;
        }//end if
        if(lo == hi) {
            break;
        }//end if

        f = e_etn_find(etn, s + 1 + dot, lo, hi);
        if(f == E_ETN_NOT_FOUND) {
            break;
        }//end if

        u = etn->nodes[f] >> (etn->nodes_bits_text_offset + etn->nodes_bits_text_length);
        *icann = (u & ((1 << etn->nodes_bits_ICANN) - 1)) != 0 ? true : false;
        u >>= etn->nodes_bits_ICANN;
        u = etn->children[u & ((1 << etn->nodes_bits_children) - 1)];
        lo = u & ((1 << etn->children_bits_lo) - 1);
        u >>= etn->children_bits_lo;
        hi = u & ((1 << etn->children_bits_hi) - 1);
        u >>= etn->children_bits_hi;

        type = u & ((1 << etn->children_bits_node_type) - 1);
        if(type == etn->node_type_normal) {
            suffix = 1 + dot;
        }//end if
        else if(type == etn->node_type_exception) {
            suffix = 1 + strlen(s);
            break;
        }//end if

        u >>= etn->children_bits_node_type;
        wildcard = (u & ((1 << etn->children_bits_wildcard) - 1)) != 0 ? true : false;

        if(dot == -1) {
            break;
        }//end if
        s[dot] = '\0';
    }//end while

    if(suffix == len) {
        /* if no rules match, the prevailing rule is "*" */
        tmp = strrchr(domain, '.');
        *ps = tmp ? domain + 1 + (tmp - domain) : domain;
        return;
    }//end if

    *ps = domain + suffix;
}//end e_etn_public_suffix

void e_etn_eTLD_plus_one(e_etn_t *etn, const char *domain, const char **eTLD) {
    bool        icann;
    char        *s, *tmp;
    size_t      domain_len, suffix_len, i;
    const char  *suffix;

    e_etn_public_suffix(etn, domain, &suffix, &icann);

    domain_len = strlen(domain);
    suffix_len = strlen(suffix);

    if(E_UNLIKELY(domain_len <= suffix_len)) {
        *eTLD = "";
        return;
    }//end if

    i = domain_len - suffix_len - 1;
    if(domain[i] != '.') {
        *eTLD = "";
        return;
    }//end if

    s = e_strndupa(domain, domain_len);
    s[i] = '\0';
    tmp = strrchr(s, '.');

    *eTLD = tmp ? domain + 1 + (tmp - s) : domain;
}//end e_etn_eTLD_plus_one


/* ===== private function ===== */
static inline e_errno_t e_etn_load_file(e_etn_t *etn, const char *filename) {
    FILE        *fp;
    off_t       off;
    size_t      nread, i;
    uint8_t     buf[4096];
    uint32_t    magic;

    fp = fopen(filename, "rb");
    if(!fp) {
        return E_ERR_INVAL;
    }//end if

    memset(buf, 0, sizeof(buf));
    off = 0;
    nread = fread(buf, sizeof(uint32_t), 14, fp);
    if(nread != 14) {
        fclose(fp);
        return E_ERR_INVAL;
    }//end if

    /* check magic number */
    magic = E_ETN_GET_UINT32(buf, off);
    if(magic != E_ETN_MAGIC) {
        fclose(fp);
        return E_ERR_INVAL;
    }//end if

    /* start to read simple integer part */
    etn->nodes_bits_children = E_ETN_GET_UINT32(buf, off);
    etn->nodes_bits_ICANN = E_ETN_GET_UINT32(buf, off);
    etn->nodes_bits_text_offset = E_ETN_GET_UINT32(buf, off);
    etn->nodes_bits_text_length = E_ETN_GET_UINT32(buf, off);
    etn->children_bits_wildcard = E_ETN_GET_UINT32(buf, off);
    etn->children_bits_node_type = E_ETN_GET_UINT32(buf, off);
    etn->children_bits_hi = E_ETN_GET_UINT32(buf, off);
    etn->children_bits_lo = E_ETN_GET_UINT32(buf, off);
    etn->node_type_normal = E_ETN_GET_UINT32(buf, off);
    etn->node_type_exception = E_ETN_GET_UINT32(buf, off);
    etn->node_type_parent_only = E_ETN_GET_UINT32(buf, off);
    etn->num_TLD = E_ETN_GET_UINT32(buf, off);

    /* check */
    if(E_UNLIKELY(etn->nodes_bits_text_length + etn->nodes_bits_text_offset + etn->nodes_bits_ICANN + etn->nodes_bits_children > 32)) {
        fclose(fp);
        return E_ERR_INVAL;
    }//end if

    if(E_UNLIKELY(etn->children_bits_lo + etn->children_bits_hi + etn->children_bits_node_type + etn->children_bits_wildcard > 32)) {
        fclose(fp);
        return E_ERR_INVAL;
    }//end if

    if(E_UNLIKELY(etn->nodes_bits_children != 10 ||
        etn->nodes_bits_ICANN != 1 ||
        etn->nodes_bits_text_offset != 15 ||
        etn->nodes_bits_text_length != 6 ||
        etn->children_bits_wildcard != 1 ||
        etn->children_bits_node_type != 2 ||
        etn->children_bits_hi != 14 ||
        etn->children_bits_lo != 14||
        etn->node_type_normal != 0 ||
        etn->node_type_exception != 1 ||
        etn->node_type_parent_only != 2)) {
        fclose(fp);
        return E_ERR_INVAL;
    }//end if

    /* read text */
    etn->text_length = E_ETN_GET_UINT32(buf, off);
    if(E_UNLIKELY(etn->text_length == 0)) {
        fclose(fp);
        return E_ERR_INVAL;
    }//end if

    etn->text = e_malloc(etn->text_length + 1);
    if(E_UNLIKELY(!etn->text)) {
        fclose(fp);
        return E_ERR_FAMEM;
    }//end if
    nread = fread(etn->text, sizeof(char), etn->text_length, fp);
    if(E_UNLIKELY(nread != etn->text_length)) {
        fclose(fp);
        return E_ERR_C_ERR;
    }//end if
    etn->text[etn->text_length] = '\0';
    off += etn->text_length;

    /* read node */
    memset(buf, 0, sizeof(buf));
    nread = fread(buf, sizeof(uint32_t), 1, fp);
    if(nread != 1) {
        fclose(fp);
        return E_ERR_INVAL;
    }//end if
    off = 0;

    etn->nodes_length = E_ETN_GET_UINT32(buf, off);
    if(E_UNLIKELY(etn->nodes_length == 0)) {
        fclose(fp);
        return E_ERR_INVAL;
    }//end if

    etn->nodes = e_malloc(etn->nodes_length * sizeof(uint32_t));
    if(E_UNLIKELY(!etn->nodes)) {
        fclose(fp);
        return E_ERR_FAMEM;
    }//end if

    nread = fread(etn->nodes, sizeof(uint32_t), etn->nodes_length, fp);
    if(nread != etn->nodes_length) {
        fclose(fp);
        return E_ERR_INVAL;
    }//end if

    for(i = 0 ; i < nread ; i++) {
        etn->nodes[i] = htonl(etn->nodes[i]);
    }//end for

    /* read children */
    memset(buf, 0, sizeof(buf));
    nread = fread(buf, sizeof(uint32_t), 1, fp);
    if(nread != 1) {
        fclose(fp);
        return E_ERR_INVAL;
    }//end if
    off = 0;

    etn->children_length = E_ETN_GET_UINT32(buf, off);
    if(E_UNLIKELY(etn->children_length == 0)) {
        fclose(fp);
        return E_ERR_INVAL;
    }//end if

    etn->children = e_malloc(etn->children_length * sizeof(uint32_t));
    if(E_UNLIKELY(!etn->children)) {
        fclose(fp);
        return E_ERR_FAMEM;
    }//end if

    nread = fread(etn->children, sizeof(uint32_t), etn->children_length, fp);
    if(nread != etn->children_length) {
        fclose(fp);
        return E_ERR_INVAL;
    }//end if

    for(i = 0 ; i < nread ; i++) {
        etn->children[i] = htonl(etn->children[i]);
    }//end for

    fclose(fp);
    return E_OK;
}//end e_etn_load_file

static inline int e_etn_strncmp(const char *s1, size_t s1_len, const char *s2, size_t s2_len) {
    size_t i, len;

    len = s1_len;
    if(len > s2_len) {
        len = s2_len;
    }//end if

    for(i = 0; i < len; i++) {
        if(s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }//end if
    }//end for

    return s1_len - s2_len;
}//end e_etn_strncmp

static inline uint32_t e_etn_find(e_etn_t *etn, const char *label, uint32_t lo, uint32_t hi) {
    int         ret;
    size_t      len, label_len;
    uint32_t    mid;
    const char  *s;

    label_len = strlen(label);
    while(lo < hi) {
        mid = lo + (hi - lo) / 2;
        s = e_etn_node_label(etn, mid, &len);
        ret = e_etn_strncmp(s, len, label, label_len);
        if(ret < 0) {
            lo = mid + 1;
        }//end if
        else if(ret == 0) {
            return mid;
        } else {
            hi = mid;
        }//end else
    }//end while

    return E_ETN_NOT_FOUND;
}//end e_etn_find

static inline const char *e_etn_node_label(e_etn_t *etn, uint32_t i, size_t *len) {
    uint32_t x, off;

    x = etn->nodes[i];
    *len = x & ((1 << etn->nodes_bits_text_length) - 1);
    x >>= etn->nodes_bits_text_length;
    off = x & ((1 << etn->nodes_bits_text_offset) - 1);

    return etn->text + off;
}//end e_etn_node_label
