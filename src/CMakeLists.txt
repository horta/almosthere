function(define_sources BASE_DIR _SOURCES)
    list(APPEND SOURCES almosthere.c almosthere_widget.c almosthere_time.c
                        terminal/terminal.c)
    IF (WIN32)
        list(APPEND SOURCES thread/tinycthread.c)
    ENDIF()

    limix_convert_rel_to_full(FULL_SOURCES ${BASE_DIR} "${SOURCES}")
    set(${_SOURCES} ${FULL_SOURCES} PARENT_SCOPE)
endfunction(define_sources)
