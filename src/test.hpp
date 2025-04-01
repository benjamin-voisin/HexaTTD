#pragma once

#include <memory>
#include <string>
#include <vector>

class TestCase {
  private:
};

class TestInstance {
  protected:
    int _total_tests;
    int _passed_tests;
    std::string _name;
    void test(bool condition, std::string name);
    void print_result();

  public:
    virtual void run_test();
    TestInstance(std::string name);
};

class Test {
  private:
    std::vector<std::shared_ptr<TestInstance>> _tests;

  public:
    void add_test(std::shared_ptr<TestInstance>);
    void run_tests();
    static constexpr std::string green = "\033[032m";
    static constexpr std::string orange = "\033[033m";
    static constexpr std::string red = "\033[031m";
    static constexpr std::string def = "\033[0m";
};
