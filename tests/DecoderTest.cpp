#include "../include/Decoder.h"
#include "../include/Encoder.h"
#include "../include/file_io.h"
#include <cassert>
#include <iostream>
#include <cstdio> // std::remove

namespace DecoderTests {
    void testDecoderWorksCorrectly() {
        const String inputFile("decoder_test_input.tmp");
        const String outputFile("decoder_test_output.tmp");

        Packed input;
        input.pushBack('a');
        input.pushBack('b');
        input.pushBack('a');
        input.pushBack('c');

        file_io::writeToFile(inputFile, input);

        Encoder encoder(inputFile, inputFile + ".encoded.tmp");
        encoder.encode();

        Decoder decoder(inputFile + ".encoded.tmp", outputFile);
        decoder.decode();

        Buffer decoded = file_io::readFileToBuffer(outputFile);
        assert(decoded.size() == input.size());
        for (size_t i = 0; i < input.size(); ++i)
            assert(decoded[i] == input[i]);

        std::remove(inputFile.c_str());
        std::remove((inputFile + ".encoded.tmp").c_str());
        std::remove(outputFile.c_str());
    }

    void runDecoderTest() {
        std::cout << "[DecoderTest] Running...\n";
        testDecoderWorksCorrectly();
        std::cout << "[DecoderTest] All tests passed\n";
    }
}
