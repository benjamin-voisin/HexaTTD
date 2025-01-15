#pragma once

#include "rail.hpp"
#include <string>

class Station {
  private:
    int _rail_id;
    std::string _name;

  public:
    Station(int rail_id, std::string name);
    void draw(Layout *layout, std::vector<Rail> rails);
};
