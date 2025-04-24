#include "../include/ByteEntry.h"

ByteEntry::ByteEntry(): byte(0), occurrences(0) {
}

ByteEntry::ByteEntry(const uint8_t byte, const int occurrences): byte(byte), occurrences(occurrences) {
}

void ByteEntry::swap(ByteEntry &a, ByteEntry &b) noexcept {
    const ByteEntry temp = a;
    a = b;
    b = temp;
}

std::ostream &operator<<(std::ostream &os, const ByteEntry &obj) {
    os << "{symbol: 0x" << std::hex << std::uppercase << static_cast<int>(obj.byte)
            << " freq: " << std::dec << obj.occurrences
            << " code: " << std::noboolalpha << obj.code << std::boolalpha << "}";
    return os;
}
