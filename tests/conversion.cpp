#include <v8_instance.hpp>
#include <gtest/gtest.h>
#include <boost/python.hpp>
#include "instance_fixture.hpp"

namespace pyv8_tests {
    class Conversion : public V8InstanceFixture {};

    TEST_F(Conversion, String) {
        EXPECT_EQ(instance->run_source("'Hello, World!'"),
                  boost::python::object("Hello, World!"));
    }

    TEST_F(Conversion, BooleanObject) {
        EXPECT_EQ(instance->run_source("new Boolean(1)"),
                  boost::python::object(true));
    }

    TEST_F(Conversion, BooleanPrimitive) {
        EXPECT_EQ(instance->run_source("false"),
                  boost::python::object(false));
    }

    TEST_F(Conversion, IntegerPositive) {
        EXPECT_EQ(instance->run_source("4096"),
                  boost::python::object(4096));
    }

    TEST_F(Conversion, IntegerNegative) {
        EXPECT_EQ(instance->run_source("-4096"),
                  boost::python::object(-4096));
    }

    TEST_F(Conversion, Float) {
        EXPECT_EQ(instance->run_source("-4096.12"),
                  boost::python::object(-4096.12));
    }

    TEST_F(Conversion, Undefined) {
        EXPECT_EQ(instance->run_source("undefined"),
                  boost::python::object());  // Python's None
    }

    TEST_F(Conversion, Null) {
        EXPECT_EQ(instance->run_source("null"),
                  boost::python::object());  // Python's None
    }

    TEST_F(Conversion, ArraySimple) {
        boost::python::list expected;
        for(int i = 0; i < 4; i++)
            expected.append(i + 1);
        EXPECT_EQ(instance->run_source("[1, 2, 3, 4]"),
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
        EXPECT_EQ(instance->run_source("[1, 2.4, {key: 'val'}, [1, 2, 3]]"),
                  expected);
    }

    TEST_F(Conversion, BigInt) {
        EXPECT_EQ(instance->run_source("2**34"),
                  boost::python::handle<>(PyLong_FromString("17179869184", NULL, 10)));
    }

    TEST_F(Conversion, ObjectSimple) {
        boost::python::dict expected;
        expected["key1"] = "val2";
        expected["key2"] = "val3";
        expected["key3"] = "val1";
        EXPECT_EQ(instance->run_source("var d = {key3: 'val1', key1: 'val2', key2: 'val3'}; d;"),
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
        EXPECT_EQ(instance->run_source("var d = {key1: 'val1', key2: [1, 2.12, 'str'], key3: {inner1: 12, inner2: 'val'}}; d;"),
                  expected);
    }
}
