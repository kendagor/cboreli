# CBOR library
An implementation of [CBOR](http://tools.ietf.org/html/rfc7049) specification.

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

#### Current status:
The following data types are currently supported. More to follow:
0: uint
1: int
2: byte_string / byte_array
3: text_string (must be utf8 - burden on the library user)
4: array
5: map
-
7: bool
7: null
7: undefined
7: ieee_754 32,64-bit floats

#### Examples
The tests illustrate how the library could be used.

#### Progress
This library may change often if anyone has cloned it, please
let me know so that I can start branching to minimize breaking changes :)

I would also like to bring to your attention a test site developed by
the author of the CBOR RFC:
http://cbor.me

Thanks!

Kendagor
