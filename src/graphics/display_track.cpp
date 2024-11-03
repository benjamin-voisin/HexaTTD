#include "display_track.hpp"

#ifndef DISCRETISATION
#define DISCRETISATION 100
#endif

#define GAUGE_FACTOR 100

#include <stdio.h>

ArcTrack::ArcTrack(Color color, Vector center, float radius, float gauge, float angle_min, float angle_max) : 
    color{color}, center{center}, radius{radius}, gauge{gauge},
    angle_min{angle_min}, angle_max{angle_max} {}

void ArcTrack::draw(Layout layout) {
	auto display_gauge = gauge * (layout.size.x / GAUGE_FACTOR);
    int delta = layout.size.x / 20;

    if (layout.size.x > 100) {
        for (float i=0; i<30; ++i) {
            float angle = (i / 30) * (angle_max - angle_min) + angle_min;
            DrawRing(center.to_Vector2(), radius-display_gauge/2-delta, radius+display_gauge/2+delta, angle, angle+1, 1, color);
        }
    } else if (layout.size.x > 40) {
        for (float i=0; i<=30; ++i) {
            float angle = (i / 30) * (angle_max - angle_min) + angle_min;
            Vector v1 = Vector{center.x + cosf(DEG2RAD*angle)*(radius+display_gauge/2+delta), center.y + sinf(DEG2RAD*angle)*(radius+display_gauge/2+delta)};
            Vector v2 = Vector{center.x + cosf(DEG2RAD*angle)*(radius-display_gauge/2-delta), center.y + sinf(DEG2RAD*angle)*(radius-display_gauge/2-delta)};
            DrawLineEx(v1.to_Vector2(), v2.to_Vector2(), 2, color);
        }
    } else {
        DrawRing(center.to_Vector2(), radius-display_gauge/2-delta, radius+display_gauge/2+delta, angle_min, angle_max, 30, color);
    }
    DrawCircleSectorLines(center.to_Vector2(), radius+display_gauge/2, angle_min, angle_max, DISCRETISATION, BLACK);
    DrawCircleSectorLines(center.to_Vector2(), radius-display_gauge/2, angle_min, angle_max, DISCRETISATION, BLACK);
}

StraighTrack::StraighTrack(Color color, Vector src, Vector dst, float gauge)
    : color{color}, src{src}, dst{dst}, gauge{gauge} {};

void StraighTrack::draw(Layout layout) {
    auto display_gauge = gauge * (layout.size.x / GAUGE_FACTOR);
    int delta = layout.size.x / 20;
    Vector ortho = (dst-src).orthogonal().normalise();
    int delta_traverses = layout.size.x / 40;
    if (layout.size.x > 100) {
        for (float i=0; i<=30; ++i) {
            Vector v = (dst - src) * (i / 30) + src; 
            DrawLineEx((v + ortho * (display_gauge / 2 + delta)).to_Vector2(), (v - ortho * (display_gauge / 2 + delta)).to_Vector2(), delta_traverses, color);
        }
    } else if (layout.size.x > 40) {
        for (float i=0; i<=30; ++i) {
            Vector v = (dst - src) * (i / 30) + src; 
            DrawLineEx((v + ortho * (display_gauge / 2 + delta)).to_Vector2(), (v - ortho * (display_gauge / 2 + delta)).to_Vector2(), 2, color);
        }
    } else {
        DrawLineEx(src.to_Vector2(), dst.to_Vector2(), display_gauge+2*delta, color);
    }
    DrawLineV((src+ortho*display_gauge/2).to_Vector2(), (dst+ortho*display_gauge/2).to_Vector2(), BLACK);
    DrawLineV((src-ortho*display_gauge/2).to_Vector2(), (dst-ortho*display_gauge/2).to_Vector2(), BLACK);
   
}
