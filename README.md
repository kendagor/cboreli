# CBOR library
An C++ implementation of [CBOR](http://tools.ietf.org/html/rfc7049) specification.

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

To build for the first time:

```bash
sh build.sh
cd build
make install
```

Subsequent builds can just involve:

```bash
make
make install
```

#### Examples
The tests illustrate how the library could be used.

```c++
// Encoder automatically creates a root map item so that the encoded object
// resembles a JSON object
encoder e;

// Add a number. Map key = "temperature"; value = "81"
// Data in JSON thus looks like: { "temperature" : 81 }
auto temp_f = e.add_uint(u8"temperature", 81);

// Create some items without appending to the root map item '{ ... }'
e.root_item_append(false);
auto name = e.add_utf8_string(u8"name", u8"Elisha");
auto city = e.add_utf8_string(u8"city", u8"NY");

// Resuming append to root map item
e.root_item_append(true)
map_items fans;
fans["name"] = name;
fans["city"] = city;
auto res = e.add_items_map("Fans", fans);

// Encoder returns a codec_result:
if (res->res_status == codec_result::error){
 //handle. res->data will be nullptr when an error is traced during encoding
 string error_reason = res->comment;
}

if (res->data == nullptr){
 // Please file an issue if you hit this and the status is not an error.
}

// A byte array of the data in CBOR format
return res->data->to_cbor();
```

#### Progress
This library may change often if anyone has cloned it, please
let me know so that I can start branching to minimize breaking changes :)

I would also like to bring to your attention a test site developed by
the author of the CBOR RFC:
http://cbor.me

Thanks!

Kendagor
