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

_SetColour() {
    case $1 in
        "white")  printf "\033[0;37m" ;;
        "red")    printf "\033[0;31m" ;;
        "yellow") printf "\033[0;33m" ;;
        "orange") printf "\033[38;2;255;165;0m" ;;
        "custom") printf "\033[38;2;$2;$3;${4}m" ;;
        "reset")  printf "\033[0m" ;;
        *) ;;
    esac
}