#include "../include/Vector.h"
#include <iostream>
#include <cassert>

namespace VectorTests {
    void testDefaultConstructor() {
        Vector<int> v;
        assert(v.size() == 0);
        assert(v.empty());
    }

    void testPushBackAndIndexing() {
        Vector<int> v;
        v.pushBack(10);
        v.pushBack(20);
        v.pushBack(30);

        assert(v.size() == 3);
        assert(v[0] == 10);
        assert(v[1] == 20);
        assert(v[2] == 30);
    }

    void testPopBack() {
        Vector<int> v;
        v.pushBack(1);
        v.pushBack(2);
        v.popBack();

        assert(v.size() == 1);
        assert(v[0] == 1);
    }

    void testCopyConstructor() {
        Vector<int> v1;
        v1.pushBack(42);
        Vector<int> v2(v1);

        assert(v2.size() == 1);
        assert(v2[0] == 42);
    }

    void testCopyAssignment() {
        Vector<int> v1;
        v1.pushBack(100);
        Vector<int> v2;
        v2 = v1;

        assert(v2.size() == 1);
        assert(v2[0] == 100);
    }

    void testMoveConstructor() {
        Vector<int> v1;
        v1.pushBack(77);
        Vector<int> v2(std::move(v1));

        assert(v2.size() == 1);
        assert(v2[0] == 77);
        assert(v1.size() == 0);
    }

    void testMoveAssignment() {
        Vector<int> v1;
        v1.pushBack(55);
        Vector<int> v2;
        v2 = std::move(v1);

        assert(v2.size() == 1);
        assert(v2[0] == 55);
        assert(v1.size() == 0);
    }

    void testOperatorPlusEquals() {
        Vector<int> v1;
        v1.pushBack(1);

        Vector<int> v2;
        v2.pushBack(2);
        v2.pushBack(3);

        v1 += v2;

        assert(v1.size() == 3);
        assert(v1[0] == 1);
        assert(v1[1] == 2);
        assert(v1[2] == 3);
    }

    void testOutOfRangeException() {
        Vector<int> v;
        v.pushBack(1);
        bool caught = false;
        try {
            int x = v[100];
            (void) x;
        } catch (const std::out_of_range &) {
            caught = true;
        }
        assert(caught);
    }

    void testClear() {
        Vector<int> v;
        v.pushBack(1);
        v.pushBack(2);
        v.clear();

        assert(v.size() == 0);
        assert(v.empty());
    }

    void runVectorTest() {
        std::cout << "[VectorTest] Running...\n";
        testDefaultConstructor();
        testPushBackAndIndexing();
        testPopBack();
        testCopyConstructor();
        testCopyAssignment();
        testMoveConstructor();
        testMoveAssignment();
        testOperatorPlusEquals();
        testOutOfRangeException();
        testClear();
        std::cout << "[VectorTest] All tests passed\n";
    }
}
