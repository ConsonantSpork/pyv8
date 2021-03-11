#include "v8_instance.hpp"

using namespace v8;

namespace pyv8 {
    V8Instance::V8Instance() {
        this->create_params = new Isolate::CreateParams();
        create_params->array_buffer_allocator =
            ArrayBuffer::Allocator::NewDefaultAllocator();
        this->isolate = Isolate::New(*create_params);
    }

    V8Instance::~V8Instance() {
        isolate->Dispose();
        delete create_params->array_buffer_allocator;
    }

    bpy::object V8Instance::py_run_source(std::string source) {
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);
        Local<Context> context = Context::New(isolate);
        Context::Scope context_scope(context);

        Local<Value> value = run_source(source, context);

        return convert(value, context);
    }

    Local<Value> V8Instance::run_source(std::string source, Local<Context> context) {
        Local<String> v8_source =
            String::NewFromUtf8(isolate, source.c_str())
            .ToLocalChecked();

        TryCatch try_catch(isolate);

        MaybeLocal<Script> script_dirty =
            Script::Compile(context, v8_source);
        Local<Script> script;
        if (!script_dirty.ToLocal(&script)) {
            return try_catch.HasCaught() ?
                try_catch.Exception() :
                Local<Value>();
        }

        MaybeLocal<Value> result_dirty = script->Run(context);
        Local<Value> result_checked;
        if(!result_dirty.ToLocal(&result_checked)) {
            return try_catch.HasCaught() ?
                try_catch.Exception() :
                Local<Value>();
        }

        return result_checked;
    }

    template <typename T>
    bpy::object v8_local_to_py_object(Local<Value> value,
                                      MaybeLocal<T> (Value::*ToType)(Local<Context>) const,
                                      Local<Context> ctx) {
        Value* unwrapped = *value;
        MaybeLocal<T> converted = (unwrapped->*ToType)(ctx);
        Local<T> checked;
        if(!converted.ToLocal(&checked)) {
            return bpy::object();
        }
        return bpy::object(checked->Value());
    }

    bpy::object v8_local_to_py_object(Local<Object> value,
                                      Local<Context> ctx) {
        bpy::dict ret;
        MaybeLocal<Array> keys_maybe = value->GetOwnPropertyNames(ctx);
        Local<Array> keys;
        if (!keys_maybe.ToLocal(&keys)) {
            return bpy::object();
        }

        for (uint32_t i = 0; i < keys->Length(); i++) {
            Local<Value> key = keys->Get(ctx, i).ToLocalChecked();
            Local<Value> val = value->Get(ctx, key).ToLocalChecked();

            ret[convert(key, ctx)] = convert(val, ctx);
        }

        return ret;
    }

    bpy::object v8_local_to_py_object(Local<Array> value,
                                      Local<Context> ctx) {
        bpy::list ret;
        for (uint32_t i = 0; i < value->Length(); i++) {
            Local<Value> val = value->Get(ctx, i).ToLocalChecked();
            ret.append(convert(val, ctx));
        }
        return ret;
    }

    bpy::object v8_local_to_py_object(Local<String> value, Local<Context> ctx) {
        String::Utf8Value str_value(ctx->GetIsolate(), value);
        return bpy::object((std::string)*str_value);
    }

    bpy::object convert(Local<Value> value, Local<Context> ctx) {
        if (value->IsBoolean() || value->IsBooleanObject()) {
            bool ret = value->BooleanValue(ctx->GetIsolate());
            return bpy::object(ret);
        }
        else if (value->IsInt32() || value->IsUint32())
            return v8_local_to_py_object<v8::Integer>(value, (&Value::ToInteger), ctx);
        else if (value->IsNumber()) {
            return v8_local_to_py_object<v8::Number>(value, &Value::ToNumber, ctx);
        }
        else if (value->IsNullOrUndefined()) {
            return bpy::object();
        }
        else if (value->IsArray()) {
            return v8_local_to_py_object(value.As<Array>(), ctx);
        }
        else if (value->IsString() || value->IsStringObject()) {
            return v8_local_to_py_object(value.As<String>(), ctx);
        }
        else {  // Interpret as js Object, return as python dict
            MaybeLocal<Object> maybe_obj = Local<Object>::Cast(value);
            auto str = value->TypeOf(ctx->GetIsolate());
            String::Utf8Value str_str(ctx->GetIsolate(), str);
            Local<Object> obj;
            if (!maybe_obj.ToLocal(&obj))
                return bpy::object();
            return v8_local_to_py_object(obj, ctx);
        }
    }
}
