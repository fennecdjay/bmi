// src: bmi.c
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

#include "bmi.h"
#include <string.h> // memset
#include <stdlib.h> // malloc
#include <stdio.h> // fprintf, fwrite
#include <errno.h> // errno, strerror

static inline size_t bmi_buffer_component_size(const bmi_buffer* buffer) {
    return BMI_COMPONENT_SIZE_FROM_FL(buffer->flags);
}

#define BMI_GET_INDEX(buffer, x, y) \
    (((buffer)->width * (y) + (x)) * bmi_buffer_component_size(buffer))

const char* bmi_version_string(const uint8_t version) {
    static char result[6] = { 0, '.', 0, '.', 0, 0 };
    result[0] = '0' + ((version >> 6) & 0x3);
    result[2] = '0' + ((version >> 3) & 0x7);
    result[4] = '0' + ((version >> 0) & 0x7);
    return result;
}

inline size_t bmi_buffer_content_size(const bmi_buffer* buffer) {
    return buffer->width * buffer->height * bmi_buffer_component_size(buffer);
}

#define BMI_GRAY_WRITE(buffer, i, p) \
    (buffer)->contents[i] = (uint8_t)(p)
#define BMI_RGB_WRITE(buffer, i, p) \
    (buffer)->contents[i] = (uint8_t)(p); \
    (buffer)->contents[(i) + 1] = (uint8_t)((p) >> 8); \
    (buffer)->contents[(i) + 2] = (uint8_t)((p) >> 16)

void bmi_buffer_draw_point(bmi_buffer* buffer, bmi_point p,
                           bmi_component pixel) {
    const size_t index = BMI_GET_INDEX(buffer, p.x, p.y);
    if (buffer->flags & BMI_FL_IS_GRAYSCALE) {
        BMI_GRAY_WRITE(buffer, index, pixel);
    } else {
        BMI_RGB_WRITE(buffer, index, pixel);
    }
}

#define _MAX(x, y) ((x) > (y) ? (x) : (y))
#define _MIN(x, y) ((x) < (y) ? (x) : (y))

void bmi_clip_rect(bmi_rect* rect, const bmi_rect bounds) {
    const uint32_t max_x = _MIN(rect->x + rect->w, bounds.x + bounds.w);
    const uint32_t max_y = _MIN(rect->y + rect->h, bounds.y + bounds.h);;
    rect->x = _MAX(rect->x, bounds.x);
    rect->y = _MAX(rect->y, bounds.y);
    rect->w = max_x - rect->x;
    rect->h = max_y - rect->y;
}

void bmi_buffer_fill_rect(bmi_buffer* buffer, bmi_rect r, bmi_component pixel) {
    bmi_clip_rect(&r, BMI_RECT(0, 0, buffer->width, buffer->height));
    if (buffer->flags & BMI_FL_IS_GRAYSCALE) {
        for (uint32_t i = 0; i < r.h; i++) {
            void* row = buffer->contents + BMI_GET_INDEX(buffer, r.x, r.y + i);
            memset(row, (uint8_t)pixel, r.w);
        }
    } else {
        for (uint32_t i = 0; i < r.h; i++) {
            const size_t index = BMI_GET_INDEX(buffer, r.x, r.y + i);
            for (uint32_t j = 0; j < r.w; j++) {
                BMI_RGB_WRITE(buffer, index + j * 3, pixel);
            }
        }
    }
}
