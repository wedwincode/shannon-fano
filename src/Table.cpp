#include "../include/Table.h"

#include <cmath>


Table::Table(const Buffer &buffer) {
    if (buffer.empty())
        throw FileException("File is empty");

    countByteFrequencies(buffer);
    Vector<ByteEntry> tableVector = toVector();
    sortTableVectorByFrequency(tableVector);
    buildFanoCodes(tableVector, 0, tableVector.size());
    fromVector(tableVector);
}

Table::Table() = default;

Table::Table(const Table &) = default;

Table::Table(Table &&) noexcept = default;

Table &Table::operator=(const Table &) = default;

Table &Table::operator=(Table &&) noexcept = default;

Table::~Table() = default;

Encoded Table::encode() {
    const size_t bitsPerCode = findMaxCodeLength();
    if (bitsPerCode > 255)
        throw TableException("Code is too long");

    Encoded encodedTable;
    encodedTable += bit_utils::addLeadingZerosToCode(bit_utils::byteToBits(table.size()), 8);
    encodedTable += bit_utils::addLeadingZerosToCode(bit_utils::byteToBits(bitsPerCode + 1), 8);

    for (const Pair<const uint8_t &, const ByteEntry &> pair: table) {
        encodedTable += bit_utils::byteToBits(pair.first);
        encodedTable += bit_utils::normalizeCode(pair.second.code, bitsPerCode);
    }

    return encodedTable;
}

void Table::decode(const Encoded &encodedTable, const uint8_t bitsPerCode) {
    table.clear();
    reverseTable.clear();
    const size_t recordSize = 8 + bitsPerCode;

    for (size_t i = 0; i + recordSize <= encodedTable.size(); i += recordSize) {
        Vector<bool> symbolBits(encodedTable.begin() + i, encodedTable.begin() + i + 8);
        Vector<bool> codeBits(encodedTable.begin() + i + 8, encodedTable.begin() + i + recordSize);

        ByteEntry byteEntry;
        byteEntry.byte = bit_utils::bitsToByte(symbolBits);
        byteEntry.code = bit_utils::processCodeBuffer(codeBits);
        table[byteEntry.byte] = byteEntry;
    }

    buildReverseTable();
}

double Table::calculateEntropy() const {
    int totalQuantity = 0;
    double entropy = 0.0f;
    for (const Pair<const uint8_t&, const ByteEntry&> &pair: table) {
        totalQuantity += pair.second.occurrences;
    }
    for (const Pair<const uint8_t&, const ByteEntry&> &pair: table) {
        const double freq = static_cast<double>(pair.second.occurrences) / totalQuantity;
        entropy += freq * std::log2(freq);
    }
    return -1 * entropy;
}

const Encoded &Table::getCodeForByte(const uint8_t byte) const {
    return table[byte].code;
}

bool Table::getByteByCode(const Encoded &code, uint8_t &outByte) const {
    if (!reverseTable.contains(code))
        return false;

    outByte = reverseTable[code];
    return true;
}

const UnorderedMap<uint8_t, ByteEntry> & Table::getRawTable() const {
    return table;
}

void Table::countByteFrequencies(const Buffer &buffer) {
    for (const uint8_t byte: buffer) {
        if (!table.contains(byte))
            table[byte].byte = byte;
        table[byte].occurrences++;
    }
}

Vector<ByteEntry> Table::toVector() const {
    Vector<ByteEntry> tableVector;
    for (const Pair<uint8_t, ByteEntry> pair: table)
        tableVector.pushBack(pair.second);
    return tableVector;
}

void Table::sortTableVectorByFrequency(Vector<ByteEntry> &tableVector) {
    for (int i = 0; i < tableVector.size() - 1; ++i)
        for (int j = 0; j < tableVector.size() - i - 1; ++j)
            if (tableVector[j].occurrences < tableVector[j + 1].occurrences)
                ByteEntry::swap(tableVector[j], tableVector[j + 1]);
}

void Table::buildFanoCodes(Vector<ByteEntry> &tableVector, const size_t start, const size_t end) {
    if (end - start == 1) {
        tableVector[start].code.pushBack(false);
        return;
    }

    size_t total = 0;
    for (size_t i = start; i < end; i++)
        total += tableVector[i].occurrences;

    size_t split = start;
    long long sum = tableVector[split].occurrences;

    while (split < end && sum * 2 <= total) {
        split++;
        sum += tableVector[split].occurrences;
    }
    if (split == start) ++split;
    if (split == end) --split;

    for (size_t i = start; i < split; i++)
        tableVector[i].code.pushBack(false);
    for (size_t i = split; i < end; i++)
        tableVector[i].code.pushBack(true);

    buildFanoCodes(tableVector, start, split);
    buildFanoCodes(tableVector, split, end);
}

void Table::fromVector(Vector<ByteEntry> &tableVector) {
    for (const ByteEntry &byteEntry: tableVector)
        table[byteEntry.byte] = byteEntry;
}

size_t Table::findMaxCodeLength() {
    if (table.empty())
        throw TableException("Table is empty");

    size_t max = 0;

    for (const Pair<const uint8_t &, const ByteEntry &> pair: table)
        max = std::max(max, pair.second.code.size());

    return max;
}

void Table::buildReverseTable() {
    for (const Pair<uint8_t, ByteEntry> pair: table)
        reverseTable[pair.second.code] = pair.first;
}
