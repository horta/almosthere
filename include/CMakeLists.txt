function(define_public_headers BASE_DIR _PUBLIC_HEADERS)
        set(${_PUBLIC_HEADERS} "${BASE_DIR}/athr.h"
        PARENT_SCOPE)
endfunction(define_public_headers)
