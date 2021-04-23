#pragma once

namespace pyv8 {
    /**
    * @brief Circular reference tracker interface.
    * @tparam Value value type.
    * @tparam Reference reference type.
    */
    template<typename Value, typename Reference>
    struct ICircularReferenceTracker {
        /**
         * @brief Check if adding this reference will create a cycle.
         * @param reference reference to check
         * @returns whether adding this reference creates a cycle
         */
        virtual bool reference_creates_cycle(Reference reference) = 0;

        /**
         * @brief Get value by reference.
         * @param reference
         * @return value associated with reference
         */
        virtual Value get_by_reference(Reference reference) = 0;

        /**
         * @brief Add reference and check if it creates a cycle.
         * @param value value to add
         * @throws std::runtime_error if adding this reference creates a cycle
         * @param reference unique reference to associate with value
         */
        virtual void add_reference(Value value, Reference reference) = 0;

        /**
         * @brief Clear all references.
         */
        virtual void clear() = 0;
    };
}