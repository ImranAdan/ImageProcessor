C++ ImageProcessor
==============

Image Processing Tool

An Image Processing tool used to alter images using various images processing techniques, these include:
  -	Reading and displaying images in RAW format 
  -	Image Pixel Value Re-Scaling 
  -	Image Pixel Shifting 
  -	Image Convolution using 3x3 Mask 
  
  Smoothing 
    - Averaging and Weight Averaging 
    - Edge detection  
    - Laplacian 4 and 8 neighbourhood
    - Sobel X with absolute value conversion 
    - Sobel Y with absolute value conversion
    - Roberts with absolute value conversion 
  
  Generating Salt and Pepper noise
  
  Order Statistics Filtering 
    - Min Filter 
    - Midpoint Filter 
    - Max Filter 
    - Median Filter
  
  Image Enhancements using Point Processing
    - Negative Linear Transform 
    - Logarithmic Function 
    - Power-Law 
    - Random Look-up Table  

  Image Enhancements using Histogram and Histogram Equalisation

  Image Thresholding
   - Simple Thresholding
   - Automated Thresholding


Prerequisites 
==============
To run this program you must have the following:

  - linux OS
  - wxwidgets
  - OpenGL

Running 
==============

Open Terminal and cd to file
  - make MakeFile
  - ./window


## Example - Histogram Equalisation of Lena  

### Before Equalisation ###

![Before_Equalisation](http://i.imgur.com/loALKvl.png) 


### After Equalisation ###

![After_Equalisation](http://i.imgur.com/GLKOpLI.png) 
  

  
  

