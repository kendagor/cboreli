/*-----------------------------------------------------------------------------
cboreli - CBOR library

data_item.cpp

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

#include <cstdlib>
#include <limits>
#include <cfloat>
#include "data_item.h"
#include "cbor_header.h"
#include "endian.h"

using namespace std;
using namespace cboreli;

data_item::data_item():
    value_type(t_uint64),
    v_uint64(0){}

data_item::data_item(const data_item& v):
    data_item(const_cast<data_item*>(&v)){}

data_item::data_item(data_item* pv){
    if (pv == nullptr){
        return;
    }
    value_type = pv->value_type;
    switch(value_type){
        case t_uint8:
        case t_uint16:
        case t_uint32:
        case t_uint64:
            v_uint64 = pv->v_uint64;
            break;
        case t_int8:
        case t_int16:
        case t_int32:
        case t_int64:
            v_int64 = pv->v_int64;
            break;
        case t_byte_string:
            new(&p_byte_string) arr_uint8_ptr();
            p_byte_string = make_shared<arr_uint8>(pv->p_byte_string.get(), true);
            break;
        case t_text_string:
            new(&v_utf8str) string(pv->v_utf8str);
            break;
        case t_array:
            new(&p_array_items) vector_items_ptr();
            p_array_items = make_shared<vector_items>(*(pv->p_array_items));
            break;
        case t_bool:
            v_boolean = pv->v_boolean;
            break;
        default:
            break;
    }
}

data_item::~data_item(){
    switch (value_type){
        case t_text_string:
            v_utf8str.~basic_string<char>();
            break;
        case t_array:
            p_array_items.~shared_ptr<vector<item_ptr>>();
            break;
        case t_map:
            p_map_items.~shared_ptr<unordered_map<string, item_ptr>>();
            break;
        case t_byte_string:
        case t_cbor_raw:
            p_byte_string.~arr_uint8_ptr();
            break;
        default:
            break;
    }
}

data_type data_item::type(){
    return value_type;
}

arr_uint8_ptr data_item::encode_unsigned_ints(uint8_t major_type, uint64_t unencoded_val){

    uint8_t val_type = 24;
    uint8_t val_size = 1;
    uint8_t* val_ptr = nullptr;
    if (unencoded_val <= UINT8_MAX){
        val_ptr = new uint8_t[val_size];
        val_ptr[0] = unencoded_val;
    }else if (unencoded_val <= UINT16_MAX){
        val_type = 25;
        val_size = 2;
        uint16_t v = unencoded_val;
        val_ptr = new uint8_t[val_size];
        memcpy(val_ptr, &v, val_size);
    }else if (unencoded_val <= UINT32_MAX){
        val_type = 26;
        val_size = 4;
        uint32_t v = unencoded_val;
        val_ptr = new uint8_t[val_size];
        memcpy(val_ptr, &v, val_size);
    }else{
        val_type = 27;
        val_size = 8;
        uint64_t v = unencoded_val;
        val_ptr = new uint8_t[val_size];
        memcpy(val_ptr, &v, val_size);
    }

    auto additional_info = [&major_type, &unencoded_val, &val_type] () -> uint8_t {
        switch(major_type){
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                return static_cast<uint8_t>(unencoded_val < 24 ? unencoded_val : val_type);
            case 7:
            default:
                return static_cast<uint8_t>(unencoded_val);
        }
    };

    cbor_header h(major_type, additional_info());
    uint8_t h_cast8 = h.to_uint8();

    arr_uint8_ptr r;
    if (unencoded_val >= 24){
        uint8_t n = val_size+1;
        r = make_shared<arr_uint8>(new uint8_t[n]{h_cast8}, n);
        endian::sys_to_big(val_ptr, val_size);
        memcpy(r->data()+1, val_ptr, val_size);
    }else{
        r = make_shared<arr_uint8>(new uint8_t[1]{h_cast8}, 1);
    }
    if (val_ptr != nullptr){
        delete[] val_ptr;
    }
    return r;
}

arr_uint8_ptr data_item::encode_signed_ints(int64_t unencoded_val){
    int64_t val = unencoded_val;
    uint8_t major_type = 0; //same as unsigned
    if (val < 0){
        major_type = 1;
        val = std::abs(val) - 1;
    }
    arr_uint8_ptr r = encode_unsigned_ints(major_type, val);
    return r;
}

arr_uint8_ptr data_item::encode_arr_uint8(arr_uint8_ptr arr){
    arr_uint8_ptr r = encode_unsigned_ints(2, arr->length());
    r->append(arr);
    return r;
}

arr_uint8_ptr data_item::encode_utf8_str(const_str str){
    arr_uint8_ptr r = encode_unsigned_ints(3, str.size());
    r->append((uint8_t*)(str.c_str()), str.size());
    return r;
}

arr_uint8_ptr data_item::encode_vector_items(vector_items_ptr v){
    if (v == nullptr){
        return nullptr;
    }
    arr_uint8_ptr r = encode_unsigned_ints(4, v->size());
    for(item_ptr val : *v){
        arr_uint8_ptr arr = val->to_cbor();
        r->append(arr);
    }
    return r;
}

arr_uint8_ptr data_item::encode_map_items(map_items_ptr m){
    arr_uint8_ptr r = encode_unsigned_ints(5, m->size());
    for(auto kv : *m){
        arr_uint8_ptr key = encode_utf8_str(kv.first);
        arr_uint8_ptr val = kv.second->to_cbor();
        r->append(key);
        r->append(val);
    }
    return r;
}

arr_uint8_ptr data_item::encode_boolean(const bool b){
    arr_uint8_ptr r = encode_unsigned_ints(7, b==false? 20 : 21);
    return r;
}

arr_uint8_ptr data_item::encode_null(){
    arr_uint8_ptr r = encode_unsigned_ints(7, 22);
    return r;
}

arr_uint8_ptr data_item::encode_undefined(){
    arr_uint8_ptr r = encode_unsigned_ints(7, 23);
    return r;
}

arr_uint8_ptr data_item::encode_float(const double f){
    if (numeric_limits<double>::is_iec559){
        uint8_t n = sizeof(double);
        uint8_t* p = nullptr;
        uint8_t info = 27; // IEEE 754 Double (64bits)
        if (f >= FLT_MIN && f <= FLT_MAX){
            n = sizeof(float);
            float f32 = f;
            p = new uint8_t[n];
            memcpy(p, &f32, n);
            info = 26; // IEEE 754 Single (32bits)
        }else{
            n = sizeof(double);
            p = new uint8_t[n];
            memcpy(p, &f, n);
        }
        endian::sys_to_big(p, n);
        arr_uint8_ptr r = make_shared<arr_uint8>(p, n,
                arr_uint8_ownership::ref_delete_ok);
        cbor_header h(7, info);
        arr_uint8_ptr arr = make_shared<arr_uint8>(
                new uint8_t[1]{h.to_uint8()}, 1);
        arr->append(r);
        return arr;
    }else{
        // todo: Manually encode to ieee754
    }
    return nullptr;
}

arr_uint8_ptr data_item::to_cbor(){
    switch (value_type){
        case t_uint8:
        case t_uint16:
        case t_uint32:
        case t_uint64:
            return encode_unsigned_ints(0, v_uint64);
        case t_int8:
        case t_int16:
        case t_int32:
        case t_int64:
            return encode_signed_ints(v_int64);
        case t_byte_string:
            return encode_arr_uint8(p_byte_string);
        case t_text_string:
            return encode_utf8_str(v_utf8str);
        case t_array:
            return encode_vector_items(p_array_items);
        case t_map:
            return encode_map_items(p_map_items);
        case t_bool:
            return encode_boolean(v_boolean);
        case t_null:
            return encode_null();
        case t_undefined:
            return encode_undefined();
        case t_ieee754_float:
            return encode_float(v_double);
        default:
             return nullptr;
    }
}

uint64_t data_item::value_uint(){
    switch(value_type){
        case t_uint8:
        case t_uint16:
        case t_uint32:
        case t_uint64:
            return v_uint64;
        case t_int8:
        case t_int16:
        case t_int32:
        case t_int64:
            return v_int64 > 0? v_int64 : 0;
        default:
            return 0;
    }
}

int64_t data_item::value_int(){
    switch(value_type){
        case t_uint8:
        case t_uint16:
        case t_uint32:
        case t_uint64:
            return v_uint64 > INT64_MAX? 0 : v_uint64;
        case t_int8:
        case t_int16:
        case t_int32:
        case t_int64:
            return v_int64;
        default:
            return 0;
    }
}

bool data_item::value_bool(){
    if (value_type == t_bool){
        return v_boolean;
    }
    return false;
}

double data_item::value_float(){
    if(value_type == t_ieee754_float){
        return v_double;
    }
    return 0;
}

string data_item::value_string(){
    if (value_type == t_text_string){
        return v_utf8str;
    }
    return "invalid";
}

vector_items_ptr data_item::value_array(){
    if (value_type == t_array){
        return p_array_items;
    }
    return nullptr;
}

map_items_ptr data_item::value_map(){
    if (value_type == t_map){
        return p_map_items;
    }
    return nullptr;
}

arr_uint8_ptr data_item::value_byte_array(){
    if (value_type == t_byte_string){
        return p_byte_string;
    }
    return nullptr;
}

