#include "display_track.hpp"

#ifndef DISCRETISATION
#define DISCRETISATION 100
#endif

#include <stdio.h>

ArcTrack::ArcTrack(Vector center, float radius, float gauge, float angle_min, float angle_max) : 
    center{center}, radius{radius}, gauge{gauge},
    angle_min{angle_min}, angle_max{angle_max} {}

void ArcTrack::draw() {
   
    int delta = 5;
    for (float i=0; i<=30; ++i) {
        float angle = (i / 30) * (angle_max - angle_min) + angle_min;
        Vector v1 = Vector{center.x + cosf(DEG2RAD*angle)*(radius+gauge/2+delta), center.y + sinf(DEG2RAD*angle)*(radius+gauge/2+delta)};
        Vector v2 = Vector{center.x + cosf(DEG2RAD*angle)*(radius-gauge/2-delta), center.y + sinf(DEG2RAD*angle)*(radius-gauge/2-delta)};
        DrawLineEx(v1.to_Vector2(), v2.to_Vector2(), 2, BROWN);
    }

    DrawCircleSectorLines(center.to_Vector2(), radius+gauge/2, angle_min, angle_max, DISCRETISATION, BLACK);
	DrawCircleSectorLines(center.to_Vector2(), radius-gauge/2, angle_min, angle_max, DISCRETISATION, BLACK);

}