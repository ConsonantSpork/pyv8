#pragma once

#include <exception>
#include <string>
#include <boost/python.hpp>
#include <v8.h>

namespace pyv8 {
    /**
     * \brief Exception class for exceptions from JavaScript
     *
     *  This class wraps exceptions thrown by JavaScript.
     */
    class V8Exception : public std::exception {
        private:
            std::string msg_;
            std::string file_;
            int lineno_{};
            std::string func_;
            std::string value;
            void set_attributes(std::string msg, std::string file, int lineno, std::string func);
        public:
            /**
             * Construct V8Exception
             * @param msg exception message
             * @param file name of file where exception was thrown or "undefined" if not known
             * @param lineno line number where exception was thrown or -1 if not known
             * @param func function name where exception was thrown or "undefined" if not known
             */
            V8Exception(std::string msg, std::string file, int lineno, std::string func);

            /** Extract exception information from a v8::TryCatch reference */
            V8Exception(v8::TryCatch&, v8::Local<v8::Context>);

            /** Get value of exception which is combined value of all the other fields */
            const char* what() const throw();

            /** Get exception message */
            std::string msg() const;

            /** Get name of file where exception was thrown or "undefined" if not known */
            std::string file() const;

            /** Get line number where exception was thrown or -1 if not known */
            int lineno() const;

            /** Get function name where exception was thrown or "undefined" if not known */
            std::string func() const;
    };
}
