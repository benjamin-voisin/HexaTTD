#include "vector.hpp"
#include "layout.hpp"

class ArcTrack {
    public:
        Vector center;
        float radius;
        float gauge;
        float angle_min;
        float angle_max;

        ArcTrack(Vector center, float radius, float gauge, float angle_min, float angle_max);
        void draw();

};

class StraighTrack {
    public:
        Vector src;
        Vector dst;
        float gauge;

        StraighTrack(Vector src, Vector dst, float gauge);
        void draw();


};