#include "v8_instance.hpp"
#include "v8_exception.hpp"
#include <chrono>

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

    Isolate* V8Instance::get_isolate() const {
        return isolate;
    }

    bpy::object V8Instance::run(const std::string& source) {
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);
        if (!isolate->InContext())
            new_context();
        Local<Value> value = run_source(source);
        return convert_wrapper(value);
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

    Local<Value> V8Instance::run_source(const std::string& source) {
        Local<String> v8_source =
            String::NewFromUtf8(isolate, source.c_str())
            .ToLocalChecked();

        TryCatch try_catch(isolate);

        MaybeLocal<Script> script_dirty =
            Script::Compile(isolate->GetCurrentContext(), v8_source);
        Local<Script> script;
        if (!script_dirty.ToLocal(&script))
            return handle_exception(try_catch, isolate->GetCurrentContext());

        MaybeLocal<Value> result_dirty = script->Run(isolate->GetCurrentContext());
        Local<Value> result_checked;
        if (!result_dirty.ToLocal(&result_checked))
            return handle_exception(try_catch, isolate->GetCurrentContext());

        return result_checked;
    }

    void V8Instance::new_context() {
        HandleScope handle_scope(isolate);
        v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
        Context::New(isolate, nullptr, global)->Enter();
    }

    void V8Instance::exit_context() {
        if (isolate->InContext()) {
            HandleScope handle_scope(isolate);
            isolate->GetCurrentContext()->Exit();
        }
    }



    bpy::object V8Instance::v8_local_to_py_object(Local<Object> value) {
        int hash = value->GetIdentityHash();
        if (crt.reference_creates_cycle(hash))
            return crt.get_by_reference(hash);

        bpy::dict ret;
        crt.add_reference(ret, hash);
        MaybeLocal<Array> keys_maybe = value->GetOwnPropertyNames(isolate->GetCurrentContext());
        Local<Array> keys;
        if (!keys_maybe.ToLocal(&keys)) {
            return bpy::object();
        }

        for (uint32_t i = 0; i < keys->Length(); i++) {
            Local<Value> key = keys->Get(isolate->GetCurrentContext(), i).ToLocalChecked();
            Local<Value> val = value->Get(isolate->GetCurrentContext(), key).ToLocalChecked();

            ret[convert(key)] = convert(val);
        }

        return ret;
    }

    bpy::object V8Instance::v8_local_to_py_object(Local<Array> value) {
        int hash = value->GetIdentityHash();
        if (crt.reference_creates_cycle(hash))
            return crt.get_by_reference(hash);

        bpy::list ret;
        crt.add_reference(ret, hash);
        for (uint32_t i = 0; i < value->Length(); i++) {
            Local<Value> val = value->Get(isolate->GetCurrentContext(), i).ToLocalChecked();
            ret.append(convert(val));
        }
        return ret;
    }

    bpy::object V8Instance::v8_local_to_py_object(Local<TypedArray> value) {
        int hash = value->GetIdentityHash();
        if (crt.reference_creates_cycle(hash))
            return crt.get_by_reference(hash);

        char* data = new char[value->ByteLength()];
        value->CopyContents(data, value->ByteLength());
        return bpy::object(bpy::handle<>(
                               PyByteArray_FromStringAndSize(data, value->ByteLength())
                           ));
    }

    bpy::object V8Instance::v8_local_to_py_object(Local<ArrayBuffer> value) {
        int hash = value->GetIdentityHash();
        if (crt.reference_creates_cycle(hash))
            return crt.get_by_reference(hash);

        auto contents = value->GetContents();
        char* data = static_cast<char*>(contents.Data());
        bpy::object ret = bpy::object(bpy::handle<>(
                PyByteArray_FromStringAndSize(data, contents.ByteLength())
        ));
        crt.add_reference(ret, hash);
        return ret;
    }

    bpy::object V8Instance::v8_local_to_py_object(Local<String> value) {
        String::Utf8Value str_value(isolate, value);
        return bpy::object((std::string)*str_value);
    }

    bpy::object V8Instance::v8_local_to_py_object(Local<BigInt> value) {
        String::Utf8Value utf_str(isolate, value);
        std::string str_val = *utf_str;
        bpy::handle<> handle(PyLong_FromString(str_val.c_str(), NULL, 10));
        return bpy::object(handle);
    }

    bpy::object V8Instance::convert(Local<Value> value) {
        bpy::object ret_obj;
        if (value->IsBoolean() || value->IsBooleanObject()) {
            bool ret = value->BooleanValue(isolate);
            ret_obj = bpy::object(ret);
        }
        else if (value->IsInt32() || value->IsUint32())
            ret_obj = v8_local_to_py_object<v8::Integer>(value, &Value::ToInteger);
        else if (value->IsNumber()) {
            ret_obj = v8_local_to_py_object<v8::Number>(value, &Value::ToNumber);
        }
        else if (value->IsNullOrUndefined()) {
            ret_obj = bpy::object();
        }
        else if (value->IsArray()) {
            ret_obj = v8_local_to_py_object(value.As<Array>());
        }
        else if (value->IsString() || value->IsStringObject()) {
            ret_obj = v8_local_to_py_object(value.As<String>());
        }
        else if (value->IsTypedArray())
            ret_obj = v8_local_to_py_object(value.As<TypedArray>());
        else if (value->IsArrayBuffer()) {
            ret_obj = v8_local_to_py_object(value.As<ArrayBuffer>());
        }
        else if (value->IsBigInt()) {
            ret_obj = v8_local_to_py_object(value.As<BigInt>());
        }
        else if (value->IsObject()) {
            MaybeLocal<Object> maybe_obj = Local<Object>::Cast(value);
            Local<Object> obj;
            if (!maybe_obj.ToLocal(&obj))
                ret_obj = bpy::object();
            else
                ret_obj = v8_local_to_py_object(obj);
        }
        else {
            throw V8Exception("Conversion error", "undefined", -1, "undefined");
        }
        return ret_obj;
    }

    bpy::object V8Instance::convert_wrapper(Local<Value> value) {
        bpy::object ret = convert(value);
        crt.clear();
        return ret;
    }

    int V8Instance::try_get_hash(Local<Value> value) {
        MaybeLocal<Object> maybe_obj = Local<Object>::Cast(value);
        Local<Object> obj;
        if (!maybe_obj.ToLocal(&obj))
            return 0;
        return obj->GetIdentityHash();
    }
}
