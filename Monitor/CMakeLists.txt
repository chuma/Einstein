# Monitor CMakeLists

file(GLOB headers *.h)
file(GLOB sources *.cp)

set_source_files_properties(${sources} PROPERTIES LANGUAGE CXX)

add_library(monitor STATIC ${sources})

target_include_directories(monitor PUBLIC
    ../
../Emulator/JIT/
../Emulator/JIT/Generic/
../Emulator/)

if(${WIN32})
    target_include_directories(monitor PUBLIC
        ../Win32)
endif()

target_link_libraries(monitor k)
