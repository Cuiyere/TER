/*
 - main.c -
 15.10.2018
 Arthur Bierlaire
 */
#include<fstream>
#include<iostream>
#include<string>
using namespace std;

int main () {

	//system(("mkdir -p .//Images"));
	ofstream flux;
	string filename = "picture.ppm";
	flux.open(filename, ios::out);

	if (flux) { cout << "Bonjour, je suis une image." << endl; }
	else { cout << "Bite" << endl; }

	flux.close();
	return 0;
}
