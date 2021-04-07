// src: bmi-draw.c
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

// bmi_clip_rect, bmi_inset_rect, bmi_set_rect
#include "bmi-geometry.h"

// bmi_buffer_component_size
#include "bmi-draw.h"

// memset
#include <string.h>

#define BMI_GET_INDEX(buffer, x, y) \
    (((buffer)->width * (y) + (x)) * bmi_buffer_component_size(buffer))

#define BMI_GRAY_WRITE(buffer, i, p) \
    (buffer)->contents[i] = (uint8_t)BMI_GRY_V(p)
#define BMI_RGB_WRITE(buffer, i, p) \
    (buffer)->contents[i] = (uint8_t)BMI_RGB_R(p); \
    (buffer)->contents[(i) + 1] = (uint8_t)BMI_RGB_G(p); \
    (buffer)->contents[(i) + 2] = (uint8_t)BMI_RGB_B(p)

void bmi_buffer_draw_point(bmi_buffer* buffer, bmi_point p,
                           bmi_component pixel) {
    const size_t index = BMI_GET_INDEX(buffer, p.x, p.y);
    if (buffer->flags & BMI_FL_IS_GRAYSCALE) {
        BMI_GRAY_WRITE(buffer, index, pixel);
    } else {
        BMI_RGB_WRITE(buffer, index, pixel);
    }
}

void bmi_buffer_fill_rect(bmi_buffer* buffer, bmi_rect r, bmi_component pixel) {
    // Clip the rectangle to prevent out-of-bounds drawing
    bmi_clip_rect(&r, BMI_RECT(0, 0, buffer->width, buffer->height));
    
    // Initialize the row to the specified pixels
    if (buffer->flags & BMI_FL_IS_GRAYSCALE) {
        // Grayscale offers an optimized path using memset because each pixel is
        // just one byte
        for (uint32_t i = 0; i < r.h; i++) {
            void* row = buffer->contents + BMI_GET_INDEX(buffer, r.x, r.y + i);
            memset(row, (uint8_t)pixel, r.w);
        }
    } else {
        // RGB requires us to loop through the row, and this is probably not
        // vectorized because writing an RGB pixel writes 24 bits
        for (uint32_t i = 0; i < r.h; i++) {
            const size_t index = BMI_GET_INDEX(buffer, r.x, r.y + i);
            for (uint32_t j = 0; j < r.w; j++) {
                BMI_RGB_WRITE(buffer, index + j * 3, pixel);
            }
        }
    }
}

void bmi_buffer_stroke_rect(bmi_buffer* buffer, bmi_rect r, uint32_t t,
                            bmi_component pixel) {
    // Clip the rectangle to prevent out-of-bounds drawing
    bmi_clip_rect(&r, BMI_RECT(0, 0, buffer->width, buffer->height));
    
    // Initialize all te edges to the original rect
    bmi_rect left, right, top, bottom;
    left = right = top = bottom = r;
    
    bmi_inset_rect(&left, t, BMI_RECT_EDGE_TOP);
    bmi_set_rect(&left, t, BMI_RECT_EDGE_LEFT);
    bmi_inset_rect(&left, t, BMI_RECT_EDGE_BOTTOM);
    
    bmi_inset_rect(&right, t, BMI_RECT_EDGE_TOP);
    bmi_set_rect(&right, t, BMI_RECT_EDGE_RIGHT);
    bmi_inset_rect(&right, t, BMI_RECT_EDGE_BOTTOM);

    bmi_set_rect(&top, t, BMI_RECT_EDGE_TOP);
    bmi_set_rect(&bottom, t, BMI_RECT_EDGE_BOTTOM);
    
    bmi_buffer_fill_rect(buffer, left, pixel);
    bmi_buffer_fill_rect(buffer, right, pixel);
    bmi_buffer_fill_rect(buffer, top, pixel);
    bmi_buffer_fill_rect(buffer, bottom, pixel);
}

#define _SWAP(x, y, T) do { \
    const T temp = *(x); \
    *(x) = *(y); \
    *(y) = temp; \
} while (0)

// Modified from: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void bmi_buffer_stroke_line(bmi_buffer* buffer, bmi_point s, bmi_point e,
                            uint32_t t, bmi_component pixel) {
    // Clip the points to prevent out-of-bounds drawing
    bmi_clip_point(&s, BMI_RECT(0, 0, buffer->width, buffer->height));
    bmi_clip_point(&e, BMI_RECT(0, 0, buffer->width, buffer->height));
    
    // Bresenham works from lower to higher
    if (e.x < s.x) {
        _SWAP(&s.x, &e.x, uint32_t);
    }
    if (e.y < s.y) {
        _SWAP(&s.y, &e.y, uint32_t);
    }
    if ((e.y - s.y) > (e.x - s.x)) {
        // Vertical path
        const uint32_t dx = e.x - s.x;
        const uint32_t dy = e.y - s.y;
        int32_t rolling_error = (int32_t)(2 * dx) - (int32_t)dy;
        uint32_t x = s.x;

        for (uint32_t y = s.y; y < e.y; y++) {
            bmi_buffer_draw_point(buffer, BMI_POINT(x, y), pixel);
            if (rolling_error > 0) {
                x++;
                rolling_error -= 2 * dy;
            }
            rolling_error += 2 * dx;
        }
    } else {
        // Horizontal path
        const uint32_t dx = e.x - s.x;
        const uint32_t dy = e.y - s.y;
        int32_t rolling_error = (int32_t)(2 * dy) - (int32_t)dx;
        uint32_t y = s.y;

        for (uint32_t x = s.x; x < e.x; x++) {
            bmi_buffer_draw_point(buffer, BMI_POINT(x, y), pixel);
            if (rolling_error > 0) {
                y++;
                rolling_error -= 2 * dx;
            }
            rolling_error += 2 * dy;
        }
    }
}
