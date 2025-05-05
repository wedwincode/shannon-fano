#ifndef VECTOR_H
#define VECTOR_H
#include <stdexcept>
#include <iostream>


template <class T>
class Vector
{
public:
  Vector();

  Vector(size_t count, const T& value);

  template <typename InputIt>
  Vector(InputIt first, InputIt last);

  Vector(const Vector& other);

  Vector& operator=(const Vector& other);

  Vector(Vector&& other) noexcept;

  Vector& operator=(Vector&& other) noexcept;

  ~Vector();

  T* data();

  const T* data() const;

  void pushBack(const T& value);

  void popBack();

  T& operator[](size_t index);

  const T& operator[](size_t index) const;

  Vector& operator+=(const Vector& other);

  size_t size() const;

  bool empty() const;

  void clear();

  T* begin();

  T* end();

  const T* begin() const;

  const T* end() const;

  friend std::ostream& operator<<(std::ostream& os, const Vector& vec)
  {
    os << "[";
    for (size_t i = 0; i < vec.size_; ++i)
    {
      os << vec.data_[i];
      if (i < vec.size_ - 1)
        os << ", ";
    }
    os << "]";
    return os;
  }

  friend bool operator==(const Vector& lhs, const Vector& rhs)
  {
    if (lhs.size_ != rhs.size_)
      return false;

    for (size_t i = 0; i < lhs.size_; ++i)
    {
      if (lhs.data_[i] != rhs.data_[i])
        return false;
    }
    return true;
  }

  friend bool operator!=(const Vector& lhs, const Vector& rhs)
  {
    return !(lhs == rhs);
  }

private:
  static const size_t DEFAULT_CAPACITY = 4;

  size_t capacity_;
  size_t size_;
  T* data_;

  void resize();
};


template <class T>
Vector<T>::Vector() : capacity_(DEFAULT_CAPACITY), size_(0), data_(new T[capacity_])
{
}

template <class T>
Vector<T>::Vector(const size_t count, const T& value)
  : capacity_(count), size_(count), data_(new T[capacity_])
{
  for (size_t i = 0; i < size_; i++)
    data_[i] = value;
}

template <class T>
template <typename InputIt>
Vector<T>::Vector(InputIt first, InputIt last)
{
  size_ = capacity_ = std::distance(first, last);
  data_ = new T[capacity_];
  size_t i = 0;

  for (InputIt it = first; it != last; ++it, i++)
    data_[i] = *it;
}

template <class T>
Vector<T>::Vector(const Vector& other) : capacity_(other.capacity_),
                                        size_(other.size_),
                                        data_(new T[other.capacity_])
{
  for (size_t i = 0; i < other.size_; i++)
    data_[i] = other.data_[i];
}

template <class T>
Vector<T>& Vector<T>::operator=(const Vector& other)
{
  if (this != &other)
  {
    delete[] data_;

    capacity_ = other.capacity_;
    size_ = other.size_;
    data_ = new T[capacity_];

    for (size_t i = 0; i < size_; i++)
      data_[i] = other.data_[i];
  }
  return *this;
}

template <class T>
Vector<T>::Vector(Vector&& other) noexcept : capacity_(other.capacity_),
                                            size_(other.size_),
                                            data_(other.data_)
{
  other.data_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}

template <class T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
{
  if (this != &other)
  {
    delete[] data_;
    capacity_ = other.capacity_;
    size_ = other.size_;
    data_ = other.data_;

    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }
  return *this;
}

template <class T>
Vector<T>::~Vector()
{
  delete[] data_;
}

template <class T>
T* Vector<T>::data()
{
  return data_;
}

template <class T>
const T* Vector<T>::data() const
{
  return data_;
}

template <class T>
void Vector<T>::pushBack(const T& value)
{
  if (size_ >= capacity_)
    resize();

  data_[size_++] = value;
}

template <class T>
void Vector<T>::popBack()
{
  if (size_ > 0)
    size_--;
}

template <class T>
T& Vector<T>::operator[](size_t index)
{
  if (index >= size_)
    throw std::out_of_range("Index out of range");

  return data_[index];
}

template <class T>
const T& Vector<T>::operator[](size_t index) const
{
  if (index >= size_)
    throw std::out_of_range("Index out of range");

  return data_[index];
}

template <class T>
Vector<T>& Vector<T>::operator+=(const Vector& other)
{
  for (const T& el : other)
    pushBack(el);

  return *this;
}

template <class T>
size_t Vector<T>::size() const
{
  return size_;
}

template <class T>
bool Vector<T>::empty() const
{
  return size_ == 0;
}

template <class T>
void Vector<T>::clear()
{
  size_ = 0;
}

template <class T>
T* Vector<T>::begin() { return data_; }

template <class T>
T* Vector<T>::end() { return data_ + size_; }

template <class T>
const T* Vector<T>::begin() const { return data_; }

template <class T>
const T* Vector<T>::end() const { return data_ + size_; }

template <class T>
void Vector<T>::resize()
{
  capacity_ *= 2;
  T* newdata_ = new T[capacity_];

  for (size_t i = 0; i < size_; i++)
    newdata_[i] = data_[i];

  delete[] data_;
  data_ = newdata_;
}
#endif //VECTOR_H
