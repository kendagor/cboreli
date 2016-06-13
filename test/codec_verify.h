/*-----------------------------------------------------------------------------
cboreli - CBOR library

test/codec_verify.h

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
#include "sabit.h"
#include "../src/encoder.h"
#include "../src/cbor_header.h"
#include "../src/endian.h"

using namespace std;
using namespace sabit;
using namespace cboreli;
using sabit::result_ptr;

class codec_verify {
    private:
        scenario* test;
    public:
        codec_verify(scenario* t):
            test(t){}

        string print_cbor(arr_uint8_ptr r){
            ostringstream raw_str(ios_base::ate);
            raw_str << "\n";
            for (uint64_t i = 0; i < r->length(); i++){
               raw_str << "["    << dec << int(r->data()[i])
                       << "::0x" << hex << int(r->data()[i])
                       << "]";
            }
            raw_str << "\n........ Length:"
                    << to_string(r->length())
                    << ".........\n";
            for (uint64_t i = 0; i < r->length(); i++){
               raw_str << hex << setw(2) << setfill('0') << int(r->data()[i]);
            }
            return raw_str.str();
        }

        result_ptr verify(string test_data, codec_res_ptr res,
                uint64_t encoded_val_uint = 0, int64_t encoded_val_int = 0){
            if (res == nullptr){
                return test->post_result(test_data, scenario::test_result::failed, error_level::error,
                        "Argument 'res' == nullptr");
            }
            if (res->res_status == codec_result::error){
                return test->post_result(test_data, scenario::test_result::failed, error_level::error,
                        "Res posted an error: " + res->comment);
            }
            if (res->data == nullptr){
                return test->post_result(test_data, scenario::test_result::failed, error_level::error,
                        "res->data is nullptr");
            }

            arr_uint8_ptr r = res->data->to_cbor();
            if (r == nullptr){
                return test->post_result(
                        test_data,
                        scenario::test_result::failed,
                        error_level::error,
                        "data_item::to_cbor returned nullptr");
            }
            if (r->length() == 0){
                return test->post_result(
                        test_data,
                        scenario::test_result::failed,
                        error_level::error,
                        "data_item::to_cbor() raw.size == 0");
            }
            if (r->data() == nullptr){
                return test->post_result(
                        test_data,
                        scenario::test_result::failed,
                        error_level::error,
                        "data_item::to_cbor() raw.bytes_ptr == nullptr");
            }

            ostringstream raw_str(ios_base::ate);
            raw_str << "\n********************************\n"
                    << "data_item '"  << test_data << "':"
                    << "\nlog= " << res->comment 
                    << "\nto_cbor()=" << print_cbor(r)
                    << "\n";

            switch(res->data->type()){
                case data_type::t_uint8:
                case data_type::t_uint16:
                case data_type::t_uint32:
                case data_type::t_uint64:
                case data_type::t_int8:
                case data_type::t_int16:
                case data_type::t_int32:
                case data_type::t_int64:
                    if_fail_return(verify_number(encoded_val_uint, encoded_val_int, res));
                    break;
                default:
                    break;
            }
            return test->post_result(test_data, scenario::test_result::passed,
                    error_level::noerror, raw_str.str());
        }

        result_ptr verify_number(uint64_t encoded_uint, int64_t encoded_int, codec_res_ptr res){
            string test_data = "verify_number: uint:" + to_string(encoded_uint)
                + ", int:" + to_string(encoded_int);
            arr_uint8_ptr r = res->data->to_cbor();
            cbor_header h(r->data()[0]);

            uint8_t major_type = h.major_type();
            if (major_type != 0 && major_type != 1){
                return test->post_result(test_data, scenario::test_result::failed,
                        error_level::error,
                        "verify_number: header major_type == "
                        + to_string(major_type) + "; Expected '0' or '1'");
            }
            uint8_t extra_size = 0;
            if (h.additional_info() > 23){
                switch(h.additional_info()){
                    case 24:
                        extra_size = 1;
                        break;
                    case 25:
                        extra_size = 2;
                        break;
                    case 26:
                        extra_size = 4;
                        break;
                    case 27:
                        extra_size = 8;
                        break;
                }
            }
            if (h.major_type() == 0){
                uint64_t raw_val = h.additional_info();
                if (extra_size > 0){
                    memcpy(&raw_val, r->data()+1, extra_size);
                    endian::big_to_sys(
                            reinterpret_cast<uint8_t*>(&raw_val),
                            extra_size);
                }
                if (encoded_uint != raw_val){
                    return test->post_result(
                            test_data,
                            scenario::test_result::failed,
                            error_level::error,
                            log::string_format("verify_unsigned: Expected: %d -- but got %d",
                                encoded_uint, raw_val));
                }
                return test->post_result(test_data, scenario::test_result::passed,
                        error_level::noerror, "Verified value: " + to_string(encoded_uint));
            }
            if (h.major_type() == 1){
                return verify_negative(encoded_int, res);
            }
            return test->post_result(test_data, scenario::test_result::failed,
                    error_level::warning, "verify_number: should've returned before here.");
        }

        result_ptr verify_negative(int64_t encoded_val, codec_res_ptr res){
            string test_data = "verify_negative: " + to_string(encoded_val);
            arr_uint8_ptr r = res->data->to_cbor();
            cbor_header h(r->data()[0]);

            uint8_t major_type = h.major_type();
            if (major_type != 1){
                return test->post_result(test_data, scenario::test_result::failed, error_level::error,
                        "verify_negative: header major_type != 1");
            }
            uint8_t extra_size = 0;
            if (h.additional_info() > 23){
                switch(h.additional_info()){
                    case 24:
                        extra_size = 1;
                        break;
                    case 25:
                        extra_size = 2;
                        break;
                    case 26:
                        extra_size = 4;
                        break;
                    case 27:
                        extra_size = 8;
                        break;
                }
            }
            int64_t raw_val = h.additional_info();
            if (extra_size > 0){
                memcpy(&raw_val, r->data()+1, extra_size);
                endian::big_to_sys(
                        reinterpret_cast<uint8_t*>(&raw_val),
                        extra_size);
            }
            raw_val = -1 * (raw_val + 1);
            if (encoded_val != raw_val){
                return test->post_result(
                        test_data,
                        scenario::test_result::failed,
                        error_level::error,
                        log::string_format("verify_negative: Expected: %d -- but got %d",
                            encoded_val, raw_val));
            }
            return test->post_result(test_data, scenario::test_result::passed, error_level::noerror,
                    "Verified value: " + to_string(encoded_val));
        }
};

