/*-----------------------------------------------------------------------------
cboreli - CBOR library

encoder.cpp

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

thread_local bool encoder::append_to_base_item = true;

encoder::encoder(){
    create_base_item();
}

encoder::encoder(encoder* e):
    encoder(){
        // todo
}

void encoder::create_base_item(){
    map_items m;
    random_number r;
    string key = "cbor_" + to_string(r.next());
    auto res = add_items_map(key, m);
    base_item = res->data;
}

item_ptr encoder::root_item(){
    return base_item;
}

codec_res_ptr encoder::make_result(
        codec_result::status status,
        const_str comment,
        const_str key,
        item_ptr value,
        item_ptr container){

    codec_res_ptr res = make_shared<codec_result>(status, comment, nullptr);

    if (status == codec_result::status::error){
        return res;
    }

    if (key.empty()){
        res->res_status = codec_result::status::error;
        res->comment += " -- encoder::make_result - Key is empty";
        return res;
    }

    item_ptr c = container;
    if (c != nullptr
            && (c->value_type != t_map
                || c->value_type != t_array)){
        res->res_status = codec_result::status::error;
        res->comment = "Container is not an array or a map";
        return res;
    }else{
        if (append_to_base_item){
            c = root_item();
        }
    }

    if (c != nullptr){
        switch(c->value_type){
            case t_array:
                    c->p_array_items->push_back(value);
                    break;
            case t_map:{
                       if (c->p_map_items->find(key) != c->p_map_items->cend()){
                           res->res_status = codec_result::status::error;
                           res->comment = "Key already exists in the container";
                           return res;
                       }
                       c->p_map_items->insert({key, value});
                       string debugstr = "Added key: '" + key
                           + "' to map: '" + c->key 
                           + "' Size=" + to_string(c->p_map_items->size());
                       res->comment += debugstr;
                       res->res_status = codec_result::status::warning;
                   }
                   break;
            default:
                   res->res_status = codec_result::status::error;
                   res->comment = "Container is not an array or a map";
                   return res;
        }
    }
    value->key = key;
    res->data = value;
    return res;
}

