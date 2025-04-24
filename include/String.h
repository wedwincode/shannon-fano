#ifndef STRING_H
#define STRING_H

#include <cstring>
#include <ostream>

class String {
public:
    String();

    String(const char *str);

    String(const String &other);

    String(String &&other) noexcept;

    String &operator=(const String &other);

    String &operator=(String &&other) noexcept;

    ~String();

    char &operator[](size_t index);

    const char &operator[](size_t index) const;

    size_t size() const;

    bool empty() const;

    const char *c_str() const;

    bool operator==(const String &other) const;

    bool operator!=(const String &other) const;

    String operator+(const char* rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const String &str);

    friend std::istream &operator>>(std::istream &is, String &str);

private:
    char *data;
    size_t length;
};

inline String::String(): data(nullptr), length(0) {
}

inline String::String(const char *str) {
    if (str) {
        length = std::strlen(str);
        data = new char[length + 1];
        std::strcpy(data, str);
    } else {
        data = nullptr;
        length = 0;
    }
}

inline String::String(const String &other) {
    length = other.length;
    data = new char[length + 1];
    std::strcpy(data, other.data);
}

inline String::String(String &&other) noexcept: data(other.data), length(other.length) {
    other.data = nullptr;
    other.length = 0;
}

inline String &String::operator=(const String &other) {
    if (this != &other) {
        delete[] data;
        length = other.length;
        data = new char[length + 1];
        std::strcpy(data, other.data);
    }

    return *this;
}

inline String &String::operator=(String &&other) noexcept {
    if (this != &other) {
        delete[] data;
        data = other.data;
        length = other.length;
        other.data = nullptr;
        other.length = 0;
    }

    return *this;
}

inline String::~String() {
    delete[] data;
}

inline char &String::operator[](const size_t index) {
    return data[index];
}

inline const char &String::operator[](const size_t index) const {
    return data[index];
}

inline size_t String::size() const {
    return length;
}

inline bool String::empty() const {
    return size() == 0;
}

inline const char *String::c_str() const {
    return data ? data : "";
}

inline bool String::operator==(const String &other) const {
    return std::strcmp(this->c_str(), other.c_str()) == 0;
}

inline bool String::operator!=(const String &other) const {
    return !(*this == other);
}

inline String String::operator+(const char *rhs) const {
    const size_t newLen = this->size() + std::strlen(rhs);
    char* newData = new char[newLen + 1];
    std::strcpy(newData, this->c_str());
    std::strcat(newData, rhs);

    String result(newData);
    delete[] newData;
    return result;
}

inline std::ostream &operator<<(std::ostream &os, const String &str) {
    return os << str.c_str();
}

inline std::istream &operator>>(std::istream &is, String &str) {
    char buffer[1024];
    is >> buffer;

    delete[] str.data;

    str.length = std::strlen(buffer);
    str.data = new char[str.length + 1];
    std::strcpy(str.data, buffer);

    return is;
}

#endif //STRING_H
