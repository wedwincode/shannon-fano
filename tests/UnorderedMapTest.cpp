#include "../include/UnorderedMap.h"
#include <cassert>
#include <iostream>

namespace UnorderedMapTests {
    void testInsertAndAccess() {
        UnorderedMap<int, std::string> map;
        map.insert(1, "one");
        map.insert(2, "two");
        assert(map.size() == 2);
        assert(map[1] == "one");
        assert(map[2] == "two");
    }

    void testOverwriteValue() {
        UnorderedMap<int, std::string> map;
        map.insert(1, "one");
        map.insert(1, "uno");
        assert(map[1] == "uno");
        assert(map.size() == 1);
    }

    void testOperatorAccessCreates() {
        UnorderedMap<int, int> map;
        map[5] = 100;
        assert(map.contains(5));
        assert(map[5] == 100);
        assert(map.size() == 1);
    }

    void testErase() {
        UnorderedMap<int, std::string> map;
        map.insert(10, "ten");
        assert(map.erase(10));
        assert(!map.contains(10));
        assert(map.size() == 0);
    }

    void testEraseNonexistent() {
        UnorderedMap<int, std::string> map;
        assert(!map.erase(42));
    }

    void testContains() {
        UnorderedMap<char, int> map;
        map.insert('x', 99);
        assert(map.contains('x'));
        assert(!map.contains('y'));
    }

    void testClear() {
        UnorderedMap<int, int> map;
        map.insert(1, 10);
        map.insert(2, 20);
        map.clear();
        assert(map.empty());
        assert(map.size() == 0);
    }

    void testRehashing() {
        UnorderedMap<int, int> map;
        for (int i = 0; i < 100; ++i)
            map.insert(i, i * i);
        for (int i = 0; i < 100; ++i)
            assert(map[i] == i * i);
    }

    void testIteratorAndRange() {
        UnorderedMap<int, int> map;
        map.insert(1, 10);
        map.insert(2, 20);
        int count = 0;
        for (auto it = map.begin(); it != map.end(); ++it) {
            auto pair = *it;
            assert(pair.second == pair.first * 10);
            count++;
        }
        assert(count == 2);
    }

    void testConstIterator() {
        UnorderedMap<int, int> map;
        map.insert(5, 25);
        const auto &constMap = map;
        for (auto it = constMap.begin(); it != constMap.end(); ++it) {
            auto pair = *it;
            assert(pair.first == 5);
            assert(pair.second == 25);
        }
    }

    void runUnorderedMapTest() {
        std::cout << "[UnorderedMapTest] Running...\n";
        testInsertAndAccess();
        testOverwriteValue();
        testOperatorAccessCreates();
        testErase();
        testEraseNonexistent();
        testContains();
        testClear();
        testRehashing();
        testIteratorAndRange();
        testConstIterator();
        std::cout << "[UnorderedMapTest] All tests passed\n";
    }
}
