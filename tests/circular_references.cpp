#include <gtest/gtest.h>
#include <boost/python.hpp>
#include "instance_fixture.hpp"

namespace pyv8_tests {
    class CircularReferences : public V8InstanceFixture {
    };

    TEST_F(CircularReferences, list_circular_reference) {
        boost::python::object obj = instance->run("var l = []; l.push(l); l;");
        boost::python::list list(obj);
        EXPECT_EQ(list, list[0]);
    }

    TEST_F(CircularReferences, object_circular_reference) {
        boost::python::object obj = instance->run("var l = {}; l.circ = l; l;");
        EXPECT_EQ(obj, obj["circ"]);
    }

    TEST_F(CircularReferences, complex_circular_reference) {
        boost::python::object obj = instance->run("var l = {key1: [1, 2], key2: 'hello'}; l.key1.push(l); l");
        EXPECT_EQ(obj["key2"], "hello");
        EXPECT_EQ(obj["key1"][0], 1);
        EXPECT_EQ(obj["key1"][1], 2);
        EXPECT_EQ(obj["key1"][2], obj);
    }

    TEST_F(CircularReferences, multiple_circular_references) {
        boost::python::object obj = instance->run("var o = {}; var l = []; o.key = o; l.push(l); o.list = l; o;");
        EXPECT_EQ(obj["key"], obj);
        EXPECT_EQ(obj["list"], obj["list"][0]);
    }
}