#ifndef TABLE_H
#define TABLE_H
#include <fstream>

#include "bit_utils.h"
#include "ByteEntry.h"
#include "UnorderedMap.h"
#include "Vector.h"
#include "Pair.h"
#include "types.h"
#include "FanoExceptions.h"

class Table {
public:
    explicit Table(const Buffer &buffer);

    Table();

    Table(const Table &);

    Table(Table &&) noexcept;

    Table &operator=(const Table &);

    Table &operator=(Table &&) noexcept;

    ~Table();

    Encoded encode();

    void decode(const Encoded &encodedTable, uint8_t bitsPerCode);

    const Encoded &getCodeForByte(uint8_t byte) const;

    bool getByteByCode(const Encoded &code, uint8_t &outByte) const;

    const UnorderedMap<uint8_t, ByteEntry> &getRawTable() const;

private:
    void countByteFrequencies(const Buffer &buffer);

    Vector<ByteEntry> toVector() const;

    static void sortTableVectorByFrequency(Vector<ByteEntry> &tableVector);

    static void buildFanoCodes(Vector<ByteEntry> &tableVector, size_t start, size_t end);

    void fromVector(Vector<ByteEntry> &tableVector);

    size_t findMaxCodeLength();

    void buildReverseTable();

    UnorderedMap<uint8_t, ByteEntry> table;
    UnorderedMap<Vector<bool>, uint8_t, BoolVectorHasher<Vector<bool> > > reverseTable;
};


#endif //TABLE_H
