#include "../include/bit_utils.h"
#include <cassert>
#include <iostream>

namespace BitUtilsTests {

    void testByteToBitsAndBack() {
        uint8_t original = 0b10101010;
        Vector<bool> bits = bit_utils::byteToBits(original);
        assert(bits.size() == 8);
        uint8_t result = bit_utils::bitsToByte(bits);
        assert(result == original);
    }

    void testBitsToByteThrows() {
        Vector<bool> tooLong;
        for (int i = 0; i < 9; i++) tooLong.pushBack(true);
        bool caught = false;
        try {
            bit_utils::bitsToByte(tooLong);
        } catch (const FanoException &) {
            caught = true;
        }
        assert(caught);
    }

    void testPackBits() {
        Vector<bool> bits;
        bits.pushBack(true);
        bits.pushBack(false);
        bits.pushBack(true);
        bits.pushBack(false);
        bits.pushBack(true);
        bits.pushBack(false);
        bits.pushBack(true);
        bits.pushBack(false);
        Vector<uint8_t> packed = bit_utils::packBits(bits);
        assert(packed.size() == 1);
        assert(packed[0] == 0b01010101);
    }

    void testProcessCodeBuffer() {
        Vector<bool> input;
        input.pushBack(false);
        input.pushBack(false);
        input.pushBack(false);
        input.pushBack(true);
        input.pushBack(true);
        input.pushBack(false);
        input.pushBack(true);
        Vector<bool> processed = bit_utils::processCodeBuffer(input);
        assert(processed.size() == 3);
        assert(processed[0] == true);
        assert(processed[1] == false);
        assert(processed[2] == true);
    }

    void testAddLeadingZerosToCode() {
        Vector<bool> code;
        code.pushBack(true);
        code.pushBack(false);
        code.pushBack(true);
        Vector<bool> padded = bit_utils::addLeadingZerosToCode(code, 6);
        assert(padded.size() == 6);
        assert(padded[3] == true);
        assert(padded[4] == false);
        assert(padded[5] == true);
    }

    void testNormalizeCode() {
        Vector<bool> code;
        code.pushBack(true);
        code.pushBack(false);
        code.pushBack(true);
        Vector<bool> normalized = bit_utils::normalizeCode(code, 6);
        assert(normalized.size() == 7);
        assert(normalized[3] == true); // inserted 1
        assert(normalized[4] == true);
        assert(normalized[5] == false);
        assert(normalized[6] == true);
    }

    void testByteBufferToBits() {
        Vector<uint8_t> buffer;
        buffer.pushBack(0b00001111);
        Vector<bool> bits = bit_utils::byteBufferToBits(buffer);
        assert(bits.size() == 8);
        for (int i = 0; i < 4; i++) assert(bits[i] == true);
        for (int i = 4; i < 8; i++) assert(bits[i] == false);
    }

    void runBitUtilsTest() {
        std::cout << "[BitUtilsTest] Running...\n";
        testByteToBitsAndBack();
        testBitsToByteThrows();
        testPackBits();
        testProcessCodeBuffer();
        testAddLeadingZerosToCode();
        testNormalizeCode();
        testByteBufferToBits();
        std::cout << "[BitUtilsTest] All tests passed\n";
    }

}
