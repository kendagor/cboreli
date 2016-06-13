/*-----------------------------------------------------------------------------
cboreli - CBOR library

encoder.h
Encodes user provided data into the internal CBOR structure.

The data object created from encoding can be used to produce
the CBOR output by calling:

     data->to_cbor()

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
#include <cstdint>
#include <string>
#include <ctime>
#include <vector>
#include <array>
#include "codec_result.h"

using namespace std;
using namespace cboreli;

namespace cboreli {
    class encoder {
        private:
            thread_local bool static append_to_base_item;
            item_ptr base_item;
            void create_base_item();

            codec_res_ptr make_result(
                    codec_result::status status,
                    const_str comment,
                    const_str key,
                    item_ptr value,
                    item_ptr container=nullptr);

        public:
            encoder();
            encoder(encoder* e);

            item_ptr root_item();
            void root_item_append(bool append){
                append_to_base_item = append;
            }

            codec_res_ptr add_uint(const_str key, const uint64_t value,
                    item_ptr container=nullptr);

            codec_res_ptr add_int(const_str key, const int64_t value,
                    item_ptr container=nullptr);

            codec_res_ptr add_float(const_str key, const double value,
                    item_ptr container=nullptr);

            codec_res_ptr add_bool(const_str key, const bool value,
                    item_ptr container=nullptr);

            codec_res_ptr add_null(const_str key, item_ptr container=nullptr);

            codec_res_ptr add_undefined(const_str key, item_ptr container=nullptr);

            codec_res_ptr add_utf8_string(const_str key, const_str value,
                    item_ptr container=nullptr);

            codec_res_ptr add_utf8_str(const_str key, const_str value,
                    item_ptr container=nullptr){
                return add_utf8_string(key, value, container);
            }

            codec_res_ptr add_byte_array(const_str key, vector<uint8_t>& value,
                    item_ptr container=nullptr);

            codec_res_ptr add_byte_array(const_str key, arr_uint8& value,
                    item_ptr container=nullptr);

            codec_res_ptr add_byte_array(const_str key, arr_uint8&& value,
                    item_ptr container=nullptr);

            codec_res_ptr add_items_array(const_str key, vector_items& value,
                    item_ptr container=nullptr);

            codec_res_ptr add_items_map(const_str key, map_items& value,
                    item_ptr container=nullptr);
    };
}

