cmake_minimum_required(VERSION 3.28)
project(fano)

set(CMAKE_CXX_STANDARD 14)

# Main target
add_executable(fano main.cpp
        include/Vector.h
        src/ByteEntry.cpp
        include/ByteEntry.h
        src/bit_utils.cpp
        include/bit_utils.h
        src/Table.cpp
        include/Table.h
        src/Data.cpp
        include/Data.h
        src/Encoder.cpp
        include/Encoder.h
        src/Decoder.cpp
        include/Decoder.h
        src/file_io.cpp
        include/file_io.h
        include/UnorderedMap.h
        include/Pair.h
        include/String.h
        include/types.h
        src/ScopedTimer.cpp
        include/ScopedTimer.h
        include/FanoExceptions.h
)

# Tests target
add_executable(tests
        tests/VectorTest.cpp
        tests/allTests.cpp
        src/ByteEntry.cpp
        src/bit_utils.cpp
        src/Table.cpp
        src/Data.cpp
        src/Encoder.cpp
        src/Decoder.cpp
        src/file_io.cpp
        src/ScopedTimer.cpp
        tests/UnorderedMapTest.cpp
        tests/StringTest.cpp
        tests/BitUtilsTest.cpp
        tests/FileIOTest.cpp
        tests/ScopedTimerTest.cpp
        tests/TableTest.cpp
        tests/DataTest.cpp
        tests/EncoderTest.cpp
        tests/DecoderTest.cpp
)

