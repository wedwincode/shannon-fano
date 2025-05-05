#include "../include/String.h"

String::String() : data_(nullptr), length_(0)
{
}

String::String(const char* string)
{
  if (string)
  {
    length_ = std::strlen(string);
    data_ = new char[length_ + 1];
    std::strcpy(data_, string);
  }
  else
  {
    data_ = nullptr;
    length_ = 0;
  }
}

String::String(const String& other)
{
  length_ = other.length_;
  data_ = new char[length_ + 1];
  std::strcpy(data_, other.data_);
}

String::String(String&& other) noexcept : data_(other.data_),
                                          length_(other.length_)
{
  other.data_ = nullptr;
  other.length_ = 0;
}

String& String::operator=(const String& other)
{
  if (this != &other)
  {
    delete[] data_;
    length_ = other.length_;
    data_ = new char[length_ + 1];
    std::strcpy(data_, other.data_);
  }

  return *this;
}

String& String::operator=(String&& other) noexcept
{
  if (this != &other)
  {
    delete[] data_;
    data_ = other.data_;
    length_ = other.length_;
    other.data_ = nullptr;
    other.length_ = 0;
  }

  return *this;
}

String::~String()
{
  delete[] data_;
}

char& String::operator[](const size_t index)
{
  return data_[index];
}

const char& String::operator[](const size_t index) const
{
  return data_[index];
}

size_t String::size() const
{
  return length_;
}

bool String::empty() const
{
  return size() == 0;
}

const char* String::c_str() const
{
  return data_ ? data_ : "";
}

bool String::operator==(const String& other) const
{
  return std::strcmp(this->c_str(), other.c_str()) == 0;
}

bool String::operator!=(const String& other) const
{
  return !(*this == other);
}

String String::operator+(const char* otherString) const
{
  const size_t newLen = this->size() + std::strlen(otherString);
  char* newData = new char[newLen + 1];
  std::strcpy(newData, this->c_str());
  std::strcat(newData, otherString);

  String result(newData);
  delete[] newData;
  return result;
}
