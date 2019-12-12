#ifndef INVALID_WIDTH_EXCEPTION_HPP
#define INVALID_WIDTH_EXCEPTION_HPP

#include <string>

class InvalidWidthException : public std::exception {
    private:
        const char* description;

    public:
        explicit InvalidWidthException(const char* description) noexcept;
        virtual const char* what() const noexcept;
};

#endif