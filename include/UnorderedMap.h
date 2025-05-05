#ifndef UNORDEREDMAP_H
#define UNORDEREDMAP_H
#include <cstddef>
#include <ostream>

#include "Pair.h"

template <typename K>
struct DefaultHasher
{
  static constexpr size_t HASH_PRIME = 2654435761;

  size_t operator()(const K& key) const
  {
    return static_cast<size_t>(key) % HASH_PRIME;
  }
};

template <typename K>
struct BoolVectorHasher
{
  // FNV-1a algorithm
  size_t operator()(const K& key) const
  {
    size_t hash = 0xcbf29ce484222325;
    for (const bool bit : key)
    {
      hash ^= static_cast<size_t>(bit);
      hash *= 0x100000001b3;
    }
    return hash;
  }
};

template <typename K, typename V, typename Hasher = DefaultHasher<K>>
class UnorderedMap
{
public:
  UnorderedMap();

  UnorderedMap(const UnorderedMap& other);

  UnorderedMap(UnorderedMap&& other) noexcept;

  UnorderedMap& operator=(const UnorderedMap& other);

  UnorderedMap& operator=(UnorderedMap&& other) noexcept;

  ~UnorderedMap();

  V& operator[](const K& key);

  const V& operator[](const K& key) const;

  void insert(const K& key, const V& value);

  bool contains(const K& key) const;

  bool erase(const K& key);

  void clear();

  size_t size() const;

  bool empty() const;

  struct Entry
  {
    bool occupied = false;
    bool deleted = false;
    K key;
    V value;
  };

  class Iterator
  {
  public:
    Iterator(Entry* ptr, Entry* end);

    Iterator& operator++();

    bool operator!=(const Iterator& other) const;

    Pair<const K&, V&> operator*() const;

  private:
    Entry* current_;
    Entry* end_;

    void skipToValid();
  };

  class ConstIterator
  {
  public:
    ConstIterator(const Entry* ptr, const Entry* end);

    ConstIterator& operator++();

    bool operator!=(const ConstIterator& other) const;

    Pair<const K&, const V&> operator*() const;

  private:
    const Entry* current_;
    const Entry* end_;

    void skipToValid();
  };

  Iterator begin();

  Iterator end();

  ConstIterator begin() const;

  ConstIterator end() const;

  ConstIterator cbegin() const;

  ConstIterator cend() const;

  friend std::ostream& operator<<(std::ostream& os, const UnorderedMap& map)
  {
    os << "{\n";
    bool first = true;
    for (size_t i = 0; i < map.tableSize_; ++i)
    {
      const Entry& entry = map.table_[i];
      if (entry.occupied && !entry.deleted)
      {
        if (!first) os << ", \n";
        os << static_cast<int>(entry.key) << ": " << entry.value;
        first = false;
      }
    }
    os << "\n}";
    return os;
  }

private:
  Entry* table_;
  size_t tableSize_;
  size_t elementCount_;
  static constexpr float MAX_LOAD_FACTOR = 0.75f;
  static constexpr size_t INITIAL_CAPACITY = 16;

  size_t findSlot(const K& key) const;

  V& insertOrGet(const K& key);

  void rehash();
};

template <typename K, typename V, typename Hasher>
UnorderedMap<K, V, Hasher>::UnorderedMap() : tableSize_(INITIAL_CAPACITY),
                                             elementCount_(0)
{
  table_ = new Entry[tableSize_];
}

template <typename K, typename V, typename Hasher>
UnorderedMap<K, V, Hasher>::UnorderedMap(const UnorderedMap& other) :
  tableSize_(other.tableSize_),
  elementCount_(other.elementCount_)
{
  table_ = new Entry[tableSize_];
  for (size_t i = 0; i < tableSize_; i++)
    table_[i] = other.table_[i];
}

template <typename K, typename V, typename Hasher>
UnorderedMap<K, V, Hasher>::UnorderedMap(UnorderedMap&& other) noexcept :
  table_(other.table_), tableSize_(other.tableSize_),
  elementCount_(other.elementCount_)
{
  other.table_ = nullptr;
  other.tableSize_ = 0;
  other.elementCount_ = 0;
}

template <typename K, typename V, typename Hasher>
UnorderedMap<K, V, Hasher>& UnorderedMap<K, V, Hasher>::operator=(
  const UnorderedMap& other)
{
  if (this != &other)
  {
    delete[] table_;
    tableSize_ = other.tableSize_;
    elementCount_ = other.elementCount_;
    table_ = new Entry[tableSize_];
    for (size_t i = 0; i < tableSize_; i++)
      table_[i] = other.table_[i];
  }
  return *this;
}

template <typename K, typename V, typename Hasher>
UnorderedMap<K, V, Hasher>& UnorderedMap<K, V, Hasher>::operator=(
  UnorderedMap&& other) noexcept
{
  if (this != &other)
  {
    delete[] table_;
    table_ = other.table_;
    tableSize_ = other.tableSize_;
    elementCount_ = other.elementCount_;
    other.table_ = nullptr;
    other.tableSize_ = 0;
    other.elementCount_ = 0;
  }
  return *this;
}

template <typename K, typename V, typename Hasher>
UnorderedMap<K, V, Hasher>::~UnorderedMap()
{
  delete[] table_;
}

template <typename K, typename V, typename Hasher>
V& UnorderedMap<K, V, Hasher>::operator[](const K& key)
{
  return insertOrGet(key);
}

template <typename K, typename V, typename Hasher>
const V& UnorderedMap<K, V, Hasher>::operator[](const K& key) const
{
  size_t index = findSlot(key);
  if (!table_[index].occupied || table_[index].deleted)
    throw std::out_of_range("Key not found in const operator[]");

  return table_[index].value;
}

template <typename K, typename V, typename Hasher>
void UnorderedMap<K, V, Hasher>::insert(const K& key, const V& value)
{
  insertOrGet(key) = value;
}

template <typename K, typename V, typename Hasher>
bool UnorderedMap<K, V, Hasher>::contains(const K& key) const
{
  size_t index = findSlot(key);
  return table_[index].occupied && !table_[index].deleted;
}

template <typename K, typename V, typename Hasher>
bool UnorderedMap<K, V, Hasher>::erase(const K& key)
{
  size_t index = findSlot(key);

  if (table_[index].occupied && !table_[index].deleted)
  {
    table_[index].deleted = true;
    --elementCount_;
    return true;
  }

  return false;
}

template <typename K, typename V, typename Hasher>
void UnorderedMap<K, V, Hasher>::clear()
{
  delete[] table_;
  tableSize_ = INITIAL_CAPACITY;
  table_ = new Entry[tableSize_];
  elementCount_ = 0;
}

template <typename K, typename V, typename Hasher>
size_t UnorderedMap<K, V, Hasher>::size() const
{
  return elementCount_;
}

template <typename K, typename V, typename Hasher>
bool UnorderedMap<K, V, Hasher>::empty() const
{
  return elementCount_ == 0;
}

template <typename K, typename V, typename Hasher>
UnorderedMap<
  K, V, Hasher>::Iterator::Iterator(Entry* ptr, Entry* end) : current_(ptr),
  end_(end)
{
  skipToValid();
}

template <typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::Iterator& UnorderedMap<
  K, V, Hasher>::Iterator::operator++()
{
  ++current_;
  skipToValid();
  return *this;
}

template <typename K, typename V, typename Hasher>
bool UnorderedMap<K, V, Hasher>::Iterator::operator!=(
  const Iterator& other) const
{
  return current_ != other.current_;
}

template <typename K, typename V, typename Hasher>
Pair<const K&, V&> UnorderedMap<K, V, Hasher>::Iterator::operator*() const
{
  return {current_->key, current_->value};
}

template <typename K, typename V, typename Hasher>
void UnorderedMap<K, V, Hasher>::Iterator::skipToValid()
{
  while (current_ != end_ && (!current_->occupied || current_->deleted))
    ++current_;
}

template <typename K, typename V, typename Hasher>
UnorderedMap<K, V, Hasher>::ConstIterator::ConstIterator(
  const Entry* ptr, const Entry* end) : current_(ptr), end_(end)
{
  skipToValid();
}

template <typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::ConstIterator& UnorderedMap<
  K, V, Hasher>::ConstIterator::operator++()
{
  ++current_;
  skipToValid();
  return *this;
}

template <typename K, typename V, typename Hasher>
bool UnorderedMap<K, V, Hasher>::ConstIterator::operator!=(
  const ConstIterator& other) const
{
  return current_ != other.current_;
}

template <typename K, typename V, typename Hasher>
Pair<const K&, const V&> UnorderedMap<K, V, Hasher>::ConstIterator::operator
*() const
{
  return {current_->key, current_->value};
}

template <typename K, typename V, typename Hasher>
void UnorderedMap<K, V, Hasher>::ConstIterator::skipToValid()
{
  while (current_ != end_ && (!current_->occupied || current_->deleted))
    ++current_;
}

template <typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::Iterator UnorderedMap<
  K, V, Hasher>::begin()
{
  return Iterator(table_, table_ + tableSize_);
}

template <typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::Iterator UnorderedMap<K, V, Hasher>::end()
{
  return Iterator(table_ + tableSize_, table_ + tableSize_);
}

template <typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::ConstIterator UnorderedMap<
  K, V, Hasher>::begin() const
{
  return ConstIterator(table_, table_ + tableSize_);
}

template <typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::ConstIterator UnorderedMap<
  K, V, Hasher>::end() const
{
  return ConstIterator(table_ + tableSize_, table_ + tableSize_);
}

template <typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::ConstIterator UnorderedMap<
  K, V, Hasher>::cbegin() const
{
  return begin();
}

template <typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::ConstIterator UnorderedMap<
  K, V, Hasher>::cend() const
{
  return end();
}

template <typename K, typename V, typename Hasher>
size_t UnorderedMap<K, V, Hasher>::findSlot(const K& key) const
{
  size_t mask = tableSize_ - 1;
  size_t hash = Hasher{}(key) & mask;
  size_t step = 1;
  while (table_[hash].occupied &&
    (table_[hash].deleted || !(table_[hash].key == key)))
  {
    hash = hash + step * step & mask;
    step++;
  }
  return hash;
}

template <typename K, typename V, typename Hasher>
V& UnorderedMap<K, V, Hasher>::insertOrGet(const K& key)
{
  if (static_cast<float>(elementCount_ + 1) / tableSize_ > MAX_LOAD_FACTOR)
    rehash();

  size_t mask = tableSize_ - 1;
  size_t hash = Hasher{}(key) & mask;
  size_t step = 1;
  size_t firstDeleted = tableSize_;

  while (table_[hash].occupied)
  {
    if (!table_[hash].deleted && table_[hash].key == key)
      return table_[hash].value;

    if (table_[hash].deleted && firstDeleted == tableSize_)
      firstDeleted = hash;

    hash = hash + step * step & mask;
    step++;
  }

  size_t target = firstDeleted != tableSize_ ? firstDeleted : hash;
  table_[target].occupied = true;
  table_[target].deleted = false;
  table_[target].key = key;
  elementCount_++;
  return table_[target].value;
}

template <typename K, typename V, typename Hasher>
void UnorderedMap<K, V, Hasher>::rehash()
{
  const size_t newCapacity = tableSize_ * 2;
  Entry* oldTable = table_;
  const size_t oldSize = tableSize_;

  table_ = new Entry[newCapacity];
  tableSize_ = newCapacity;
  elementCount_ = 0;

  for (size_t i = 0; i < oldSize; i++)
    if (oldTable[i].occupied && !oldTable[i].deleted)
      insert(oldTable[i].key, oldTable[i].value);

  delete[] oldTable;
}
#endif //UNORDEREDMAP_H
