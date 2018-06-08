cmake_minimum_required(VERSION 3.10)

function (add_install_header interface_file headers)
    set(install_dir /usr/local/include/Stealth)
    file (WRITE ${interface_file} "")
    foreach(header ${headers})
        file(APPEND ${interface_file} "#include \"${header}\"\n")
    endforeach()
    install(FILES ${interface_file} DESTINATION ${install_dir})
    add_custom_target(uninstall COMMAND cat install_manifest.txt | sudo xargs rm -f COMMAND cat install_manifest.txt | xargs dirname | sudo xargs rmdir --ignore-fail-on-non-empty -p)
endfunction()

macro (add_openmp)
    find_package(OpenMP)
    if (OPENMP_FOUND)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    endif()
endmacro()

project(Tensor3)
include_directories(include/)

add_compile_options(-std=c++17 -O3 -flto -march=native)
# Enable OpenMP
add_openmp()

# Find headers and set interface file.
file(GLOB_RECURSE headers include/*.hpp)
# Install!
add_install_header(include/interfaces/Tensor3 "${headers}")

# Add test executable
add_executable(test0 test/test.cpp ${interface_file})
target_link_libraries(test0 -flto)
enable_testing()
add_test(NAME test COMMAND test0)
