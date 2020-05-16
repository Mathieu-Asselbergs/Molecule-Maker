#include "Ion.h"

Ion::Ion(string abbreviation, short charge) {
	this->abbreviation = abbreviation;
	this->charge = charge;
}

void Ion::addAtom(string abbreviation, short charge, string fileName) {
	// If the atom has no charge, tell the user it cannot be added
	if (charge == 0) {
		cout << "An atom without a charge cannot be added to an ion." << endl << endl;

		// AskForEnter from MoleculeMaker.cpp
		cout << endl << "Press Enter to continue..." << endl;
		while (_getch() != 13);
		system("cls");
	}

	else {
		string newAbbreviation;

		// If the atom is already included in the ion, look for it and increase its occurrance number
		if (this->abbreviation.find(abbreviation) != -1 && this->abbreviation[this->abbreviation.find(abbreviation) + abbreviation.length()] <= 'Z') {
			short numIndex = this->abbreviation.find(abbreviation) + abbreviation.length();
			newAbbreviation = this->abbreviation.substr(0, numIndex);

			if (this->abbreviation[this->abbreviation.find(abbreviation) + abbreviation.length()] > '0' && this->abbreviation[this->abbreviation.find(abbreviation) + abbreviation.length()] < '9') {
				newAbbreviation += to_string(this->abbreviation[numIndex] - '0' + 1);
				newAbbreviation += this->abbreviation.substr(numIndex + 1);
			}
			else {
				newAbbreviation += "2";
				newAbbreviation += this->abbreviation.substr(numIndex);
			}

		}

		// If the atom is not yet in the ion, add it, while upholding an alphabetical order
		else {
			short atomIndex = -1;
			short specificAtomIndex = -1;
			short firstBound, lastBound;

			for (int i = abbreviation[0]; i <= 'Z'; i++) {
				if (this->abbreviation.find(i) != -1) {
					firstBound = this->abbreviation.find(i);

					if (i != abbreviation[0] || (i == abbreviation[0] && abbreviation.length() == 1)) {
						atomIndex = firstBound;
					}
					else
					{
						for (int x = i + 1; x <= 'Z'; x++) {
							if (this->abbreviation.find(x) != -1) {
								lastBound = this->abbreviation.find(x);
							}

							else if (x == 'Z') {
								lastBound = this->abbreviation.length();
							}
						}
					}

					break;
				}

				else if (i == 'Z') {
					atomIndex = this->abbreviation.length();
				}
			}

			if (atomIndex == -1) {
				if (abbreviation.length() == 1) {
					atomIndex = firstBound;
				}
				else {
					string range = this->abbreviation.substr(firstBound, lastBound - firstBound);

					for (int i = abbreviation[1]; i <= 'z'; i++) {
						if (range.find(i) != -1) {
							specificAtomIndex = range.find(i) - 1;

							break;
						}

						else if (i == 'z') {
							specificAtomIndex = range.length();
						}
					}

					atomIndex = firstBound + specificAtomIndex;
				}
			}

			newAbbreviation = this->abbreviation.substr(0, atomIndex);
			newAbbreviation += abbreviation;
			if (atomIndex != this->abbreviation.length()) {
				newAbbreviation += this->abbreviation.substr(atomIndex);
			}
		}

		this->abbreviation = newAbbreviation;
		this->charge += charge;

		if (charge == 0 && fileName != "") {
			fstream moleculesFile(fileName, ios::out | ios::ate);
			moleculesFile << this->abbreviation;
			moleculesFile.close();
		}
	}
}

bool Ion::operator==(Ion comparedIon) {
	if (this->abbreviation == comparedIon.getAbbreviation() && this->charge == comparedIon.getCharge()) {
		return true;
	}
	
	return false;
}