/*-----------------------------------------------------------------------------
cboreli - CBOR library

data_item.h
A single piece of CBOR data. The structure of a data item may contain zero,
one, or more nested data items.

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
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <string>
#include <cstring>
#include "data_types.h"
#include "array_t.h"

using namespace std;

namespace cboreli {

    class encoder;
    class decoder;
    class data_item;
    using item_ptr = shared_ptr<data_item>;
    using const_str = const string&;
    using vector_items = vector<item_ptr>;
    using vector_items_ptr = shared_ptr<vector_items>;
    using map_items = unordered_map<string, item_ptr>;
    using map_items_ptr = shared_ptr<map_items>;

    class data_item {
        friend class cboreli::encoder;
        friend class cboreli::decoder;
        private:
            arr_uint8_ptr encode_unsigned_ints(uint8_t major_type, uint64_t unencoded_val);
            arr_uint8_ptr encode_signed_ints(int64_t unencoded_val);
            arr_uint8_ptr encode_arr_uint8(arr_uint8_ptr arr);
            arr_uint8_ptr encode_utf8_str(const_str str);
            arr_uint8_ptr encode_vector_items(vector_items_ptr v);
            arr_uint8_ptr encode_map_items(map_items_ptr m);
            arr_uint8_ptr encode_boolean(const bool b);
            arr_uint8_ptr encode_null();
            arr_uint8_ptr encode_undefined();
            arr_uint8_ptr encode_float(const double f);

            string key;
            data_type value_type;
            union {
                uint64_t v_uint64;
                int64_t v_int64;
                bool v_boolean;
                double v_double;
                string v_utf8str;
                vector_items_ptr p_array_items;
                map_items_ptr p_map_items;
                arr_uint8_ptr p_byte_string;
            };

        public:
            data_item();
            data_item(const data_item& v);
            data_item(data_item* pv);
            ~data_item();

            item_ptr operator[](const_str i);
            item_ptr operator[](const uint64_t i);

            bool operator==(item_ptr rhs);
            bool operator==(const data_item& rhs);

            item_ptr operator=(item_ptr rhs);
            item_ptr operator=(const data_item& rhs);

            data_type type();

            uint64_t value_uint();
            int64_t value_int();
            bool value_bool();
            double value_float();
            string value_string();
            vector_items_ptr value_array();
            map_items_ptr value_map();
            arr_uint8_ptr value_byte_array();

            arr_uint8_ptr to_cbor();
    };
}

