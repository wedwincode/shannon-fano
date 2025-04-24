#include "../include/Encoder.h"

Encoder::Encoder(const String &inputFilePath, const String &outputFilePath): inputFilePath_(inputFilePath),
                                                                             outputFilePath_(outputFilePath) {
    file_io::checkFiles(inputFilePath, outputFilePath);
}

Encoder::Encoder(const Encoder &) = default;

Encoder::Encoder(Encoder &&) noexcept = default;

Encoder &Encoder::operator=(const Encoder &) = default;

Encoder &Encoder::operator=(Encoder &&) noexcept = default;

Encoder::~Encoder() = default;

void Encoder::encode() const {
    try {
        ScopedTimer scopedTimer("Encoder");
        const Buffer buffer = file_io::readFileToBuffer(inputFilePath_);
        Data data(buffer);
        Table table(buffer);

        const Encoded encodedTable = table.encode();
        const Encoded encodedData = data.encode(table);

        const Packed packed = packEncodedTableAndData(encodedTable, encodedData);

        file_io::writeToFile(outputFilePath_, packed);

        getStatistics();
    } catch (std::exception &ex) {
        std::cerr << ex.what() << "\n";
    }
}

Packed Encoder::packEncodedTableAndData(const Encoded &encodedTable, const Encoded &encodedData) {
    Encoded encodedAll;

    const uint8_t unusedBitsQuantity = (8 - (encodedTable.size() + encodedData.size()) % 8) % 8;
    const Encoded encodedQuantity = bit_utils::byteToBits(unusedBitsQuantity);
    encodedAll += encodedQuantity;
    encodedAll += encodedTable;
    encodedAll += encodedData;

    Packed packed = bit_utils::packBits(encodedAll);
    return packed;
}

void Encoder::getStatistics() const {
    const size_t inputSize = file_io::getFileSize(inputFilePath_);
    const size_t outputSize = file_io::getFileSize(outputFilePath_);
    std::cout << "[Encoder] Input size: " << inputSize << " bytes\n";
    std::cout << "[Encoder] Encoded size: " << outputSize << " bytes\n";

    const double compressionRatio = static_cast<double>(outputSize) / inputSize;
    std::cout << "[Encoder] Compression ratio: " << compressionRatio << "\n";
}
