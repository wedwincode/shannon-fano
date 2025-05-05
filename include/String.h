#ifndef STRING_H
#define STRING_H

#include <cstring>
#include <iostream>

class String
{
public:
  String();

  String(const char* string);

  String(const String& other);

  String(String&& other) noexcept;

  String& operator=(const String& other);

  String& operator=(String&& other) noexcept;

  ~String();

  char& operator[](size_t index);

  const char& operator[](size_t index) const;

  size_t size() const;

  bool empty() const;

  const char* c_str() const;

  bool operator==(const String& other) const;

  bool operator!=(const String& other) const;

  String operator+(const char* otherString) const;

  friend std::ostream& operator<<(std::ostream& os, const String& string) {
    return os << string.c_str();
  }

  friend std::istream& operator>>(std::istream& is, String& string) {
    char buffer[BUFFER_SIZE];
    is >> buffer;

    delete[] string.data_;

    string.length_ = std::strlen(buffer);
    string.data_ = new char[string.length_ + 1];
    std::strcpy(string.data_, buffer);

    return is;
  }

private:
  static const size_t BUFFER_SIZE = 1024;

  char* data_;
  size_t length_;
};

#endif //STRING_H
