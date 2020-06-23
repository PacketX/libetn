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

static inline void usage(const char *cmd) E_NO_RETURN;

int main(int argc, char *argv[]) {
    int         c;
    bool        icann;
    size_t      i;
    e_etn_t     *etn;
    const char  *file, *eTLD, *eTLD_1, *manager;
    const char  *domains[] = {
        "amazon.co.uk",
        "books.amazon.co.uk",
        "www.books.amazon.co.uk",
        "amazon.com",
        "",
        "example0.debian.net",
        "example1.debian.org",
        "",
        "golang.dev",
        "golang.net",
        "play.golang.org",
        "gophers.in.space.museum",
        "",
        "0emm.com",
        "a.0emm.com",
        "b.c.d.0emm.com",
        "",
        "there.is.no.such-tld",
        "",
        "foo.org",
        "foo.co.uk",
        "foo.dyndns.org",
        "foo.blogspot.co.uk",
        "cromulent",
    };

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

    etn = e_etn_new(file);
    if(E_UNLIKELY(!etn)) {
        fprintf(stderr, "Failed to create etn\n");
        return 1;
    }//end if

    printf("  %24s%24s%24s      %s\n", "Domain", "eTLD", "eTLD+1", "Manage By");
    for(i = 0 ; i < E_N_ELEMENTS(domains); i++) {
        if(strlen(domains[i]) == 0) {
            printf("\n");
            continue;
        }//end if

        e_etn_public_suffix(etn, domains[i], &eTLD, &icann);
        e_etn_eTLD_plus_one(etn, domains[i], &eTLD_1);

        manager = "Unmanaged";
        if(icann) {
            manager = "ICANN Managed";
        }//end if
        else if(strrchr(eTLD, '.')) {
            manager = "Privately Managed";
        }//end if

        printf("> %24s%24s%24s  is  %s\n", domains[i], eTLD, eTLD_1, manager);
    }//end for

    e_etn_free(etn);
    return 0;
}//end main


/* ===== private function ===== */
static inline void usage(const char *cmd) {
    fprintf(stderr, "%s [-d public suffix compiled file]\n", cmd);
    exit(1);
}//end usage
