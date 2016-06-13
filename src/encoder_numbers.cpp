/*-----------------------------------------------------------------------------
cboreli - CBOR library

encoder_numbers.cpp

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

codec_res_ptr encoder::add_uint(const_str key, const uint64_t value,
        item_ptr container){
    item_ptr p = make_shared<data_item>();
    p->v_uint64 = value;
    p->value_type = t_uint64;
    return make_result(codec_result::status::ok, codec_result::res_comment_ok,
            key, p, container);
}

codec_res_ptr encoder::add_int(const_str key, const int64_t value,
        item_ptr container){
    if (value > 0){
        return add_uint(key, value, container);
    }
    item_ptr p = make_shared<data_item>();
    p->v_int64 = value;
    p->value_type = t_int64;
    return make_result(codec_result::status::ok, codec_result::res_comment_ok,
            key, p, container);
}

codec_res_ptr encoder::add_float(const_str key, const double value,
        item_ptr container){
    item_ptr p = make_shared<data_item>();
    p->v_double = value;
    p->value_type = t_ieee754_float;
    return make_result(codec_result::status::ok, codec_result::res_comment_ok,
            key, p, container);
}

codec_res_ptr encoder::add_bool(const_str key, const bool value,
        item_ptr container){
    item_ptr p = make_shared<data_item>();
    p->v_boolean = value;
    p->value_type = t_bool;
    return make_result(codec_result::status::ok, codec_result::res_comment_ok,
            key, p, container);
}

codec_res_ptr encoder::add_null(const_str key, item_ptr container){
    item_ptr p = make_shared<data_item>();
    p->value_type = t_null;
    return make_result(codec_result::status::ok, codec_result::res_comment_ok,
            key, p, container);
}

codec_res_ptr encoder::add_undefined(const_str key, item_ptr container){
    item_ptr p = make_shared<data_item>();
    p->value_type = t_undefined;
    return make_result(codec_result::status::ok, codec_result::res_comment_ok,
            key, p, container);
}

