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
    char        *str, *tmp;
    size_t      len, tmp_len;
    e_string_t  *string, *string2;

    string = e_string_new("1234");
    e_assert_true(string);
    e_string_get_c_string(string, &str, &len);
    e_assert_true(strcmp(str, "1234") == 0 && len == 4);
    string2 = e_string_new("1234");
    e_assert_true(string2);
    e_assert_true(e_string_equal(string, string2));
    e_assert_true(string2);
    e_string_free(string);
    e_string_free(string2);

    string = e_string_new(NULL);
    e_assert_true(string);

    e_assert_errno(E_OK, e_string_assign(string, "test string"));
    e_string_get_c_string(string, &str, &len);
    e_assert_true(strcmp(str, "test string") == 0 && len == strlen("test string"));

    e_string_truncate(string, 0);
    e_string_get_c_string(string, NULL, &len);
    e_assert_true(len == 0);

    /* test insert */
    e_assert_errno(E_OK, e_string_insert_len(string, 0, "abc", 3));
    e_assert_errno(E_OK, e_string_append(string, " 1234"));
    e_assert_errno(E_OK, e_string_append_len(string, " XYZ", 4));
    e_assert_errno(E_OK, e_string_append_c(string, 'H'));
    e_assert_errno(E_OK, e_string_append_unichar(string, L'🐶'));
    e_assert_errno(E_OK, e_string_prepend(string, " "));
    e_assert_errno(E_OK, e_string_prepend_c(string, ' '));
    e_assert_errno(E_OK, e_string_prepend_unichar(string, L'🐦'));
    e_assert_errno(E_OK, e_string_prepend_len(string, "TEST", 4));
    e_assert_errno(E_OK, e_string_insert(string, 4, " Hello "));
    e_assert_errno(E_OK, e_string_insert_c(string, 10, 'G'));
    e_assert_errno(E_OK, e_string_insert_unichar(string, 12, L'🎵'));

    e_string_get_c_string(string, &str, &len);
    e_assert_true(strcmp(str, "TEST HelloG 🎵🐦  abc 1234 XYZH🐶") == 0 &&
        len == strlen("TEST HelloG 🎵🐦  abc 1234 XYZH🐶"));

    /* test overwrite */
    e_assert_errno(E_OK, e_string_overwrite(string, 12, "Music12 "));
    e_assert_errno(E_OK, e_string_overwrite_len(string, 0, "1234", 4));
    e_string_get_c_string(string, &str, &len);

    e_assert_true(strcmp(str, "1234 HelloG Music12   abc 1234 XYZH🐶") == 0 &&
        len == strlen("1234 HelloG Music12   abc 1234 XYZH🐶"));

    /* test erase */
    e_assert_errno(E_OK, e_string_erase(string, 10, -1));

    e_string_get_c_string(string, &str, &len);
    e_assert_true(strcmp(str, "1234 Hello") == 0 && len == strlen("1234 Hello"));

    /* test up and down */
    e_string_ascii_down(string);
    e_string_get_c_string(string, &str, &len);
    e_assert_true(strcmp(str, "1234 hello") == 0 && len == strlen("1234 hello"));
    e_string_ascii_up(string);
    e_string_get_c_string(string, &str, &len);
    e_assert_true(strcmp(str, "1234 HELLO") == 0 && len == strlen("1234 HELLO"));

    /* clear all */
    e_string_truncate(string, 0);
    e_string_get_c_string(string, &str, &len);
    e_assert_true(strcmp(str, "") == 0 && len == 0);

    /* test take duplicated */
    str = NULL;
    e_assert_errno(E_OK, e_string_get_c_string_dup(string, &str, &len));
    e_string_get_c_string(string, &tmp, &tmp_len);
    e_assert_true(strcmp(tmp, str) == 0 && tmp_len == len);
    e_free(str);

    e_string_free(string);
    return 0;
}//end main
