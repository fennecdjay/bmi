// include: bmi-color.h
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

#ifndef _BMI_INTERNAL_COLOR_H
#define _BMI_INTERNAL_COLOR_H

#include <stdint.h>

typedef uint32_t bmi_channel;
typedef uint32_t bmi_component;

// Packs components based on input values for RGB or grayscale
#define BMI_RGB(r, g, b) ((bmi_component)((r) | (g) << 8 | (b) << 16))
#define BMI_GRY(g) ((bmi_component)(g))

// Predefined colors
#define BMI_RGB_WHITE() BMI_RGB(255, 255, 255)
#define BMI_RGB_BLACK() BMI_RGB(0, 0, 0)

#define BMI_RGB_RED() BMI_RGB(255, 0, 0)
#define BMI_RGB_GREEN() BMI_RGB(0, 255, 0)
#define BMI_RGB_BLUE() BMI_RGB(0, 0, 255)

#define BMI_RGB_MAGENTA() BMI_RGB(255, 0, 255)
#define BMI_RGB_YELLOW() BMI_RGB(255, 255, 0)
#define BMI_RGB_CYAN() BMI_RGB(0, 255, 255)

#endif /* _BMI_INTERNAL_COLOR_H */
