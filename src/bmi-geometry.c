// src: bmi-geometry.c
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

#define _BMI_USE_INTERNAL

#include "bmi-geometry.h"

// fprintf
#include <stdio.h>

void bmi_dump_point(FILE* dest, const bmi_point point) {
    fprintf(dest, "(x: %u, y: %u)\n", point.x, point.y);
}

void bmi_dump_rect(FILE* dest, const bmi_rect rect) {
    fprintf(dest, "(x: %u, y: %u): %u by %u\n", rect.x, rect.y, rect.width,
            rect.height);
}

#define _MAX(x, y) ((x) > (y) ? (x) : (y))
#define _MIN(x, y) ((x) < (y) ? (x) : (y))

void bmi_clip_point(bmi_point* point, const bmi_rect bounds) {
    point->x = _MAX(point->x, bounds.x);
    point->y = _MAX(point->y, bounds.y);
    point->x = _MIN(point->x, bounds.x + bounds.width);
    point->y = _MIN(point->y, bounds.y + bounds.height);
}

void bmi_clip_rect(bmi_rect* rect, const bmi_rect bounds) {
    const uint32_t max_x = _MIN(rect->x + rect->width, bounds.x + bounds.width);
    const uint32_t max_y = _MIN(rect->y + rect->height,
                                bounds.y + bounds.height);
    rect->x = _MAX(rect->x, bounds.x);
    rect->y = _MAX(rect->y, bounds.y);
    rect->width = max_x - rect->x;
    rect->height = max_y - rect->y;
}

void bmi_inset_rect(bmi_rect* rect, uint32_t delta, const bmi_rect_edge edge) {
    switch (edge) {
        case BMI_RECT_EDGE_LEFT:
            rect->x += _MIN(delta, rect->width);
            break;
        case BMI_RECT_EDGE_RIGHT:
            rect->width -= _MIN(delta, rect->width);
            break;
        case BMI_RECT_EDGE_TOP:
            rect->y += _MIN(delta, rect->height);
            break;
        case BMI_RECT_EDGE_BOTTOM:
            rect->height -= _MIN(delta, rect->height);
            break;
    }
}

void bmi_set_rect(bmi_rect* rect, uint32_t width, bmi_rect_edge edge) {
    switch (edge) {
        case BMI_RECT_EDGE_LEFT:
            rect->width = _MIN(width, rect->width);
            break;
        case BMI_RECT_EDGE_RIGHT: {
            const uint32_t delta = _MIN(width, rect->width);
            rect->x += rect->width - delta;
            rect->width = delta;
            break;
        }
        case BMI_RECT_EDGE_TOP:
            rect->height = _MIN(width, rect->height);
            break;
        case BMI_RECT_EDGE_BOTTOM:{
            const uint32_t delta = _MIN(width, rect->height);
            rect->y += rect->height - delta;
            rect->height = delta;
            break;
        }
    }
}
