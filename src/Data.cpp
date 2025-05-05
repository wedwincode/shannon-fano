#include "../include/Data.h"

Data::Data(const Buffer& buffer) : data_(buffer)
{
  if (buffer.empty())
    throw FileException("File is empty");
}

Data::Data() = default;

Data::Data(const Data&) = default;

Data::Data(Data&&) noexcept = default;

Data& Data::operator=(const Data&) = default;

Data& Data::operator=(Data&&) noexcept = default;

Data::~Data() = default;

Encoded Data::encode(const Table& table)
{
  Encoded encodedData;
  for (const uint8_t byte : data_)
    encodedData += table.getCodeForByte(byte);

  return encodedData;
}

void Data::decode(const Table& table, const Encoded& encodedData)
{
  data_.clear();
  Encoded currentCode;
  uint8_t byte;
  for (bool bit : encodedData)
  {
    currentCode.pushBack(bit);

    if (table.getByteByCode(currentCode, byte))
    {
      data_.pushBack(byte);
      currentCode.clear();
    }
  }
  if (!currentCode.empty())
    throw DataException("Leftover bits in buffer");
}

const Vector<uint8_t>& Data::getData() const
{
  return data_;
}
