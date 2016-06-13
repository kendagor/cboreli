/*-----------------------------------------------------------------------------
cboreli - CBOR library

codec_result.h
Encapsulates the return value of a typical encoding/decoding operation

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
#include <memory>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <string>
#include <cstring>
#include "data_item.h"

using namespace std;

namespace cboreli {
    struct codec_result{
        enum status {
            ok,
            warning,
            error
        };
        status res_status;
        string comment;
        item_ptr data;
        
        codec_result();
        codec_result(codec_result* r);
        codec_result(status s, const_str c, item_ptr d);

        void set_error(const_str error_str);
        void set_warning(const_str warning_str);
        void set_info(const_str info_str);

        const static string res_comment_ok;
        const static string res_comment_error;
    };
    using codec_res_ptr = shared_ptr<codec_result>;
}
