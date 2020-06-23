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


#include <libetn.h>

int main(int argc, char *argv[]) {
    char            buf[256], *tmp1, *tmp2;
    ssize_t         nread, nwrite;
    e_unicode8_t    *uni8;
    e_unicode16_t   *uni16, *uni16_tmp;
    e_unicode32_t   *uni32;

    e_assert_true(e_unicode_toupper(0x0000006d) == 0x0000004d); /* m to M */
    e_assert_true(e_unicode_toupper(0x0000004fu) == 0x0000004fu); /* O to O */
    e_assert_true(e_unicode_toupper(0x0000002eu) == 0x0000002eu); /* . to . */

    e_assert_true(e_unicode_tolower(0x0000004d) == 0x0000006d); /* M to m */
    e_assert_true(e_unicode_tolower(0x00000061) == 0x00000061); /* a to a */
    e_assert_true(e_unicode_tolower(0x0000002eu) == 0x0000002eu); /* . to . */

    e_assert_true(e_unicode_digit_value(0x00000035) == 5);
    e_assert_true(e_unicode_digit_value(0x00000078) == -1);
    e_assert_true(e_unicode_xdigit_value(0x00000066) == '\x0f');
    e_assert_true(e_unicode_xdigit_value(0x00000078) == -1);

    e_assert_true(e_utf8_strlen("123456", -1) == 6);

    snprintf(buf, sizeof(buf), "1234567890");
    e_assert_true(e_utf8_strncpy(buf, "abc", strlen("abc")) && !strcmp(buf, "abc"));

    tmp1 = "1234567890";
    e_assert_true((tmp2 = e_utf8_strchr(tmp1, strlen(tmp1), 0x00000034)) && (tmp2 == tmp1 + 3));
    e_assert_false(e_utf8_strchr(tmp1, strlen(tmp1), 0x00000061));

    tmp1 = "01234567890";
    e_assert_true((tmp2 = e_utf8_strrchr(tmp1, strlen(tmp1), 0x00000030)) && (tmp2 == tmp1 + 10));
    e_assert_false(e_utf8_strrchr(tmp1, strlen(tmp1), 0x00000061));

    tmp1 = "abc123456789abc";
    e_assert_true((tmp2 = e_utf8_strreverse(tmp1, strlen(tmp1))) && !strcmp(tmp2, "cba987654321cba"));
    e_free(tmp2);

    tmp1 = "01234567890";
    e_assert_true((tmp2 = e_utf8_substring(tmp1, 3, 5)) && !strcmp(tmp2, "34"));
    e_free(tmp2);

    e_assert_true(e_unicode32_to_utf8(0x00000041, buf) == 1 && buf[0] == 'A');

    uni8 = (e_unicode8_t *)"測試.com";
    e_assert_errno(E_OK, e_utf8_to_utf16(uni8, strlen((const char *)uni8), &nread, &uni16, &nwrite));
    e_assert_true(nread == 10 && nwrite == 6 && uni16);
    e_assert_errno(E_OK, e_utf16_to_utf8(uni16, nwrite, &nread, &uni8, &nwrite));
    e_assert_true(nread == 6 && nwrite == 10 && uni8);
    e_assert_true(!memcmp(uni8, "測試.com", nwrite));
    e_free(uni8);
    e_free(uni16);

    uni8 = (e_unicode8_t *)"測試.com";
    e_assert_errno(E_OK, e_utf8_to_ucs4(uni8, strlen((const char *)uni8), &nread, &uni32, &nwrite));
    e_assert_true(nread == 10 && nwrite == 6 && uni32);
    e_assert_errno(E_OK, e_ucs4_to_utf8(uni32, nwrite, &nread, &uni8, &nwrite));
    e_assert_true(nread == 6 && nwrite == 10 && uni8);
    e_assert_true(!memcmp(uni8, "測試.com", nwrite));
    e_free(uni8);
    e_free(uni32);

    uni8 = (e_unicode8_t *)"測試.com";
    e_assert_errno(E_OK, e_utf8_to_ucs4_fast(uni8, strlen((const char *)uni8), &uni32, &nwrite));
    e_assert_true(nwrite == 6 && uni32);
    e_assert_errno(E_OK, e_ucs4_to_utf8(uni32, nwrite, &nread, &uni8, &nwrite));
    e_assert_true(nread == 6 && nwrite == 10 && uni8);
    e_assert_true(!memcmp(uni8, "測試.com", nwrite));
    e_free(uni8);
    e_free(uni32);

    uni8 = (e_unicode8_t *)"測試.com";
    e_assert_errno(E_OK, e_utf8_to_utf16(uni8, strlen((const char *)uni8), &nread, &uni16_tmp, &nwrite));
    e_assert_true(nread == 10 && nwrite == 6 && uni16_tmp);

    e_assert_errno(E_OK, e_utf16_to_ucs4(uni16_tmp, nwrite, &nread, &uni32, &nwrite));
    e_assert_true(nread == 6 && nwrite == 6 && uni32);
    e_assert_errno(E_OK, e_ucs4_to_utf16(uni32, nwrite, &nread, &uni16, &nwrite));
    e_assert_true(nread == 6 && nwrite == 6 && uni16);
    e_free(uni16_tmp);
    e_free(uni16);
    e_free(uni32);

    return 0;
}//end main
