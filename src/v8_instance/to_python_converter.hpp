#include <utility>
#include <v8.h>
#include <boost/python.hpp>

using namespace v8;
namespace bpy = boost::python;

namespace pyv8 {
    class V8LocalContextPairToPython {
        public:
            static PyObject* convert(std::pair<Local<Value>, Persistent<Context>>);

        private:
            static bpy::object convert(Local<Value>, Local<Context>);

            template <typename T>
            static bpy::object v8_local_to_py_object(Local<Value> value,
                                                     MaybeLocal<T> (Value::*ToType)(Local<Context>),
                                                     Local<Context> ctx) {
                Value* unwrapped = *value;
                MaybeLocal<T> converted = (unwrapped->*ToType)(ctx);
                Local<T> checked;
                if(!converted.ToLocal(&checked))
                    return bpy::object();
                return bpy::object(checked->Value());
            }

            static bpy::object v8_local_to_py_object(Local<Object> value,
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

            static bpy::object v8_local_to_py_object(Local<Array> value,
                                                     Local<Context> ctx) {
                bpy::list ret;
                for (uint32_t i = 0; i < value->Length(); i++) {
                    Local<Value> val = value->Get(ctx, i).ToLocalChecked();
                    ret.append(convert(val, ctx));
                }
                return ret;
            }

            static bpy::object v8_local_to_py_object(Local<String> value,
                                                     Local<Context> ctx) {
                String::Utf8Value str_value(ctx->GetIsolate(), value);
                return bpy::object((std::string)*str_value);
            }

            static bpy::object v8_local_to_py_object(Local<BigInt> value,
                                                     Local<Context> ctx) {
                String::Utf8Value utf_str(ctx->GetIsolate(), value);
                std::string str_val = *utf_str;
                bpy::handle<> handle(PyLong_FromString(str_val.c_str(), NULL, 10));
                return bpy::object(handle);
            }
    };
}
