# Copyright 2020 PacketX Technology
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#


AM_CFLAGS=@CFLAGS_SET@ -Wno-unused-function
AM_CPPFLAGS=-I$(top_srcdir)/lib/includes -I$(top_srcdir)/tests -include $(top_srcdir)/config.h
AM_LDFLAGS=@LDFLAGS_SET@ -static

if ENABLE_SHARED
LDADD=$(top_srcdir)/lib/.libs/*.o
else
LDADD=$(top_srcdir)/lib/libetn.la
endif
LDADD+=@LIBS_SET@

check_PROGRAMS= \
    atomic \
    etn \
    idn \
    list \
    punycode \
    refcount \
    strfuncs \
    string \
    timer \
    unicode

atomic_SOURCES=test_atomic.c
etn_SOURCES=test_etn.c
idn_SOURCES=test_idn.c
list_SOURCES=test_list.c
punycode_SOURCES=test_punycode.c
refcount_SOURCES=test_refcount.c
strfuncs_SOURCES=test_strfuncs.c
string_SOURCES=test_string.c
timer_SOURCES=test_timer.c
unicode_SOURCES=test_unicode.c

TESTS=$(check_PROGRAMS)

test_etn.o: public_suffix_compiled.dat
public_suffix_compiled.dat:
	go run $(top_srcdir)/ci/precompile.go -output public_suffix_compiled.dat

clean-local:
	rm -f public_suffix_compiled.dat

.PHONY: valgrind

valgrind: $(check_PROGRAMS)
	@list='$(check_PROGRAMS)'; test -n "$$list" || exit 0; \
	arr='' || true; \
	for p in $$list; do \
		sudo valgrind --error-exitcode=3 --leak-check=full --track-origins=yes --track-fds=yes ./"$$p"; \
		if test $$? -eq 3 ;then arr="`echo $$arr $$p`"; fi; \
	done; \
	if test -n "$$arr"; then \
		printf "\n>>>>> valgrind reported errors on \"$$arr\" <<<<<\n\n"; \
	fi
