#include "v8_instance.hpp"
#include "v8_exception.hpp"

using namespace v8;

namespace pyv8 {
    V8Instance::V8Instance() {
        this->create_params = new Isolate::CreateParams();
        create_params->array_buffer_allocator =
            ArrayBuffer::Allocator::NewDefaultAllocator();
        this->isolate = Isolate::New(*create_params);
        v8::HandleScope handle_scope(isolate);
        this->contexts = new std::vector<Persistent<Context, CopyablePersistentTraits<Context>>>();
        this->contexts->push_back(Persistent<Context, CopyablePersistentTraits<Context>>(isolate, Context::New(isolate)));
        this->current_ctx = 0;
    }

    V8Instance::~V8Instance() {
        delete contexts;
        isolate->Dispose();
        delete create_params->array_buffer_allocator;
    }

    Isolate* V8Instance::get_isolate() const {
        return isolate;
    }

    size_t V8Instance::create_new_context() {
        v8::HandleScope handle_scope(isolate);
        contexts->push_back(Persistent<Context, CopyablePersistentTraits<Context>>(
                               isolate, Context::New(isolate)));
        return contexts->size();
    }

    void V8Instance::set_context(size_t context_id) {
        current_ctx = context_id;
    }

    Local<Context> V8Instance::get_current_ctx() {
        return Local<Context>::New(isolate, contexts->at(current_ctx));
    }

    bpy::object V8Instance::run_source(std::string source) {
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);
        Context::Scope context_scope(get_current_ctx());
        Local<Value> value = run_source(source, get_current_ctx());
        return convert(value, get_current_ctx());
    }

    Local<Value> handle_exception(TryCatch& try_catch, Local<Context> ctx) {
        if (try_catch.HasCaught()) {
            V8Exception exc(try_catch, ctx);
            try_catch.Reset();
            throw exc;
        } else {
            throw V8Exception("Tried to handle exception but no exception was caught", "undefined", -1, "undefined");
        }
    }

    Local<Value> V8Instance::run_source(std::string source, Local<Context> context) {
        Local<String> v8_source =
            String::NewFromUtf8(isolate, source.c_str())
            .ToLocalChecked();

        TryCatch try_catch(isolate);

        MaybeLocal<Script> script_dirty =
            Script::Compile(context, v8_source);
        Local<Script> script;
        if (!script_dirty.ToLocal(&script))
            return handle_exception(try_catch, context);

        MaybeLocal<Value> result_dirty = script->Run(context);
        Local<Value> result_checked;
        if (!result_dirty.ToLocal(&result_checked))
            return handle_exception(try_catch, context);

        return result_checked;
    }

    template <typename T>
    bpy::object V8Instance::v8_local_to_py_object(Local<Value> value,
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

    bpy::object V8Instance::v8_local_to_py_object(Local<Object> value,
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

    bpy::object V8Instance::v8_local_to_py_object(Local<Array> value,
                                                  Local<Context> ctx) {
        bpy::list ret;
        for (uint32_t i = 0; i < value->Length(); i++) {
            Local<Value> val = value->Get(ctx, i).ToLocalChecked();
            ret.append(convert(val, ctx));
        }
        return ret;
    }

    bpy::object V8Instance::v8_local_to_py_object(Local<String> value, Local<Context> ctx) {
        String::Utf8Value str_value(ctx->GetIsolate(), value);
        return bpy::object((std::string)*str_value);
    }

    bpy::object V8Instance::v8_local_to_py_object(Local<BigInt> value, Local<Context> ctx) {
        String::Utf8Value utf_str(ctx->GetIsolate(), value);
        std::string str_val = *utf_str;
        bpy::handle<> handle(PyLong_FromString(str_val.c_str(), NULL, 10));
        return bpy::object(handle);
    }

    bpy::object V8Instance::convert(Local<Value> value, Local<Context> ctx) {
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
        else if (value->IsBigInt()) {
            return v8_local_to_py_object(value.As<BigInt>(), ctx);
        }
        else if (value->IsObject()) {
            MaybeLocal<Object> maybe_obj = Local<Object>::Cast(value);
            auto str = value->TypeOf(ctx->GetIsolate());
            String::Utf8Value str_str(ctx->GetIsolate(), str);
            Local<Object> obj;
            if (!maybe_obj.ToLocal(&obj))
                return bpy::object();
            return v8_local_to_py_object(obj, ctx);
        }
        else {
            throw V8Exception("Conversion error", "undefined", -1, "undefined");
        }
    }
}
