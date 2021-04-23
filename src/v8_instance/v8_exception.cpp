#include "v8_exception.hpp"

#include <utility>

using namespace v8;

namespace pyv8 {
    void V8Exception::set_attributes(std::string msg, std::string file, int lineno, std::string func) {
        msg_ = std::move(msg);
        file_ = std::move(file);
        lineno_ = lineno;
        func_ = std::move(func);
        std::stringstream ss;
        ss << msg_ << " at line " << lineno_ << " in " << func_ << " in file " << file_;
        value = ss.str();
    }

    V8Exception::V8Exception(std::string msg, std::string file, int lineno, std::string func)
    {
        set_attributes(std::move(msg), std::move(file), lineno, std::move(func));
    }

    std::string get_topmost_function_name(Local<StackTrace> st,
                                          Local<Context> ctx) {
        Local<String> func_name_local = st->GetFrame(ctx->GetIsolate(), 0)->GetFunctionName();
        String::Utf8Value func_name(ctx->GetIsolate(), func_name_local);
        return *func_name;
    }

    std::string get_topmost_script_name(Local<StackTrace> st,
                                          Local<Context> ctx) {
        Local<String> func_name_local = st->GetFrame(ctx->GetIsolate(), 0)->GetScriptName();
        String::Utf8Value func_name(ctx->GetIsolate(), func_name_local);
        return *func_name;
    }

    V8Exception::V8Exception(TryCatch& try_catch, Local<Context> ctx) {
        v8::String::Utf8Value exception(ctx->GetIsolate(), try_catch.Exception());
        std::string exception_str = *exception;
        MaybeLocal<Value> maybe_stack_trace_val = try_catch.StackTrace(ctx);
        Local<Message> message = try_catch.Message();

        v8::String::Utf8Value filename(ctx->GetIsolate(),
                                       message->GetScriptOrigin().ResourceName());
        set_attributes(exception_str, *filename, message->GetLineNumber(ctx).FromJust(), "undefined");
    }

    const char* V8Exception::what() const noexcept {
        return value.c_str();
    }

    std::string V8Exception::msg() const {
        return msg_;
    }

    std::string V8Exception::file() const {
        return file_;
    }

    int V8Exception::lineno() const {
        return lineno_;
    }

    std::string V8Exception::func() const {
        return func_;
    }
}
