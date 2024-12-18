#include "rail.hpp"
#include "geometry/vector.hpp"
#include "graphics/display_track.hpp"
#include "raylib.h"

#include <assert.h>
#include <stdio.h>

#define GAUGE 25

int Rail::number_phases() { return 3; }

Rail::Rail(Hex tile, int src_side, int dst_side, int width)
    : _hex{tile}, _width{width}, deleted{false} {
    int src_side_mod = src_side % 6;
    int dst_side_mod = dst_side % 6;
    assert(src_side_mod != dst_side_mod);
    if (src_side_mod < dst_side_mod) {
        assert(src_side_mod + 1 < dst_side_mod);
        this->_src_neighbor = src_side_mod;
        this->_dst_neighbor = dst_side_mod;
    } else {
        assert(dst_side_mod + 1 < src_side_mod);
        this->_src_neighbor = dst_side_mod;
        this->_dst_neighbor = src_side_mod;
    }
};

Hex Rail::get_hex() { return _hex; }

void setpp(FILE *f, std::set<int> _on_track_train) {
    fprintf(f, "[");
    for (auto i = _on_track_train.begin(); i != _on_track_train.end(); ++i) {
        if (i != _on_track_train.begin())
            fprintf(f, ", ");
        fprintf(f, "%d", *i);
    }
    fprintf(f, "]");
}

void Rail::pp(FILE *f) {
    fprintf(f, "Rail{src=%d, dst=%d, trains=", _src_neighbor, _dst_neighbor);
    setpp(f, _on_track_train);
    fprintf(f, "}");
}

std::set<int> Rail::get_on_track_train() { return this->_on_track_train; }

void Rail::add_on_track_train(int train) {
    this->_on_track_train.insert(train);
}

void Rail::del_on_track_train(int train) { this->_on_track_train.erase(train); }

train_pos Rail::get_position(Layout layout, float progression) {
    if (_src_neighbor + 3 == _dst_neighbor) {
        // The rail is a straight line
        Vector src = _hex.center_side(layout, _src_neighbor);
        Vector dst = _hex.center_side(layout, _dst_neighbor);
        Vector position = (dst - src) * progression + src;
        Vector orientation = (dst - src).normalise();
        return {position, orientation};
    } else {
        if (_dst_neighbor == _src_neighbor + 2) {
            Vector center = _hex.neighbor(_src_neighbor + 1).center(layout);
            float angle_min =
                _hex.corner_angle(layout, _dst_neighbor + 3) * 180.f / M_PI;
            float angle_max =
                _hex.corner_angle(layout, _src_neighbor + 4) * 180.f / M_PI;
            float angle = (progression) * (angle_max - angle_min) + angle_min;
            Vector v1 = Vector{
                center.x + cosf(DEG2RAD * angle) * (layout.size.x * 1.5f),
                center.y + sinf(DEG2RAD * angle) * (layout.size.x * 1.5f)};
            return {v1, Vector(angle * DEG2RAD).orthogonal()};
        } else {
            Vector center = _hex.neighbor(_src_neighbor - 1).center(layout);
            float angle_min =
                _hex.corner_angle(layout, _dst_neighbor - 2) * 180.f / M_PI;
            float angle_max =
                _hex.corner_angle(layout, _src_neighbor + 3) * 180.f / M_PI;
            float angle = (progression) * (angle_max - angle_min) + angle_min;
            Vector v1 = Vector{
                center.x + cosf(DEG2RAD * angle) * (layout.size.x * 1.5f),
                center.y + sinf(DEG2RAD * angle) * (layout.size.x * 1.5f)};
            return {v1, Vector(angle * DEG2RAD).orthogonal()};
        }
    }
}

bool Rail::is_on_track(Layout layout, Vector v) {
    auto display_gauge = GAUGE * (layout.size.x / GAUGE_FACTOR);
    int delta = layout.size.x / 20;
    if (_src_neighbor + 3 == _dst_neighbor) {
        // The rail is a straight line
        Vector src = _hex.center_side(layout, _src_neighbor);
        Vector dst = _hex.center_side(layout, _dst_neighbor);
        Vector dir = (dst - src).orthogonal();
        Vector pr = dir.proj(v - src);
        return pr.norme() < display_gauge / 2 + delta;
    } else {
        Vector center = {0, 0};
        if (_dst_neighbor == _src_neighbor + 2) {
            center = _hex.neighbor(_src_neighbor + 1).center(layout);
        } else {
            center = _hex.neighbor(_src_neighbor - 1).center(layout);
        }
        Vector dist = v - center;
        float norm = dist.norme();
        return (norm > layout.size.x * 1.5f - display_gauge / 2 - delta) &&
               (norm < layout.size.x * 1.5f + display_gauge / 2 + delta);
    }
}

void Rail::draw(Layout layout, Color c, int phase) {
    // Vector center = Vector(_hex.center(layout));
    std::vector<Vector2> corners = _hex.corners(layout);

    if (this->_on_track_train.size() > 0)
        c = GREEN;

    // Hex tile_src = _hex.neighbor(_src_neighbor);
    // Hex tile_dst = _hex.neighbor(_dst_neighbor);

    /* DEBUG STRUCT RAILS */
    /*
    DrawLineEx(_hex.center_side(layout, _src_neighbor), _hex.center(layout),
    _width, GREEN); DrawLineEx(_hex.center(layout), _hex.center_side(layout,
    _dst_neighbor), _width, PURPLE);

    DrawCircleV(_hex.center_side(layout, _src_neighbor), 6, PURPLE);
    DrawCircleV(_hex.corner(layout, _src_neighbor), 6, BLUE);
    DrawCircleV(_hex.corner(layout, _src_neighbor+1), 6, BLUE);


    DrawCircleV(_hex.center_side(layout, _dst_neighbor), 6, GREEN);
    DrawCircleV(_hex.corner(layout, _dst_neighbor), 6, YELLOW);
    DrawCircleV(_hex.corner(layout, _dst_neighbor+1), 6, YELLOW);
    */

    if (_src_neighbor + 3 == _dst_neighbor) {
        // Si l'on doit faire une ligne droite
        StraighTrack track =
            StraighTrack(c, _hex.center_side(layout, _src_neighbor),
                         _hex.center_side(layout, _dst_neighbor), GAUGE);
        track.draw(layout, phase);
        // DrawLineEx(coffset_droite_src.to_Vector2(),
        // coffset_droite_dst.to_Vector2(), _width, BLACK);
        // DrawLineEx(coffset_gauche_src.to_Vector2(),
        // coffset_gauche_dst.to_Vector2(), _width, BLACK);
    } else {
        if (_dst_neighbor == _src_neighbor + 2) {
            Hex tile_curb = _hex.neighbor(_src_neighbor + 1);
            ArcTrack track =
                ArcTrack{c,
                         tile_curb.center(layout),
                         layout.size.x * 1.5f,
                         GAUGE,
                         _hex.corner_angle(layout, _dst_neighbor + 3) * 180 /
                             (float)M_PI,
                         _hex.corner_angle(layout, _src_neighbor + 4) * 180 /
                             (float)M_PI};
            track.draw(layout, phase);
        } else {
            Hex tile_curb = _hex.neighbor(_src_neighbor - 1);
            ArcTrack track =
                ArcTrack{c,
                         tile_curb.center(layout),
                         layout.size.x * 1.5f,
                         GAUGE,
                         _hex.corner_angle(layout, _dst_neighbor - 2) * 180 /
                             (float)M_PI,
                         _hex.corner_angle(layout, _src_neighbor + 3) * 180 /
                             (float)M_PI};
            track.draw(layout, phase);
        }
    }
    /*
    Vector c1 = Vector(corners[_entry_neighbor]);
    Vector c2 = Vector(corners[_entry_neighbor + 1]);
    Vector c3 = Vector(corners[_exit_neighbor]);
    Vector c4 = Vector(corners[_exit_neighbor + 1]);

    Vector coffset_center_src = Vector(GetSplinePointLinear(c1.to_Vector2(),
    c2.to_Vector2(), 0.50)); Vector coffset_droite_src =
    Vector(GetSplinePointLinear(c1.to_Vector2(), c2.to_Vector2(), 0.25)); Vector
    coffset_gauche_src = Vector(GetSplinePointLinear(c1.to_Vector2(),
    c2.to_Vector2(), 0.75));

    Vector coffset_center_dst = Vector(GetSplinePointLinear(c3.to_Vector2(),
    c4.to_Vector2(), 0.50)); Vector coffset_droite_dst =
    Vector(GetSplinePointLinear(c3.to_Vector2(), c4.to_Vector2(), 0.75)); Vector
    coffset_gauche_dst = Vector(GetSplinePointLinear(c3.to_Vector2(),
    c4.to_Vector2(), 0.25));


    if (_entry_neighbor % 3 == _exit_neighbor % 3) {
            // Si l'on doit faire une ligne droite
            DrawLineEx(coffset_droite_src.to_Vector2(),
    coffset_droite_dst.to_Vector2(), _width, BLACK);
            DrawLineEx(coffset_gauche_src.to_Vector2(),
    coffset_gauche_dst.to_Vector2(), _width, BLACK); } else {
            // Si il faut calculler un virage
            //assert(_entry_neighbor == _exit_neighbor + 2);
            //assert(_entry_neighbor + 2 == _exit_neighbor);

            Hex tile_circle = _hex.neighbor((_entry_neighbor+1) / 2);

            Hex tile_circle2 = _hex.neighbor(3);

            Hex neighbor_src = _hex.neighbor(_entry_neighbor);
            Hex neighbor_dst = _hex.neighbor(_exit_neighbor);

            //DrawLineEx(neighbor_src.center(layout), _hex.center(layout),
    _width, BLACK);
            //DrawLineEx(neighbor_dst.center(layout), _hex.center(layout),
    _width, BLACK);

            ArcTrack track = ArcTrack{tile_circle.center(layout), 150, 25,
    _hex.corner_angle(layout, _exit_neighbor-2) * 180 / M_PI,
    _hex.corner_angle(layout, _entry_neighbor+3) * 180 / M_PI}; track.draw();

            DrawCircleV(tile_circle.center(layout), 4, GREEN);
            DrawCircleV(tile_circle2.center(layout), 4, RED);
            DrawCircleV(_hex.center(layout), 4, PURPLE);


            Vector2 p1[3];

            Vector offset_droite_src = coffset_droite_src-coffset_center_src;
            Vector offset_droite_dst = coffset_droite_dst-coffset_center_dst;

            Vector offset_gauche_src = coffset_gauche_src-coffset_center_src;
            Vector offset_gauche_dst = coffset_gauche_dst-coffset_center_dst;

            DrawLineEx(coffset_droite_src.to_Vector2(),
    (center+offset_droite_src).to_Vector2(), 2, PURPLE);
            DrawLineEx(coffset_droite_dst.to_Vector2(),
    (center+offset_droite_dst).to_Vector2(), 2, PURPLE);

            DrawLineEx(center.to_Vector2(),
    (center+offset_droite_src).to_Vector2(), 2, GREEN);
            DrawLineEx(center.to_Vector2(),
    (center+offset_droite_dst).to_Vector2(), 2, BLUE);
            DrawLineEx(center.to_Vector2(),
    (center+offset_droite_src.proj(offset_droite_dst)).to_Vector2(), 2, GREEN);
            DrawLineEx(center.to_Vector2(),
    (center+offset_droite_dst.proj(offset_droite_src)).to_Vector2(), 2, BLUE);

            Vector pt_controle_droite = center +
    ((offset_droite_dst.proj(offset_droite_src) +
    offset_droite_src.proj(offset_droite_dst)));

            p1[1] = pt_controle_droite.to_Vector2();
            p1[0] = coffset_droite_src.to_Vector2();
            p1[2] = coffset_droite_dst.to_Vector2();

            Vector pt_controle_gauche = center +
    offset_gauche_dst.proj(offset_gauche_src) +
    offset_gauche_src.proj(offset_gauche_dst); Vector2 p2[3]; p2[1] =
    pt_controle_gauche.to_Vector2(); p2[0] = coffset_gauche_src.to_Vector2();
            p2[2] = coffset_gauche_dst.to_Vector2();


            DrawSplineBezierQuadratic(p1, 3, _width, BLACK);
            DrawSplineBezierQuadratic(p2, 3, _width, BLACK);
            DrawCircleV(p1[1], 4, RED);

    } */
}

void Rail::draw(Layout layout, Color c) {
    if (_hex.is_visible(layout)) {
        for (int phase = 0; phase < number_phases(); phase++) {
            draw(layout, c, phase);
        }
    }
}

bool Rail::is_train_on_track() { return !_on_track_train.empty(); }

int Rail::get_src_neighbor() { return _src_neighbor; }

int Rail::get_dst_neighbor() { return _dst_neighbor; }
