#pragma once

#define GAUGE_FACTOR 100
#define N_TRAVERSES 20

#include "../geometry/layout.hpp"
#include "../geometry/vector.hpp"
class DSPTrack {
  protected:
    Color color;
    float gauge;

  public:
    DSPTrack(Color color, float gauge);
    float get_display_gauge(Layout *layout);
    int get_delta(Layout *layout);
};

class ArcTrack : public DSPTrack {
  private:
    void draw_base(Layout *layout);
    void draw_traverse(Layout *layout);
    void draw_rails(Layout *layout);

  public:
    Vector center;
    float radius;
    float angle_min;
    float angle_max;

    ArcTrack(Color color, Vector center, float radius, float gauge,
             float angle_min, float angle_max);
    void draw(Layout *layout, int phase);
};

class StraighTrack : public DSPTrack {
  private:
    void draw_base(Layout *layout);
    void draw_traverse(Layout *layout);
    void draw_rails(Layout *layout);

  public:
    Vector src;
    Vector dst;

    StraighTrack(Color color, Vector src, Vector dst, float gauge);
    void draw(Layout *layout, int phase);
};
