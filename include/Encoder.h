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
    Encoder(const String &inputFilePath, const String &outputFilePath);

    Encoder(const Encoder &);

    Encoder(Encoder &&) noexcept;

    Encoder &operator=(const Encoder &);

    Encoder &operator=(Encoder &&) noexcept;

    ~Encoder();

    void encode() const;

private:
    static Packed packEncodedTableAndData(const Encoded &encodedTable, const Encoded &encodedData);

    void getStatistics() const;

    String inputFilePath_;
    String outputFilePath_;
};


#endif //ENCODER_H
