// include: bmi-draw.h
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

#ifndef _BMI_INTERNAL_DRAW_H
#define _BMI_INTERNAL_DRAW_H

#include "bmi-file.h"
#include "bmi-geometry.h"
#include "bmi-color.h"

// Draws a pixel at the specified coordinates
void bmi_buffer_draw_point(bmi_buffer* buffer, bmi_point p,
                           bmi_component pixel);

// Fills a rectangle in the specified bounds
void bmi_buffer_fill_rect(bmi_buffer* buffer, bmi_rect r, bmi_component pixel);

// Strokes a rectangle in the specified bounds with specified thickness
void bmi_buffer_stroke_rect(bmi_buffer* buffer, bmi_rect r, uint32_t t,
                            bmi_component pixel);

// Fills an ellipse in the specified bounds
void bmi_buffer_fill_ellipse(bmi_buffer* buffer, bmi_rect r,
                             bmi_component pixel);

// Strokes a line between the specified points with specified thickness
void bmi_buffer_stroke_line(bmi_buffer* buffer, bmi_point s, bmi_point e,
                            uint32_t t, bmi_component pixel);

#endif /* _BMI_INTERNAL_DRAW_H */
