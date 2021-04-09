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

// BMI_GET_INDEX
#include "bmi-file.h"

// bmi_set_error
#include "bmi-error.h"

// bmi_clip_rect, bmi_inset_rect, bmi_set_rect
#include "bmi-geometry.h"

// bmi_buffer_component_size
#include "bmi-draw.h"

// bmi_buffer_get_pixel
#include "bmi-util.h"

// memset
#include <string.h>

// abs
#include <stdlib.h>

#define BMI_GRAY_WRITE(buffer, i, p) \
    (buffer)->contents[i] = (uint8_t)BMI_GRY_V(p)
#define BMI_RGB_WRITE(buffer, i, p) \
    (buffer)->contents[i] = (uint8_t)BMI_RGB_R(p); \
    (buffer)->contents[(i) + 1] = (uint8_t)BMI_RGB_G(p); \
    (buffer)->contents[(i) + 2] = (uint8_t)BMI_RGB_B(p)

INLINE void bmi_buffer_draw_point(bmi_buffer* buffer, bmi_point point,
                           bmi_component pixel) {
    const size_t index = BMI_GET_INDEX(buffer, point.x, point.y);
    if (buffer->flags & BMI_FL_IS_GRAYSCALE) {
        BMI_GRAY_WRITE(buffer, index, pixel);
    } else {
        BMI_RGB_WRITE(buffer, index, pixel);
    }
}

void bmi_buffer_fill_rect(bmi_buffer* buffer, bmi_rect bounds,
                          bmi_component pixel) {
    // Clip the rectangle to prevent out-of-bounds drawing
    bmi_clip_rect(&bounds, BMI_RECT(0, 0, buffer->width, buffer->height));
    
    // Initialize the row to the specified pixels
    if (buffer->flags & BMI_FL_IS_GRAYSCALE) {
        // Grayscale offers an optimized path using memset because each pixel is
        // just one byte
        for (uint32_t i = 0; i < bounds.height; i++) {
            void* row = buffer->contents + BMI_GET_INDEX(buffer, bounds.x,
                                                         bounds.y + i);
            memset(row, (uint8_t)pixel, bounds.width);
        }
    } else {
        // RGB requires us to loop through the row, and this is probably not
        // vectorized because writing an RGB pixel writes 24 bits
        for (uint32_t i = 0; i < bounds.height; i++) {
            const size_t index = BMI_GET_INDEX(buffer, bounds.x, bounds.y + i);
            for (uint32_t j = 0; j < bounds.width; j++) {
                BMI_RGB_WRITE(buffer, index + j * 3, pixel);
            }
        }
    }
}

void bmi_buffer_stroke_rect(bmi_buffer* buffer, bmi_rect bounds,
                            uint32_t thickness, bmi_component pixel) {
    // Clip the rectangle to prevent out-of-bounds drawing
    bmi_clip_rect(&bounds, BMI_RECT(0, 0, buffer->width, buffer->height));
    
    // Initialize all te edges to the original rect
    bmi_rect left, right, top, bottom;
    left = right = top = bottom = bounds;
    
    bmi_inset_rect(&left, thickness, BMI_RECT_EDGE_TOP);
    bmi_set_rect(&left, thickness, BMI_RECT_EDGE_LEFT);
    bmi_inset_rect(&left, thickness, BMI_RECT_EDGE_BOTTOM);
    
    bmi_inset_rect(&right, thickness, BMI_RECT_EDGE_TOP);
    bmi_set_rect(&right, thickness, BMI_RECT_EDGE_RIGHT);
    bmi_inset_rect(&right, thickness, BMI_RECT_EDGE_BOTTOM);

    bmi_set_rect(&top, thickness, BMI_RECT_EDGE_TOP);
    bmi_set_rect(&bottom, thickness, BMI_RECT_EDGE_BOTTOM);
    
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
void bmi_buffer_stroke_line(bmi_buffer* buffer, bmi_point start, bmi_point end,
                            uint32_t thickness, bmi_component pixel) {
    // Clip the points to prevent out-of-bounds drawing
    bmi_clip_point(&start, BMI_RECT(0, 0, buffer->width, buffer->height));
    bmi_clip_point(&end, BMI_RECT(0, 0, buffer->width, buffer->height));
        
    if (abs((int32_t)end.y - (int32_t)start.y)
        > abs((int32_t)end.x - (int32_t)start.x)) {
        // Vertical path
        
        if (end.y < start.y) {
            _SWAP(&start.x, &end.x, uint32_t);
            _SWAP(&start.y, &end.y, uint32_t);
        }
        
        int32_t dx = (int32_t)end.x - (int32_t)start.x;
        const uint32_t dy = end.y - start.y;
        int32_t xi = 1;
        if (dx < 0) {
            xi = -1;
            dx = -dx;
        }
        int32_t rolling_error = (int32_t)(2 * dx) - (int32_t)dy;
        uint32_t x = start.x;

        for (uint32_t y = start.y; y < end.y; y++) {
            bmi_buffer_draw_point(buffer, BMI_POINT(x, y), pixel);
            if (rolling_error > 0) {
                x += xi;
                rolling_error -= 2 * dy;
            }
            rolling_error += 2 * dx;
        }
    } else {
        // Horizontal path
        
        if (end.x < start.x) {
            _SWAP(&start.x, &end.x, uint32_t);
            _SWAP(&start.y, &end.y, uint32_t);
        }
        
        const uint32_t dx = end.x - start.x;
        int32_t dy = (int32_t)end.y - (int32_t)start.y;
        int32_t yi = 1;
        if (dy < 0) {
            yi = -1;
            dy = -dy;
        }
        int32_t rolling_error = (int32_t)(2 * dy) - (int32_t)dx;
        uint32_t y = start.y;

        for (uint32_t x = start.x; x < end.x; x++) {
            bmi_buffer_draw_point(buffer, BMI_POINT(x, y), pixel);
            if (rolling_error > 0) {
                y += yi;
                rolling_error -= 2 * dx;
            }
            rolling_error += 2 * dy;
        }
    }
}

int bmi_buffer_overdraw_buffer(bmi_buffer* buffer, bmi_rect region,
                               const bmi_buffer* layer) {
    // Clip the rectangle to prevent out-of-bounds drawing
    bmi_clip_rect(&region, BMI_RECT(0, 0, buffer->width, buffer->height));
    
    // Handle errors to ensure integrity
    if (region.width > layer->width) {
        bmi_set_error("bmi_buffer_overdraw_buffer: Attempted to draw a region "
                      "wider than the given buffer");
        return BMI_FAILURE;
    } else if (region.height > layer->height) {
        bmi_set_error("bmi_buffer_overdraw_buffer: Attempted to draw a region "
                      "taller than the given buffer");
        return BMI_FAILURE;
    }
    
    
    // Draw the buffer from the top left into the region
    for (uint32_t y = 0; y < region.height; y++) {
        for (uint32_t x = 0; x < region.width; x++) {
            const bmi_point src = BMI_POINT(x, y);
            const bmi_point dst = BMI_POINT(x + region.x, y + region.y);
            bmi_buffer_draw_point(buffer, dst,
                                  bmi_buffer_get_pixel(layer, src));
        }
    }
    
    return BMI_SUCCESS;
}
