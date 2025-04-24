#include "../include/Decoder.h"

Decoder::Decoder(const String &inputFilePath, const String &outputFilePath): inputFilePath_(inputFilePath),
                                                                             outputFilePath_(outputFilePath) {
    file_io::checkFiles(inputFilePath, outputFilePath);
}

Decoder::Decoder(const Decoder &) = default;

Decoder::Decoder(Decoder &&) noexcept = default;

Decoder &Decoder::operator=(const Decoder &) = default;

Decoder &Decoder::operator=(Decoder &&) noexcept = default;

Decoder::~Decoder() = default;

void Decoder::decode() const {
    /*
        ===== BINARY FILE DATA STORAGE SCHEME =====

        [0]      [1]            [2]
        ┌──────┬──────────────┬────────────┐
        │  1B  │     1B       │    1B      │
        │unused│ numOfEntries │ bitsPerCode│
        └──────┴──────────────┴────────────┘
             │        │             │
             ▼        ▼             ▼
         Number   Number of     Length of
         of       unique        code (with
         unused   symbols       leading 1
         bits     in table      included)

        Then comes the encoding table block:
        Repeated numOfEntries times:
            ┌────────────┬─────────────────────────────┐
            │   1 byte   │ bitsPerCode bits            │
            │  symbol    │ normalized code             │
            └────────────┴─────────────────────────────┘

            normalized code = [0 ... 0][1][actual_code]
                              ^ padding ^

        After the table: encoded data block
            ┌──────────────────────────────────────────────┐
            │ encoded data (bit vector)                    │
            └──────────────────────────────────────────────┘
            (trailing unused bits are ignored)

        All data after the first 3 bytes is treated as a bit stream.
        */

    try {
        ScopedTimer scopedTimer("Decoder");
        const Buffer rawBuffer = file_io::readFileToBuffer(inputFilePath_);

        if (rawBuffer.size() < 3)
            throw DecoderException("Incorrect header format");

        const uint8_t unusedBitsQuantity = rawBuffer[0];
        const uint8_t numberOfEntries = rawBuffer[1];
        const uint8_t bitsPerCode = rawBuffer[2];

        const Buffer cleanedBuffer(rawBuffer.begin() + 3, rawBuffer.end());
        const Encoded encodedBuffer = bit_utils::byteBufferToBits(cleanedBuffer);

        const size_t tableBitSize = getTableBitSize(numberOfEntries, bitsPerCode);
        const Encoded encodedTable(
            encodedBuffer.begin(),
            encodedBuffer.begin() + tableBitSize
        );
        const Encoded encodedData(
            encodedBuffer.begin() + tableBitSize,
            encodedBuffer.end() - unusedBitsQuantity
        );

        Table table;
        table.decode(encodedTable, bitsPerCode);

        Data data;
        data.decode(table, encodedData);

        file_io::writeToFile(outputFilePath_, data.getData());
    } catch (std::exception &ex) {
        std::cerr << ex.what() << "\n";
    }
}

size_t Decoder::getTableBitSize(const uint8_t numberOfEntries, const uint8_t bitsPerCode) {
    return numberOfEntries * (8 + bitsPerCode);
}
