#ifndef RESULTPP_RESULTIMPL_HXX
#define RESULTPP_RESULTIMPL_HXX

#include <string>// std::string
#include <type_traits>

namespace resultpp::internal {
    /**
     * @class ResultImpl
     * @brief Template class for representing an outcome
     * @tparam T The type of data/ value to be encapsulated
     *
     * @details The `ResultImpl` class can be used in different ways:
     *
     * \- Creating an instance with both data and an optional message.
     * \- Setting an error message.
     * \- Extracting the stored data or error message.
     * \- Checking if the result is in an error state.
     *
     * @note It's important to use this class with care and ensure that it
     * accurately represents the intended result of encapsulated
     */
    template<typename T>
    class ResultImpl {
        using resultimpl_t = ResultImpl<T>;

    protected:
        T _type;
        std::string _message;

    public:
        /**
         * @brief Default constructor that initializes the instance with default values.
         */
        ResultImpl() = default;

        /**
         * @brief Constructor to create an instance with both data and an optional message.
         *
         * @param type The data or value to be stored.
         * @param msg An optional error message (default is an empty string).
         */
        ResultImpl(T &&type, std::string &&msg = "")
                : _type(std::forward<T>(type)), _message(std::forward<std::string>(msg)) {}

        ResultImpl(const T &type, const std::string &msg = "")
                : _type(type), _message(msg) {}

        /**
         * @brief Operator to set the error message.
         * @param message The error message to set.
         */
        void operator=(const std::string &message) { this->_message = message; }

        /**
         * @brief Operator to override the current values with those of another ResultImpl instance.
         * @param ri The ResultImpl instance from which to copy the values.
         */
        void operator=(const resultimpl_t &ri) {
            if (this == &ri) return;

            this->_type = std::move(ri._type);
            this->_message = ri._message;
        }

        /**
         * @brief Equality operator for comparing two ResultImpl instances.
         * @param lhs The left-hand side ResultImpl instance for comparison.
         * @return `true` if the instances are equal, `false` otherwise.
         */
        inline bool operator==(const resultimpl_t &lhs) const noexcept {
            return _type == lhs._type;
        }

        /**
         * @brief Swap the content of two ResultImpl instances.
         *
         * @param r1 The first ResultImpl<T> instance.
         * @param r2 The second ResultImpl<T> instance.
         */
        friend void swap(resultimpl_t &r1, resultimpl_t &r2) {
            std::swap(r1._type, r2._type);
            std::swap(r1._message, r2._message);
        }

        /**
         * @brief Operator to override the stored data with new data.
         *
         * @param data The new data to be stored.
         * @return A reference to the updated ResultImpl instance.
         */
        resultimpl_t &operator<<(const T &data) {
            this->_type = data;
            return *this;
        }

        /**
         * @brief Get the stored data.
         * @return The stored data or value.
         */
        [[nodiscard]] const T &Data() const { return _type; }

        /**
         * @brief Get the associated error message.
         * @return The associated error message.
         */
        [[nodiscard]] const std::string &Message() const { return _message; }

        /**
         * @brief Set the data using rvalue reference.
         * @param data The new data to be stored.
         */
        void SetData(T &&data) { _type = std::forward<T>(data); }

        /**
         * @brief Set the data using an lvalue reference.
         * @param data The new data to be stored.
         */
        void SetData(const T &data) { _type = data; }

        /**
         * @brief Check if the instance represents a successful result (Ok).
         * @return `true` if the instance is in a success state (message is empty), `false` otherwise.
         */
        bool IsOk() const { return _message.empty(); }

        /**
         * @brief Check if the instance represents an error result (Err).
         * @return `true` if the instance is in an error state (message is not empty), `false` otherwise.
         */
        bool IsErr() const { return !_message.empty(); }
    };
}// namespace resultpp::internal

#endif//RESULTPP_RESULTIMPL_HXX