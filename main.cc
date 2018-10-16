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

	system(("mkdir -p ./Desktop/eclipse-workspace/PicturesTER/Images"));
	ofstream flux;
	string filename = "./Desktop/eclipse-workspace/PicturesTER/Images/picture1.bmp";
	flux.open(filename, ios::out);

	if (flux) { cout << "Bonjour, je suis une image." << endl; }
	else { cout << "Bite" << endl; }

	flux.close();
	return 0;
}
