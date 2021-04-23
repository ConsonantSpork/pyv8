#include <gtest/gtest.h>
#include <boost/python.hpp>
#include "instance_fixture.hpp"

namespace pyv8_tests {
    class Conversion : public V8InstanceFixture {};

    TEST_F(Conversion, String) {
        EXPECT_EQ(instance->run("'Hello, World!'"),
                  boost::python::object("Hello, World!"));
    }

    TEST_F(Conversion, BooleanObject) {
        EXPECT_EQ(instance->run("new Boolean(1)"),
                  boost::python::object(true));
    }

    TEST_F(Conversion, BooleanPrimitive) {
        EXPECT_EQ(instance->run("false"),
                  boost::python::object(false));
    }

    TEST_F(Conversion, IntegerPositive) {
        EXPECT_EQ(instance->run("4096"),
                  boost::python::object(4096));
    }

    TEST_F(Conversion, IntegerNegative) {
        EXPECT_EQ(instance->run("-4096"),
                  boost::python::object(-4096));
    }

    TEST_F(Conversion, Float) {
        EXPECT_EQ(instance->run("-4096.12"),
                  boost::python::object(-4096.12));
    }

    TEST_F(Conversion, Undefined) {
        EXPECT_EQ(instance->run("undefined"),
                  boost::python::object());  // Python's None
    }

    TEST_F(Conversion, Null) {
        EXPECT_EQ(instance->run("null"),
                  boost::python::object());  // Python's None
    }

    TEST_F(Conversion, ArraySimple) {
        boost::python::list expected;
        for(int i = 0; i < 4; i++)
            expected.append(i + 1);
        EXPECT_EQ(instance->run("[1, 2, 3, 4]"),
                  expected);
    }

    TEST_F(Conversion, ArrayMixed) {
        boost::python::dict inner_dict;
        inner_dict["key"] = "val";
        boost::python::list inner_list;
        inner_list.append(1);
        inner_list.append(2);
        inner_list.append(3);
        boost::python::list expected;
        expected.append(1);
        expected.append(2.4);
        expected.append(inner_dict);
        expected.append(inner_list);
        EXPECT_EQ(instance->run("[1, 2.4, {key: 'val'}, [1, 2, 3]]"),
                  expected);
    }

    TEST_F(Conversion, BigInt) {
        EXPECT_EQ(instance->run("2**34"),
                  boost::python::handle<>(PyLong_FromString("17179869184", NULL, 10)));
    }

    TEST_F(Conversion, array_buffer_conversion) {
        EXPECT_EQ(instance->run("new Int8Array([258, 2, 0, -2]).buffer"),
                  boost::python::handle<>(PyByteArray_FromStringAndSize("\x02\x02\x00\xfe", 4)));
    }

    TEST_F(Conversion, uint8_array_conversion) {
        EXPECT_EQ(instance->run("new Uint8Array([258, 2, 0, -2]).buffer"),
                  boost::python::handle<>(PyByteArray_FromStringAndSize("\x02\x02\x00\xfe", 4)));
    }

    TEST_F(Conversion, int8_array_conversion) {
        EXPECT_EQ(instance->run("new Int8Array([130, -2, 3, 256, 0, 1])"),
                  boost::python::handle<>(PyByteArray_FromStringAndSize("\x82\xfe\x03\x00\x00\x01", 6)));
    }

    TEST_F(Conversion, uint16_array_conversion) {
        EXPECT_EQ(instance->run("new Uint16Array([256, -1])"),
                  boost::python::handle<>(PyByteArray_FromStringAndSize("\x00\x01\xff\xff", 4)));
    }

    TEST_F(Conversion, int16_array_conversion) {
        EXPECT_EQ(instance->run("new Int16Array([256, -1])"),
                  boost::python::handle<>(PyByteArray_FromStringAndSize("\x00\x01\xff\xff", 4)));
    }

    TEST_F(Conversion, uint32_array_conversion) {
        EXPECT_EQ(instance->run("new Uint32Array([65536, -1])"),
                  boost::python::handle<>(PyByteArray_FromStringAndSize("\x00\x00\x01\x00\xff\xff\xff\xff", 8)));
    }

    TEST_F(Conversion, int32_array_conversion) {
        EXPECT_EQ(instance->run("new Int32Array([65536, -1])"),
                  boost::python::handle<>(PyByteArray_FromStringAndSize("\x00\x00\x01\x00\xff\xff\xff\xff", 8)));
    }


    TEST_F(Conversion, float32_array_conversion) {
        EXPECT_EQ(instance->run("new Float32Array([12.3])"),
                  boost::python::handle<>(PyByteArray_FromStringAndSize("\xcd\xcc\x44\x41", 4)));
    }

    TEST_F(Conversion, float64_array_conversion) {
        EXPECT_EQ(instance->run("new Float64Array([12.3])"),
                  boost::python::handle<>(PyByteArray_FromStringAndSize("\x9a\x99\x99\x99\x99\x99\x28\x40", 8)));
    }

    TEST_F(Conversion, ObjectSimple) {
        boost::python::dict expected;
        expected["key1"] = "val2";
        expected["key2"] = "val3";
        expected["key3"] = "val1";
        EXPECT_EQ(instance->run("var d = {key3: 'val1', key1: 'val2', key2: 'val3'}; d;"),
                  expected);
    }

    TEST_F(Conversion, ObjectComplex) {
        boost::python::list inner_list;
        inner_list.append(1);
        inner_list.append(2.12);
        inner_list.append("str");
        boost::python::dict inner_dict;
        inner_dict["inner1"] = 12;
        inner_dict["inner2"] = "val";
        boost::python::dict expected;
        expected["key1"] = "val1";
        expected["key2"] = inner_list;
        expected["key3"] = inner_dict;
        EXPECT_EQ(instance->run("var d = {key1: 'val1', key2: [1, 2.12, 'str'], key3: {inner1: 12, inner2: 'val'}}; d;"),
                  expected);
    }
}
