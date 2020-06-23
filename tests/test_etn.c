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
#include <getopt.h>

#define DATA_FILE "public_suffix_compiled.dat"

struct {
    const char *domain;
    const char *want;
} public_suffix_cases[] = {
    /* empty string */
    { "", "" },

    /* the .ao rules are: */
    /* ao */
    /* ed.ao */
    /* gv.ao */
    /* og.ao */
    /* co.ao */
    /* pb.ao */
    /* it.ao */
    { "ao", "ao" },
    { "www.ao", "ao" },
    { "pb.ao", "pb.ao" },
    { "www.pb.ao", "pb.ao" },
    { "www.xxx.yyy.zzz.pb.ao", "pb.ao" },

    /* the .ar rules are: */
    /* ar */
    /* com.ar */
    /* edu.ar */
    /* gob.ar */
    /* gov.ar */
    /* int.ar */
    /* mil.ar */
    /* net.ar */
    /* org.ar */
    /* tur.ar */
    /* blogspot.com.ar */
    { "ar", "ar" },
    { "www.ar", "ar" },
    { "nic.ar", "ar" },
    { "www.nic.ar", "ar" },
    { "com.ar", "com.ar" },
    { "www.com.ar", "com.ar" },
    { "blogspot.com.ar", "blogspot.com.ar" },
    { "www.blogspot.com.ar", "blogspot.com.ar" },
    { "www.xxx.yyy.zzz.blogspot.com.ar", "blogspot.com.ar" },
    { "logspot.com.ar", "com.ar" },
    { "zlogspot.com.ar", "com.ar" },
    { "zblogspot.com.ar", "com.ar" },

    /* the .arpa rules are: */
    /* arpa */
    /* e164.arpa */
    /* in-addr.arpa */
    /* ip6.arpa */
    /* iris.arpa */
    /* uri.arpa */
    /* urn.arpa */
    { "arpa", "arpa" },
    { "www.arpa", "arpa" },
    { "urn.arpa", "urn.arpa" },
    { "www.urn.arpa", "urn.arpa" },
    { "www.xxx.yyy.zzz.urn.arpa", "urn.arpa" },

    /* the relevant {kobe,kyoto}.jp rules are: */
    /* jp */
    /* *.kobe.jp */
    /* !city.kobe.jp */
    /* kyoto.jp */
    /* ide.kyoto.jp */
    { "jp", "jp" },
    { "kobe.jp", "jp" },
    { "c.kobe.jp", "c.kobe.jp" },
    { "b.c.kobe.jp", "c.kobe.jp" },
    { "a.b.c.kobe.jp", "c.kobe.jp" },
    { "city.kobe.jp", "kobe.jp" },
    { "www.city.kobe.jp", "kobe.jp" },
    { "kyoto.jp", "kyoto.jp" },
    { "test.kyoto.jp", "kyoto.jp" },
    { "ide.kyoto.jp", "ide.kyoto.jp" },
    { "b.ide.kyoto.jp", "ide.kyoto.jp" },
    { "a.b.ide.kyoto.jp", "ide.kyoto.jp" },

    /* the .tw rules are: */
    /* tw */
    /* edu.tw */
    /* gov.tw */
    /* mil.tw */
    /* com.tw */
    /* net.tw */
    /* org.tw */
    /* idv.tw */
    /* game.tw */
    /* ebiz.tw */
    /* club.tw */
    /* 網路.tw (xn--zf0ao64a.tw) */
    /* 組織.tw (xn--uc0atv.tw) */
    /* 商業.tw (xn--czrw28b.tw) */
    /* blogspot.tw */
    { "tw", "tw" },
    { "aaa.tw", "tw" },
    { "www.aaa.tw", "tw" },
    { "xn--czrw28b.aaa.tw", "tw" },
    { "edu.tw", "edu.tw" },
    { "www.edu.tw", "edu.tw" },
    { "xn--czrw28b.edu.tw", "edu.tw" },
    { "xn--czrw28b.tw", "xn--czrw28b.tw" },
    { "www.xn--czrw28b.tw", "xn--czrw28b.tw" },
    { "xn--uc0atv.xn--czrw28b.tw", "xn--czrw28b.tw" },
    { "xn--kpry57d.tw", "tw" },

    /* the .uk rules are: */
    /* uk */
    /* ac.uk */
    /* co.uk */
    /* gov.uk */
    /* ltd.uk */
    /* me.uk */
    /* net.uk */
    /* nhs.uk */
    /* org.uk */
    /* plc.uk */
    /* police.uk */
    /* *.sch.uk */
    /* blogspot.co.uk */
    { "uk", "uk" },
    { "aaa.uk", "uk" },
    { "www.aaa.uk", "uk" },
    { "mod.uk", "uk" },
    { "www.mod.uk", "uk" },
    { "sch.uk", "uk" },
    { "mod.sch.uk", "mod.sch.uk" },
    { "www.sch.uk", "www.sch.uk" },
    { "blogspot.co.uk", "blogspot.co.uk" },
    { "blogspot.nic.uk", "uk" },
    { "blogspot.sch.uk", "blogspot.sch.uk" },

    /* the .рф rules are */
    /* рф (xn--p1ai) */
    { "xn--p1ai", "xn--p1ai" },
    { "aaa.xn--p1ai", "xn--p1ai" },
    { "www.xxx.yyy.xn--p1ai", "xn--p1ai" },

    /* the .bd rules are: */
    /* *.bd */
    { "bd", "bd" },
    { "www.bd", "www.bd" },
    { "zzz.bd", "zzz.bd" },
    { "www.zzz.bd", "zzz.bd" },
    { "www.xxx.yyy.zzz.bd", "zzz.bd" },

    /* there are no .nosuchtld rules */
    { "nosuchtld", "nosuchtld" },
    { "foo.nosuchtld", "nosuchtld" },
    { "bar.foo.nosuchtld", "nosuchtld" },
};

struct {
    const char *domain;
    const char *want;
} eTLD_plus_one_cases[] = {
    /* empty input */
    { "", "" },

    /* unlisted TLD */
    { "example", "" },
    { "example.example", "example.example" },
    { "b.example.example", "example.example" },
    { "a.b.example.example", "example.example" },

    /* TLD with only 1 rule */
    { "biz", "" },
    { "domain.biz", "domain.biz" },
    { "b.domain.biz", "domain.biz" },
    { "a.b.domain.biz", "domain.biz" },

    /* TLD with some 2-level rules */
    { "com", "" },
    { "example.com", "example.com" },
    { "b.example.com", "example.com" },
    { "a.b.example.com", "example.com" },
    { "uk.com", "" },
    { "example.uk.com", "example.uk.com" },
    { "b.example.uk.com", "example.uk.com" },
    { "a.b.example.uk.com", "example.uk.com" },
    { "test.ac", "test.ac" },

    /* TLD with only 1 (wildcard) rule */
    { "mm", "" },
    { "c.mm", "" },
    { "b.c.mm", "b.c.mm" },
    { "a.b.c.mm", "b.c.mm" },

    /* More complex TLD */
    { "jp", "" },
    { "test.jp", "test.jp" },
    { "www.test.jp", "test.jp" },
    { "ac.jp", "" },
    { "test.ac.jp", "test.ac.jp" },
    { "www.test.ac.jp", "test.ac.jp" },
    { "kyoto.jp", "" },
    { "test.kyoto.jp", "test.kyoto.jp" },
    { "ide.kyoto.jp", "" },
    { "b.ide.kyoto.jp", "b.ide.kyoto.jp" },
    { "a.b.ide.kyoto.jp", "b.ide.kyoto.jp" },
    { "c.kobe.jp", "" },
    { "b.c.kobe.jp", "b.c.kobe.jp" },
    { "a.b.c.kobe.jp", "b.c.kobe.jp" },
    { "city.kobe.jp", "city.kobe.jp" },
    { "www.city.kobe.jp", "city.kobe.jp" },

    /* TLD with a wildcard rule and exceptions */
    { "ck", "" },
    { "test.ck", "" },
    { "b.test.ck", "b.test.ck" },
    { "a.b.test.ck", "b.test.ck" },
    { "www.ck", "www.ck" },
    { "www.www.ck", "www.ck" },

    /* US K12 */
    { "us", "" },
    { "test.us", "test.us" },
    { "www.test.us", "test.us" },
    { "ak.us", "" },
    { "test.ak.us", "test.ak.us" },
    { "www.test.ak.us", "test.ak.us" },
    { "k12.ak.us", "" },
    { "test.k12.ak.us", "test.k12.ak.us" },
    { "www.test.k12.ak.us", "test.k12.ak.us" },

    /* Punycoded IDN labels */
    { "xn--85x722f.com.cn", "xn--85x722f.com.cn" },
    { "xn--85x722f.xn--55qx5d.cn", "xn--85x722f.xn--55qx5d.cn" },
    { "www.xn--85x722f.xn--55qx5d.cn", "xn--85x722f.xn--55qx5d.cn" },
    { "shishi.xn--55qx5d.cn", "shishi.xn--55qx5d.cn" },
    { "xn--55qx5d.cn", "" },
    { "xn--85x722f.xn--fiqs8s", "xn--85x722f.xn--fiqs8s" },
    { "www.xn--85x722f.xn--fiqs8s", "xn--85x722f.xn--fiqs8s" },
    { "shishi.xn--fiqs8s", "shishi.xn--fiqs8s" },
    { "xn--fiqs8s", "" },
};

static inline void usage(const char *cmd) E_NO_RETURN;
static inline void test_load(const char *filename);
static inline void test_ICANN(const char *filename);
static inline void test_public_suffix(const char *filename);
static inline void test_eTLD_plus_one(const char *filename);
static inline void benchmark(const char *filename);

int main(int argc, char *argv[]) {
    int         c;
    const char  *file;

    opterr = 0;
    file = DATA_FILE;
    while((c = getopt(argc, argv, "d:")) != EOF) {
        switch(c) {
            case 'd':
                file = optarg;
                break;
            default:
                usage(argv[0]);
        }//end switch
    }//end while

    test_load(file);
    test_ICANN(file);
    test_public_suffix(file);
    test_eTLD_plus_one(file);
    benchmark(file);

    return 0;
}//end main


/* ===== private function ===== */
static inline void usage(const char *cmd) {
    fprintf(stderr, "%s [-d public suffix compiled file]\n", cmd);
    exit(1);
}//end usage

static inline void test_load(const char *filename) {
    double      spent;
    e_etn_t     *etn;
    e_timer_t   *timer;

    e_assert_true(timer = e_timer_new());
    e_assert_true(etn = e_etn_new(filename));
    e_assert_errno(E_OK, e_timer_elapsed(timer, &spent, NULL));
    printf("Load '%s' spent: %f seconds\n", filename, spent);
    e_timer_free(timer);
    e_etn_free(etn);
}//end test_load

static inline void test_ICANN(const char *filename) {
    bool        icann;
    size_t      i;
    struct {
        const char *domain;
        bool        icann;
    } cases[] = {
        { "foo.org",            true, },
        { "foo.co.uk",          true, },
        { "foo.dyndns.org",     false, },
        { "foo.go.dyndns.org",  false, },
        { "foo.blogspot.co.uk", false, },
        { "foo.intranet",       false, },
    };
    e_etn_t     *etn;
    const char  *ps;

    e_assert_true(etn = e_etn_new(filename));

    for(i = 0 ; i < E_N_ELEMENTS(cases) ; i++) {
        e_etn_public_suffix(etn, cases[i].domain, &ps, &icann);
        e_assert_true(cases[i].icann == icann);
    }//end for

    e_etn_free(etn);
}//end test_ICANN

static inline void test_public_suffix(const char *filename) {
    bool        icann;
    size_t      i;
    e_etn_t     *etn;
    const char  *ps;

    e_assert_true(etn = e_etn_new(filename));

    for(i = 0 ; i < E_N_ELEMENTS(public_suffix_cases) ; i++) {
        e_etn_public_suffix(etn, public_suffix_cases[i].domain, &ps, &icann);
        e_assert_true(!strcmp(public_suffix_cases[i].want, ps));
    }//end for

    e_etn_free(etn);
}//end test_public_suffix

static inline void test_eTLD_plus_one(const char *filename) {
    size_t      i;
    e_etn_t     *etn;
    const char  *eTLD;

    e_assert_true(etn = e_etn_new(filename));

    for(i = 0 ; i < E_N_ELEMENTS(eTLD_plus_one_cases) ; i++) {
        e_etn_eTLD_plus_one(etn, eTLD_plus_one_cases[i].domain, &eTLD);
        e_assert_true(!strcmp(eTLD_plus_one_cases[i].want, eTLD));
    }//end for

    e_etn_free(etn);
}//end test_eTLD_plus_one

static inline void benchmark(const char *filename) {
    bool        icann;
    size_t      i, j;
    double      spent;
    e_etn_t     *etn;
    e_timer_t   *timer;
    const char  *ps, *eTLD;

    e_assert_true(etn = e_etn_new(filename));

    e_assert_true(timer = e_timer_new());
    for(i = 0 ; i < 10000 ; i++) {
        for(j = 0 ; j < E_N_ELEMENTS(public_suffix_cases) ; j++) {
            e_etn_public_suffix(etn, public_suffix_cases[j].domain, &ps, &icann);
            e_assert_true(!strcmp(public_suffix_cases[j].want, ps));
        }//end for
    }//end for
    e_assert_errno(E_OK, e_timer_elapsed(timer, &spent, NULL));
    printf("Get public suffix %"PRIuSIZE" times, spent %f seconds\n",
        10000 * E_N_ELEMENTS(public_suffix_cases), spent);

    e_timer_reset(timer);
    for(i = 0 ; i < 10000 ; i++) {
        for(j = 0 ; j < E_N_ELEMENTS(eTLD_plus_one_cases) ; j++) {
            e_etn_eTLD_plus_one(etn, eTLD_plus_one_cases[j].domain, &eTLD);
            e_assert_true(!strcmp(eTLD_plus_one_cases[j].want, eTLD));
        }//end for
    }//end for
    e_assert_errno(E_OK, e_timer_elapsed(timer, &spent, NULL));
    printf("Get eTLD %"PRIuSIZE" times, spent %f seconds\n",
        10000 * E_N_ELEMENTS(eTLD_plus_one_cases), spent);

    e_timer_free(timer);
    e_etn_free(etn);
}//end benchmark_public_suffix
