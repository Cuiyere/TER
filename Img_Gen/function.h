/* function.h
	Bierlaire Arthur
	22.10.18

	Déclaration des fonctions de traitement d'image
*/

#include<fstream>

// Fonction de traitement pour un cercle
void DrawCircle (std::ofstream& img, int height, int width, int radius);
bool IsInCirle (int i, int j, int radius, int xCen, int yCen);

// Fonction de dessin de base
void DrawGradient (std::ofstream& img, int height, int width);