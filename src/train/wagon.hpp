#pragma once

#include "geometry/layout.hpp"
#include "rail.hpp"
#include <string>

class Wagon {
  private:
    float _filling;
    float _capacity;
    std::string _type;

  public:
    Wagon(std::string type, float capacity);
    virtual void draw(Layout layout, std::vector<Rail> rails, int rail_id,
                      float _progression);
    virtual ~Wagon();
};

class Locomotive : public Wagon {
  public:
    Locomotive();
    void draw(Layout layout, std::vector<Rail> rails, int rail_id,
              float _progression) override;
    ~Locomotive();
};
