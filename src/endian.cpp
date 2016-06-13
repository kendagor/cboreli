/*-----------------------------------------------------------------------------
cboreli - CBOR library

endian.cpp

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


#include "endian.h"

using namespace std;
using namespace cboreli;

endian::endianness endian::system = endian::little_endian;

void endian::test(){
    if (system == unknown){
         uint16_t n = 0xff;
         if (reinterpret_cast<uint8_t*>(&n)[0] == 0xff){
             system = little_endian;
         }else{
             system = endianness::big_endian;
         }
    }
}

void endian::flip(uint8_t* p, uint8_t size){
    uint8_t i = 0;
    while (size - i > i){
        uint8_t tmp = p[i];
        uint8_t k = size - i - 1;
        p[i] = p[k];
        p[k] = tmp;
        i++;
    }
}

void endian::sys_to_big(uint8_t* p, uint8_t size){
    if (size < 2){
        return;
    }
    test();
    if (system == big_endian){
        return;
    }
    flip(p, size);
}

void endian::big_to_sys(uint8_t* p, uint8_t size){
    if (size < 2){
        return;
    }
    test();
    if (system == big_endian){
        return;
    }
    flip(p, size);
}

