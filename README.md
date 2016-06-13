# CBOR library
An implementation of [CBOR](http://tools.ietf.org/html/rfc7049) specification.

#### Current status:
The following data types are currently supported. More to follow.
 - 0 uint
 - 1 int
 - 2 byte_string / byte_array
 - 3 text_string (must be utf8 - burden on the library user)
 - 4 array
 - 5 map
 - 7 bool
 - 7 null
 - 7 undefined
 - 7 ieee_754 32,64-bit floats
 
### Build
Verified to build correctly in ubuntu linux. You'll need:
- gcc or clang
- cmake

Take a look at build.sh and build_clang.sh
You could also simply use those scripts to build as well
'''
sh build.sh
'''


#### 

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
