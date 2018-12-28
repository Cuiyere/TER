#pragma once

#include <vector>
#include <string>

class Datafile {
private:
	int _width, _height;
	std::vector< std::vector<int> > _pixels;

	/* Methods */
	std::vector<int> SplitLine(const std::string & line, char delimiter);
	int Evaluate(int i, int j);

public:
	Datafile() : _pixels(0) {};
	~Datafile(){};

	/* Methods */
	void Readfile(const std::string & imgPath);
	void PixelsToImg ();
	void PixelsToEdges ();

	/* Getters */
	std::vector< std::vector<int> > GetPixlelGrid () {return _pixels;};
};