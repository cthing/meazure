
set(HOOKS_SRCS
    hooks.cpp
    hooks.h
)

add_library(hooks SHARED ${HOOKS_SRCS})

target_precompile_headers(hooks PRIVATE pch.h)
