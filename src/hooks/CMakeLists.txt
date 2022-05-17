
set(hooks_SRCS
    hooks.cpp
    hooks.h
)

add_library(hooks SHARED ${hooks_SRCS})

target_precompile_headers(hooks PRIVATE pch.h)
