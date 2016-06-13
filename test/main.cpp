/*-----------------------------------------------------------------------------
cboreli - CBOR library

test/main.cpp
Tests main

The MIT License (MIT)

Copyright (c) 2016 Elisha Kendagor

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


#include "sabit.h"
#include "encoder_tests.h"

using namespace std;
using namespace sabit;
using namespace cboreli;
using sabit::result_ptr;

int main(int argc, char* argv[]){
    sabit::suite s(
            "cboreli",
            true /*stop_on_first_failure*/);

    auto et = make_shared<encoder_test>(encoder_test());
    s.add_scenario(et);
    s.add_scenario(et->get_decoder_tests());
    s.run();
    return 0;
}
