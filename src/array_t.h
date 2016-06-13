/*-----------------------------------------------------------------------------
cboreli - CBOR library

array_t.h
A simple raw array manager

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
#include <cstdint>
#include <memory>
#include <vector>
#include <cstring>
#include <initializer_list>

using namespace std;

namespace cboreli{
    template<typename T>
    class array_t {
        public:
            enum ownership : uint8_t {
                copy,
                ref_delete_not,
                ref_delete_ok
            };
        private:
            uint64_t t_length = 0;
            T* t_ptr = nullptr;
            bool t_ref_delete_not = false;
            uint32_t* t_ref_count = new uint32_t(0);
            size_t t_size = sizeof(T);
            void t_memcpy(T* arr, uint64_t len){
                t_ptr = new T[len];
                memcpy(t_ptr, arr, t_size * len);
            }
            void cleanup(){
                if (t_ref_delete_not){
                    delete t_ref_count;
                    return;
                }
                if (t_ref_count != nullptr && *t_ref_count > 0){
                    --(*t_ref_count);
                    return;
                }
                if (t_ptr != nullptr){
                    delete[] t_ptr;
                }
                delete t_ref_count;
            }
        public:            
            array_t(){}

            array_t(initializer_list<T> l):
                array_t(){
                t_length = l.size();
                t_ptr = new T[t_length];
                uint64_t i = 0;
                for (T t : l){
                    t_ptr[i] = t;
                    ++i;
                }
            }

            array_t(T* arr, uint64_t length, ownership own):
                t_length(length),
                t_ptr(arr){
                switch(own){
                    case ownership::copy:
                        t_memcpy(arr, length);
                        break;
                    case ownership::ref_delete_not:
                        t_ref_delete_not = true;
                        break;
                    default:
                        break;
                }
            }

            array_t(T* arr, uint64_t length):
                array_t(const_cast<T*>(arr), length, ownership::ref_delete_ok){}

            array_t(vector<T>& v):
                t_length(v.size()){
                t_memcpy(v.data(), v.size());
            }
            
            array_t(array_t<T>* r, bool copy=false){
                if (r == nullptr){
                    return;
                }
                t_length = r->t_length;
                if (copy){
                    if (t_length > 0 && r->t_ptr != nullptr){
                        t_memcpy(r->t_ptr, t_length);
                    }
                }else{
                    t_ptr = r->t_ptr;
                    delete t_ref_count;
                    t_ref_count = r->t_ref_count;
                    ++(*t_ref_count);
                }
            }

            array_t(array_t<T>& arr, bool copy=false)
                :array_t(&arr, copy){}

            array_t(const array_t<T>& arr)
                :array_t(const_cast<array_t<T>*>(&arr),true){}

            array_t(const array_t<T>&& arr)
                :array_t(arr.t_ptr, arr.t_length){}

            array_t(shared_ptr<array_t<T>> ptr)
                :array_t(ptr.get()){}
            
            ~array_t(){
                cleanup();
            }

            T* detach(){
                t_ref_delete_not = true;
                return t_ptr;
            }
            
            T* data(){
                return t_ptr;
            }
            
            uint64_t length(){
                return t_length;
            }
            
            void append(T* ptr, uint64_t length){
                if (ptr == nullptr || length == 0){
                    return;
                }
                uint64_t new_length = length + t_length;
                T* tmp = new T[new_length];
                if (t_ptr != nullptr){
                    memcpy(tmp, t_ptr, t_size * t_length);
                    delete[] t_ptr;
                }
                memcpy(tmp + t_length, ptr, t_size * length);
                t_ptr = tmp;
                t_length = new_length;
            }

            void append(array_t<T>& arr){
                append(arr.data(), arr.length());
            }

            void append(array_t<T>* ptr){
                append(ptr->data(), ptr->length());
            }

            void append(shared_ptr<array_t<T>> ptr){
                append(ptr->data(), ptr->length());
            }

            void append(vector<T>& v){
                append(v.data(), v.size());
            }

            shared_ptr<array_t<T>> slice(uint64_t start, uint64_t count,
                    ownership own=ownership::ref_delete_ok){
                if (start >= t_length){
                    return nullptr;
                }
                uint64_t effective_count = count;
                if (start + count > t_length){
                    effective_count = t_length - start;
                }
                auto p = make_shared<array_t<T>>(
                        &t_ptr[start], effective_count, own);
                if (own != ownership::copy){
                    ++(*t_ref_count);
                    delete p->t_ref_count;
                    p->t_ref_count = t_ref_count;
                }
                return p;
            }
    };
    using arr_uint8 = array_t<uint8_t>;
    using arr_uint8_ptr = shared_ptr<arr_uint8>;
    using arr_uint8_ownership = array_t<uint8_t>::ownership;
}

