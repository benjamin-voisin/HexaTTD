#pragma once

#include <memory>
#include <string>
#include <vector>

class TestInstance {
	public:
		std::string name;
		virtual void run_test();
		TestInstance(std::string name);
};

class Test {
	private:
		std::vector<std::shared_ptr<TestInstance>> _tests;
	public:
		void add_test(std::shared_ptr<TestInstance>);
		void run_tests();
};
