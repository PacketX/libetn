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
    char    *tmp1, *tmp2;
    char    **strv1, **strv2;
    size_t  ntoken;

    tmp1 = "1234567890";
    e_assert_true((tmp2 = e_strdup(tmp1)));
    e_assert_true(strcmp(tmp1, tmp2) == 0);
    e_free(tmp2);
    e_assert_true((tmp2 = e_strndup(tmp1, 5)));
    e_assert_true(strcmp(tmp2, "12345") == 0);
    e_free(tmp2);

    tmp1 = "1234567890";
    e_assert_true((tmp2 = e_strdupa(tmp1)));
    e_assert_true(strcmp(tmp1, tmp2) == 0);

    e_assert_true((tmp2 = e_strndupa(tmp1, 5)));
    e_assert_true(strcmp(tmp2, "12345") == 0);

    e_assert_true(e_ascii_toupper('c') == 'C');
    e_assert_true(e_ascii_toupper('D') == 'D');
    e_assert_true(e_ascii_toupper('#') == '#');

    e_assert_true(e_ascii_tolower('c') == 'c');
    e_assert_true(e_ascii_tolower('D') == 'd');
    e_assert_true(e_ascii_tolower('#') == '#');

    e_assert_true(e_str_has_suffix("libxxx.so", ".so"));
    e_assert_false(e_str_has_suffix("libxxx.so", "1.so"));
    e_assert_true(e_str_has_prefix("libxxx.so", "lib"));
    e_assert_false(e_str_has_prefix("libxxx.so", "liba"));

    tmp1 = "123 456 789 123";
    e_assert_true((tmp2 = e_strrstr(tmp1, "123")) && tmp2 == tmp1 + 12);
    e_assert_false(e_strrstr(tmp1, "abc"));

    e_assert_true((tmp2 = e_strstr_len(tmp1, -1, "123")) && tmp2 == tmp1);
    e_assert_false(e_strstr_len(tmp1, -1, "abc"));

    e_assert_true((tmp2 = e_strrstr_len(tmp1, -1, "123")) && tmp2 == tmp1 + 12);
    e_assert_false(e_strrstr(tmp1, "abc"));

    e_assert_true((strv1 = e_strsplit(":a:bc::d:", ":", &ntoken)) && ntoken == 6);
    e_assert_true(!strcmp(strv1[0], "") && !strcmp(strv1[1], "a") && !strcmp(strv1[2], "bc") && 
        !strcmp(strv1[3], "") && !strcmp(strv1[4], "d") && !strcmp(strv1[5], ""));
    e_strfreev(strv1);

    e_assert_true((strv1 = e_strsplit_set("abc:def/ghi", ":/", &ntoken)) && ntoken == 3);
    e_assert_true(!strcmp(strv1[0], "abc") && !strcmp(strv1[1], "def") && !strcmp(strv1[2], "ghi"));
    e_strfreev(strv1);

    e_assert_true((strv1 = e_strsplit_set(":def/ghi:", ":/", &ntoken)) && ntoken == 4);
    e_assert_true(!strcmp(strv1[0], "") && !strcmp(strv1[1], "def") && !strcmp(strv1[2], "ghi") &&
        !strcmp(strv1[3], ""));
    e_strfreev(strv1);

    e_assert_true((strv1 = e_strsplit_set("/C=TW/L=\\xE8\\x87\\xBA\\xE5\\x8C\\x97\\xE5\\xB8\\x82/O=\\xE8\\x87\\xBA\\xE5\\x8C\\x97\\xE5\\xB8\\x82\\xE6\\x94\\xBF\\xE5\\xBA\\x9C/CN=*.gov.taipei", "\\x", &ntoken)) && (ntoken == e_strlenv((const char * const*)strv1)));
    e_assert_true((strv2 = e_strsanitisev((const char * const*)strv1, &ntoken)) && (ntoken == e_strlenv((const char * const*)strv2)));
    e_assert_true(tmp1 = e_strjoinv((const char * const*)strv2, NULL));

    e_free(tmp1);
    e_strfreev(strv1);
    e_strfreev(strv2);

    e_assert_true((strv1 = e_strsplit_set("/C=TW/L=\\xE8\\x87\\xBA\\xE5\\x8C\\x97\\xE5\\xB8\\x82/O=\\xE8\\x87\\xBA\\xE5\\x8C\\x97\\xE5\\xB8\\x82\\xE6\\x94\\xBF\\xE5\\xBA\\x9C/CN=*.gov.taipei", "\\x", &ntoken)) && (ntoken == e_strlenv((const char * const*)strv1)));
    e_assert_true((strv2 = e_strdupv((const char * const*)strv1)));
    e_assert_true(e_strv_equal((const char *const *)strv1, (const char *const *)strv2));
    e_assert_true(e_strv_contains((const char *const *)strv1, "/C=TW/L="));

    e_strfreev(strv1);
    e_strfreev(strv2);

    return 0;
}//end main
