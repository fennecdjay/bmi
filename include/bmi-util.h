// include: bmi-util.h
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

#ifndef _BMI_INTERNAL_UTIL_H
#define _BMI_INTERNAL_UTIL_H

#include "bmi-file.h"
#include "bmi-geometry.h"
#include "bmi-color.h"
#include <stdio.h>

// Returns the pixel at the given point in the BMI buffer
bmi_pixel bmi_buffer_get_pixel(const bmi_buffer* buffer, bmi_point point);

// Allocates a new BMI buffer to be freed initialized with the given attributes
bmi_buffer* bmi_buffer_new(uint32_t width, uint32_t height, uint32_t flags);

// Reads in and allocates a new BMI buffer from the given file
bmi_buffer* bmi_buffer_from_file(FILE* source);

// Saves the BMI buffer to a file
int bmi_buffer_to_file(FILE* dest, const bmi_buffer* buffer);

// Saves the BMI buffer to a file as a PPM
int bmi_buffer_to_ppm(FILE* dest, const bmi_buffer* buffer);

// Saves the BMI buffer to a file as a BMP
int bmi_buffer_to_bmp(FILE* dest, const bmi_buffer* buffer);

#endif /* _BMI_INTERNAL_UTIL_H */
