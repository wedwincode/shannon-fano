#include "../include/Encoder.h"
#include "../include/file_io.h"
#include <cassert>
#include <iostream>
#include <cstdio>

namespace EncoderTests {

    void testEncoderWorksAndProducesOutput() {
        const String inputFile("test_encoder_input.tmp");
        const String outputFile("test_encoder_output.tmp");

        Packed input;
        input.pushBack('H');
        input.pushBack('e');
        input.pushBack('l');
        input.pushBack('l');
        input.pushBack('o');
        file_io::writeToFile(inputFile, input);

        Encoder::encode(inputFile, outputFile);

        size_t outputSize = file_io::getFileSize(outputFile);
        assert(outputSize > 0);

        std::remove(inputFile.c_str());
        std::remove(outputFile.c_str());
    }

    void runEncoderTest() {
        std::cout << "[EncoderTest] Running...\n";
        testEncoderWorksAndProducesOutput();
        std::cout << "[EncoderTest] All tests passed\n";
    }

}
