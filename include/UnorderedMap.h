#ifndef UNORDEREDMAP_H
#define UNORDEREDMAP_H
#include <cstddef>
#include <ostream>

#include "Pair.h"

template<typename K>
struct DefaultHasher {
    size_t operator()(const K &key) const {
        return static_cast<size_t>(key) % 2654435761;
    }
};

template<typename K>
struct BoolVectorHasher {
    // FNV-1a algorithm
    size_t operator()(const K &key) const {
        size_t hash = 0xcbf29ce484222325;
        for (const bool bit: key) {
            hash ^= static_cast<size_t>(bit);
            hash *= 0x100000001b3;
        }
        return hash;
    }
};

template<typename K, typename V, typename Hasher = DefaultHasher<K> >
class UnorderedMap {
public:
    UnorderedMap();

    UnorderedMap(const UnorderedMap &other);

    UnorderedMap(UnorderedMap &&other) noexcept;

    UnorderedMap &operator=(const UnorderedMap &other);

    UnorderedMap &operator=(UnorderedMap &&other) noexcept;

    ~UnorderedMap();

    V &operator[](const K &key);

    const V &operator[](const K &key) const;

    void insert(const K &key, const V &value);

    bool contains(const K &key) const;

    bool erase(const K &key);

    void clear();

    size_t size() const;

    bool empty() const;

    struct Entry {
        bool occupied = false;
        bool deleted = false;
        K key;
        V value;
    };

    class Iterator {
    public:
        Iterator(Entry *ptr, Entry *end);

        Iterator &operator++();

        bool operator!=(const Iterator &other) const;

        Pair<const K &, V &> operator*() const;

    private:
        Entry *current;
        Entry *end;

        void skipToValid();
    };

    class ConstIterator {
    public:
        ConstIterator(const Entry *ptr, const Entry *end);

        ConstIterator &operator++();

        bool operator!=(const ConstIterator &other) const;

        Pair<const K &, const V &> operator*() const;

    private:
        const Entry *current;
        const Entry *end;

        void skipToValid();
    };

    Iterator begin();

    Iterator end();

    ConstIterator begin() const;

    ConstIterator end() const;

    ConstIterator cbegin() const;

    ConstIterator cend() const;

    friend std::ostream &operator<<(std::ostream &os, const UnorderedMap &map) {
        os << "{\n";
        bool first = true;
        for (size_t i = 0; i < map.tableSize; ++i) {
            const Entry &entry = map.table[i];
            if (entry.occupied && !entry.deleted) {
                if (!first) os << ", \n";
                os << static_cast<int>(entry.key) << ": " << entry.value;
                first = false;
            }
        }
        os << "\n}";
        return os;
    }

private:
    Entry *table;
    size_t tableSize;
    size_t elementCount;
    static constexpr float MAX_LOAD_FACTOR = 0.75f;
    static constexpr size_t INITIAL_CAPACITY = 16;

    size_t findSlot(const K &key) const;

    V &insertOrGet(const K &key);

    void rehash();
};

template<typename K, typename V, typename Hasher>
UnorderedMap<K, V, Hasher>::UnorderedMap(): tableSize(INITIAL_CAPACITY), elementCount(0) {
    table = new Entry[tableSize];
}

template<typename K, typename V, typename Hasher>
UnorderedMap<K, V, Hasher>::UnorderedMap(const UnorderedMap &other): tableSize(other.tableSize),
                                                                     elementCount(other.elementCount) {
    table = new Entry[tableSize];
    for (size_t i = 0; i < tableSize; i++)
        table[i] = other.table[i];
}

template<typename K, typename V, typename Hasher>
UnorderedMap<K, V, Hasher>::UnorderedMap(UnorderedMap &&other) noexcept: table(other.table), tableSize(other.tableSize),
                                                                         elementCount(other.elementCount) {
    other.table = nullptr;
    other.tableSize = 0;
    other.elementCount = 0;
}

template<typename K, typename V, typename Hasher>
UnorderedMap<K, V, Hasher> &UnorderedMap<K, V, Hasher>::operator=(const UnorderedMap &other) {
    if (this != &other) {
        delete[] table;
        tableSize = other.tableSize;
        elementCount = other.elementCount;
        table = new Entry[tableSize];
        for (size_t i = 0; i < tableSize; i++)
            table[i] = other.table[i];
    }
    return *this;
}

template<typename K, typename V, typename Hasher>
UnorderedMap<K, V, Hasher> &UnorderedMap<K, V, Hasher>::operator=(UnorderedMap &&other) noexcept {
    if (this != &other) {
        delete[] table;
        table = other.table;
        tableSize = other.tableSize;
        elementCount = other.elementCount;
        other.table = nullptr;
        other.tableSize = 0;
        other.elementCount = 0;
    }
    return *this;
}

template<typename K, typename V, typename Hasher>
UnorderedMap<K, V, Hasher>::~UnorderedMap() {
    delete[] table;
}

template<typename K, typename V, typename Hasher>
V &UnorderedMap<K, V, Hasher>::operator[](const K &key) {
    return insertOrGet(key);
}

template<typename K, typename V, typename Hasher>
const V &UnorderedMap<K, V, Hasher>::operator[](const K &key) const {
    size_t idx = findSlot(key);
    if (!table[idx].occupied || table[idx].deleted)
        throw std::out_of_range("Key not found in const operator[]");

    return table[idx].value;
}

template<typename K, typename V, typename Hasher>
void UnorderedMap<K, V, Hasher>::insert(const K &key, const V &value) {
    insertOrGet(key) = value;
}

template<typename K, typename V, typename Hasher>
bool UnorderedMap<K, V, Hasher>::contains(const K &key) const {
    size_t idx = findSlot(key);
    return table[idx].occupied && !table[idx].deleted;
}

template<typename K, typename V, typename Hasher>
bool UnorderedMap<K, V, Hasher>::erase(const K &key) {
    size_t idx = findSlot(key);

    if (table[idx].occupied && !table[idx].deleted) {
        table[idx].deleted = true;
        --elementCount;
        return true;
    }

    return false;
}

template<typename K, typename V, typename Hasher>
void UnorderedMap<K, V, Hasher>::clear() {
    delete[] table;
    tableSize = INITIAL_CAPACITY;
    table = new Entry[tableSize];
    elementCount = 0;
}

template<typename K, typename V, typename Hasher>
size_t UnorderedMap<K, V, Hasher>::size() const {
    return elementCount;
}

template<typename K, typename V, typename Hasher>
bool UnorderedMap<K, V, Hasher>::empty() const {
    return elementCount == 0;
}

template<typename K, typename V, typename Hasher>
UnorderedMap<K, V, Hasher>::Iterator::Iterator(Entry *ptr, Entry *end): current(ptr), end(end) {
    skipToValid();
}

template<typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::Iterator &UnorderedMap<K, V, Hasher>::Iterator::operator++() {
    ++current;
    skipToValid();
    return *this;
}

template<typename K, typename V, typename Hasher>
bool UnorderedMap<K, V, Hasher>::Iterator::operator!=(const Iterator &other) const {
    return current != other.current;
}

template<typename K, typename V, typename Hasher>
Pair<const K &, V &> UnorderedMap<K, V, Hasher>::Iterator::operator*() const {
    return {current->key, current->value};
}

template<typename K, typename V, typename Hasher>
void UnorderedMap<K, V, Hasher>::Iterator::skipToValid() {
    while (current != end && (!current->occupied || current->deleted))
        ++current;
}

template<typename K, typename V, typename Hasher>
UnorderedMap<K, V, Hasher>::ConstIterator::ConstIterator(const Entry *ptr, const Entry *end): current(ptr), end(end) {
    skipToValid();
}

template<typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::ConstIterator &UnorderedMap<K, V, Hasher>::ConstIterator::operator++() {
    ++current;
    skipToValid();
    return *this;
}

template<typename K, typename V, typename Hasher>
bool UnorderedMap<K, V, Hasher>::ConstIterator::operator!=(const ConstIterator &other) const {
    return current != other.current;
}

template<typename K, typename V, typename Hasher>
Pair<const K &, const V &> UnorderedMap<K, V, Hasher>::ConstIterator::operator*() const {
    return {current->key, current->value};
}

template<typename K, typename V, typename Hasher>
void UnorderedMap<K, V, Hasher>::ConstIterator::skipToValid() {
    while (current != end && (!current->occupied || current->deleted))
        ++current;
}

template<typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::Iterator UnorderedMap<K, V, Hasher>::begin() {
    return Iterator(table, table + tableSize);
}

template<typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::Iterator UnorderedMap<K, V, Hasher>::end() {
    return Iterator(table + tableSize, table + tableSize);
}

template<typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::ConstIterator UnorderedMap<K, V, Hasher>::begin() const {
    return ConstIterator(table, table + tableSize);
}

template<typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::ConstIterator UnorderedMap<K, V, Hasher>::end() const {
    return ConstIterator(table + tableSize, table + tableSize);
}

template<typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::ConstIterator UnorderedMap<K, V, Hasher>::cbegin() const {
    return begin();
}

template<typename K, typename V, typename Hasher>
typename UnorderedMap<K, V, Hasher>::ConstIterator UnorderedMap<K, V, Hasher>::cend() const {
    return end();
}

template<typename K, typename V, typename Hasher>
size_t UnorderedMap<K, V, Hasher>::findSlot(const K &key) const {
    size_t mask = tableSize - 1;
    size_t hash = Hasher{}(key) & mask;
    size_t step = 1;
    while (table[hash].occupied && (table[hash].deleted || !(table[hash].key == key))) {
        hash = hash + step * step & mask;
        step++;
    }
    return hash;
}

template<typename K, typename V, typename Hasher>
V &UnorderedMap<K, V, Hasher>::insertOrGet(const K &key) {
    if (static_cast<float>(elementCount + 1) / tableSize > MAX_LOAD_FACTOR)
        rehash();

    size_t mask = tableSize - 1;
    size_t hash = Hasher{}(key) & mask;
    size_t step = 1;
    size_t firstDeleted = tableSize;

    while (table[hash].occupied) {
        if (!table[hash].deleted && table[hash].key == key)
            return table[hash].value;

        if (table[hash].deleted && firstDeleted == tableSize)
            firstDeleted = hash;

        hash = hash + step * step & mask;
        step++;
    }

    size_t target = firstDeleted != tableSize ? firstDeleted : hash;
    table[target].occupied = true;
    table[target].deleted = false;
    table[target].key = key;
    elementCount++;
    return table[target].value;
}

template<typename K, typename V, typename Hasher>
void UnorderedMap<K, V, Hasher>::rehash() {
    const size_t newCapacity = tableSize * 2;
    Entry *oldTable = table;
    const size_t oldSize = tableSize;

    table = new Entry[newCapacity];
    tableSize = newCapacity;
    elementCount = 0;

    for (size_t i = 0; i < oldSize; i++)
        if (oldTable[i].occupied && !oldTable[i].deleted)
            insert(oldTable[i].key, oldTable[i].value);

    delete[] oldTable;
}
#endif //UNORDEREDMAP_H
