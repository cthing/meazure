
set(hooks_SRCS
    stdafx.cpp
    stdafx.h
    hooks.cpp
    hooks.h
)

add_library(hooks SHARED ${hooks_SRCS})
