#------------------------------------------------------------------------------
# cboreli - CBOR library
#
# src/CMakeLists.txt
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

set(libsrc
    endian.cpp
    cbor_header_uint.cpp
    cbor_header_float.cpp
    codec_result.cpp
    data_item.cpp
    encoder.cpp
    encoder_numbers.cpp
    encoder_containers.cpp
    encoder_strings.cpp
    decoder.cpp)

add_library(cboreli SHARED ${libsrc})
add_library(_cboreli STATIC ${libsrc})

install (TARGETS cboreli
    LIBRARY DESTINATION lib)

set (include_files
    encoder.h
    decoder.h
    array_t.h
    codec_result.h
    data_item.h
    data_types.h
    random_number.h)

set (include_dir include/cboreli)

install (FILES ${include_files}
    DESTINATION ${include_dir})

