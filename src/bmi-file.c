// src: bmi-file.c
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

// bmi_buffer_component_size
#include "bmi-file.h"

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
