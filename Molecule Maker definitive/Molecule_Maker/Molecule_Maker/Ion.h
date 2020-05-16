#pragma once
#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>

using namespace std;

class Ion {
	protected:
		string abbreviation = "";
		short charge = 0;

	public:
		Ion() {};
		Ion(string, short);

		string getAbbreviation() { return abbreviation; }
		short getCharge() { return charge; }

		void addAtom(string, short, string = "");

		bool operator==(Ion);
};