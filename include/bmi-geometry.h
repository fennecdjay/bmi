// include: bmi-geometry.h
// Copyright (C) 2021 Ethan Uppal
//
// bmi is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// bmi is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with bmi. If not, see <https://www.gnu.org/licenses/>.

#ifndef _BMI_INTERNAL_GEOMETRY_H
#define _BMI_INTERNAL_GEOMETRY_H

#include "bmi-file.h"
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint32_t x;
    uint32_t y;
} bmi_point;

typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
} bmi_rect;

// Constructors for point and rectangle structs
#define BMI_POINT(_x, _y) \
    ((bmi_point){ .x = (_x), .y = (_y) })
#define BMI_RECT(_x, _y, _w, _h) \
    ((bmi_rect){ .x = (_x), .y = (_y), .w = (_w), .h = (_h) })

// Clips the specified rectangle to the given bounds
void bmi_clip_rect(bmi_rect* rect, const bmi_rect bounds);

typedef enum {
    BMI_RECT_EDGE_LEFT,
    BMI_RECT_EDGE_RIGHT,
    BMI_RECT_EDGE_TOP,
    BMI_RECT_EDGE_BOTTOM,
} bmi_rect_edge;

// Prints a debug description of the specified point to the given file
void bmi_dump_point(FILE* dest, const bmi_point point);

// Prints a debug description of the specified rect to the given file
void bmi_dump_rect(FILE* dest, const bmi_rect rect);

// Insets the specified rectangle by the given amount from the specified edge
void bmi_inset_rect(bmi_rect* rect, uint32_t delta, bmi_rect_edge edge);

// Sets the specified edge of the specified rectangle to the given amount
void bmi_set_rect(bmi_rect* rect, uint32_t width, bmi_rect_edge edge);

#endif /* _BMI_INTERNAL_GEOMETRY_H */
