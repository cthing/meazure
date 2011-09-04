
add_definitions(-DEXPAT_EXPORTS -DCOMPILED_FROM_DSP)

set(expat_SRCS
    xmlparse.c
    xmlrole.c
    xmltok.c
)
source_group("Source Files" FILES ${expat_SRCS})

set(expat_INCS
    xmltok_impl.c
    xmltok_ns.c
)
set_source_files_properties(xmltok_impl.c PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties(xmltok_ns.c PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Included Source Files" FILES ${expat_INCS})

set(expat_HDRS
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
source_group("Header Files" FILES ${expat_HDRS})

add_library(libexpat SHARED ${expat_SRCS} ${expat_INCS} ${expat_HDRS} libexpat.def)
