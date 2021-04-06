//
//  tests.c
//  bitmap-image
//
//  Created by Ethan Uppal on 4/1/21.
//  Copyright © 2021 Ethan Uppal. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "include/bmi.h"

int test_create_concentric_rects() {
    bmi_buffer* buffer = bmi_buffer_new(256, 256, 0);
    if (buffer == NULL) {
        fprintf(stderr, "%s\n", bmi_last_error());
        return 1;
    }
    
    bmi_buffer_fill_rect(buffer, BMI_RECT(0, 0, 256, 256), BMI_RGB_BLACK());
    bmi_buffer_stroke_rect(buffer, BMI_RECT(0, 0, 256, 256), 8,  BMI_RGB_RED());
    bmi_buffer_stroke_rect(buffer, BMI_RECT(16, 16, 224, 224), 8,  BMI_RGB_ORANGE());
    bmi_buffer_stroke_rect(buffer, BMI_RECT(16, 16, 224, 224), 8,  BMI_RGB_YELLOW());
    bmi_buffer_stroke_rect(buffer, BMI_RECT(32, 32, 192, 192), 8,  BMI_RGB_GREEN());
    bmi_buffer_stroke_rect(buffer, BMI_RECT(48, 48, 160, 160), 8,  BMI_RGB_BLUE());
    bmi_buffer_stroke_rect(buffer, BMI_RECT(64, 64, 128, 128), 8,  BMI_RGB_MAGENTA());
    bmi_buffer_stroke_rect(buffer, BMI_RECT(80, 80, 96, 96), 8,  BMI_RGB_CYAN());
    bmi_buffer_stroke_rect(buffer, BMI_RECT(96, 96, 64, 64), 8,  BMI_RGB_WHITE());
    bmi_buffer_stroke_rect(buffer, BMI_RECT(112, 112, 32, 32), 8,  BMI_RGB(128, 64, 32));

    FILE* file = fopen("test.ppm", "w");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }
    if (bmi_buffer_to_ppm(file, buffer) != BMI_SUCCESS) {
        fprintf(stderr, "%s\n", bmi_last_error());
        return 1;
    }
    if (fclose(file) != 0) {
        perror("fclose");
        return 1;
    }
    
    free(buffer);
    
    return 0;
}

int test_read_smile_bmi() {
    FILE* file = fopen("test.bmi", "r");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }
    bmi_buffer* buffer = bmi_buffer_from_file(file);
    if (buffer == NULL) {
        fprintf(stderr, "%s\n", bmi_last_error());
        return 1;
    }
    if (fclose(file) != 0) {
        perror("fclose");
        return 1;
    }
    
    file = fopen("test.ppm", "w");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }
    if (bmi_buffer_to_ppm(file, buffer) != BMI_SUCCESS) {
        fprintf(stderr, "%s\n", bmi_last_error());
        return 1;
    }
    if (fclose(file) != 0) {
        perror("fclose");
        return 1;
    }
    
    free(buffer);
    
    return 0;
}

int test_create_smile_bmi() {
    bmi_buffer* buffer = bmi_buffer_new(256, 256, 0);
    if (buffer == NULL) {
        fprintf(stderr, "%s\n", bmi_last_error());
        return 1;
    }
    
    bmi_buffer_fill_rect(buffer, BMI_RECT(0, 0, 256, 256), BMI_RGB_YELLOW());
    
    bmi_buffer_fill_rect(buffer, BMI_RECT(32, 32, 48, 48), BMI_RGB_BLACK());
    bmi_buffer_fill_rect(buffer, BMI_RECT(176, 32, 48, 48), BMI_RGB_BLACK());
    bmi_buffer_fill_rect(buffer, BMI_RECT(32, 124, 48, 96), BMI_RGB_BLACK());
    bmi_buffer_fill_rect(buffer, BMI_RECT(180, 124, 48, 96), BMI_RGB_BLACK());
    bmi_buffer_fill_rect(buffer, BMI_RECT(32, 172, 196, 48), BMI_RGB_BLACK());
    
    FILE* file = fopen("test.bmi", "w");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }
    bmi_buffer_to_file(file, buffer);
    if (fclose(file) != 0) {
        perror("fclose");
        return 1;
    }
    
    free(buffer);
    
    return 0;
}

int test_create_smile_ppm() {
    bmi_buffer* buffer = bmi_buffer_new(256, 256, 0);
    if (buffer == NULL) {
        fprintf(stderr, "%s\n", bmi_last_error());
        return 1;
    }
    
    bmi_buffer_fill_rect(buffer, BMI_RECT(0, 0, 256, 256), BMI_RGB_YELLOW());
    
    bmi_buffer_fill_rect(buffer, BMI_RECT(32, 32, 48, 48), BMI_RGB_BLACK());
    bmi_buffer_fill_rect(buffer, BMI_RECT(176, 32, 48, 48), BMI_RGB_BLACK());
    bmi_buffer_fill_rect(buffer, BMI_RECT(32, 124, 48, 96), BMI_RGB_BLACK());
    bmi_buffer_fill_rect(buffer, BMI_RECT(180, 124, 48, 96), BMI_RGB_BLACK());
    bmi_buffer_fill_rect(buffer, BMI_RECT(32, 172, 196, 48), BMI_RGB_BLACK());
    
    FILE* file = fopen("test.ppm", "w");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }
    if (bmi_buffer_to_ppm(file, buffer) != BMI_SUCCESS) {
        fprintf(stderr, "%s\n", bmi_last_error());
        return 1;
    }
    if (fclose(file) != 0) {
        perror("fclose");
        return 1;
    }
    
    free(buffer);
    
    return 0;
}
