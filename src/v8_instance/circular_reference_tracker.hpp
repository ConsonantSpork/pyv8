#pragma once

#include <iostream>
#include <map>
#include <stdexcept>
#include "i_circular_reference_tracker.hpp"

namespace pyv8 {
    /**
     * @brief An implementation of pyduk::ICircularReferenceTracker interface.
     * @tparam Value value type
     * @tparam Reference unique reference type
     */
    template<typename Value, typename Reference>
    class CircularReferenceTracker : public ICircularReferenceTracker<Value, Reference> {
    private:
        std::map<Reference, Value> reference_map;
    public:
        bool reference_creates_cycle(Reference reference) override {
            return reference_map.find(reference) != reference_map.end();
        }

        Value get_by_reference(Reference reference) override {
            return reference_map[reference];
        }

        void add_reference(Value value, Reference reference) override {
            if(reference_map.find(reference) == reference_map.end()) {
                reference_map[reference] = value;
                return;
            }
            throw std::runtime_error("Tried to add a reference cycle");
        }

        void clear() override {
            reference_map.clear();
        }
    };
}
