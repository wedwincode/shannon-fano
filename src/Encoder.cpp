#include "../include/Encoder.h"

#include <iomanip>

Encoder::Encoder() = default;

Encoder::Encoder(const Encoder &) = default;

Encoder::Encoder(Encoder &&) noexcept = default;

Encoder &Encoder::operator=(const Encoder &) = default;

Encoder &Encoder::operator=(Encoder &&) noexcept = default;

Encoder::~Encoder() = default;

void Encoder::encode(const String &inputFilePath, const String &outputFilePath) {
    try {
        ScopedTimer scopedTimer("Encoder");
        file_io::checkFiles(inputFilePath, outputFilePath);
        const Buffer buffer = file_io::readFileToBuffer(inputFilePath);
        Table table(buffer);
        Data data(buffer);

        const Encoded encodedTable = table.encode();
        const Encoded encodedData = data.encode(table);

        const Packed packed = packEncodedTableAndData(encodedTable, encodedData);

        file_io::writeToFile(outputFilePath, packed);

        getStatistics(inputFilePath, outputFilePath);
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

void Encoder::getStatistics(const String &inputFilePath, const String &outputFilePath) {
    const size_t inputSize = file_io::getFileSize(inputFilePath);
    const size_t outputSize = file_io::getFileSize(outputFilePath);
    std::cout << "[Encoder] Input size: " << inputSize << " bytes\n";
    std::cout << "[Encoder] Encoded size: " << outputSize << " bytes\n";

    const double compressionRatio = (1 - static_cast<double>(outputSize) / static_cast<double>(inputSize)) * 100;
    std::cout << "[Encoder] Compression ratio: " << std::fixed << std::setprecision(2) << compressionRatio << "%\n";
}
