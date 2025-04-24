#include "../include/Data.h"
#include "../include/Table.h"
#include <cassert>
#include <iostream>

namespace DataTests {

    void testDataConstructorThrowsOnEmpty() {
        bool caught = false;
        try {
            Buffer empty;
            Data d(empty);
        } catch (const FileException &) {
            caught = true;
        }
        assert(caught);
    }

    void testDecodeWithCorruptedDataThrows() {
        Buffer buffer;
        buffer.pushBack('x');
        buffer.pushBack('y');

        Table table(buffer);
        Data data(buffer);
        Encoded encoded = data.encode(table);

        encoded.pushBack(true);

        Data corrupted;
        bool caught = false;
        try {
            corrupted.decode(table, encoded);
        } catch (const DataException &) {
            caught = true;
        }
        assert(caught);
    }

    void runDataTest() {
        std::cout << "[DataTest] Running...\n";
        testDataConstructorThrowsOnEmpty();
        testDecodeWithCorruptedDataThrows();
        std::cout << "[DataTest] All tests passed\n";
    }

}
