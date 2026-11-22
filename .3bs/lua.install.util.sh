: '############################################################################'
: '# Copyright (c) 2026 Connor R. Beeney                                      #'
: '#                                                                          #'
: '# Permission to use, copy, modify, and/or distribute this software for     #'
: '# any purpose with or without fee is hereby granted.                       #'
: '#                                                                          #'
: '# THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL            #'
: '# WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES #'
: '# OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE   #'
: '# FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY       #'
: '# DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER  #'
: '# IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING   #'
: '# OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.    #'
: '############################################################################'

set -e

. .3bs/*essential.sh

_DependencyInstallLua() {

    _EnsureProgramExists() {
        program="$1"

        if test -n "`which $program`"; then
            _DisplayTrace "$program found."
        else
            _DisplayError "$program not found."
            return 1
        fi

        shift && test -n $1 && _EnsureProgramExists $@
    }

    _EnsureProgramExists tcc git

    tmp=_CreateTempIfNotExists DependencyInstallLua

    git clone 


}; $@