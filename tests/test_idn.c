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

struct {
    const char *fqdn;
    const char *expect;
} idn[] = {
    { "中文字.com", "xn--fiqu43as1h.com" },
    { "abc.中文字test.測試123.com", "abc.xn--test-zf5fh11eujq.xn--123-yx5gk81j.com" },
    { "abc.test.123.com", "abc.test.123.com" },
    { "中文字test.com", "xn--test-zf5fh11eujq.com" },
    { "ليهمابتكلموشعربي؟.com", "xn--egbpdaj6bu4bxfgehfvwxn.com" },
    { "他们为什么不说中文.com", "xn--ihqwcrb4cv8a8dqg056pqjye.com" },
    { "他們爲什麽不說中文.com", "xn--ihqwctvzc91f659drss3x8bo0yb.com" },
    { "pročprostěnemluvíčesky.com", "xn--proprostnemluvesky-uyb24dma41a.com" },
    { "למההםפשוטלאמדבריםעברית.com", "xn--4dbcagdahymbxekheh6e0a7fei0b.com" },
    { "यहलोगहिन्दीक्योंनहींबोलसकतेहैं.com", "xn--i1baa7eci9glrd9b2ae1bj0hfcgg6iyaf8o0a1dig0cd.com" },
    { "なぜみんな日本語を話してくれないのか.com", "xn--n8jok5ay5dzabd5bym9f0cm5685rrjetr6pdxa.com" },
    { "세계의모든사람들이한국어를이해한다면얼마나좋을까.com", "xn--989aomsvi5e83db1d2a355cv1e0vak1dwrv93d5xbh15a0dt30a5jpsd879ccm6fea98c.com" },
    { "почемужеонинеговорятпорусски.com", "xn--b1abfaaepdrnnbgefbadotcwatmq2g4l.com" },
    { "porquénopuedensimplementehablarenespañol.com", "xn--porqunopuedensimplementehablarenespaol-fmd56a.com" },
    { "tạisaohọkhôngthểchỉnóitiếngviệt.com", "xn--tisaohkhngthchnitingvit-kjcr8268qyxafd2f1b9g.com" },
    { "3年b組金八先生.com", "xn--3b-ww4c5e180e575a65lsy2b.com" },
    { "安室奈美恵-with-super-monkeys.com", "xn---with-super-monkeys-pc58ag80a8qai00g7n9n.com" },
    { "hello-another-way-それぞれの場所.com", "xn--hello-another-way--fc4qua05auwb3674vfr0b.com" },
    { "ひとつ屋根の下2.com", "xn--2-u9tlzr9756bt3uc0v.com" },
    { "majiでkoiする5秒前.com", "xn--majikoi5-783gue6qz075azm5e.com" },
    { "パフィーdeルンバ.com", "xn--de-jg4avhby1noc0d.com" },
    { "そのスピードで.com", "xn--d9juau41awczczp.com" },
};

int main(int argc, char *argv[]) {
    char    *encoded_fqdn, *decoded_fqdn;
    size_t  i, encoded_fqdn_len, decoded_fqdn_len;

    for(i = 0 ; i < E_N_ELEMENTS(idn) ; i++) {
        /* check encode */
        e_assert_errno(E_OK, e_idn_encode(idn[i].fqdn, strlen(idn[i].fqdn), &encoded_fqdn, &encoded_fqdn_len));
        printf("(%"PRIuSIZE") '%s' <-> '%s'\n", i, idn[i].fqdn, idn[i].expect);
        printf("(%"PRIuSIZE") '%s' encoded to '%s'\n", i, idn[i].fqdn, encoded_fqdn);
        e_assert_true(!strcmp(idn[i].expect, encoded_fqdn) && strlen(idn[i].expect) == encoded_fqdn_len);

        /* check decode */
        e_assert_errno(E_OK, e_idn_decode(encoded_fqdn, encoded_fqdn_len, &decoded_fqdn, &decoded_fqdn_len));
        printf("(%"PRIuSIZE") '%s' decoded to '%s'\n\n", i, encoded_fqdn, decoded_fqdn);
        e_assert_true(!strcmp(idn[i].fqdn, decoded_fqdn) && strlen(idn[i].fqdn) == decoded_fqdn_len);

        e_free(encoded_fqdn);
        e_free(decoded_fqdn);
    }//end for

    return 0;
}//end main
