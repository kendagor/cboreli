/*-----------------------------------------------------------------------------
cboreli - CBOR library

test/decoder_tests.h

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

#pragma once
#include <string>
#include <list>
#include <tuple>
#include "sabit.h"
#include "codec_verify.h"
#include "../src/decoder.h"
#include "../src/cbor_header.h"
#include "../src/endian.h"

using namespace std;
using namespace sabit;
using namespace cboreli;
using sabit::result_ptr;

class decoder_test : public scenario {
    private:
        decoder d;
        list<tuple<string, item_ptr, uint64_t, int64_t>> tests;
        unique_ptr<codec_verify> cv;

    public:
        decoder_test():
            scenario("decoder tests"){}

        decoder_test(decoder_test* t):
            scenario("decoder tests"),
            tests(t->tests){}

        void add_test(string name, item_ptr res, 
                uint64_t val_uint=0, int64_t val_int=0){
            tests.push_back(make_tuple(name, res, val_uint, val_int));
        }

        virtual result_ptr run() override final {
            try{
                cv = make_unique<codec_verify>(codec_verify(this));
                for (auto test : tests){
                    string test_name = get<string>(test);
                    cout << "\n------------------------------Decoder Test: "
                         << test_name << "------------------\n";
                    get<item_ptr>(test);
                    auto res = cv->verify(
                            test_name, 
                            d.decode(get<item_ptr>(test)->to_cbor()),
                            get<uint64_t>(test), get<int64_t>(test));
                    if_fail_return(res);
                }
                return post_result(
                        "Decoder_tests",
                        test_result::passed,
                        error_level::noerror,
                        "decoder tests passed");
            }catch(...){
                return post_result(
                        "Decoder_tests",
                        test_result::failed,
                        error_level::error,
                        "decoder_test::run - An unhandled exception");
            }
        }
};

