# [CBOR](http://cbor.io) library
A C++ implementation of [CBOR](http://tools.ietf.org/html/rfc7049) specification.

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
auto person = e.add_utf8_string(u8"name", u8"Elisha");
auto location = e.add_utf8_string(u8"city", u8"Fuquay Varina");

// Resuming append to root map item
e.root_item_append(true)
map_items fans;
fans["person"] = person;
fans["location"] = location;
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

// or return the entire cummulative root CBOR blob:
return e.root_item()->to_cbor();
```

#### Contributions welcome
* Streams support
* CBOR major_type 6
* to_json()
* Fix reported issues

Relevant links:
* CBOR homepage: http://cbor.io
* A test site developed by the author of the CBOR RFC: http://cbor.me
* Updated RFC with errata fixed: https://github.com/cbor/spec-with-errata-fixed


Thanks!

Elisha Kendagor
