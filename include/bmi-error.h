// include: bmi-error.h
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

#ifndef _BMI_INTERNAL_ERROR_H
#define _BMI_INTERNAL_ERROR_H

#ifdef _BMI_USE_INTERNAL
void bmi_set_error(const char* error);
#endif

const char* bmi_last_error(void);

#define BMI_SUCCESS 0
#define BMI_FAILURE 1
#define BMI_BUG 2

// NULL
#include <stddef.h>
#define BMI_PTR_FAILURE NULL

#define _BMI_THIRD(x, y, z, ...) z
#define _BMI_SELECT(x, y, ...) _BMI_THIRD(__VA_ARGS__, x, y, ~)

#define _BMI_IS_FAILABLE_bmi_buffer_overdraw_buffer ~, ~
#define _BMI_IS_FAILABLE_bmi_buffer_new ~, ~
#define _BMI_IS_FAILABLE_bmi_buffer_from_file ~, ~
#define _BMI_IS_FAILABLE_bmi_buffer_to_file ~, ~
#define _BMI_IS_FAILABLE_bmi_buffer_to_ppm ~, ~
#define _BMI_IS_FAILABLE_bmi_buffer_to_bmp ~, ~

#define BMI_IS_FAILABLE(func) _BMI_SELECT(1, 0, _BMI_IS_FAILABLE_##func)

#endif /* _BMI_INTERNAL_ERROR_H */
