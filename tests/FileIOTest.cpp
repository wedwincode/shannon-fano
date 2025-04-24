#include "../include/file_io.h"
#include "../include/String.h"
#include <cassert>
#include <iostream>
#include <cstdio>  // for std::remove

namespace FileIOTests {

    void testCheckFilesValid() {
        String in("input.tmp");
        String out("output.tmp");
        file_io::checkFiles(in, out);
    }

    void testCheckFilesSameThrows() {
        bool caught = false;
        try {
            String path("same.tmp");
            file_io::checkFiles(path, path);
        } catch (const FileException &) {
            caught = true;
        }
        assert(caught);
    }

    void testCheckFilesEmptyThrows() {
        bool caught = false;
        try {
            String in("");
            String out("some.tmp");
            file_io::checkFiles(in, out);
        } catch (const FileException &) {
            caught = true;
        }
        assert(caught);
    }

    void testWriteAndReadFile() {
        String filename("test_io.tmp");

        // prepare data
        Packed packed;
        packed.pushBack(0x41);  // 'A'
        packed.pushBack(0x42);  // 'B'
        packed.pushBack(0x43);  // 'C'

        file_io::writeToFile(filename, packed);

        Buffer readBack = file_io::readFileToBuffer(filename);
        assert(readBack.size() == packed.size());
        for (size_t i = 0; i < readBack.size(); ++i)
            assert(readBack[i] == packed[i]);

        std::remove(filename.c_str());
    }

    void testReadNonexistentThrows() {
        String missing("this_file_does_not_exist.tmp");
        bool caught = false;
        try {
            file_io::readFileToBuffer(missing);
        } catch (const FileException &) {
            caught = true;
        }
        assert(caught);
    }

    void testGetFileSize() {
        String filename("size_check.tmp");

        Packed packed;
        for (int i = 0; i < 100; ++i)
            packed.pushBack(static_cast<uint8_t>(i));

        file_io::writeToFile(filename, packed);
        size_t size = file_io::getFileSize(filename);
        assert(size == 100);

        std::remove(filename.c_str());
    }

    void runFileIOTest() {
        std::cout << "[FileIOTest] Running...\n";
        testCheckFilesValid();
        testCheckFilesSameThrows();
        testCheckFilesEmptyThrows();
        testWriteAndReadFile();
        testReadNonexistentThrows();
        testGetFileSize();
        std::cout << "[FileIOTest] All tests passed\n";
    }

}
