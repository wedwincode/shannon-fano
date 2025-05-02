#ifndef ENCODER_H
#define ENCODER_H
#include <cstdint>
#include <fstream>

#include "bit_utils.h"
#include "Data.h"
#include "file_io.h"
#include "String.h"
#include "Table.h"
#include "Vector.h"
#include "ScopedTimer.h"

class Encoder {
public:
    Encoder();

    Encoder(const Encoder &);

    Encoder(Encoder &&) noexcept;

    Encoder &operator=(const Encoder &);

    Encoder &operator=(Encoder &&) noexcept;

    ~Encoder();

    static void encode(const String &inputFilePath, const String &outputFilePath);

private:
    static Packed packEncodedTableAndData(const Encoded &encodedTable, const Encoded &encodedData);

    static void getStatistics(const String &inputFilePath, const String &outputFilePath, const Table &table);
};


#endif //ENCODER_H
