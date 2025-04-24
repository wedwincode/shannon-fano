# Fano Coding Implementation in C++

This project implements a custom data compression and decompression algorithm using **Fano coding** — a technique similar to Huffman coding but based on a top-down frequency split.

The project is written in **pure C++** with **no usage of STL containers** (as per assignment constraints). Instead, all required structures like `Vector`, `UnorderedMap`, and `String` are implemented manually.

---

## Features

- Custom implementation of:
    - `Vector<T>` — dynamic array
    - `String` — null-terminated character buffer
    - `UnorderedMap<K, V>` — hash table with open addressing
- Full Fano encoder/decoder:
    - Builds code table from byte frequencies
    - Encodes input using generated binary codes
    - Stores header + encoded stream to output
    - Decodes back to original data
- File I/O operations with exception safety
- Timing measurements using `ScopedTimer`
- Modular structure
- Unit tests