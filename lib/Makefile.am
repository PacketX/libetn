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


SUBDIRS=includes

pkgconfigdir=$(libdir)/pkgconfig
pkgconfig_DATA=libetn.pc
DISTCLEANFILES=$(pkgconfig_DATA)

AM_CFLAGS=@LIB_CFLAGS_SET@
AM_LDFLAGS=@LIB_LDFLAGS_SET@
LDADD=@LIB_LIBS_SET@

AM_CPPFLAGS=-include $(top_srcdir)/config.h \
    -I${top_srcdir}/lib \
    -I$(builddir)/includes \
    -DBUILDING_ETN

OBJECTS=\
    e_atomic.h \
    e_err.c \
    e_err.h \
    e_etn.c \
    e_etn.h \
    e_hash.c \
    e_hash.h \
    e_idn.c \
    e_idn.h \
    e_list.c \
    e_list.h \
    e_macros.h \
    e_mem.c \
    e_mem.h \
    e_punycode.c \
    e_punycode.h \
    e_refcount.h \
    e_strfuncs.c \
    e_strfuncs.h \
    e_string.c \
    e_string.h \
    e_testutils.c \
    e_testutils.h \
    e_time.h \
    e_timer.c \
    e_timer.h \
    e_unicode.c \
    e_unicode.h \
    e_version.h \
    e_visibility.h \
    libetn.h

lib_LTLIBRARIES=libetn.la
libetn_la_SOURCES=$(OBJECTS)
libetn_la_LDFLAGS=-avoid-version
