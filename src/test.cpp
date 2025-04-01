#include <iostream>
#include <memory>

#include "test.hpp"

void Test::run_tests() {
    for (size_t i = 0; i < _tests.size(); i++) {
        _tests[i]->run_test();
    }
}

void Test::add_test(std::shared_ptr<TestInstance> test) {
    _tests.push_back(test);
}

void TestInstance::run_test() { std::cout << "Testing " << _name << "\n"; }

void TestInstance::print_result() {
    std::cout << _passed_tests << " out of " << _total_tests << " test passed"
              << "\n";
}

TestInstance::TestInstance(std::string name) : _name{name} {
    _total_tests = 0;
    _passed_tests = 0;
}

void TestInstance::test(bool condition, std::string name) {
    _total_tests++;
    if (condition) {
        _passed_tests++;
        std::cout << name << Test::green << " \tPASSED\n" << Test::def;
    } else {
        std::cout << name << Test::red << " \tFAILED\n" << Test::def;
    }
}

#ifdef TEST

#include "geometry/hex.hpp"

int main() {
    Test test;
    test.add_test(std::make_shared<HexTest>(HexTest()));
    test.run_tests();
}
#endif // TEST
