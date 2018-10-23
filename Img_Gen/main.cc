 /* main.c
 	Bierlaire Arthur
	15.10.2018

	Programme principal
 */

#include "function.h"
#include<fstream>
#include<iostream>
#include<string>

using namespace std;

int main () {
	const int width = 1023, height = 1023;
	const int radius;

	// ------ Création du fichier ------
	string fileName;
	system("mkdir -p ./Images");
	cout << "Entrez le nom du fichier:" << endl;
	cin >> fileName;
	ofstream img ("./Images/"+fileName+".ppm");

	// ------ Syntaxe pour un fichier .ppm ------
	img << "P3" << endl;
	img << width << " " << height << endl;
	img << "255" << endl;

	// ------ Création de l'image ------
	cout << "Entrez le rayon du cercle:" << endl;
	cin >> radius;
	//DrawGradient (img, height, width); // Uncomment to draw a gradient
	DrawCircle(img, height, width, radius);

	// ------ Ouverture de l'image -------
	string cmd = "open ./Images/"+fileName+".ppm";
	system(cmd.c_str());
	return 0;
}
