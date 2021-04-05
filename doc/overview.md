# Overview

BMI is an extremely simple file format for bitmap images. It has a 16 byte header and following that are the bits that compose the image.

![BMI Layout](/img/header.svg)

BMI supports both 8-bit grayscale and 24-bit RGB. The maximum dimensions of a BMI image are 4,294,967,295 by 4,294,967,295 pixels.

The way that the pixel data is laid out is not specified at the binary level. Thus a BMI file saved on a little-endian machine cannot be accessed on a big-endian machine, and vice-versa.

You can read more about usage [in the docs](usage.md) and write safe and robust code following [this guide](integrity.md).
