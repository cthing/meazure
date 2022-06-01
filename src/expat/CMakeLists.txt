
add_definitions(-DEXPAT_EXPORTS -DCOMPILED_FROM_DSP)

set(EXPAT_SRCS
    xmlparse.c
    xmlrole.c
    xmltok.c
)
source_group("Source Files" FILES ${EXPAT_SRCS})

set(EXPAT_INCS
    xmltok_impl.c
    xmltok_ns.c
)
set_source_files_properties(xmltok_impl.c PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties(xmltok_ns.c PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Included Source Files" FILES ${EXPAT_INCS})

set(EXPAT_HDRS
    ascii.h
    asciitab.h
    expat.h
    iasciitab.h
    internal.h
    latin1tab.h
    macconfig.h
    nametab.h
    utf8tab.h
    winconfig.h
    xmlrole.h
    xmltok.h
    xmltok_impl.h
)
source_group("Header Files" FILES ${EXPAT_HDRS})

add_library(libexpat SHARED ${EXPAT_SRCS} ${EXPAT_INCS} ${EXPAT_HDRS} libexpat.def)
