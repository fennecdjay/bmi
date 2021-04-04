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
