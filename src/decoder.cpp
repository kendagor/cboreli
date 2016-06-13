/*-----------------------------------------------------------------------------
cboreli - CBOR library

decoder.cpp

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

#include <cmath>
#include "endian.h"
#include "cbor_header_uint.h"
#include "cbor_header_float.h"
#include "decoder.h"

using namespace cboreli;

codec_res_ptr decoder::result_default(){
    return make_shared<codec_result>(
            codec_result::status::ok,
            codec_result::res_comment_ok,
            nullptr);
}

codec_res_ptr decoder::decode(arr_uint8_ptr d, uint64_t& pos){
    e.root_item_append(false);
    codec_res_ptr res = result_default();
    if (d == nullptr){
        res->set_error("nullptr param to decode");
        return res;
    }
    if (d->length() == 0){
        res->set_error("decode arr_uint8_ptr has zero length");
        return res;
    }
    cbor_header_uint h(d);
    if (!h.valid()){
        res->set_error(h.comment());
        return res;
    }
    switch(h.major_type()){
        case 0:
            res = e.add_uint("uint", h.value());
            pos += h.width();
            break;
        case 1:
            res = e.add_int("int", (-1 * h.value()) - 1);
            pos += h.width();
            break;
        case 2:{
                auto entry = d->slice(h.width(), h.value());
                if (entry == nullptr){
                    res->set_error("Bytes too few to decode byte string");
                    return res;
                }
                res = e.add_byte_array("byte_string", entry);
                pos += h.width() + h.value();
            }
            break;
        case 3:{
                auto cstr_t = d->slice(h.width(), h.value());
                string str(reinterpret_cast<char*>(cstr_t->data()),
                        static_cast<size_t>(h.value()));
                res = e.add_utf8_string("utf8_str", str);
                pos += h.width() + h.value();
            }
            break;
        case 4:{
                vector<item_ptr> v;
                uint64_t i = 0;
                uint64_t pos_tmp = h.width();
                while (i < h.value()){
                    auto entry = make_shared<arr_uint8>(
                            d->slice(pos_tmp, d->length()));
                    auto entry_res = decode(entry, pos_tmp);
                    if (entry_res->data == nullptr){
                        return entry_res;
                    }
                    v.push_back(entry_res->data);
                    ++i;
                }
                pos += pos_tmp;
                res = e.add_items_array("vector<item_ptr>", v);
            }
            break;
        case 5:{
                unordered_map<string, item_ptr> m;
                uint64_t i = 0;
                uint64_t pos_tmp = h.width();
                while (i < h.value()){
                    auto key_arr = make_shared<arr_uint8>(
                            d->slice(pos_tmp, d->length()));
                    auto key_res = decode(key_arr, pos_tmp);
                    if (key_res->data == nullptr){
                        return key_res;
                    }
                    auto val_arr = make_shared<arr_uint8>(
                            d->slice(pos_tmp, d->length()));
                    auto val_res = decode(val_arr, pos_tmp);
                    if (val_res->data == nullptr){
                        return val_res;
                    }
                    m[key_res->data->v_utf8str] = val_res->data;
                    ++i;
                }
                pos += pos_tmp;
                res = e.add_items_map("map<key, item_ptr>", m);
            }
            break;
        case 7:
            switch(h.additional_info()){
                case 20:
                    res = e.add_bool("bool", false);
                    pos += h.width();
                    break;
                case 21:
                    res = e.add_bool("bool", true);
                    pos += h.width();
                    break;
                case 22:
                    res = e.add_null("null");
                    pos += h.width();
                    break;
                case 23:
                    res = e.add_undefined("undefined");
                    pos += h.width();
                    break;
                case 26:
                case 27:{
                        cbor_header_float h_entry(d);
                        if (!h_entry.valid()){
                            res->set_error(h_entry.comment());
                            return res;
                        }
                        res = e.add_float("float", h_entry.value());
                        pos += h_entry.width();
                    }
                    break;
                default:
                    res->set_error("Unsupported CBOR type 7 value "
                            + to_string(h.value()));
                    return res;
            }
            break;
        default:
            res->set_error("Type not supported");
            break;
    }
    return res;
}

codec_res_ptr decoder::decode(arr_uint8_ptr d){
    uint64_t verify_width = 0;
    auto res = decode(d, verify_width);
    if (res->data != nullptr){
        if (verify_width != d->length()){
            res->set_error("Decode message width mismatch:"
                    + to_string(verify_width) + " bytes instead of "
                    + to_string(d->length()) + " bytes");
        }
    }
    return res;
}


