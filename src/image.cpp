/*
 * image.cpp
 *
 *  Created on: Oct 15, 2018
 *      Author: arthur
 */

#include "image.h"
#include<iostream>
#include<vector>

using namespace std;

Image::Image(int width, int length) {
	_width = width;
	_length = length;
	_pixels.resize(width, vector< vector<int>() >)

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < length; j++) {

		}
	}

}

Image::~Image() {
	cout << "Yo je suis détruit" << endl;
}

Image::SetPixel (int i, int j, std::vector<int> rgbValues) {
// Bonjour je suis le TER de l'ambiance
}

