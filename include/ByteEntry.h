#ifndef BYTEENTRY_H
#define BYTEENTRY_H
#include <cstdint>

#include "Vector.h"

struct ByteEntry
{
  ByteEntry();

  ByteEntry(uint8_t byte, int occurrences);

  static void swap(ByteEntry& a, ByteEntry& b) noexcept;

  friend std::ostream& operator<<(std::ostream& os, const ByteEntry& obj);

  uint8_t byte;
  int occurrences;
  Vector<bool> code;
};


#endif //BYTEENTRY_H
