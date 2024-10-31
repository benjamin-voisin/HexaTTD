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