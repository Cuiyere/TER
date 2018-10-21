/*
 - main.c -
 15.10.2018
 Arthur Bierlaire
 */

#include<fstream>

using namespace std;

int main () {
	const int width = 255, height = 255;

	ofstream img ("picture.ppm");
	img << "P3" << endl;
	img << width << " " << height << endl;
	img << "255" << endl;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int r = i % 255;
			int g = j % 255;
			int b = i * j % 255;

			img << r << " " << g << " " << b << endl;
		}
	}

	system("open picture.ppm");
	return 0;
}
