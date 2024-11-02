#pragma once 

#include "../vector.hpp"
#include "../layout.hpp"

class ArcTrack {
    public:
        Color color;
        Vector center;
        float radius;
        float gauge;
        float angle_min;
        float angle_max;

        ArcTrack(Color color, Vector center, float radius, float gauge, float angle_min, float angle_max);
        void draw(Layout layout);

};

class StraighTrack {
    public:
        Color color;
        Vector src;
        Vector dst;
        float gauge;

        StraighTrack(Color color, Vector src, Vector dst, float gauge);
        void draw(Layout layout);


};
