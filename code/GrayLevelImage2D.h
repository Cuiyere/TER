    /* 

      GrayLevelImage.h - Librairie de traitement d'image
      TER 2018 - 2019 
      Arthur BIERLAIRE - Roxane DARVENNE - Benjamin SACCO - Vincent GISBERT

      "Etude des matériaux composites à l'échelle des torons"
       
    */

#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class GrayLevelImage2D {

public:
  // ------ Définition des types personnalisés ------
  typedef int GrayLevel; // le type pour les niveaux de gris.
  typedef std::vector<GrayLevel> Container; // le type pour stocker les niveaux de gris de l'image.

  // ------ Fonctions pour le traitement d'image ------
  GrayLevelImage2D(){}; // Constructeur par défaut
  GrayLevelImage2D (std::string name); // Constructeur utilisé ici
  GrayLevelImage2D (int w, int h, GrayLevel g = 0); // Constructeur avec taille w x h. Remplit tout avec la valeur g (par défaut 0).

  // ------ Import et export en fichier image ------
  bool ImportPGM(std::istream & input);
  bool ExportPGM(std::ostream & output, Container data) const;
  void ExportFibers(std::string name, Container circlesCenter, Container circlesRadius, int width, int height, int zoom);
  void ConvertToPNG(std::string name, Container data);

  // ------ Fonctions pour le traitement d'image ------
  void GetHistogram (std::string name);
  void EqualizeHistogram (std::string name);
  void LevelImage (int threshold);
  void FindEdges (); // Cherche les bords dans l'image
  int Evaluate (int i); // Applique un filtre de Sobel
  void HoughTransform (int minRadius, int maxRadius, std::string name, int zoom); // Cherche les cerlces présents dans l'image
  Container Clean (int dataSize);
  Container DetectCircle (int radius, int pixelIndex, Container data); // Trace un cercle au pixel d'indice pixelIndex
  Container DrawCircle (int radius, int pixelIndex, Container data); // Trace un cercle au pixel d'indice pixelIndex
  Container DrawFilledCircle (int radius, int pixelIndex, Container data); // Trace un cercle rempli au pixel d'indice pixelIndex
  Container FindLocalMinimum (Container data);
  bool CheckOverlaping (int pixelIndex, int radius, Container data);
  // ------ Fonctions pour extraire des informations physiques ------
  float FVol (Container data); // Retourne la fraction volumique pour un niveau donné
  float MixtureRule (float fvol, float youngMat, float youngFiber); // Calcule les modules d'Young L et T en théorie par la loi des mélanges


  // ------ Getters ------
  int GetWidth () {return _IMG_WIDTH;};
  int GetHeight () {return _IMG_HEIGHT;};
  int GetSize () {return _IMG_DATA.size();};
  Container GetData () {return _IMG_DATA;};
  void SetData (Container data) {_IMG_DATA = data;};
  GrayLevel At(int i, int j) const {return _IMG_DATA.at(Index(i, j));};

private:
  Container           _IMG_DATA;        // pixels de l'image
  std::string         _IMG_NAME;        // nom de l'image
  int                 _IMG_WIDTH;       // largeur de l'image
  int                 _IMG_HEIGHT;      // hauteur de l'image
  std::vector<int>    _IMG_HISTOGRAM;   // histogramme de l'image
  std::vector<int>    _IMG_AHISTOGRAM;  // histogramme accumulé

  int Index (int x, int y) const {return x + _IMG_WIDTH * y;};
  int GetX (int pixelIndex) const {return pixelIndex - _IMG_WIDTH * GetY(pixelIndex);};
  int GetY (int pixelIndex) const {return pixelIndex / _IMG_WIDTH;};

    /* Split a line with a delimiter */
  std::vector<int> SplitLine(const std::string & line, char delimiter) {
    std::vector<int> tokens;
    std::string token;
    std::istringstream tokenStream(line);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(stoi(token));
    }
    return tokens;
  }
};