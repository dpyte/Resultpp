#ifndef RESULTPP_RESULTIMPL_HXX
#define RESULTPP_RESULTIMPL_HXX

#include <functional>// std::function
#include <string>    // std::string
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
         *
         * @param lhs The left-hand side ResultImpl instance for comparison.
         * @return `true` if the instances are equal, `false` otherwise.
         */
        inline bool operator==(const resultimpl_t &lhs) const noexcept {
            return _type == lhs._type;
        }

        /**
         * @brief Equality operator for comparing two ResultImpl instances.
         *
         * @param lhs The left-hand side ResultImpl instance for comparison.
         * @return `true` if the instances are not equal, `false` otherwise.
         */
        inline bool operator!=(const resultimpl_t &lhs) const noexcept {
            return _type != lhs._type;
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
        [[nodiscard]] T &Data() const { return _type; }

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

        /**
         * @brief Maps the data value of the Result to a new value using a provided mapping function.
         *
         * If the Result is in an 'Ok' state, the provided function is applied to the data, resulting in a new 'Ok'
         * Result with the mapped data. If the Result is in an 'Err' state, the function is not applied, and a new 'Err'
         * Result is returned with the original error message.
         *
         * @param func A function that takes the current data value and returns a new value of type U.
         * @return A new Result with the mapped data if 'Ok', or a new Result with the original error message if 'Err'.
         */
        template<typename U>
        ResultImpl<U> Map(std::function<U(const T &)> func) const {
            if (IsOk()) return ResultImpl<U>(func(Data()));
            return ResultImpl<U>(Message());
        }

        /**
         * @brief Maps the error value of the Result to a new value using a provided function.
         * If the Result is in an 'Ok' state, the function is not applied, and a new 'Ok' Result is returned
         * with the original data. If the Result is in an 'Err' state, the provided function is applied to
         * the error message to produce a new 'Err' Result with the mapped error value.
         *
         * @param func A function that takes the current error message and returns a new error value of type U.
         * @return A new Result with the original data if 'Ok', or a new Result with the mapped error if 'Err'.
         */
        template<typename U>
        ResultImpl<U> MapErr(std::function<U(const T &)> func) const {
            if (IsOk()) return ResultImpl<U>(func(Message()));
            return ResultImpl<U>(Data());
        }

        /**
         * @brief Combines the current Result with another Result.
         *
         * If the current Result is in an 'Ok' state, it returns a new Result with the data of the current Result.
         * If the current Result is in an 'Err' state, it returns a new Result with the error message of the other Result.
         * If both Results are in an 'Err' state, it returns a new Result with the error message of the current Result.
         *
         * @tparam U The type to be encapsulated by the new Result. By default, it's the same as the current Result type.
         * @param other The other Result to combine with the current Result.
         * @return A new Result with the encapsulated data or error message.
         */
        template<typename U = T>
        resultimpl_t Or(const resultimpl_t &other) const {
            if (IsOk()) return resultimpl_t(Data());
            if (IsErr()) return resultimpl_t(other.Message());
            return resultimpl_t(Message());
        }

        /**
         * @brief Combines the current Result with another Result or a new Result generated by a function.
         *
         * If the current Result is in an 'Ok' state, it returns a new Result with the data of the current Result.
         * If the current Result is in an 'Err' state, it applies the provided function to the error message
         * to generate a new Result.
         *
         * @tparam U The type to be encapsulated by the new Result. By default, it's the same as the current Result type.
         * @param func A function that takes the current error message and returns a new Result.
         * @return A new Result with the encapsulated data or error message.
         */
        template<typename U = T>
        resultimpl_t OrElse(std::function<resultimpl_t(const std::string &)> func) {
            if (IsOk()) return resultimpl_t(Data());
            return func(Message());
        }
    };
}// namespace resultpp::internal

#endif//RESULTPP_RESULTIMPL_HXX