#pragma once 

#define GAUGE_FACTOR 100
#define N_TRAVERSES 20

#include "../vector.hpp"
#include "../layout.hpp"
class DSPTrack {
    protected:
        Color color;
        float gauge;
    public:
        DSPTrack(Color color, float gauge);
        float get_display_gauge(Layout layout);
        int get_delta(Layout layout);
};

class ArcTrack : public DSPTrack {
    public:
        Vector center;
        float radius;
        float angle_min;
        float angle_max;

        ArcTrack(Color color, Vector center, float radius, float gauge, float angle_min, float angle_max);
        void draw(Layout layout);

};

class StraighTrack : public DSPTrack {
    public:
        Vector src;
        Vector dst;

        StraighTrack(Color color, Vector src, Vector dst, float gauge);
        void draw(Layout layout);


};
