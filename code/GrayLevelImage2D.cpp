	/* 

	   GrayLevelImage.cpp - Librarie de traitement d'image
	   TER 2018 - 2019 
	   Arthur BIERLAIRE - Roxane DARVENNE - Benjamin SACCO - Vincent GISBERT

	   "Etude des matériaux composites à l'échelle des torons"

	*/

#include "GrayLevelImage2D.h"
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>

		/* Constructeur */

GrayLevelImage2D::GrayLevelImage2D (std::string name) {
	_IMG_NAME = name;
	std::vector<int> histogram(255);
	_IMG_HISTOGRAM = histogram; _IMG_AHISTOGRAM = histogram;
}

GrayLevelImage2D::GrayLevelImage2D( int w, int h,  GrayLevel g) {
	_IMG_WIDTH = w;
	_IMG_HEIGHT = h;

	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {
			_IMG_DATA.push_back(g);
		}
	}

	std::vector<int> histogram(255);
	_IMG_HISTOGRAM = histogram; _IMG_AHISTOGRAM = histogram;
}


		/* ************************************************ */
		/* Fonctions d'import et d'export de fichiers image */
		/* ************************************************ */


	/* 
		ImportPGM : Importe une image au format .pgm et la stocke dans un Container
		@input    : Fichier à partir duquel on lit les lignes qui définissent les pixels
	*/

bool GrayLevelImage2D::ImportPGM (std::istream & input) {

	std::string line;
	int count = 0;

	std::cout << "Reading file." << std::endl;

	while(!input.eof()) {

		getline(input, line);

		if (count == 1) {
			std::vector<int> size = SplitLine(line, ' ');
			_IMG_HEIGHT = size[0]; _IMG_WIDTH = size[1];
		} else if (count > 2) {
			std::vector<int> values = SplitLine(line, ' ');
			for (int i = 0; i < values.size(); i++) {
				_IMG_DATA.push_back(values[i]);
			}
		}
		count++;
	}
	return true;
}

	/* 
		ExportPGM : Interface entre l'interprétation d'une image par notre code et la représentation visuelle de celle-ci
		@output   : Fichier .pgm qui représente l'image qu'on souhaite exporter
		@data     : Contient les informations des pixels de l'image sous forme d'un vecteur d'entiers
	*/

bool GrayLevelImage2D::ExportPGM (std::ostream & output, Container data) const {

	/* ------ Syntaxe pour un fichier .ppm ------ */
	output << "P2" << std::endl;
	output << _IMG_HEIGHT << " " << _IMG_WIDTH << std::endl;
	output << "255" << std::endl;

	/* ------ Création de l'image ------ */
	for (int j = 0; j < _IMG_HEIGHT; j++) {
		for (int i = 0; i < _IMG_WIDTH; i++) {
			output << data[Index(i, j)] << " ";
		}
		output << std::endl;
	}
	return true;
}

	/*
		ConvertToPNG : Permet de former une image au format .png à partir des données contenues dans un Container
		@name        : Nom du fichier image en sortie de la fonction
		@data        : Contient les informations des pixels de l'image sous forme d'un vecteur d'entiers
	*/

void GrayLevelImage2D::ConvertToPNG (std::string name, Container data) {
		
		/* ------ Enregistrement de l'image ------ */
	std::ofstream output(name + ".pgm");
	bool ok = ExportPGM(output, data);
	if (!ok) {
	   	std::cerr << "ERROR: Can't write " + name + " file." << std::endl;
	   	return;
	}
	output.close();

	  	/* ------ Conversion de l'image en png ------ */
	std::string cmd;
	cmd = "convert " + name + ".pgm " + name + ".png";
	system(cmd.c_str());
	cmd = "rm " + name + ".pgm";
	system(cmd.c_str());
}


void GrayLevelImage2D::ExportFibers (std::string name, Container circlesCenter, Container circlesRadius, int width, int height, int zoom) {

	float distByPixel = (zoom==50)?0.14:0.07;
	distByPixel *= 0.001;

	std::ofstream output(name + ".txt");
	
	output << "dim_x" << std::endl;
	output << width * distByPixel << std::endl;
	output << "dim_y" << std::endl;
	output << height * distByPixel << std::endl;
	output << "cirles" << std::endl;

	for (int i = 0; i < _IMG_DATA.size(); i++) {
		if (circlesCenter[i] < 1) {
			output << GetX(i) * distByPixel << " " << GetY(i) * distByPixel << " " << circlesRadius[i] * distByPixel << std::endl;
		}
	}
	output.close();
}



			/* ***************************************** */	
			/* Fonctions relatives au traitement d'image */
			/* ***************************************** */


	/* 
		GetHistogram : Fonction qui calcule l'histogramme et l'histogramme cumulé de l'image en cours
		@name        : Nom du fichier .png où les graphes des histogrammes seront sauvegardés
	*/

void GrayLevelImage2D::GetHistogram (std::string name) {

	std::cout << "	Getting Histograms" << std::endl;

		/* ------ Processing de l'histogramme ------ */

	for (int i = 0; i < _IMG_DATA.size(); i++) {
		_IMG_HISTOGRAM[_IMG_DATA[i]] += 1;
	}

	_IMG_AHISTOGRAM[0] = _IMG_HISTOGRAM[0];
	for (int i = 1; i < 256; i++) {
		_IMG_AHISTOGRAM[i] = _IMG_HISTOGRAM[i] + _IMG_AHISTOGRAM[i-1];
	}

		/* ------ Sauvegarde de l'histogramme sous forme de graphe ------ */

	float max = 0;
	std::ofstream data(name + ".dat");
	for (int i = 0; i < 256; i++) {
		float percentage = _IMG_HISTOGRAM[i];
		data << i << " " << percentage << std::endl;
		if (percentage > max) max = percentage;
	}
	data.close();

	float maxAcc = 0;
	std::ofstream accData(name + "-acc.dat");
	for (int i = 0; i < 256; i++) {
		float percentage = _IMG_AHISTOGRAM[i];
		accData << i << " " << percentage << std::endl;
		if (percentage > maxAcc) maxAcc = percentage;
	}
	accData.close();

		/* ------ Utilise un fichier de configuration gnuplot pour tracer et enregister les graphes ------ */

	std::ofstream gnu_config(name + "-gnu_config.gnu"); // Create gnuplot config
	gnu_config << "set terminal png" << std::endl;
	gnu_config << "set output '" + name + "-histograms.png" << std::endl;
	gnu_config << "set multiplot layout 2, 1" << std::endl;
	gnu_config << "set xrange [0:255]" << std::endl;
	gnu_config << "set yrange [0:" + std::to_string(max*1.2) + "]" << std::endl;
	gnu_config << "plot '" + name + ".dat' with filledcurves above x1 lt -1" << std::endl;
	gnu_config << "unset key" << std::endl;
	gnu_config << "set xrange [0:255]" << std::endl;
	gnu_config << "set yrange [0:" << std::to_string(maxAcc*1.2)<< "]" << std::endl;
	gnu_config << "plot '" + name + "-acc.dat' with filledcurves above x1 lt -1" << std::endl;
	gnu_config << "unset key" << std::endl;
	gnu_config.close();

	std::string cmd = "gnuplot '" + name + "-gnu_config.gnu'";
	system(cmd.c_str());

	cmd = "rm " + name + "-gnu_config.gnu " + name + ".dat " + name + "-acc.dat";
	system(cmd.c_str());
}


void GrayLevelImage2D::EqualizeHistogram (std::string name) {

	std::cout << "	Equalizing Image Histogram." << std::endl;

	float max = 0;
	for (int i = 0; i < _IMG_DATA.size(); i++) {
		if (max < _IMG_DATA[i]) max = _IMG_DATA[i];
	}

	for (int i = 0; i < _IMG_DATA.size(); i++) {
		_IMG_DATA[i] *= (float)255 / max;
	}

	std::ofstream img(name+"-equalized");
	bool ok = ExportPGM(img, _IMG_DATA);
	if (!ok) {
		std::cout << "ERROR: Exporting Histogram Data ! " << std::endl;
		return;
	}
	ConvertToPNG(name+"-equalized", _IMG_DATA);

	std::string cmd = "rm " + name + "-equalized";
	system(cmd.c_str());

	/* Calculate Histograms */

	std::vector<int> equalHistogram(255);
	for (int i = 0; i < _IMG_DATA.size(); i++) {
		equalHistogram[_IMG_DATA[i]] += 1;
	}

	std::vector<int> equalAHistogram(255);
	equalAHistogram[0] = equalHistogram[0];
	for (int i = 1; i < 256; i++) {
		equalAHistogram[i] = equalHistogram[i] + equalAHistogram[i-1];
	}

	max = 0;
	std::ofstream data(name + "-new-histogram.dat"); // Print acc Histogram values in .dat file
	for (int i = 0; i < 256; i++) {
		data << i << " " << equalHistogram[i] << std::endl;
		if (equalHistogram[i] > max) max = equalHistogram[i];
	}
	data.close();

	float maxAcc = 0;
	std::ofstream accData(name + "-new-acchistogram.dat"); // Print acc Histogram values in .dat file
	for (int i = 0; i < 256; i++) {
		accData << i << " " << equalAHistogram[i] << std::endl;
		if (equalAHistogram[i] > maxAcc) maxAcc = equalAHistogram[i];
	}
	accData.close();

	std::ofstream gnu_config(name + "-gnu_config.gnu"); // Create gnuplot config
	gnu_config << "set terminal png" << std::endl;
	gnu_config << "set output '" + name + "-e-histograms.png" << std::endl;
	gnu_config << "set multiplot layout 2, 1" << std::endl;
	gnu_config << "set xrange [0:255]" << std::endl;
	gnu_config << "set yrange [0:" << std::to_string(max*1.2) << "]" << std::endl;
	gnu_config << "plot '" + name + "-new-histogram.dat' with filledcurves above x1 lt -1" << std::endl;
	gnu_config << "set xrange [0:255]" << std::endl;
	gnu_config << "set yrange [0:" << std::to_string(maxAcc*1.2) << "]" << std::endl;
	gnu_config << "plot '" + name + "-new-acchistogram.dat' with filledcurves above x1 lt -1" << std::endl;
	gnu_config.close();

	cmd = "gnuplot '" + name + "-gnu_config.gnu'";
	system(cmd.c_str());

	cmd = "rm " + name + "-gnu_config.gnu " + name + "-new-histogram.dat " + name + "-new-acchistogram.dat";
	system(cmd.c_str());
}


void GrayLevelImage2D::LevelImage (int threshold) {
	for (int i = 0; i < _IMG_DATA.size(); i++) {
		if (_IMG_DATA[i] < threshold) {
			_IMG_DATA[i] = 255;
		} else {
			_IMG_DATA[i] = 0;
		}
	}
}


	/* 
		Find Edges : Loop on every pixel of the image 
					 and apply the Sobel Filter to find the edges
	*/

void GrayLevelImage2D::FindEdges () {

	std::cout << "Resolving Edges." << std::endl;

	Container nData = _IMG_DATA;

	for (int i = 0; i < _IMG_WIDTH; i++) {
		for (int j = 0; j < _IMG_HEIGHT; j++) {
			nData[Index(i, j)] = Evaluate(Index(i,j));
		}
	}
	_IMG_DATA = nData;
}


	/*
		Evaluate : Apply Sobel-Feldman Operator to each pixel of the image
		@i       : Pixel index where the filter is applied
	*/

int GrayLevelImage2D::Evaluate(int i) {

	int valueGradX = 0;
	int valueGradY = 0;

	int neighbours[] = {i-_IMG_HEIGHT-1, i-_IMG_HEIGHT, i+_IMG_HEIGHT+1, i-1, i+1, i+_IMG_HEIGHT-1, i+_IMG_HEIGHT, i+_IMG_HEIGHT+1};

	for (int k = 0; k < 8; k++) {
		/* If the neighbour exists in the image */
		if (neighbours[k] > -1 && neighbours[k] < _IMG_DATA.size() - 1) {
			if (k == 0) {
				valueGradX -= 3 * _IMG_DATA[neighbours[k]];
				valueGradY -= 3 * _IMG_DATA[neighbours[k]];
			} else if (k == 1) {
				valueGradY -= 10 * _IMG_DATA[neighbours[k]];
			} else if (k == 2) {
				valueGradX += 3 * _IMG_DATA[neighbours[k]];
				valueGradY -= 3 * _IMG_DATA[neighbours[k]];
			} else if (k == 3) {
				valueGradX -= 10 * _IMG_DATA[neighbours[k]];
			} else if (k == 4) {
				valueGradX += 10 * _IMG_DATA[neighbours[k]];
			} else if (k == 5) {
				valueGradX -= 3 * _IMG_DATA[neighbours[k]];
				valueGradY += 3 * _IMG_DATA[neighbours[k]];
			} else if (k == 6) {
				valueGradY += 10 * _IMG_DATA[neighbours[k]];
			} else if (k == 7) {
				valueGradX += 3 * _IMG_DATA[neighbours[k]];
				valueGradY += 3 * _IMG_DATA[neighbours[k]];
			}
 		}
	}
	return sqrt(valueGradX * valueGradX + valueGradY * valueGradY);
}


	/*
		Hough Transform : find circles in an images with a radius between minRadius and maxRadius
		@minRadius      : Minimum permited radius of the circles, to avoid false detections
		@maxRaidus      : Maximum permited radius of the circles
		@minNumber      : Minimum number of intersections to find a circle's center
		@name           : Name of the image file saved after processing
	*/

void GrayLevelImage2D::HoughTransform (int minRadius, int maxRadius, std::string name, int zoom) {

	std::cout << "Hough Transform Processing." << std::endl;


	Container circlesCenter = Clean(_IMG_DATA.size());
	Container circlesRadius = Clean(_IMG_DATA.size());
	Container circlesFilled = Clean(_IMG_DATA.size());
	Container circleDrawContainer = Clean(_IMG_DATA.size());


	for (int k = minRadius; k < maxRadius+1; k++) {

		std::cout << "	Current Radius being processed: " << k << "." << std::endl;

		Container circleDetectionContainer = Clean(_IMG_DATA.size());

			/* Circle detection */
		for (int j = _IMG_WIDTH; j < _IMG_DATA.size()-_IMG_WIDTH; j++) {
			if (_IMG_DATA[j] > 254) {
				circleDetectionContainer = DetectCircle(k, j, circleDetectionContainer);
			}
		}
		ConvertToPNG(name+"-_-detected", circleDetectionContainer);


			/* Search minimum intensity in the image */
		int minNumber = 255;
		for (int i = 0; i < circleDetectionContainer.size(); i++) {
			if (circleDetectionContainer[i] < minNumber && circleDetectionContainer[i] > -1) {
				minNumber = circleDetectionContainer[i];
			}
		}
		minNumber += 20;

			/* Search circles' center */
		for (int i = 0; i < _IMG_DATA.size(); i++) {
			if (circleDetectionContainer[i] >= minNumber) {
				circleDetectionContainer[i] = 255;
			}
		}

			/* Resolving center accumulation */
		circleDetectionContainer = FindLocalMinimum(circleDetectionContainer);

			/* Assign each center with a radius */
		for (int i = 0; i < _IMG_DATA.size(); i++) {
			bool noOverlap = CheckOverlaping(i, k, circlesFilled);
			if (circleDetectionContainer[i] < 1 && circlesCenter[i] > 254 && circlesFilled[i] > 254 && noOverlap) {
				circlesFilled = DrawFilledCircle(k, i, circlesFilled);
				circlesCenter[i] = 0; // Set center on the center map
				circlesRadius[i] = k; // Set the radius
			}
		}
		ConvertToPNG(name+"-fibers", circlesFilled);
	}

		/* Export data file for meshing */
	ExportFibers(name+"-circles", circlesCenter, circlesRadius, _IMG_WIDTH, _IMG_HEIGHT, zoom);

		/* Draw a circle with assign radius when there's a center */
	for (int i = 0; i < _IMG_DATA.size(); i++) {
		if (circlesCenter[i] < 1) {
			circleDrawContainer = DrawCircle(circlesRadius[i], i, circleDrawContainer); 
		}
	}
	ConvertToPNG(name+"-_-circles", circleDrawContainer);
	ConvertToPNG(name+"-drawed", circlesFilled);
	float newFVol = FVol(circlesFilled);
	std::cout << "New Volumic Fraction: " << newFVol * 100 << " % of fiber." << std::endl;
	std::string cmd("open " + name + "-_-circles.png");
	system(cmd.c_str());
}


GrayLevelImage2D::Container GrayLevelImage2D::Clean (int dataSize) {
	Container data;
	for (int i = 0; i < dataSize; i++) {
		data.push_back(255);
	}
	return data;
}


GrayLevelImage2D::Container GrayLevelImage2D::DetectCircle (int radius, int pixelIndex, Container data) {

	int x = 0;
	int x_centre = GetX(pixelIndex);
	int y_centre = GetY(pixelIndex); 
	int m = 5 - 4 * radius;
	while(x <= radius) {           // tant qu'on est dans le second octant
		data[Index(x + x_centre, radius + y_centre)] -= 1;
		data[Index(radius + x_centre, x + y_centre)] -= 1;
		data[Index(-x + x_centre, radius + y_centre)] -= 1;
		data[Index(-radius + x_centre, x + y_centre)] -= 1;
		data[Index(x + x_centre, -radius + y_centre)] -= 1;
		data[Index(radius + x_centre, -x + y_centre)] -= 1;
		data[Index(-x + x_centre, - radius + y_centre)] -= 1;
		data[Index(-radius + x_centre, -x + y_centre)] -= 1;
		if (m > 0) {
			radius -= 1 ;
			m -= 8 * radius ;
		}
		x += 1;
		m += 8 * x + 4;
	}
	return data;
}


GrayLevelImage2D::Container GrayLevelImage2D::DrawCircle (int radius, int pixelIndex, Container data) {

	int x = 0;
	int x_centre = GetX(pixelIndex);
	int y_centre = GetY(pixelIndex); 
	int m = 5 - 4 * radius;
	while(x <= radius) {           // tant qu'on est dans le second octant
		data[Index(x + x_centre, radius + y_centre)] = 0;
		data[Index(radius + x_centre, x + y_centre)] = 0;
		data[Index(-x + x_centre, radius + y_centre)] = 0;
		data[Index(-radius + x_centre, x + y_centre)] = 0;
		data[Index(x + x_centre, -radius + y_centre)] = 0;
		data[Index(radius + x_centre, -x + y_centre)] = 0;
		data[Index(-x + x_centre, - radius + y_centre)] = 0;
		data[Index(-radius + x_centre, -x + y_centre)] = 0;
		if (m > 0) {
			radius -= 1 ;
			m -= 8 * radius ;
		}
		x += 1;
		m += 8 * x + 4;
	}
	return data;
}


GrayLevelImage2D::Container GrayLevelImage2D::DrawFilledCircle (int radius, int pixelIndex, Container data) {

	for(int y = -radius; y <= radius; y++) {
    	for(int x = -radius; x <= radius; x++) {
        	if(x * x + y * y <= radius * radius) {
        		int index = Index(GetX(pixelIndex) + x, GetY(pixelIndex) + y);
            	data[index] = 0;
        	}
    	}
	}
	return data;
}


GrayLevelImage2D::Container GrayLevelImage2D::FindLocalMinimum (Container data) {

	for (int i = 0; i < data.size(); i++) {

		if (data[i] < 255) {

			int neighbours[] = {i-2*_IMG_HEIGHT-2, i-2*_IMG_HEIGHT-1, i-2*_IMG_HEIGHT, i-2*_IMG_HEIGHT+1, i-2*_IMG_HEIGHT+2, 
								i-_IMG_HEIGHT-2  , i-_IMG_HEIGHT-1  , i-_IMG_HEIGHT  , i-_IMG_HEIGHT+1  , i-_IMG_HEIGHT+2  ,
								i-2              , i-1                               , i+1              , i+2              ,
								i+_IMG_HEIGHT-2  , i+_IMG_HEIGHT-1  , i+_IMG_HEIGHT  , i+_IMG_HEIGHT+1  , i+_IMG_HEIGHT+2  ,
								i+2*_IMG_HEIGHT-2, i+2*_IMG_HEIGHT-1, i+2*_IMG_HEIGHT, i+2*_IMG_HEIGHT+1, i+2*_IMG_HEIGHT+2};
			bool hasNeighbours = false;
			for (int k = 0; k < 24; k++) {
			/* If the neighbour exists in the image and it has a lower value*/
				if (neighbours[k] > -1 && neighbours[k] < data.size() - 1) {
					if (data[neighbours[k]] <= data[i]) {
						data[i] = 255; // The pixel is not a center
						hasNeighbours = true;
					}
				}
			}
			if (!hasNeighbours) data[i] = 0;
		}
	}
	return data;
}


bool GrayLevelImage2D::CheckOverlaping (int pixelIndex, int radius, Container data) {

	for(int y = -radius; y <= radius; y++) {
    	for(int x = -radius; x <= radius; x++) {
    		int index = Index(GetX(pixelIndex) + x, GetY(pixelIndex) + y);
        	if(x * x + y * y <= radius * radius && data[index] == 0) {
        		return false;
        	}
    	}
	}

	return true;
}


			/* ********************************************** */	
			/* Fonctions relatives aux informations physiques */
			/* ********************************************** */


float GrayLevelImage2D::FVol (Container data) {
	int fvol = 0;
	for (int i = 0; i < data.size(); i++) {
		if (data[i] < 1) fvol++;
	}
	return (float)fvol/data.size();
}

float GrayLevelImage2D::MixtureRule (float fvol, float youngMat, float youngFiber) {
	float youngLat = fvol * youngMat + (1-fvol) * youngFiber;
	std::cout << "Mixture Rule: E_L = " << std::to_string(youngLat) << std::endl;
	//float youngTrans = 
	return 0.;
}