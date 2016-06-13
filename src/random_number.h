/*-----------------------------------------------------------------------------
cboreli - CBOR library

random_generator.h
A random number generator based on Xorshift algorithm as described in
wikipedia: https://en.wikipedia.org/wiki/Xorshift

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
#include <random>
#include <iostream>

using namespace std;

namespace cboreli {
    class random_number {
        private:
            uint64_t seed[2];            

            uint64_t xorshift128plus() {
                uint64_t x = seed[0];
                uint64_t const y = seed[1];
                seed[0] = y;
                x ^= (x << 23);
                seed[1] = x ^ y ^ (x >> 17) ^ (y >> 26);
                return seed[1] + 1;
            }

        public:
            random_number() {
                random_device rd;
                for (int i = 0; i < 2; i++) {
                    uint64_t rand = rd();
                    rand <<= 32;
                    rand |= rd();
                    seed[i] = rand;
                }
            }

            uint64_t next() {
                return xorshift128plus();
            }
    };
}

