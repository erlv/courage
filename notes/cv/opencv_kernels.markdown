# The kernels in OpenCV library
This doc is about the OpenCV library kernels


Core OpenCV modules:
- Core
- Imgproc


## Type conversion from Float to short
To display the image, int16_t is required, however most of the processing are
done by floating point operations. Therefore, type conversion from float to
int16_t is important. Meanwhile, to make the display meaningful, saturation is
required sometime.


OpenCV have template cast operation `saturate_cast`.
TODO: analyze the code.


## Binary Image Threshold
This is commonly used in image segmentation. It requires element-wise comparison
between a pixel of interest and a threshold value.


## Gaussian Blur
Image blurring is an operation taht convolves an image with a blurring or
smoothing filter. Each image pixel, the result is the weighted sum of itself and
the neighborhood pixels, where the weights come from the entry of the filter.


## Sobel Filter



## Edge Detection
Sobel Filter is often used for edge detection.
