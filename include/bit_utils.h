#ifndef BITUTILS_H
#define BITUTILS_H
#include <cstdint>

#include "Vector.h"
#include "FanoExceptions.h"


namespace bit_utils
{
  constexpr size_t DEFAULT_LINE_WIDTH = 100;

  static constexpr int BITS_IN_BYTE = 8;

  void printBits(const Vector<bool>& bits,
                 size_t lineWidth = DEFAULT_LINE_WIDTH);

  Vector<bool> byteToBits(uint8_t byte);

  uint8_t bitsToByte(const Vector<bool>& bits);

  Vector<uint8_t> packBits(const Vector<bool>& bits);

  Vector<bool> processCodeBuffer(const Vector<bool>& buffer);

  Vector<bool> addLeadingZerosToCode(const Vector<bool>& code, size_t length);

  Vector<bool> normalizeCode(const Vector<bool>& code, size_t length);

  Vector<bool> byteBufferToBits(const Vector<uint8_t>& buffer);
}


#endif //BITUTILS_H
