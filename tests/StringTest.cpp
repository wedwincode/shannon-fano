#include "../include/String.h"
#include <cassert>
#include <iostream>

namespace StringTests {
    void testStringDefaultConstructor() {
        String s;
        assert(s.size() == 0);
        assert(s.empty());
        assert(s.c_str()[0] == '\0');
    }

    void testStringFromCString() {
        String s("hello");
        assert(s.size() == 5);
        assert(!s.empty());
        assert(s[0] == 'h');
        assert(std::strcmp(s.c_str(), "hello") == 0);
    }

    void testStringCopyConstructor() {
        String s1("copy");
        String s2(s1);
        assert(s1 == s2);
        assert(s1.c_str() != s2.c_str());
    }

    void testStringMoveConstructor() {
        String s1("move");
        String s2(std::move(s1));
        assert(std::strcmp(s2.c_str(), "move") == 0);
        assert(s1.c_str()[0] == '\0' || s1.c_str() == nullptr);
    }

    void testStringCopyAssignment() {
        String s1("copy assign");
        String s2;
        s2 = s1;
        assert(s1 == s2);
        assert(s1.c_str() != s2.c_str());
    }

    void testStringMoveAssignment() {
        String s1("move assign");
        String s2;
        s2 = std::move(s1);
        assert(std::strcmp(s2.c_str(), "move assign") == 0);
        assert(s1.c_str()[0] == '\0' || s1.c_str() == nullptr);
    }

    void testStringComparison() {
        String a("abc");
        String b("abc");
        String c("xyz");
        assert(a == b);
        assert(a != c);
    }

    void testStringIndexAccess() {
        String s("test");
        s[0] = 'b';
        assert(s[0] == 'b');
        assert(std::strcmp(s.c_str(), "best") == 0);
    }

    void runStringTest() {
        std::cout << "[StringTest] Running...\n";
        testStringDefaultConstructor();
        testStringFromCString();
        testStringCopyConstructor();
        testStringMoveConstructor();
        testStringCopyAssignment();
        testStringMoveAssignment();
        testStringComparison();
        testStringIndexAccess();
        std::cout << "[StringTest] All tests passed\n";
    }
}
