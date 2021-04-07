// src: bmi-color.c
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

#include "bmi-color.h"

#define _u32(...) ((uint32_t)(__VA_ARGS__))

bmi_component bmi_rgb_blend(bmi_component c0, uint32_t i0, bmi_component c1,
                            uint32_t i1) {
    const bmi_channel red = (_u32(BMI_RGB_R(c0)) * _u32(i0)
                             + _u32(BMI_RGB_R(c1)) * _u32(i1)) / 256;
    const bmi_channel green = (_u32(BMI_RGB_G(c0)) * _u32(i0)
                               + _u32(BMI_RGB_G(c1)) * _u32(i1)) / 256;
    const bmi_channel blue = (_u32(BMI_RGB_B(c0)) * _u32(i0)
                              + _u32(BMI_RGB_B(c1)) * _u32(i1)) / 256;
    return BMI_RGB(red, green, blue);
}
