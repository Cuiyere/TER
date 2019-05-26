 # README

## /code
 Folder where the final image processing code is.
 The code can be compile by the makefile, but it needs image magick to process images.
 The code is built to take an strand image as an input, and it outputs a simplified version with only circles in it.
  
## /outdated
 First version of the code, totally unusable but draw pretty circles on images.

## What I learned
 * Work with images
 * Image processing basics
 * Hough transform for circles detection

## Disclaimer

 This code is not optimized at all, a lot can be done to make it faster because during the project it was not the top 1 priority.
 It needs the image magick library installed because the images needs to be converted in the .pgm format (in order to process them easily in C++).
