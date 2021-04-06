# Usage

This document describes the macros, data types and functions declared within the header files. All header files are included when `bmi.h` is included. Each data type is thus said to be declared only in the file where it is defined and not `bmi.h`.

The last section of this document explains how to piece together the components specified here.

Every item has a description that includes information on how it is defined, where it is defined and what dependencies it has.

Each item description also has a `Status` section. This is used to help facilitate usage of the item. If the status is marked `Static`, it can safely be used across BMI versions and in linking with compiled code. Otherwise, if the status is marked `Volatile`, then it may be unique to each version and thus should not be depended on. If an item has a dependency on a volatile component, it must too be volatile. In such cases, the status is marked `Derived`.

## Sections

1. Macros
2. Data Types
3. Functions

## 1. Macros

#### `BMI_TEXTUAL_SHORT`
_Expands to a string of the BMI file extension. Declared in `include/bmi-file.h`._  
**Status**: Static  
**Dependencies**: None

#### `BMI_TEXTUAL_LONG`
_Expands to a string of a long description of the BMI file format. Declared in `include/bmi-file.h`._  
**Status**: Static  
**Dependencies**: None

#### `BMI_HEADER_0`
_Expands to an integer constant expression. Declared in `include/bmi-file.h`._  
**Status**: Static  
**Dependencies**: None

#### `BMI_HEADER_1`
_Expands to an integer constant expression. Declared in `include/bmi-file.h`._  
**Status**: Static  
**Dependencies**: None

#### `BMI_HEADER_2`
_Expands to an integer constant expression. Declared in `include/bmi-file.h`._  
**Status**: Static  
**Dependencies**: None

#### `BMI_VERSION_CURRENT`
_Expands to an integer constant expression encoding the BMI version of the implementation. Defined in `include/bmi-file.h`._  
**Status**: Volatile  
**Dependencies**: None

#### `BMI_VERSION_PACK(maj, min, pat)`
_Expands to an expression that computes the encoded BMI version given the parameters. Defined in `include/bmi-file.h`_  
**Status**: Static  
**Dependencies**: None

##### Parameters
Name | Description
---- | -----------
`maj` | The major component of the version. Must be between 0 and 3, inclusive.
`min` | The minor component of the version. Must be between 0 and 7, inclusive.
`pat` | The patch component of the version. Must be between 0 and 7, inclusive.

#### `BMI_VERSION_IS_CURRENT(ver)`
_Expands to an expression that computes the whether encoded BMI version parameter matches the current BMI version of the implementation. Defined in `include/bmi-file.h`_  
**Status**: Derived  
**Dependencies**: `BMI_VERSION_CURRENT`

#### `BMI_VERSION_IS_OUTDATED(ver)`
_Expands to an expression that computes whether the encoded BMI version parameter encodes a version before the current BMI version of the implementation. Defined in `include/bmi-file.h`_  
**Status**: Derived  
**Dependencies**: `BMI_VERSION_CURRENT`

#### `BMI_VERSION_IS_LATER(ver)`
_Expands to an expression that computes whether the encoded BMI version parameter encodes a version after the current BMI version of the implementation. Defined in `include/bmi-file.h`_  
**Status**: Derived  
**Dependencies**: `BMI_VERSION_CURRENT`

#### `BMI_FILE_IS_VALID(buffer)`
_Expands to an expression that computes whether the given BMI file header provided by value, not reference, is a valid BMI file header. Defined in `include/bmi-file.h`_  
**Status**: Derived  
**Dependencies**: `bmi_buffer`, `BMI_HEADER_0`, `BMI_HEADER_1`, `BMI_HEADER_2`

#### `BMI_SUCCESS`
_Expands to an integer constant expression representing the success of a failable function returning an `int`. Defined in `include/bmi-error.h`_  
**Status**: Static  
**Dependencies**: None

#### `BMI_FAILURE`
_Expands to an integer constant expression representing the failure of a failable function returning an `int`. Defined in `include/bmi-error.h`_  
**Status**: Static  
**Dependencies**: None

#### `BMI_BUG`
_Expands to an integer constant expression representing the occurrence of a bug within BMI itself if a failable function returning an `int`. Defined in `include/bmi-error.h`_  
**Status**: Static  
**Dependencies**: None

#### `BMI_PTR_FAILURE`
_Expands to the constant `NULL` representing the failure of a failable function returning a pointer. Defined in `include/bmi-error.h`_  
**Status**: Static  
**Dependencies**: None

#### `BMI_POINT(_x, _y)`
_Expands to a constructor for a `bmi_point` structure with the given x and y coordinates. Defined in `include/bmi-geometry.h`._  
**Status**: Derived  
**Dependencies**: `bmi_point`

#### `BMI_RECT(_x, _y, _w, _h)`
_Expands to a constructor for a `bmi_rect` structure with the given origin, width and height. Defined in `include/bmi-geometry.h`._  
**Status**: Derived  
**Dependencies**: `bmi_rect`

#### `BMI_RGB(r, g, b)`
_Expands to an expression that computes a BMI color witht the given red, green and blue channels. Defined in `include/bmi-color.h`._  
**Status**: Derived  
**Dependencies**: `bmi_component`

#### `BMI_GRY(v)`
_Expands to an expression that computes a BMI color witht the given value channel. Defined in `include/bmi-color.h`._  
**Status**: Derived  
**Dependencies**: `bmi_component`

#### `BMI_RGB_R(c)`
_Expands to an expression that computes the red component of the BMI color. Defined in `include/bmi-color.h`._  
**Status**: Derived  
**Dependencies**: `bmi_channel`, `bmi_component`

#### `BMI_RGB_G(c)`
_Expands to an expression that computes the green component of the BMI color. Defined in `include/bmi-color.h`._  
**Status**: Derived  
**Dependencies**: `bmi_channel`, `bmi_component`

#### `BMI_RGB_B(c)`
_Expands to an expression that computes the blue component of the BMI color. Defined in `include/bmi-color.h`._  
**Status**: Derived  
**Dependencies**: `bmi_channel`, `bmi_component`

#### `BMI_GRY_V(c)`
_Expands to an expression that computes the gray component of the BMI color. Defined in `include/bmi-color.h`._  
**Status**: Derived  
**Dependencies**: `bmi_channel`, `bmi_component`

### 2. Data Types

#### enum `bmi_flags`
_Defines the flags used to configure the interpretation of a BMI file. Defined in `include/bmi-file.h`._  
**Status**: Static  
**Dependencies**: None  

**Values**
1. `BMI_FL_IS_GRAYSCALE`  
    Denotes that each pixel is 8-bit grayscale rather than 24 bit RGB.

#### struct `bmi_buffer`
_Defines the structure of a BMI file. Defined in `include/bmi-file.h`._
```c
typedef struct {
    uint8_t header[3];
    uint8_t version[1];
    uint32_t width;
    uint32_t height;
    uint32_t flags;
    uint8_t contents[];
} bmi_buffer;
```
**Status**: Static  
**Dependencies**: None  

#### struct `bmi_point`
_Defines a structure representing a position of a pixel in a BMI buffer. Defined in `include/bmi-geometry`._
```c
typedef struct {
    uint32_t x;
    uint32_t y;
} bmi_point;
```
**Status**: Static  
**Dependencies**: None  

#### struct `bmi_rect`
_Defines a structure representing a region of pixels in a BMI buffer. Defined in `include/bmi-geometry`._
```c
typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
} bmi_rect;
```
**Status**: Static  
**Dependencies**: None  

### enum `bmi_rect_edge`
_Defines the flags used to configure the interpretation of a BMI file. Defined in `include/bmi-geometry.h`._  
**Status**: Static  
**Dependencies**: None  

**Values**
1. `BMI_RECT_EDGE_LEFT`  
    The left edge of the rectangle.
2. `BMI_RECT_EDGE_RIGHT`  
    The right edge of the rectangle.
3. `BMI_RECT_EDGE_TOP`  
    The top edge of the rectangle.
4. `BMI_RECT_EDGE_BOTTOM`  
    The bottom edge of the rectangle.

#### typedef `bmi_channel`
_Defines a type capable of representing a channel in a pixel component. Defined in `include/bmi-color.h`._
```c
typedef uint32_t bmi_channel;
```

#### typedef `bmi_component`
_Defines a type capable of representing an entire pixel component. Defined in `include/bmi-color.h`._
```c
typedef uint32_t bmi_component;
```

### 3. Functions

#### `bmi_version_string`
_Returns a read-only textual description of the packed version. Defined in `include/bmi-file.h`._
```c
const char* bmi_version_string(const uint8_t version);
```
**Status**: Static  
**Dependencies**: None  

**Parameters**

Name | Description
---- | -----------
`version` | An encoded BMI version

**Return Value**

A read-only string describing the packed version in a textual format. This string is valid until the next call of the function, so use a mutex here when multithreading.

#### `bmi_buffer_total_size`
_Returns the total size, in bytes, of the BMI buffer's contents. Defined in `include/bmi-file.h`._
```c
size_t bmi_buffer_content_size(const bmi_buffer* buffer);
```  
**Status**: Derived  
**Dependencies**: `BMI_COMPONENT_SIZE_FROM_FL`, `bmi_buffer`  

**Parameters**

Name | Description
---- | -----------
`buffer` | A pointer to the BMI buffer that is to be inspected

**Return Value**

In bytes, the size of the pixel contents after the 16-byte header.

#### `bmi_last_error`
_Returns the latest error in BMI. Defined in `include/bmi-error.h`._
```c
const char* bmi_last_error(void);
```  
**Status**: Static  
**Dependencies**: None

**Return Value**

A read-only string describing the latest BMI error. This string is valid until the next error is set, so be sure to avoid performing failable functions while this string is being processed if multithreading.

#### `bmi_clip_rect`
_Clips the specified rectangle to the given bounds. Defined in `include/bmi-geometry.h`_
```c
void bmi_clip_rect(bmi_rect* rect, const bmi_rect bounds);
```  
**Status**: Derived  
**Dependencies**: `bmi_rect`

**Parameters**

Name | Description
---- | -----------
`rect` | A pointer to the BMI rect that should be bounded
`bounds` | A box which should define the extent to be clipped to

#### `bmi_dump_point`
_Prints a debug description of the specified point to the given file. Defined in `include/bmi-geometry.h`._
```c
void bmi_dump_point(FILE* dest, const bmi_point point);
```  
**Status**: Derived  
**Dependencies**: `bmi_point`

**Parameters**

Name | Description
---- | -----------
`dest` | A file pointer for output
`point` | The point to be printed

#### `bmi_dump_rect`
_Prints a debug description of the specified rect to the given file. Defined in `include/bmi-geometry.h`._
```c
void bmi_dump_rect(FILE* dest, const bmi_rect rect);
```  
**Status**: Derived  
**Dependencies**: `bmi_rect`

**Parameters**

Name | Description
---- | -----------
`dest` | A file pointer for output
`rect` | The rectangle to be printed

#### `bmi_inset_rect`
_Insets the specified rectangle by the given amount from the specified edge. Defined in `include/bmi-geometry.h`._
```c
void bmi_inset_rect(bmi_rect* rect, uint32_t delta, bmi_rect_edge edge);
```  
**Status**: Derived  
**Dependencies**: `bmi_rect`, `bmi_rect_edge`

**Parameters**

Name | Description
---- | -----------
`rect` | A pointer to the rectangle which is to be inset
`delta` | The amount, in pixels, to inset the rectangle
`edge` | The side to inset the rectangle on

#### `bmi_set_rect`
_Sets the specified edge of the specified rectangle to the given amount. Defined in `include/bmi-geometry.h`._
```c
void bmi_set_rect(bmi_rect* rect, uint32_t width, bmi_rect_edge edge);
```  
**Status**: Derived  
**Dependencies**: `bmi_rect`, `bmi_rect_edge`

**Parameters**

Name | Description
---- | -----------
`rect` | A pointer to the rectangle which is to be set
`delta` | The amount, in pixels, to set the rectangle
`edge` | The side to set the rectangle on

#### `bmi_buffer_draw_point`
_Draws a pixel at the specified coordinates. Defined in `include/bmi-draw.h`._
```c
void bmi_buffer_draw_point(bmi_buffer* buffer, bmi_point p, bmi_component pixel);
```  
**Status**: Derived  
**Dependencies**: `bmi_buffer`, `bmi_point`, `bmi_component`

**Parameters**

Name | Description
---- | -----------
`buffer` | A pointer to the BMI buffer that is to be drawn to
`p` | The position of the pixel to be written
`pixel` | The pixel to be written

#### `bmi_buffer_fill_rect`
_Fills a rectangle in the specified bounds. Defined in `include/bmi-draw.h`._
```c
void bmi_buffer_fill_rect(bmi_buffer* buffer, bmi_rect r, bmi_component pixel);
```  
**Status**: Derived  
**Dependencies**: `bmi_buffer`, `bmi_rect`, `bmi_component`

**Parameters**

Name | Description
---- | -----------
`buffer` | A pointer to the BMI buffer that is to be drawn to
`r` | The region encompassing the pixels to be written
`pixel` | The pixel to be written

#### `bmi_buffer_stroke_rect`
_Strokes a rectangle in the specified bounds with specified thickness. Defined in `include/bmi-draw.h`._
```c
void bmi_buffer_stroke_rect(bmi_buffer* buffer, bmi_rect r, uint32_t t, bmi_component pixel);
```  
**Status**: Derived  
**Dependencies**: `bmi_buffer`, `bmi_rect`, `bmi_component`

**Parameters**

Name | Description
---- | -----------
`buffer` | A pointer to the BMI buffer that is to be drawn to
`r` | The region surrounding the pixels to be written
`t` | The width of the stroke line
`pixel` | The pixel to be written

#### `bmi_buffer_new`
_Allocates a new BMI buffer to be freed initialized with the given attributes. Defined in `include/bmi-util.h`._
```c
bmi_buffer* bmi_buffer_new(uint32_t width, uint32_t height, uint32_t flags);
```  
**Status**: Derived  
**Dependencies**: `bmi_buffer`

**Parameters**

Name | Description
---- | -----------
`width` | The width of the image, in pixels
`height` | The height of the image, in pixels
`flags` | The configuration of the buffer

**Return Value**

An allocated BMI buffer suitable for being drawn to and written to disk. This must be freed at some point with a call to `free`.

#### `bmi_buffer_from_file`
_Reads in and allocates a new BMI buffer from the given file. Defined in `include/bmi-util.h`._
```c
bmi_buffer* bmi_buffer_from_file(FILE* source);
```  
**Status**: Derived  
**Dependencies**: `bmi_buffer`

**Parameters**
Name | Description
---- | -----------
`source` | The file to be read from

**Return Value**
An allocated BMI buffer with contents matching the file suitable for being drawn to and written to disk. This must be freed at some point with a call to `free`.

#### `bmi_buffer_to_file`
_Saves the BMI buffer to a file. Defined in `include/bmi-util.h`._
```c
int bmi_buffer_to_file(FILE* dest, const bmi_buffer* buffer);
```  
**Status**: Derived  
**Dependencies**: `bmi_buffer`

**Parameters**
Name | Description
---- | -----------
`dest` | The file to be written to
`buffer` | The BMI buffer to write

**Return Value**
Status of function.

#### `bmi_buffer_to_ppm`
_Saves the BMI buffer to a file as a PPM. Defined in `include/bmi-util.h`._
```c
int bmi_buffer_to_ppm(FILE* dest, const bmi_buffer* buffer);
```  
**Status**: Derived  
**Dependencies**: `bmi_buffer`

**Parameters**
Name | Description
---- | -----------
`dest` | The file to be written to
`buffer` | The BMI buffer to write

**Return Value**
Status of function.

#### `bmi_buffer_to_bmp`
_Saves the BMI buffer to a file as a BMP. Defined in `include/bmi-util.h`._
```c
int bmi_buffer_to_bmp(FILE* dest, const bmi_buffer* buffer);
```  
**Status**: Derived  
**Dependencies**: `bmi_buffer`

**Parameters**
Name | Description
---- | -----------
`dest` | The file to be written to
`buffer` | The BMI buffer to write

**Return Value**
Status of function.
