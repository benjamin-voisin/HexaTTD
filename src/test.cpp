#include <iostream>
#include <memory>

#include "test.hpp"

void Test::run_tests() {
	for (size_t i = 0; i < _tests.size(); i++) {
		std::cout << "Testing " << _tests[i]->name << "\n";
		_tests[i]->run_test();
	}
}

void Test::add_test(std::shared_ptr<TestInstance> test) {
	_tests.push_back(test);
}

void TestInstance::run_test() {
	std::cout << "feur" << "\n";
}

TestInstance::TestInstance(std::string name) : name{name} { }

#include "hex.hpp"

#ifdef TEST
int main() {
	Test test;
	test.add_test(std::make_shared<HexTest>(HexTest()));
	test.run_tests();
}
#endif // TEST
