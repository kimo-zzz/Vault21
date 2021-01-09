set(DETOURS_SOURCE 
    "creatwth.cpp"
    "detours.cpp"
    "detours.h"
    "detver.h"
    "disasm.cpp"
    "disolarm.cpp"
    "disolarm64.cpp"
    "disolia64.cpp"
    "disolx64.cpp"
    "disolx86.cpp"
    "image.cpp"
    "modules.cpp"
)

add_library(Detours STATIC ${DETOURS_SOURCE})

target_include_directories(Detours PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})