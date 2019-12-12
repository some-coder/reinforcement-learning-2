#include "InvalidWidthException.hpp"

InvalidWidthException::InvalidWidthException(const char* description) noexcept {
    this->description = description;
}

const char * InvalidWidthException::what() const noexcept {
    return this->description;
}