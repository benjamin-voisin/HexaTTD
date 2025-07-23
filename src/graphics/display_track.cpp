#include "display_track.hpp"
#include <assert.h>

#ifndef DISCRETISATION
#define DISCRETISATION 100
#endif

#include "../utils.hpp"

DSPTrack::DSPTrack(Color color, float gauge) : color{color}, gauge{gauge} {};

float DSPTrack::get_display_gauge(Layout *layout) {
    return gauge * (layout->size.x / GAUGE_FACTOR);
}
int DSPTrack::get_delta(Layout *layout) { return layout->size.x / 20; }

ArcTrack::ArcTrack(Color color, Vector center, float radius, float gauge,
                   float angle_min, float angle_max)
    : DSPTrack(color, gauge), center{center}, radius{radius},
      angle_min{angle_min}, angle_max{angle_max} {}

void ArcTrack::draw_base(Layout *layout) {
    float display_gauge = get_display_gauge(layout) * 1.5f;
    DrawRing(center.to_Vector2(), (radius - display_gauge / 2),
             (radius + display_gauge / 2), angle_min, angle_max, DISCRETISATION,
             GRAY);
}

void ArcTrack::draw_traverse(Layout *layout) {
    float display_gauge = get_display_gauge(layout);
    int delta = get_delta(layout);

    if (layout->size.x > 50) {
        for (float i = 0; i < N_TRAVERSES; ++i) {
            float angle =
                (i / N_TRAVERSES) * (angle_max - angle_min) + angle_min;
            DrawRing(center.to_Vector2(), radius - display_gauge / 2 - delta,
                     radius + display_gauge / 2 + delta, angle, angle + 1, 1,
                     color);
        }
    } else if (layout->size.x > 20) {
        for (float i = 0; i <= N_TRAVERSES; ++i) {
            float angle =
                (i / N_TRAVERSES) * (angle_max - angle_min) + angle_min;
            Vector v1 =
                Vector{center.x + cosf(DEG2RAD * angle) *
                                      (radius + display_gauge / 2 + delta),
                       center.y + sinf(DEG2RAD * angle) *
                                      (radius + display_gauge / 2 + delta)};
            Vector v2 =
                Vector{center.x + cosf(DEG2RAD * angle) *
                                      (radius - display_gauge / 2 - delta),
                       center.y + sinf(DEG2RAD * angle) *
                                      (radius - display_gauge / 2 - delta)};
            DrawLineEx(v1.to_Vector2(), v2.to_Vector2(), 2, color);
        }
    } else {
        DrawRing(center.to_Vector2(), radius - display_gauge / 2 - delta,
                 radius + display_gauge / 2 + delta, angle_min, angle_max, 30,
                 color);
    }
}

void ArcTrack::draw_rails(Layout *layout) {
    float display_gauge = get_display_gauge(layout);
    float delta = get_delta(layout);
    float rail_delta = MAX(delta / 5.f, 1.f);
    DrawRing(center.to_Vector2(), (radius - display_gauge / 2) - rail_delta,
             (radius - display_gauge / 2) + rail_delta, angle_min, angle_max,
             DISCRETISATION, BLACK);
    DrawRing(center.to_Vector2(), (radius + display_gauge / 2) - rail_delta,
             (radius + display_gauge / 2) + rail_delta, angle_min, angle_max,
             DISCRETISATION, BLACK);
}

void ArcTrack::draw(Layout *layout, int phase) {
    switch (phase) {
    case 0:
        draw_base(layout);
        break;
    case 1:
        draw_traverse(layout);
        break;
    case 2:
        draw_rails(layout);
        break;
    default:
        assert(false);
    }
}

void StraighTrack::draw(Layout *layout, int phase) {
    switch (phase) {
    case 0:
        draw_base(layout);
        break;
    case 1:
        draw_traverse(layout);
        break;
    case 2:
        draw_rails(layout);
        break;
    default:
        assert(false);
    }
}

StraighTrack::StraighTrack(Color color, Vector src, Vector dst, float gauge)
    : DSPTrack(color, gauge), src{src}, dst{dst} {};

void StraighTrack::draw_base(Layout *layout) {
    float display_gauge = get_display_gauge(layout) * 1.5f;
    Vector ortho = (dst - src).orthogonal().normalise();

    Vector debut1 = src + (ortho * (display_gauge / 2));
    Vector fin1 = dst + (ortho * (display_gauge / 2));
    float rail_angle = (debut1 - fin1).angle();
    DrawRectanglePro(
        {debut1.x, debut1.y, display_gauge, layout->size.x * sqrt(3.f)}, {0, 0},
        rail_angle, GRAY);
}

void StraighTrack::draw_traverse(Layout *layout) {
    float display_gauge = get_display_gauge(layout);
    int delta = get_delta(layout);

    Vector ortho = (dst - src).orthogonal().normalise();
    int delta_traverses = layout->size.x / 40;
    if (layout->size.x > 50) {
        for (float i = 0; i <= N_TRAVERSES; ++i) {
            Vector v = (dst - src) * (i / N_TRAVERSES) + src;
            DrawLineEx((v + ortho * (display_gauge / 2 + delta)).to_Vector2(),
                       (v - ortho * (display_gauge / 2 + delta)).to_Vector2(),
                       delta_traverses, color);
        }
    } else if (layout->size.x > 20) {
        for (float i = 0; i <= N_TRAVERSES; ++i) {
            Vector v = (dst - src) * (i / N_TRAVERSES) + src;
            DrawLineEx((v + ortho * (display_gauge / 2 + delta)).to_Vector2(),
                       (v - ortho * (display_gauge / 2 + delta)).to_Vector2(),
                       2, color);
        }
    } else {
        DrawLineEx(src.to_Vector2(), dst.to_Vector2(),
                   display_gauge + 2 * delta, color);
    }
}

void StraighTrack::draw_rails(Layout *layout) {
    float display_gauge = get_display_gauge(layout);
    int delta = get_delta(layout);
    float rail_delta = MAX(static_cast<float>(delta) / 2.5f, 1.f);
    Vector ortho = (dst - src).orthogonal().normalise();

    Vector debut1 = src + (ortho * (display_gauge / 2 + rail_delta / 2.f));
    Vector fin1 = dst + (ortho * (display_gauge / 2 + rail_delta / 2.f));
    float rail_angle = (debut1 - fin1).angle();
    DrawRectanglePro(
        {debut1.x, debut1.y, rail_delta, layout->size.x * sqrt(3.f)}, {0, 0},
        rail_angle, BLACK);
    Vector debut2 = src - (ortho * (display_gauge / 2 - rail_delta / 2.f));
    DrawRectanglePro(
        {debut2.x, debut2.y, rail_delta, layout->size.x * sqrt(3.f)}, {0, 0},
        rail_angle, BLACK);
}
