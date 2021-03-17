#pragma once

#include <exception>
#include <string>
#include <boost/python.hpp>
#include <v8.h>

namespace pyv8 {
    class V8Exception : public std::exception {
        private:
            std::string msg_;
            std::string file_;
            int lineno_;
            std::string func_;
            std::string value;
            void set_attributes(std::string msg, std::string file, int lineno, std::string func);
        public:
            V8Exception(std::string msg, std::string file, int lineno, std::string func);
            V8Exception(v8::TryCatch&, v8::Local<v8::Context>);
            const char* what() const throw();
            const std::string msg() const;
            const std::string file() const;
            const int lineno() const;
            const std::string func() const;
    };
}
