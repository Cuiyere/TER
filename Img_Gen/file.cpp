#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include "file.h"


	/* Read a .ppm P3 image and store the pixels values in _pixels */
void Datafile::Readfile (const std::string & imgPath) {

	std::string line;
	int count = 0;
	int row = 0;

	std::ifstream img;
	img.open(imgPath);

	if (!img.is_open()) {
		std::cout << "ERROR: Can't read file " << imgPath << std::endl;
		return;
	}

	while(!img.eof()) {

		getline(img, line);

		if (line == "") {
			std::cout << "Empty line" << std::endl;
		} else if (count >= 3) {
			std::vector<int> values = SplitLine(line, ' ');
			for (int i = 0; i < values.size() / 3; i++) {
				_pixels[row][i] = ( 0.05 * values[3 * i] + 0.90 * values[3 * i + 1] + 0.05 * values[3 * i + 2] );
			}
			row++;
		} else if (count == 1) {
			std::vector<int> size = SplitLine(line, ' ');
			_height = size[0]; _width = size[1];
			std::cout << "The image is " << size[0] << " by " << size[1] << " pixels." << std::endl;
			_pixels.resize(size[0]);
			for (int i = 0; i < size[0]; i++) {
				_pixels[i].resize(size[1]);
			}
		} else {
			std::cout << line << std::endl;
		}
		count++;
	}
}


	/* Convert the _pixels table into an actual image */
void Datafile::PixelsToImg () {

	/* ------ Création du fichier ------ */
	std::string fileName = "grayscale_toron_059005_" + std::to_string(_width) + "_" + std::to_string(_height);
	system("mkdir -p ./Images");
	std::ofstream img ("./Images/"+fileName+".ppm");

	/* ------ Syntaxe pour un fichier .ppm ------ */
	img << "P3" << std::endl;
	img << _width << " " << _height << std::endl;
	img << "255" << std::endl;

	/* ------ Création de l'image ------ */
	for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {
			img << _pixels[i][j] << " " << _pixels[i][j] << " " << _pixels[i][j] << std::endl;
		}
	}

	/* ------ Ouverture de l'image ------- */
	//std::string cmd = "open ./Images/"+fileName+".ppm";
	//system(cmd.c_str());
}



	/* Uses the _pixels table to output an 'edge' image */
void Datafile::PixelsToEdges () {

	/* ------ Création du fichier ------ */
	std::string fileName = "edges_toron_059005_" + std::to_string(_width) + "_" + std::to_string(_height);
	system("mkdir -p ./Images");
	std::ofstream img ("./Images/"+fileName+".ppm");

	/* ------ Syntaxe pour un fichier .ppm ------ */
	img << "P3" << std::endl;
	img << _width << " " << _height << std::endl;
	img << "255" << std::endl;

	/* ------ Création de l'image ------ */

	for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {
			int pixelsValue = Evaluate(i, j);
			img << pixelsValue << " " << pixelsValue << " " << pixelsValue << std::endl;
		}
	}

	/* ------ Ouverture de l'image ------- */
	std::string cmd = "open ./Images/"+fileName+".ppm";
	system(cmd.c_str());
}

	/* Private functions - Tools */

	/* Split a line with a delimiter */
std::vector<int> Datafile::SplitLine(const std::string & line, char delimiter) {
	std::vector<int> tokens;
	std::string token;
	std::istringstream tokenStream(line);
	while (std::getline(tokenStream, token, delimiter))
	{
	    tokens.push_back(stoi(token));
	}
	return tokens;
}

	/* Evaluate the horizontal and vertical gradient for the pixel 
	   Uses the Sobel Operator */
int Datafile::Evaluate(int i, int j) {

	int valueGradX = 0;
	int valueGradY = 0;

	int neighX[] = {i-1, i, i+1, i-1, i+1, i-1, i, i+1};
	int neighY[] = {j-1, j-1, j-1, j, j, j+1, j+1, j+1};

	for (int k = 0; k < 8; k++) {
		/* If the neighbour exists in the image */
		if (neighX[k] > -1 && neighX[k] < _width && neighY[k] > -1 && neighY[k] < _height) {

			if (k == 0) {
				valueGradX += _pixels[i-1][j-1];
				valueGradY += _pixels[i-1][j-1];
			} else if (k == 1) {
				valueGradX += 2 * _pixels[i][j-1];
			} else if (k == 2) {
				valueGradX -= _pixels[i+1][j-1];
				valueGradY += _pixels[i+1][j-1];
			} else if (k == 3) {
				valueGradX += 2 * _pixels[i-1][j];
			} else if (k == 4) {
				valueGradX -= 2 * _pixels[i+1][j];
			} else if (k == 5) {
				valueGradX += _pixels[i-1][j+1];
				valueGradY -= _pixels[i-1][j+1];
			} else if (k == 6) {
				valueGradY -= 2 * _pixels[i][j+1];
			} else if (k == 7) {
				valueGradX -= _pixels[i+1][j+1];
				valueGradY -= _pixels[i+1][j+1];
			}
 		}
	}

	return sqrt(valueGradX * valueGradX + valueGradY * valueGradY);
}
