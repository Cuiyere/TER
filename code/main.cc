    /* 

      main.cc - Interface de communication avec l'utilisateur et éxecution du programme
      TER 2018 - 2019 
      Arthur BIERLAIRE - Roxane DARVENNE - Benjamin SACCO - Vincent GISBERT

      "Etude des matériaux composites à l'échelle des torons"
       
    */

#include <iostream>
#include <string>
#include <chrono>
#include "GrayLevelImage2D.h"

int main ( int argc, char** argv ) {

	system("clear");

	std::cout << "Note: Le fichier en entrée doit être un fichier image." << std::endl;
	std::cout << "Ce programme nécessite que l'outil ImageMagick soit installé sur votre PC." << std::endl;
	std::cout << std::endl;
	std::cout << "     Plus d'informations sur https://imagemagick.org" << std::endl;
	std::cout << std::endl;
	
	std::string bName;
	std::cout << "Enter file path: ";
	std::cin >> bName;

	std::string ext;
	std::cout << "Enter file extension: ";
	std::cin >> ext;

	int umRad = 0;
	std::cout << "Enter min radius (integer): ";
	std::cin >> umRad;

	int uxRad = 0;
	std::cout << "Enter max radius (integer): ";
	std::cin >> uxRad;

	int zoom = 0;
	std::cout << "Enter zoom (integer): " ;
	std::cin >> zoom;

	std::chrono::time_point<std::chrono::system_clock> startPhase1, endPhase1, startPhase2, endPhase2;
    startPhase1 = std::chrono::system_clock::now();

	std::string cmd = "convert " + bName + "." + ext + " -type grayscale -compress none " + bName + ".pgm";
	system(cmd.c_str());
	try {
		cmd = "mkdir " + bName;
		system(cmd.c_str());
	} catch (int e) {
		std::cout << "	Folder already exists." << std::endl;
	}


		/* Import de l'image à traiter */

	GrayLevelImage2D img(bName);
	std::ifstream input(bName + ".pgm"); // récupère le 1er argument.
	bool ok = img.ImportPGM(input);
	cmd = "rm " + bName + ".pgm ";
	system(cmd.c_str());

	std::string name = bName + "/" + bName; // Les images suivantes sont transférées dans le dossier correspondant
	
	if (!ok) {
	   	std::cerr << "Error reading input file." << std::endl;
	   	return 1;
	} else {
		std::cout << "File read successfully." << std::endl;
	}
	input.close();


		/* Processing de l'image */

	std::cout << "Image Basic Processing : " << std::endl;
	img.GetHistogram(name); // Calcul de l'histogramme
	img.EqualizeHistogram(name); // Égalisation de l'histogramme

	int thresholds[] = {30, 60, 120, 180, 240};
	std::cout << "Leveling Image ..." << std::endl;

	for (int i = 0; i < sizeof(thresholds)/sizeof(int); i++) {
		GrayLevelImage2D newImg = img;
		std::string t = std::to_string(thresholds[i]);
		newImg.LevelImage(thresholds[i]); // On nivelle l'image pour avoir des contours bien définis
		std::cout << "    - saving file n°" << i+1 << std::endl;
		newImg.ConvertToPNG(name+"-"+t+"-processed", newImg.GetData());
//		cmd = "open " + name + "-"+t+"-processed.png";
//		system(cmd.c_str());
	}

		/* Choix de l'image à traiter pour la recherche des fibres */

	endPhase1 = std::chrono::system_clock::now();
	int imageIndex = -1;
	while (imageIndex < 1 || imageIndex > sizeof(thresholds)/sizeof(int)) {
		std::cout << "Chose image number for circle research (1 to 5): ";
		std::cin >> imageIndex;

		if (imageIndex < 1 || imageIndex > sizeof(thresholds)/sizeof(int)) std::cout << "Error : Image Index not in range." << std::endl;
	}

		/* Traitement de l'image selectionnée */

	startPhase2 = std::chrono::system_clock::now();
	
	cmd = "clear";
	system(cmd.c_str());

	GrayLevelImage2D newImg = img;
	std::string t = std::to_string(thresholds[imageIndex-1]);
	std::cout << "Computations are done with the following threshold: " << t << std::endl;
	
	newImg.LevelImage(thresholds[imageIndex-1]); // Nivellation de l'image
	float fvol = newImg.FVol(newImg.GetData()); // Calcul de la fraction volumique
	std::cout << "Volumic fraction: " << fvol*100 << " % of fiber." << std::endl;
	newImg.FindEdges(); // On cherche les différents pixels de l'image susceptible d'être des côtés
	newImg.ConvertToPNG(name+"-edges", newImg.GetData());
	newImg.HoughTransform(umRad, uxRad, name, zoom);

	cmd = "open " + bName + "." + ext;
	system(cmd.c_str());

	endPhase2 = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds1 = endPhase1-startPhase1, elapsed_seconds2 = endPhase2 - startPhase2;
	std::chrono::duration<double> elapsed_seconds = elapsed_seconds1 + elapsed_seconds2;
	std::cout << "Processing done in " << elapsed_seconds.count() << "s." << std::endl;

	std::cout << "Closing application." << std::endl;

	return 0;
}