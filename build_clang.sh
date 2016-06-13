#!bash
rm -r build
mkdir -p build
cd build
cmake -DUSELLVM=TRUE ..
make

