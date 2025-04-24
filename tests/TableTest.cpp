#include "../include/Table.h"
#include <cassert>
#include <iostream>

namespace TableTests {

    void testTableBuildAndEncodeDecode() {
        Buffer buffer;
        buffer.pushBack('A');
        buffer.pushBack('B');
        buffer.pushBack('A');
        buffer.pushBack('C');
        buffer.pushBack('A');

        Table table(buffer);
        Encoded encoded = table.encode();

        const Encoded cleaned(encoded.begin() + 16, encoded.end());
        Table restored;
        uint8_t bitsPerCode = 4;
        restored.decode(cleaned, bitsPerCode);
        uint8_t byte = 0;
        for (const Pair<const uint8_t &, const ByteEntry &> pair : table.getRawTable()) {
            const Encoded &code = pair.second.code;
            bool success = restored.getByteByCode(code, byte);
            assert(success);
            assert(byte == pair.first);
        }
    }

    void testTableThrowsOnEmptyBuffer() {
        bool caught = false;
        try {
            Buffer buffer;
            Table table(buffer);
        } catch (const FileException &) {
            caught = true;
        }
        assert(caught);
    }

    void runTableTest() {
        std::cout << "[TableTest] Running...\n";
        testTableBuildAndEncodeDecode();
        testTableThrowsOnEmptyBuffer();
        std::cout << "[TableTest] All tests passed\n";
    }

}
