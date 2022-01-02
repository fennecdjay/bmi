// src: bmi-util.c
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

// bmi_buffer_content_size, BMI_COMPONENT_SIZE_FROM_FL, BMI_VERSION_IS_CURRENT,
// BMI_FILE_IS_VALID, BMI_VERSION_IS_OUTDATED, BMI_VERSION_IS_LATER
#include "bmi-file.h"

// bmi_set_error
#include "bmi-error.h"

#include "bmi-geometry.h"

#include "bmi-color.h"

// fseek, ftell, rewind, fread, fwrite, fprintf
#include <stdio.h>

// malloc
#include <stdlib.h>

// srrno, strerror
#include <errno.h>

bmi_pixel bmi_buffer_get_pixel(const bmi_buffer* buffer, bmi_point point) {
    if (point.x >= buffer->width && point.y >= buffer->height) {
        bmi_set_error("Attempt to access point out of buffer region");
        return BMI_PIXEL_INVALID;
    }
    if (buffer->flags & BMI_FL_IS_GRAYSCALE) {
        return BMI_GRY(buffer->contents[BMI_GET_INDEX(buffer, point.x,
                                                      point.y)]);
    } else {
        const size_t index = BMI_GET_INDEX(buffer, point.x, point.y);
        const bmi_pixel red = buffer->contents[index];
        const bmi_pixel green = buffer->contents[index + 1];
        const bmi_pixel blue = buffer->contents[index + 2];
        return BMI_RGB(red, green, blue);
    }
}

bmi_buffer* bmi_buffer_new(uint32_t width, uint32_t height, uint32_t flags) {
    bmi_buffer* buffer = malloc(sizeof(bmi_buffer) + width * height
                                * BMI_COMPONENT_SIZE_FROM_FL(flags));
    if (buffer == NULL) {
        bmi_set_error("bmi_buffer_new: Virtual memory exhausted");
        return BMI_PTR_FAILURE;
    }
    buffer->header[0] = BMI_HEADER_0;
    buffer->header[1] = BMI_HEADER_1;
    buffer->header[2] = BMI_HEADER_2;
    buffer->version[0] = BMI_VERSION_CURRENT;
    buffer->width = width;
    buffer->height = height;
    buffer->flags = flags;
    return buffer;
}

bmi_buffer* bmi_buffer_from_file(FILE* source) {
    fseek(source, 0, SEEK_END);
    const size_t length = ftell(source);
    rewind(source);
    
    if (length < sizeof(bmi_buffer)) {
        bmi_set_error("bmi_buffer_from_file: File is too small to be valid");
        return BMI_PTR_FAILURE;
    }
    
    bmi_buffer header;
    if (fread(&header, sizeof(bmi_buffer), 1, source) != 1) {
        bmi_set_error("bmi_buffer_from_file: An error occured while reading "
                      "the file header");
        return BMI_PTR_FAILURE;
    }
    
    if (!BMI_FILE_IS_VALID(header)) {
        bmi_set_error("bmi_buffer_from_file: File has invalid header");
        return BMI_PTR_FAILURE;
    }
    
    if (BMI_VERSION_IS_OUTDATED(*header.version)) {
        bmi_set_error("bmi_buffer_from_file: File has outdated version");
        return BMI_PTR_FAILURE;
    }
    
    if (BMI_VERSION_IS_LATER(*header.version)) {
        bmi_set_error("bmi_buffer_from_file: File has version from future");
        return BMI_PTR_FAILURE;
    }
    
    bmi_buffer* buffer = malloc(length);
    if (buffer == NULL) {
        bmi_set_error("bmi_buffer_from_file: Exhausted virtual memory");
        return BMI_PTR_FAILURE;
    }
    rewind(source);
    if (fread(buffer, sizeof(char), length, source) != length) {
        bmi_set_error("bmi_buffer_from_file: An error occured while reading "
                      "the file contents");
        return BMI_PTR_FAILURE;
    }
    
    return buffer;
}

int bmi_buffer_to_file(FILE* dest, const bmi_buffer* buffer) {
    if (fwrite(buffer, sizeof(buffer) + bmi_buffer_content_size(buffer), 1,
               dest) != 1) {
        bmi_set_error("bmi_buffer_to_file: Failed to write");
        return BMI_FAILURE;
    }
    return BMI_SUCCESS;
}

int bmi_buffer_to_ppm(FILE* dest, const bmi_buffer* buffer) {
    if (buffer->flags & BMI_FL_IS_GRAYSCALE) {
        if (fprintf(dest, "P5\n") != 3) {
            bmi_set_error("bmi_buffer_to_ppm: Failed to write 3 byte file "
                          "header");
            return BMI_FAILURE;
        }
    } else {
        if (fprintf(dest, "P6\n") != 3) {
            bmi_set_error("bmi_buffer_to_ppm: Failed to write 3 byte file "
                          "header");
            return BMI_FAILURE;
        }
    }
    if (fprintf(dest, "%u %u\n255\n", buffer->width, buffer->height) < 0) {
        bmi_set_error("bmi_buffer_to_ppm: Failed to write image size "
                      "information");
        return BMI_FAILURE;
    }
    if (fwrite(buffer->contents, bmi_buffer_content_size(buffer), 1,
               dest) != 1) {
        bmi_set_error("bmi_buffer_to_ppm: Failed to write image data");
        return BMI_FAILURE;

    }
    return BMI_SUCCESS;
}

int bmi_buffer_to_bmp(FILE* dest, const bmi_buffer* buffer) {
    bmi_set_error("bmi_buffer_to_bmp: Function not implemented");
    return BMI_BUG;
}

