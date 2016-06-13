/*-----------------------------------------------------------------------------
cboreli - CBOR library

cbor_header.h
An internal representation of the first byte of a cbor header.

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
#include <cstdint>
#include <string>

using namespace std;

namespace cboreli {
    class cbor_header{
        protected:
            uint8_t type = 0;
            uint8_t info = 0;
            bool valid_header = false;
            string valid_comment = "Uninitialized";
            string derived_name = "cbor_header";
            uint8_t effective_width = 1;

            void set_header(uint8_t h){
                type = (h & 0xE0) >> 5;
                info = h & 0x1f;
                if (type > 7){
                    invalid("Unsupported major_type");
                }else{
                    valid_header = true;
                    valid_comment = "OK";
                }
            }

            void invalid(const string& reason){
                valid_comment = derived_name + ": " + reason;
                valid_header = false;
            }

        public:
            cbor_header(uint8_t h){
                set_header(h);
            }

            cbor_header(uint8_t t, uint8_t i)
                :type(t), info(i){}

            uint8_t to_uint8(){
                uint8_t r = ((type << 5) | info);
                return r;
            }

            uint8_t major_type(){
                return type;
            }

            uint8_t additional_info(){
                return info;
            }

            bool valid(){
                return valid_header;
            }

            string comment(){
                return valid_comment;
            }

            uint8_t width(){
                return effective_width;
            }

            static const uint8_t max_width = 9;
    };
}

