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


AC_PREREQ([2.69])
AC_INIT([libetn], [1.0.0], [tubear.chen@packetx.biz], [libetn], [https://github.com/PacketX/libetn])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])

# save CFLAGS
saved_CFLAGS=$CFLAGS
AC_USE_SYSTEM_EXTENSIONS
LT_PREREQ([2.2.6])
LT_INIT
AC_ENABLE_SHARED
AC_ENABLE_STATIC
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_PROG_CXX
AC_PROG_RANLIB
CFLAGS=$saved_CFLAGS
PKG_PROG_PKG_CONFIG([0.20])

# checks for typedefs and compiler characteristics
AC_CHECK_HEADER_STDBOOL
AC_TYPE_OFF_T
AC_TYPE_SSIZE_T
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_C_INLINE
AC_C_RESTRICT

# library version
LIBRARY_VERSION=$PACKAGE_VERSION
major=`echo $PACKAGE_VERSION |cut -d. -f1 | sed -e "s/[^0-9]//g"`
minor=`echo $PACKAGE_VERSION |cut -d. -f2 | sed -e "s/[^0-9]//g"`
patch=`echo $PACKAGE_VERSION |cut -d. -f3 | cut -d- -f1 | sed -e "s/[^0-9]//g"`

PACKAGE_VERSION_NUM=`printf "0x%02x%02x%02x" "$major" "$minor" "$patch"`

AC_SUBST([LIBRARY_VERSION], [$PACKAGE_VERSION])
AC_SUBST([PACKAGE_VERSION_NUM], [$PACKAGE_VERSION_NUM])
AC_SUBST([VERSION_MAJOR], [$major])
AC_SUBST([VERSION_MINOR], [$minor])
AC_SUBST([VERSION_PATCH], [$patch])

# check go
AX_CHECK_GO()
E_GO="go"
E_GO_VERSION="`$E_GO_CMD version | awk '{print $3}' | sed -e 's/^go//'`"

# check -Werror=unused-result, for preventing memory leak
AC_MSG_CHECKING(whether compiler understands -Werror=unused-result)
old_CFLAGS="$CFLAGS"
CFLAGS="-Werror=unused-result $CFLAGS"
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([],[])],
    AC_MSG_RESULT(yes)
        CFLAGS="$old_CFLAGS"
        WARN_CFLAGS="-Werror=unused-result $WARN_CFLAGS",
    AC_MSG_RESULT(no)
        CFLAGS="$old_CFLAGS")

# check -Werror=nonnull
# warn about passing a null pointer for arguments marked as requiring a
# non-null value by the nonnull function attribute
AC_MSG_CHECKING(whether compiler understands -Werror=nonnull)
old_CFLAGS="$CFLAGS"
CFLAGS="-Werror=nonnull $CFLAGS"
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([],[])],
    AC_MSG_RESULT(yes)
        CFLAGS="$old_CFLAGS"
        WARN_CFLAGS="-Werror=nonnull $WARN_CFLAGS",
    AC_MSG_RESULT(no)
        CFLAGS="$old_CFLAGS")

# check -Werror=switch
# Warn whenever a switch statement has an index of enumerated type and
# lacks a case for one or more of the named codes of that enumeration
AC_MSG_CHECKING(whether compiler understands -Werror=switch)
old_CFLAGS="$CFLAGS"
CFLAGS="-Werror=switch $CFLAGS"
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([],[])],
    AC_MSG_RESULT(yes)
        CFLAGS="$old_CFLAGS"
        WARN_CFLAGS="-Werror=switch $WARN_CFLAGS",
    AC_MSG_RESULT(no)
        CFLAGS="$old_CFLAGS")

# check -Werror=enum-conversion
# warn when a value of enumerated type is implicitly converted to a
# different enumerated type
AC_MSG_CHECKING(whether compiler understands -Werror=enum-conversion)
old_CFLAGS="$CFLAGS"
CFLAGS="-Werror=enum-conversion $CFLAGS"
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([],[])],
    AC_MSG_RESULT(yes)
        CFLAGS="$old_CFLAGS"
        WARN_CFLAGS="-Werror=enum-conversion $WARN_CFLAGS",
    AC_MSG_RESULT(no)
        CFLAGS="$old_CFLAGS")

# check -Wno-unused-parameter
# warn whenever a function parameter is unused aside from its declaration
AC_MSG_CHECKING(whether compiler understands -Wno-unused-parameter)
    old_CFLAGS="$CFLAGS"
    CFLAGS="-Wno-unused-parameter $CFLAGS"
    AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM([],[])],
        AC_MSG_RESULT(yes)
            CFLAGS="$old_CFLAGS"
            WARN_CFLAGS="-Wno-unused-parameter $WARN_CFLAGS",
        AC_MSG_RESULT(no)
            CFLAGS="$old_CFLAGS")

# check -Wno-missing-field-initializers
AC_MSG_CHECKING(whether compiler understands -Wno-missing-field-initializers)
    old_CFLAGS="$CFLAGS"
    CFLAGS="-Wno-missing-field-initializers $CFLAGS"
    AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM([],[])],
        AC_MSG_RESULT(yes)
            CFLAGS="$old_CFLAGS"
            WARN_CFLAGS="-Wno-missing-field-initializers $WARN_CFLAGS",
        AC_MSG_RESULT(no)
            CFLAGS="$old_CFLAGS")

# check -Wno-type-limits
AC_MSG_CHECKING(whether compiler understands -Wno-type-limits)
    old_CFLAGS="$CFLAGS"
    CFLAGS="-Wno-type-limits $CFLAGS"
    AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM([],[])],
        AC_MSG_RESULT(yes)
            CFLAGS="$old_CFLAGS"
            WARN_CFLAGS="-Wno-type-limits $WARN_CFLAGS",
        AC_MSG_RESULT(no)
            CFLAGS="$old_CFLAGS")

# check -Wno-sign-compare
AC_MSG_CHECKING(whether compiler understands -Wno-sign-compare)
    old_CFLAGS="$CFLAGS"
    CFLAGS="-Wno-sign-compare $CFLAGS"
    AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM([],[])],
        AC_MSG_RESULT(yes)
            CFLAGS="$old_CFLAGS"
            WARN_CFLAGS="-Wno-sign-compare $WARN_CFLAGS",
        AC_MSG_RESULT(no)
            CFLAGS="$old_CFLAGS")

# check -Wall
AC_MSG_CHECKING(whether compiler understands -Wall)
old_CFLAGS="$CFLAGS"
CFLAGS="-Wall $CFLAGS"
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([],[])],
    AC_MSG_RESULT(yes)
        CFLAGS="$old_CFLAGS"
        WARN_CFLAGS="-Wall $WARN_CFLAGS",
    AC_MSG_RESULT(no)
        CFLAGS="$old_CFLAGS")

# extra flags
EXTRA_LIBS="-lrt $EXTRA_LIBS"

AM_CONDITIONAL([ENABLE_SHARED], [ test "x${enable_shared}" = "xyes" ])

# checks for library functions
AC_FUNC_ALLOCA
AC_FUNC_FORK
AC_CHECK_FUNCS([clock_gettime memmove memset stpcpy strcasecmp strstr strrchr])

# checks for header files
AC_CHECK_HEADERS([alloca.h sys/time.h wchar.h arpa/inet.h])

AC_SUBST([LIB_CFLAGS_SET], ["$CFLAGS $WARN_CFLAGS $EXTRA_CFLAG"])
AC_SUBST([LIB_LDFLAGS_SET], ["$LDFLAGS $EXTRA_LDFLAG"])
AC_SUBST([LIB_LIBS_SET], ["$LIBS $EXTRA_LIBS"])

AC_SUBST([CFLAGS_SET], ["$CFLAGS $WARN_CFLAGS $EXTRA_CFLAG"])
AC_SUBST([LDFLAGS_SET], ["$LDFLAGS $EXTRA_LDFLAG"])
AC_SUBST([LIBS_SET], ["$LIBS $EXTRA_LIBS"])

AC_CONFIG_FILES([ \
    Makefile \
    lib/Makefile \
    lib/includes/Makefile \
    lib/includes/libetn/e_version.h \
    lib/libetn.pc \
    examples/Makefile \
    examples/simple/Makefile \
    tests/Makefile \
])

AC_OUTPUT

AC_MSG_NOTICE([summary of build options:

        Install prefix:             ${prefix}
        Bug report address:         ${PACKAGE_BUGREPORT}
        Library:
            version                 ${LIBRARY_VERSION}
            shared:                 ${enable_shared}
            static:                 ${enable_static}
        System types:
            build:                  ${build}
            host:                   ${host}
            target:                 ${target}
        Compiler:
            C preprocessor:         ${CPP}
            C compiler:             ${CC}
            C CFLAGS:               ${CFLAGS}
            C LDFLAGS:              ${LDFLAGS}
            C LIBS:                 ${LIBS}
            C WARN_CFLAGS:          ${WARN_CFLAGS}
            C EXTRA_CFLAG:          ${EXTRA_CFLAG}
            C EXTRA_LDFLAG:         ${EXTRA_LDFLAG}
            C EXTRA_LIBS:           ${EXTRA_LIBS}
            Go compiler:            ${E_GO}
            Go compiler version:    ${E_GO_VERSION}
])
