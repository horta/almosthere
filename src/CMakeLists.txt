function(define_sources BASE_DIR _SOURCES)
    list(APPEND SOURCES athr.c athr_time.c canvas.c
    terminal/terminal.c widget/bar.c widget/line.c widget/text.c
    widget/perc.c widget/eta.c)
    IF (WIN32)
        list(APPEND SOURCES thread/tinycthread.c)
    ENDIF()

    limix_convert_rel_to_full(FULL_SOURCES ${BASE_DIR} "${SOURCES}")
    set(${_SOURCES} ${FULL_SOURCES} PARENT_SCOPE)
endfunction(define_sources)
