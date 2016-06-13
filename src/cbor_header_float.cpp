/*-----------------------------------------------------------------------------
cboreli - CBOR library

codec_result.cpp

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
#include "cbor_header_float.h"

using namespace cboreli;

cbor_header_float::cbor_header_float(arr_uint8_ptr p):
    cbor_header(0, 0){
    valid_header = false;
    derived_name = "cbor_header_float";
    if (p == nullptr || p->length() == 0){
        invalid("nullptr or length=0 constructor arg");
        return;
    }
    set_header(p->data()[0]);
    if (!valid_header){
        return;
    }
    switch(info){
        //case 25: to-do support for 16bit IEEE 754 float
        case 26:
        case 27:{
                uint8_t v = exp2(info - 24);
                uint8_t n = v + 1;
                if (p->length() < n){
                    invalid("type:25..27, arr arg too short");
                    return;
                }
                arr_uint8 arr(p->data()+1, v, arr_uint8_ownership::copy);
                endian::big_to_sys(arr.data(), v);
                switch(info){
                    case 26:
                        effective_value = *((float*)(arr.data()));
                        effective_width = 5;
                        break;
                    case 27:
                        effective_value = *((double*)(arr.data()));
                        effective_width = 9;
                        break;
                    default:
                        return;
                            
                }
                valid_header = true;
            }
            break;
        default:
            invalid("Use another header type for major_type=7 additional_info="
                    + to_string(info));
            break;
    }
}

double cbor_header_float::value(){
    return effective_value;
}

