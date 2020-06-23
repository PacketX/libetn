# ===========================================================================
#
# ===========================================================================
#
# SYNOPSIS
#
#   AX_CHECK_GO()
#
# DESCRIPTION
#
#   Look for go in a number of default spots, or in a user-selected
#   spot (via --with-go).
#
# LICENSE
#
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


AC_DEFUN([AX_CHECK_GO], [
    found=false
    AC_ARG_WITH([go],
        [AS_HELP_STRING([--with-go=DIR],
            [`go" command absolute path])],
        [
            case "$withval" in
            "" | y | ye | yes | n | no)
            AC_MSG_ERROR([Invalid --with-go value])
              ;;
            *) dirs="$withval"
              ;;
            esac
        ], [
            # use some default dirs
            dirs="/go/bin /usr/go/bin /usr/local/go/bin"
        ]
        )

    E_GO_CMD=
    if ! $found; then
        for dir in $dirs; do
            AC_MSG_CHECKING([for go in $dir])
            if test -f "$dir/go"; then
                AC_CHECK_FILE($dir/go, [
                        E_GO_CMD="$dir/go"
                        found=true
                        AC_DEFINE_UNQUOTED([E_GO_CMD], ["$E_GO_CMD"], ["`go" command path])
                        break
                    ], [])
            fi
        done
    fi
    if ! $found; then
        AC_MSG_RESULT([no])
        AC_MSG_ERROR(['`go" command is not found'])
    fi
    AC_SUBST([E_GO_CMD])
])
