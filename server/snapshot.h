#pragma once

#include <cstdint>

struct Snapshot
{
    int8_t a, b, x, y;
    int8_t l1, r1, l2, r2;
    int8_t up, down, left, right;
    int8_t select, start;
    int16_t lx, ly;
    int16_t rx, ry;
};