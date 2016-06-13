/*-----------------------------------------------------------------------------
cboreli - CBOR library

encoder_containers.cpp

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

#include "encoder.h"
#include "random_number.h"

using namespace std;
using namespace cboreli;

codec_res_ptr encoder::add_byte_array(const_str key, vector<uint8_t>& value,
        item_ptr container){
    item_ptr p = make_shared<data_item>();
    p->value_type = t_byte_string;
    new(&(p->p_byte_string)) arr_uint8_ptr();
    p->p_byte_string = make_shared<arr_uint8>(value);
    return make_result(codec_result::status::ok, codec_result::res_comment_ok,
            key, p, container);
}

codec_res_ptr encoder::add_byte_array(const_str key, arr_uint8& value,
        item_ptr container){
    item_ptr p = make_shared<data_item>();
    p->value_type = t_byte_string;
    new(&(p->p_byte_string)) arr_uint8_ptr();
    p->p_byte_string = make_shared<arr_uint8>(value);
    return make_result(codec_result::status::ok, codec_result::res_comment_ok,
            key, p, container);
}

codec_res_ptr encoder::add_byte_array(const_str key, arr_uint8&& value,
        item_ptr container){
    item_ptr p = make_shared<data_item>();
    p->value_type = t_byte_string;
    new(&(p->p_byte_string)) arr_uint8_ptr();
    p->p_byte_string = make_shared<arr_uint8>(value);
    return make_result(codec_result::status::ok, codec_result::res_comment_ok,
            key, p, container);
}

codec_res_ptr encoder::add_items_array(const_str key, vector_items& value,
        item_ptr container){
    item_ptr p = make_shared<data_item>();
    p->value_type = t_array;
    new(&(p->p_array_items)) vector_items_ptr();
    p->p_array_items = make_shared<vector_items>(value);
    return make_result(codec_result::status::ok, codec_result::res_comment_ok,
            key, p, container);
}

codec_res_ptr encoder::add_items_map(const_str key, map_items& value,
        item_ptr container){
    item_ptr p = make_shared<data_item>();
    p->value_type = t_map;
    new(&(p->p_map_items)) map_items_ptr();
    p->p_map_items = make_shared<map_items>(value);
    return make_result(codec_result::status::ok, codec_result::res_comment_ok,
            key, p, container);
}

