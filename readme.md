## Group Project - ENSEIRB-MATMECA
###### 2nd Year of Mathematics and Mechanics - Engineering Degree

**Project description:** Modelling of the mechanical behaviour of composite materials from 2D images : study at the strands' scale.

Group of 4 people - Implementation in C++ - ~90h\
Report: [Only in French](https://www.abrlr.fr/res/TER1819.pdf)

### What I learned with this code
 * Working with images in C++,
 * Basic image Processing (Histogram equalisation, Edge detection, Hough Transform for circle detection).

---

### /code
#### What is inside this repository ?
The code built to process images of strands in order to model their behaviour using a Finite Element code.

The code in this repository takes a strand image in input, and it outputs a simplified version of it, with only circles.

#### Disclaimer
This code is not optimised at all, a lot can be done to make it faster. During the project it was not the number one priority.

Things that can be improved:
* Use the Eigen library to use sparse vectors,
* Optimise functions that can be run in parallel,
* The program doesn't work for "large" images (more than ~400px wide).

It needs the image magick library installed because the images needs to be converted in the .pgm format (in order to process them easily in C++).



##### How can I use the code ?
It is pretty straight-forward. The code can be compile by the makefile only.
```
cd path/to/makefile
make
```
It is important to know that the code needs the **Image Magick** software to run properly. You can find documentation and tutorials on the [imagemagick website](https://imagemagick.org/index.php).

Also, the code uses **gnuplot** to output histograms of the images, it is not a mandatory step so you don't have to install it.

Once Image Magick has been installed, you can run the program by typing:
```
./run
```
Then, the program will require you to enter:\
1. the relative path to the image you want to process,
2. it's extension,
3. the minimum and maximum radius of the fibres visible (in pixels),
4. the zoom level of the image you want to process.

For example, with the sample image `strand.png` provided in the repo:
```
Note: Le fichier en entrée doit être un fichier image.
Ce programme nécessite que l'outil ImageMagick soit installé sur votre PC.

     Plus d'informations sur https://imagemagick.org

Enter file path: strand
Enter file extension: png
Enter min radius (integer): 10
Enter max radius (integer): 25
Enter zoom (integer): 100
```
After a few steps of preprocessing, the program will output 5 images.

Select the image where the fibres are clearly visible, without having too much noise, and input the index in the console:
```
Chose image number for circle research (1 to 5): 4
```

The program will output the volumic fraction of the chosen image:
```
Volumic fraction: 52.1415 % of fibre.
```
And after a few minutes, you will eventually have a simplified version of the image you had in the first place, **congrats** !

### /outdated
 First version of the code, totally unusable but draw pretty circles on images.
