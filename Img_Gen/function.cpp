/* function.cpp
	Bierlaire Arthur
	22.10.18

	Définition des fonctions de traitement d'image
*/

#include "function.h"
#include "math.h"
#include<iostream>
#include<fstream>
using namespace std;

void DrawCircle (ofstream& img, int height, int width, int radius) {

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			//if (IsInCirle(i, j, radius, (int)height/2, (int)width/2)) img << (i+j)%255 << " " << (i+j)%255 << " " << (i+j)%255 << endl;
			if (IsInCirle(i, j, radius, (int)height/2, (int)width/2)) img << "0 0 0" << endl;
			else img << "255 255 255" << endl;
		}
	}
}

bool IsInCirle (int i, int j, int radius, int xCen, int yCen) {

	if (sqrt(pow(i-yCen,2)+pow(j-xCen,2)) <= radius) return true;
	return false;
}


void DrawGradient (ofstream& img, int height, int width) {

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int r = i % 255;
			int g = j % 255;
			int b = i * j % 255;

			img << r << " " << g << " " << b << endl;
		}
	}
}