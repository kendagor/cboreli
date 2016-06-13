/*-----------------------------------------------------------------------------
cboreli - CBOR library

test/encoder_tests.h

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
#include <sstream>
#include <string>
#include <iomanip>
#include <functional>
#include "sabit.h"
#include "codec_verify.h"
#include "../src/encoder.h"
#include "../src/cbor_header.h"
#include "../src/endian.h"
#include "decoder_tests.h"

using namespace std;
using namespace sabit;
using namespace cboreli;
using sabit::result_ptr;

class encoder_test : public scenario {
    private:
        encoder e;
        unique_ptr<codec_verify> cv;
        shared_ptr<decoder_test> dt = make_shared<decoder_test>();

        result_ptr verify(string test_data, codec_res_ptr res,
                uint64_t encoded_val_uint=0, int64_t encoded_val_int=0){
            dt->add_test(test_data, res->data, encoded_val_uint, encoded_val_int);
            return cv->verify(test_data, res, encoded_val_uint, encoded_val_int);
        }

    public:
        encoder_test():
            scenario("encoder"){}

        result_ptr add_uint8(){
            uint_least8_t test_data = 0xaa;
            codec_res_ptr res = e.add_uint("add_uint8", test_data);
            return verify(to_string(test_data), res, test_data, 0);
        }

        result_ptr add_uint16(){
            uint_least16_t test_data = 0xaa;
            codec_res_ptr res = e.add_uint("add_uint16", test_data);
            if_fail_return(verify(to_string(test_data), res, test_data, 0));

            test_data = 0xaabb;
            res = e.add_uint("add_uint16_x", test_data);
            return verify(to_string(test_data), res, test_data, 0);
        }

        result_ptr add_uint32(){
            uint_least32_t test_data = 0xaa;
            codec_res_ptr res = e.add_uint("add_uint32", test_data);
            if_fail_return(verify(to_string(test_data), res, test_data, 0));

            test_data = 0xaabb;
            res = e.add_uint("add_uint32_1", test_data);
            if_fail_return(verify(to_string(test_data), res, test_data, 0));

            test_data = 0xaabbccdd;
            res = e.add_uint("add_uint32_2", test_data);
            return verify(to_string(test_data), res, test_data, 0);
        }

        result_ptr add_uint64(){
            uint_least64_t test_data = 0xaa;
            codec_res_ptr res = e.add_uint("add_uint64", test_data);
            if_fail_return(verify(to_string(test_data), res, test_data, 0));

            test_data = 0xaabb;
            res = e.add_uint("add_uint64_1", test_data);
            if_fail_return(verify(to_string(test_data), res, test_data, 0));

            test_data = 0xaabbccdd;
            res = e.add_uint("add_uint64_2", test_data);
            if_fail_return(verify(to_string(test_data), res, test_data, 0));

            test_data = 0xaabbccddeeff0001;
            res = e.add_uint("add_uint64_3", test_data);
            return verify(to_string(test_data), res, test_data, 0);
        }

        result_ptr add_int8(){
            int_least8_t test_data = 0x7f;
            codec_res_ptr res = e.add_uint("add_int8", test_data);
            if_fail_return(verify(to_string(test_data), res, test_data, 0));

            test_data = -0x80;
            res = e.add_int("add_int8_1", test_data);
            return verify(to_string(test_data), res, 0, test_data);
        }

        result_ptr add_int16(){
            int_least16_t test_data = -0x7fff;
            codec_res_ptr res = e.add_int("add_int16", test_data);
            if_fail_return(verify(to_string(test_data), res, 0, test_data));

            test_data = -0x1f4;
            res = e.add_int("add_int16_1", test_data);
            return verify(to_string(test_data), res, 0, test_data);
        }

        result_ptr add_int32(){
            int_least32_t test_data = -0x7fffffff;
            codec_res_ptr res = e.add_int("add_int32", test_data);
            return verify(to_string(test_data), res, 0, test_data);
        }

        result_ptr add_int64(){
            int_least64_t test_data = -0x7fffffffffffffff;
            codec_res_ptr res = e.add_int("add_int64", test_data);
            return verify(to_string(test_data), res, 0, test_data);
        }

        result_ptr add_byte_array(){
            vector<uint8_t> v{1,2,3,4,5,6,7,8,9};
            codec_res_ptr res = e.add_byte_array("vector_uint8", v);
            verify("vector_uint8", res);
            arr_uint8_ptr r = res->data->to_cbor();
            if (memcmp(v.data(), r->data()+1, 9) != 0){
                return post_result("vector_uint8", test_result::failed, error_level::error,
                        "Memory compare of encoded uint8_t array doesn't match original");
            }
            return post_result("vector_uint8", test_result::passed, error_level::noerror,
                    "memcmp matched");
        }

        result_ptr add_utf8_string(){
            string str = u8"hello world! - 水 \n\r ? zß水𝄋 # ";
            codec_res_ptr res = e.add_utf8_string("utf8_str", str);
            verify(str, res);
            arr_uint8_ptr r = res->data->to_cbor();
            if (memcmp(str.c_str(), r->data()+2, str.size()) != 0){
                return post_result(str, test_result::failed, error_level::error,
                        "Memory compare of encoded utf8 string doesn't match original");
            }
            return post_result(str, test_result::passed, error_level::noerror,
                    "memcmp matched");
        }

        result_ptr add_items_array(){
            e.root_item_append(false);
            codec_res_ptr c_name = e.add_utf8_string("Name", u8"Elisha");
            codec_res_ptr c_age = e.add_uint("Age", 38);
            codec_res_ptr c_score = e.add_uint("Score", 101);
            e.root_item_append(true);
            vector_items v { c_name->data, c_age->data, c_score->data };
            codec_res_ptr res = e.add_items_array("Person", v);
            return verify("items_array", res);
        }

        result_ptr add_items_map(){
            e.root_item_append(false);
            auto num = e.add_uint(u8"number_123", 123);
            auto str = e.add_utf8_string(u8"hello_world", "hello world!");
            vector_items v{num->data, str->data};
            auto vec = e.add_items_array(u8"Vector_items", v);
            e.root_item_append(true);
            map_items m;
            m["vec"] = vec->data;
            m["num"] = num->data;
            m["str"] = str->data;
            auto res = e.add_items_map("unordered_map", m);
            return verify("t_map", res);
        }

        result_ptr add_bool(){
            codec_res_ptr res = e.add_bool("bool_test:true", true);
            result_ptr v = verify("bool_true", res);
            if_fail_return(v);
            if(res->data->to_cbor()->data()[0] != 0xf5){
                return post_result("bool_true", test_result::failed,
                        error_level::error, "Encoding bool 'true' failed");
            }
            res = e.add_bool("bool_test:false", false);
            v = verify("bool_false", res);
            if_fail_return(v);
            if(res->data->to_cbor()->data()[0] != 0xf4){
                return post_result("bool_false", test_result::failed,
                        error_level::error, "Encoding bool 'false' failed");
            }
            return v;
        }

        result_ptr add_null(){
            codec_res_ptr res = e.add_null("null");
            result_ptr v = verify("null", res);
            if_fail_return(v);
            if(res->data->to_cbor()->data()[0] != 0xf6){
                return post_result("null", test_result::failed,
                        error_level::error, "Encoding 'null' failed");
            }
            return v;
        }
        
        result_ptr add_undefined(){
            codec_res_ptr res = e.add_undefined("undefined");
            result_ptr v = verify("undefined", res);
            if_fail_return(v);
            if(res->data->to_cbor()->data()[0] != 0xf7){
                return post_result("undefined", test_result::failed,
                        error_level::error, "Encoding 'undefined' failed");
            }
            return v;
        }

        result_ptr add_float32(){
            codec_res_ptr res = e.add_float("float32", 2016.5);
            auto v = verify("float32", res);
            if_fail_return(v);
            auto f = res->data->to_cbor();
            auto d = f->data();
            auto len = f->length();
            if (len != 5){
                return post_result("float32", test_result::failed, error_level::error,
                        "Float32 value '2016.5' cbor width = " 
                        + to_string(len) + " expected 5");
            }
            if (d[0] != 0xfa && d[1] != 0x44 && d[2] != 0xfc 
                && d[3] != 0x10 && d[4] != 0x00){
                return post_result("float32", test_result::failed, error_level::error,
                        "Encoding float32 '2016.5' failed");
            }
            return v;
        }

        shared_ptr<decoder_test> get_decoder_tests(){
            return dt;
        }

        virtual result_ptr run() override final {
            try{
                cv = make_unique<codec_verify>(codec_verify(this));
                typedef result_ptr(encoder_test::*test_fn)(void);
                vector<test_fn> tests{
                    &encoder_test::add_uint8,
                    &encoder_test::add_uint16,
                    &encoder_test::add_uint32,
                    &encoder_test::add_uint64,
                    &encoder_test::add_int8,
                    &encoder_test::add_int16,
                    &encoder_test::add_int32,
                    &encoder_test::add_int64,
                    &encoder_test::add_byte_array,
                    &encoder_test::add_utf8_string,
                    &encoder_test::add_items_array,
                    &encoder_test::add_items_map,
                    &encoder_test::add_bool,
                    &encoder_test::add_null,
                    &encoder_test::add_undefined,
                    &encoder_test::add_float32
                };
                for (auto test : tests){
                    cout << "\n------------------------------ Encoder Test ------------------\n";
                    auto res = (this->*test)();
                    if_fail_return(res);
                }
                auto arr = e.root_item()->to_cbor();
                string final_str = "Final:" + cv->print_cbor(arr);
                dt->add_test("Final", e.root_item());
                return post_result(
                        "Encoder_tests",
                        test_result::passed,
                        error_level::noerror,
                        final_str);
            }catch(...){
                return post_result(
                        "Encoder_tests",
                        test_result::failed,
                        error_level::error,
                        "encoder_test::run - An unhandled exception");
            }
        }
};

