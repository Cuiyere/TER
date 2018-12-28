 /* main.c
 	Bierlaire Arthur
	15.10.2018

	Programme principal
 */

#include<fstream>
#include<iostream>
#include<string>

#include "file.h"

using namespace std;

int main () {

	system("clear");


//	system("convert toron.ppm -compress none toron3.ppm") // Implement this later
	

	Datafile data;

	data.Readfile("Images/toron3.ppm");
	data.PixelsToImg();
	data.PixelsToEdges();


	return 0;
}
