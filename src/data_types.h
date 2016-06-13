/*-----------------------------------------------------------------------------
cboreli - CBOR library

data_types.h
CBOR data types

The MIT License (MIT)

Copyright (c) 2016 Elisha Kendagor kendagor@live.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
-----------------------------------------------------------------------------*/

#pragma once
#include <memory>

using namespace std;

namespace cboreli {
    enum data_type: uint8_t {
        // Major type 0
        t_uint8,
        t_uint16,
        t_uint32,
        t_uint64,

        // Major type 1
        t_int8,
        t_int16,
        t_int32,
        t_int64,

        // Major type 2
        t_byte_string,

        // Major type 3
        t_text_string,

        // Major type 4
        t_array,

        // Major type 5
        t_map,

        // Major type 6
        t_other,
        t_datetime_string,
        t_datetime_epoch,
        t_bignum_positive,
        t_bignum_negative,
        t_decimal_fraction,
        t_bigfloat,
        t_base64url_later,    // Expected later encoding
        t_base64_later,
        t_base16_later,
        t_cbor_raw,           // Embed CBOR data not for immediate decoding
        t_uri,
        t_base64_url,
        t_base64,
        t_regex,
        t_mime,
        
        /* This is not a data type, just a marker for a blob of data
         * without a simple way of knowing what it is. This marker at
         * the beginning of the data will indicate it's a CBOR blob */
        t_cbor_self_describe,

        // Major type 7
        t_bool,
        t_null,
        t_undefined,
        t_ieee754_float,
        t_break,                // "break" stop code for indefinite-length items
    };
}
