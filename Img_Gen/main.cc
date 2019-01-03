 /* main.c
 	Bierlaire Arthur
	15.10.2018

	Programme principal
 */

#include<fstream>
#include<iostream>
#include<string>

#include "file.h"

int main () {

	system("clear");

	std::string name;
	std::cout << "Enter filename:" << std::endl;
	std::cin >> name;
	std::cout << "--------------------" << std::endl;
	std::cout << "Warning: The image file will be resized to 170x170" << std::endl;
	std::cout << "--------------------" << std::endl;

	std::string cmd = "convert " + name + " -resize 170x170  " + name + ".jpg";
	system(cmd.c_str());

	cmd = "convert " + name + ".jpg -compress none " + name + ".ppm";
	system(cmd.c_str()); // Implement this later
	

	Datafile data(name);

	std::cout << "Reading file: " << data.GetName() << ".ppm" << std::endl;
	data.Readfile(name+".ppm");
	std::cout << "Applying Sobel Operator" << std::endl;
	data.PixelsToImg();
	std::cout << "Resolving Edges" << std::endl;
	data.PixelsToEdges();

	return 0;
}
