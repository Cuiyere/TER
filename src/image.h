/*
 * image.h
 *
 *  Created on: Oct 15, 2018
 *      Author: arthur
 */
#include<vector>

class Image {
	public:
		Image(int width, int length);
		virtual ~Image();
		SetPixel(int i, int j, std::vector<int> rgbValues);


	private:
		int _width, _length;
		std::vector< std::vector< std::vector<int> > > _pixels; // Grille de vecteurs de taille 3 qui représentent les pixels
};
