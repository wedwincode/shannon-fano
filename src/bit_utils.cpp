#include "../include/bit_utils.h"

#include <iostream>

void bit_utils::printBits(const Vector<bool>& bits, const size_t lineWidth)
{
  for (size_t i = 0; i < bits.size(); i++)
  {
    std::cout << (bits[i] ? '1' : '0');
    if ((i + 1) % lineWidth == 0)
      std::cout << '\n';
  }
  std::cout << '\n';
}

Vector<bool> bit_utils::byteToBits(const uint8_t byte)
{
  Vector<bool> bits;
  for (int i = 0; i < BITS_IN_BYTE; i++)
  {
    bool bit = byte >> i & 1;
    bits.pushBack(bit);
  }
  return bits;
}

uint8_t bit_utils::bitsToByte(const Vector<bool>& bits)
{
  if (bits.size() > BITS_IN_BYTE)
    throw FanoException("bitsToByte: size exceeds 8 bits");

  uint8_t result = 0;
  for (size_t i = 0; i < bits.size(); i++)
    if (bits[i])
      result |= static_cast<uint8_t>(1 << i);

  return result;
}

Vector<uint8_t> bit_utils::packBits(const Vector<bool>& bits)
{
  Vector<uint8_t> bytes((bits.size() + 7) / BITS_IN_BYTE, 0);

  size_t bitIndex = 0;
  for (auto const& bit : bits)
  {
    if (bit)
    {
      const size_t byteIndex = bitIndex >> 3;
      const uint8_t mask = static_cast<uint8_t>(1) << (bitIndex & 7);
      bytes[byteIndex] |= mask;
    }
    bitIndex++;
  }

  return bytes;
}

Vector<bool> bit_utils::processCodeBuffer(const Vector<bool>& buffer)
{
  const int NOT_FOUND = -1;
  int startIndex = NOT_FOUND;
  for (int i = 0; i < static_cast<int>(buffer.size()); i++)
    if (buffer[i])
    {
      startIndex = i + 1;
      break;
    }

  if (startIndex == NOT_FOUND || startIndex >= static_cast<int>(buffer.size()))
    throw FanoException("Incorrect code format");

  Vector<bool> processedBuffer;
  for (size_t i = startIndex; i < buffer.size(); i++)
    processedBuffer.pushBack(buffer[i]);

  return processedBuffer;
}

Vector<bool> bit_utils::addLeadingZerosToCode(const Vector<bool>& code,
                                              const size_t length)
{
  Vector<bool> result(length, false);
  for (int i = 1; i <= code.size(); i++)
    result[length - i] = code[code.size() - i];

  return result;
}

Vector<bool> bit_utils::normalizeCode(const Vector<bool>& code,
                                      const size_t length)
{
  Vector<bool> withZeros = addLeadingZerosToCode(code, length + 1);
  withZeros[length - code.size()] = true;
  return withZeros;
}

Vector<bool> bit_utils::byteBufferToBits(const Vector<uint8_t>& buffer)
{
  Vector<bool> bits;

  for (const uint8_t byte : buffer)
    for (int i = 0; i < BITS_IN_BYTE; i++)
      bits.pushBack(byte >> i & 1);

  return bits;
}
