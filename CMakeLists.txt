#------------------------------------------------------------------------------
# cboreli - CBOR library
#
# CMakeLists.txt
# Project
# CBOR C++ library
#
# The MIT License (MIT)
#
# Copyright (c) 2016 Elisha Kendagor kendagor@live.com
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#------------------------------------------------------------------------------



cmake_minimum_required (VERSION 3.5)

project (cboreli)

set(CMAKE_BUILD_TYPE Debug)
set(cboreli_VERSION_MAJOR 1)
set(cboreli_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 14)
set(cboreli_cxx_flags "${CMAKE_CXX_FLAGS} -g -fPIC -std=c++14")
set(cboreli_cxx_flags_debug "${CMAKE_CXX_FLAGS_DEBUG} -g -fPIC -std=c++14")
set(cboreli_linker_flags "${CMAKE_EXE_LINKER_FLAGS}")

if (${USELLVM})
    message("Using LLVM tools")
    set(llvm_libcxx "-stdlib=libc++")
    set(cboreli_cxx_flags "${cboreli_cxx_flags} ${llvm_libcxx}")
    set(cboreli_cxx_flags_debug "${cboreli_cxx_flags_debug} ${llvm_libcxx}")
    set(cboreli_linker_flags "${cboreli_linker_flags} ${llvm_libcxx}")
    set(CMAKE_CXX_COMPILER "clang++")
endif(${USELLVM})

set(CMAKE_CXX_FLAGS "${cboreli_cxx_flags}")
set(CMAKE_CXX_FLAGS_DEBUG "${cboreli_cxx_flags_debug}")
set(CMAKE_EXE_LINKER_FLAGS "${cboreli_linker_flags}")
set(LINKER_LANGUAGE "CXX")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

configure_file(
    "${PROJECT_SOURCE_DIR}/version/version.h.in"
    "${PROJECT_SOURCE_DIR}/version/version.h")

add_subdirectory(version)
add_subdirectory(src)
add_subdirectory(test)

