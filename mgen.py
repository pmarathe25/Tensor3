#!/usr/bin/python3
import msquared as m2

def main():
    make_gen = m2.MGen("./") \
        .add_flags("-O3 -flto -march=native") \
        .add_cflags("-std=c++17") \
        .register_executable("test/test", source_files="test/test.cpp", clean=True) \
        .register_custom_target("test", commands="test/test", dependencies="test/test")
    # Add installation target
    m2.add_installation(make_gen, headers="include/*/*.hpp", install_file="/usr/local/include/Stealth/Tensor3")
    make_gen.write("makefile")

if __name__ == '__main__':
    main()
