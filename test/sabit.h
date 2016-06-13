/*-----------------------------------------------------------------------------
cboreli - CBOR library

sabit.h
A very simple C++ unit test framework
Copyright (c) 2016 Elisha Kendagor kosistudio@live.com

Sabit means a porcupine in native language I know. I hope it
will help you poke around to fix bugs in your project

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
#include <cstdint>
#include <string>
#include <unordered_map>
#include <memory>
#include <cstdint>
#include <random>
#include <forward_list>
#include <tuple>
#include <iostream>

using namespace std;

namespace sabit {
    using const_str = const string&;
    enum error_level {
        noerror,
        warning,
        error
    };

    struct result{
        error_level level;
        string comment;
        result (error_level l, string c):
            level(l),
            comment(c){}
        result(result* res){
            level = res->level;
            comment = res->comment;
        }
    };
    typedef shared_ptr<result> result_ptr;

    class log : public enable_shared_from_this<log> {
        private:
            forward_list<result_ptr> history;
            void print_front(){
                if (history.empty()){
                    return;
                }
                result_ptr res = history.front();
                string err_level_str;
                switch(res->level){
                    case error_level::warning:
                        err_level_str = "Warning";
                        break;
                    case error_level::error:
                        err_level_str = "Error";
                        break;
                    default:
                        err_level_str = "OK";
                        break;
                }
                cout << err_level_str << ":\t" << res->comment << endl;
            }

        public:
            log(){}
            
            log(log* l):
                history(l->history){}

            // Snippet Credit:
            // http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
            template<typename ... Args>
            static string string_format( const std::string& format, Args ... args ){
                size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
                unique_ptr<char[]> buf( new char[ size ] );
                snprintf( buf.get(), size, format.c_str(), args ... );
                return string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
            }
    
            void append(error_level level, const_str comment){
                history.emplace_front(
                        make_shared<result>(level, comment));
                print_front();
            }

            void append(result_ptr res){
                history.push_front(res);
                print_front();
            }
    };
    typedef shared_ptr<log> log_ptr;

    class random_number {
        private:
            uint64_t seed[2];            

            uint64_t xorshift128plus() {
                uint64_t x = seed[0];
                uint64_t const y = seed[1];
                seed[0] = y;
                x ^= (x << 23);
                seed[1] = x ^ y ^ (x >> 17) ^ (y >> 26);
                return seed[1] + 1;
            }

        public:
            random_number() {
                random_device rd;
                for (int i = 0; i < 2; i++) {
                    uint64_t rand = rd();
                    rand <<= 32;
                    rand |= rd();
                    seed[i] = rand;
                }
            }

            uint64_t next() {
                return xorshift128plus();
            }
    };

    class suite;

    class scenario {
        friend class sabit::suite;
        protected:
            string name;
            uint16_t pass;
            uint16_t fail;
            log_ptr slog;

        public:
            scenario(const_str test_name):
                name(test_name),
                pass(0),
                fail(0),
                slog(make_shared<log>()){}

            virtual result_ptr run() = 0;

            uint16_t tests_passed(){
                return pass;
            }

            uint16_t tests_failed(){
                return fail;
            }

            enum test_result : bool{
                failed = false,
                passed = true
            };

            result_ptr post_result(
                    string name,
                    test_result test_res,
                    error_level level,
                    string comment){
                if (test_res == test_result::passed){
                    pass++;
                }else{
                    fail++;
                }
                result_ptr res = make_shared<result>(
                        level, name + " : " + comment);
                slog->append(res);
                return res;
            }
    };

    class suite {
        private:
            string suite_name;
            uint16_t p;
            uint16_t f;
            bool sof;
            random_number rand;
            log_ptr slog;

            using scenario_ptr = shared_ptr<scenario>;
            unordered_map<string, scenario_ptr> tests;
            vector<string> order;

            result_ptr exec(string name){
                result_ptr res = make_shared<result>(
                        error_level::error, "Undefined result");
                if (tests.find(name) == tests.cend()){
                    res->comment = "Scenario '" + name + "' was not found";
                }else{
                    try{
                        scenario_ptr s = tests[name];
                        auto sres = s->run();
                        p += s->tests_passed();
                        f += s->tests_failed();
                        res->level = error_level::noerror;
                        res->comment = log::string_format(
                                "Suite: '%s' - Scenario '%s': Passed=%d : Failed=%d",
                                suite_name.c_str(), name.c_str(), s->tests_passed(), 
                                s->tests_failed());
                    }catch(...){
                        f++;
                        res->comment = "An unhandled exception running the scenario: " + name;
                    }
                }
                slog->append(res);
                return res;
            }

        public:
            suite(const_str name, bool stop_on_first_failure, log_ptr logger = nullptr):
                p(0), f(0), sof(stop_on_first_failure), suite_name(name), rand(random_number()){
            
                if (logger != nullptr){
                    slog = logger;
                }else{
                    slog = make_shared<log>();
                }
            }

            bool stop_on_fail(){
                return sof;
            }

            uint16_t pass() {
                return p;
            }

            uint16_t fail(){
                return f;
            }

            result_ptr add_scenario(scenario_ptr s) {
                result_ptr res = make_shared<result>(
                        error_level::error, "Undefined error");

                if (s == nullptr){
                    res->comment = "add_scenario: test case param was null";
                    slog->append(res);
                    return res;
                }

                if (tests.find(s->name) != tests.cend()){
                    s->name += to_string(rand.next());
                }

                tests[s->name] = s;
                order.push_back(s->name);
                res->level = error_level::noerror;
                res->comment = "Scenario '" + s->name + "' added";
                slog->append(res);
                return res;
            }

            result_ptr run_scenario(const_str name, bool reset_count = true){
                result_ptr res = make_shared<result>(
                        error_level::error, "Undefined error");
                if (reset_count){
                    p = 0;
                    f = 0;
                }
                if (name.empty()){
                    res->comment = "scenario name is empty";
                }else if (tests.find(name) == tests.cend()){
                    res->comment = "scenario '" + name + "' not found";                    
                } else{
                    result_ptr r = exec(name);
                    slog->append(r);
                    res->level = error_level::noerror;
                    res->comment = log::string_format(
                            "Suite: '%s' : Passed=%d : Failed=%d",
                            suite_name.c_str(), p, f);
                }
                slog->append(res);
                return res;
            }

            result_ptr run(){
                result_ptr res = make_shared<result>(
                        error_level::error, "Undefined error");
                p = 0;
                f = 0;
                for (auto name : order){
                    result_ptr r = exec(name);
                    slog->append(r);
                }
                res->level = error_level::noerror;
                res->comment = log::string_format(
                        "Suite: '%s' : Passed=%d : Failed=%d",
                        suite_name.c_str(), p, f);
                slog->append(res);
                return res;
            }
    };
}
#define if_fail_return(res) if (res->level == error_level::error) { return res; } 

