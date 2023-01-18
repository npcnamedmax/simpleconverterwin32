#pragma once
#include "windows.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct result {
	long int decimal = 0;
	string hexa = "";
	string binary = "";
	bool invInput = false; //invalid input
	wstring concatenate();
};

long pow(int x, int y);

long hextodec(char hex);

string hextobin(char hex);

string dectobin(long d);

string bintohex(string s);

result engine( string s, string type);



