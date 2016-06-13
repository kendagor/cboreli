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

#include "codec_result.h"

using namespace std;
using namespace cboreli;

const string codec_result::res_comment_ok = "OK";
const string codec_result::res_comment_error = "An error occurred";

codec_result::codec_result():
    res_status(codec_result::status::ok),
    comment("OK"),
    data(nullptr){}

codec_result::codec_result(codec_result* r){
    res_status = r->res_status;
    comment = r->comment;
    data = r->data;
}

codec_result::codec_result(status s, const_str c, item_ptr d):
    res_status(s), comment(c), data(d){}

void codec_result::set_error(const_str error_str){
    res_status = status::error;
    comment = error_str;
    data = nullptr;
}

void codec_result::set_warning(const_str warning_str){
    res_status = status::warning;
    comment = warning_str;
}

void codec_result::set_info(const_str info_str){
    res_status = status::ok;
    comment = info_str;
}

