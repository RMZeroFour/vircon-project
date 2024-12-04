#pragma once

#include <cstdint>

struct Snapshot
{
    bool a, b, x, y;
    bool l1, r1, l2, r2;
    bool up, down, left, right;
    bool select, start;
    int16_t lx, ly;
    int16_t rx, ry;
};