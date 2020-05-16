#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
#include <limits>
#include <vector>
#include "ion.h"

using namespace std;

vector<string> elementProperties;

vector<string> posChargeElements = { "", "", "", "", "", "", "" };
vector<string> negChargeElements = { "", "", "", "" };

// Find and return the index of occurrance of a value in a vector
template<typename T>
int indexInVector(vector<T>& haystack, T needle) {
	auto iterator = find(haystack.begin(), haystack.end(), needle);

	if (iterator == haystack.end()) {
		return -1;
	}
	else {
		return distance(haystack.begin(), iterator);
	}
}

// Get a specific line from a text file
string getLineN(string fileName, int lineNumber) {
	ifstream file(fileName);

	if (!file.is_open()) {
		return "--Error--";
	}

	char line[50];

	for (int i = 0; i < lineNumber; i++) {
		file.getline(line, 50);
	}

	file >> line;
	return line;
}

// Ask for the user to hit Enter
void AskForEnter() {
	cout << endl << "Press Enter to continue..." << endl;
	while (_getch() != 13);
	system("cls");
}

// Check if cin.rdstate = 0. Clear it if it's not the case
bool GoodReadstate() {
	if (cin.rdstate() != 0) {
		cin.clear();
		cin.ignore(20, '\n');
		system("cls");

		return false;
	}
	return true;
}

// Count the amount of occurances of a given string in a string
short CountOccurrances(string haystack, string needle) {
	short occurrances = 0;
	while (haystack.find(needle) != -1) {
		occurrances++;
		haystack = haystack.substr(haystack.find(needle) + 1);
	}
	return occurrances;
}

// Ask for an input of number types or chars
template<typename T>
T AskInput(string request, T lowBound = numeric_limits<T>::min(), T highBound = numeric_limits<T>::max(), vector<T> badValues = {}) {
	T userInput = NULL;

	do {
		cout << request << endl << endl;
		cin >> userInput;

		system("cls");
	} while (!GoodReadstate() || userInput < lowBound || userInput > highBound || indexInVector(badValues, userInput) != -1);

	return userInput;
}

// Checks if a given string of charges meets the syntax criteria
bool CheckCharges(string charges) {
	cin.ignore(50, '\n');

	if (charges.length() % 2 == 0) {
		if (charges == "0:") {
			return true;
		}

		else {
			do {
				if (charges[0] < '1' || charges[0] > '7') {
					return false;
				}
				if (charges[1] != ':') {
					return false;
				}

				charges = charges.substr(2);
			} while (charges.length() > 0);

			return true;
		}
	}

	return false;
}

// Ask for a specific input for setting the charges
string ChargeInput(bool positiveCharge) {
	string userInput;
	string charge = positiveCharge ? "positive" : "negative";

	do {
		cout << "Please give the " << charge << " charges for the element using the following syntax." << endl << endl
			<< "Charge1:Char...:ChargeN:   -   Example  2:3:5:" << endl << endl
			<< "It is not permitted to use signs. Give just the size of the charges, devided by colons." << endl
			<< "Input \"0:\" if there are no " << charge << " charges are applicable." << endl << endl;
		cin >> userInput;

		system("cls");
	} while (!CheckCharges(userInput));

	return userInput;
}

// Set all existing element files back to the default: All files, if existing, are set to hold all properties up to the amount of protons
void ResetFiles(bool verify = true) {
	// Reset the main element file
	fstream mainFile("./Elements/Z0-Element.txt", ios::out | ios::trunc);

	fstream elementFile;
	short amountOfElementFiles = 0;

	for (int i = 0; i < 118; i++) {
		elementFile.open("./Elements/Z" + to_string(i + 1) + ".txt");
		if (elementFile.is_open()) {
			amountOfElementFiles++;
		}
		elementFile.close();
	}

	elementProperties.clear();

	mainFile << to_string(amountOfElementFiles);					elementProperties.push_back(to_string(amountOfElementFiles));
	mainFile << endl << "Name";										elementProperties.push_back("Name");
	mainFile << endl << "Abbreviation";								elementProperties.push_back("Abbreviation");
	mainFile << endl << "Positive Charges";							elementProperties.push_back("Positive charges");
	mainFile << endl << "Negative Charges";							elementProperties.push_back("Negative charges");
	mainFile << endl << "Protons";									elementProperties.push_back("Protons");

	mainFile.close();


	// Reset all specific element files
	string mainProperties[5];

	for (int i = 0; i < 118; i++) {
		fstream elementFile("./Elements/Z" + to_string(i + 1) + ".txt", ios::in | ios::out);
		if (elementFile.is_open()) {
			for (int x = 0; x < 5; x++) {
				elementFile >> mainProperties[x];
			}
			elementFile.close();

			elementFile.open("./Elements/Z" + to_string(i + 1) + ".txt", ios::out | ios::trunc);

			elementFile << mainProperties[0];
			for (int x = 0; x < 4; x++) {
				elementFile << endl << mainProperties[x + 1];
			}

			elementFile.close();
		}
		else {
			continue;
		}
	}

	if (verify) {
		cout << "All element files have been reset!" << endl;
		AskForEnter();
	}
}

// Fill the elementProperties vector with all the information from the main element file
void GetElementProperties() {
	ifstream mainFile("./Elements/Z0-Element.txt");

	if (!mainFile.is_open()) {
		mainFile.close();
		ResetFiles(false);
		mainFile.open("./Elements/Z0-Element.txt");
	}

	char property[30];

	// Load all existant element properties into the elementProperties vector
	do {
		mainFile.getline(property, 30);
		elementProperties.push_back(property);
	} while (!mainFile.eof());

	mainFile.close();


	string positiveCharges, negativeCharges;

	// Load a reference to all relevant charges into their respective charge vector
	for (short i = 1; i < 119; i++) {

		positiveCharges = getLineN("./Elements/Z" + to_string(i) + ".txt", 2);
		negativeCharges = getLineN("./Elements/Z" + to_string(i) + ".txt", 3);

		if (positiveCharges != "--Error--" && positiveCharges != "0:") {
			for (short x = 0; x < positiveCharges.length(); x += 2) {
				short charge = positiveCharges[x] - '1';
				posChargeElements[charge] += to_string(i) + ":";
			}
		}

		if (negativeCharges != "--Error--" && negativeCharges != "0:") {
			for (short x = 0; x < negativeCharges.length(); x += 2) {
				short charge = negativeCharges[x] - '1';
				negChargeElements[charge] += to_string(i) + ":";
			}
		}
	}
}

// Print all possible properties for an element to the screen. The default shows only the properties that can be removed
void displayProperties(int firstValue = 6, string elementFile = "") {
	ifstream element(elementFile);

	if (element.is_open()) {
		char value[30];
		for (int i = 0; i < elementProperties.size(); i++) {
			if (i != 0) {
				element.getline(value, 30);
				cout << elementProperties[i] << ": " << value << endl;
			}
		}
	}
	else {
		for (int i = 0; i < elementProperties.size(); i++) {
			if (i > firstValue - 1) {
				cout << elementProperties[i] << endl;
			}
		}
	}
}

// Replace a value in an element file
void replaceValue(string fileName, int line, string newValue) {
	fstream elementFile(fileName, ios::in);
	char temporary[30];
	string newFile;
	bool firstValue = true;

	for (int i = 0; i < line; i++) {
		elementFile.getline(temporary, 30);
		if (!firstValue) {
			newFile += "\n";
		}
		newFile += temporary;
		firstValue = false;
	}

	if (!firstValue) {
		newFile += "\n";
	}
	else {
		firstValue = false;
	}
	newFile += newValue;

	elementFile.getline(temporary, 30);

	while (!elementFile.eof()) {
		elementFile.getline(temporary, 30);
		newFile += "\n";
		newFile += temporary;
	}

	elementFile.close();
	elementFile.open(fileName, ios::out | ios::trunc);
	elementFile << newFile;
	elementFile.close();
}

// Add a property to the main element file and all specific element files
void AddProperty(string propertyName) {
	// Check whether the new property isn't already in the list of propeties
	for (auto& property : elementProperties) {
		if (property == propertyName) {
			cout << "The property \"" + propertyName + "\" was already in the file!" << endl;
			AskForEnter();
			return;
		}
	}

	// Add the new property to the main element file and the property vector
	fstream generalElement("./Elements/Z0-Element.txt", ios::in | ios::out | ios::ate);
	generalElement << endl << propertyName;
	cout << "The new property \"" + propertyName + "\" has been added to the list of properties!" << endl;
	generalElement.close();
	elementProperties.push_back(propertyName);

	// Add a value for the new property to every element file
	fstream elementFile;
	string valueForProperty;

	for (int i = 0; i < 118; i++) {
		elementFile.open("./Elements/Z" + to_string(i + 1) + ".txt", ios::in | ios::out | ios::ate);
		if (elementFile.is_open()) {
			system("cls");
			cout << "Please insert a value for the " + propertyName + " for element " + to_string(i + 1) << endl << endl << propertyName + ": ";
			cin >> valueForProperty;
			elementFile << endl << valueForProperty;
		}

		elementFile.close();
	}

	AskForEnter();
}

// Remove a property from the main element file and all specific element files
void RemoveProperty(string propertyName) {
	string property;
	for (int i = 0; i < elementProperties.size(); i++) {
		property = elementProperties[i];
		if (propertyName == property) {
			if (i < 6) {
				system("cls");
				cout << "This property is protected. It cannot be removed." << endl;
				AskForEnter();
				return;
			}

			fstream mainFile;

			// Remove the property from the specific element files
			for (int x = 0; x < 118; x++) {
				mainFile.open("./Elements/Z" + to_string(x + 1) + ".txt", ios::in | ios::out);
				if (mainFile.is_open()) {
					string updatedFile, nextProperty;
					short currentProperty = 0;
					do {
						if (currentProperty == i - 1) {
							mainFile >> nextProperty;
						}
						else if (currentProperty == 0) {
							mainFile >> updatedFile;
						}
						else {
							mainFile >> nextProperty;
							updatedFile += "\n" + nextProperty;
						}

						currentProperty++;
					} while (!mainFile.eof());

					mainFile.close();

					mainFile.open("./Elements/Z" + to_string(x + 1) + ".txt", ios::in | ios::out | ios::trunc);
					mainFile << updatedFile;
					mainFile.close();
				}
			}

			// Remove the property from the vector
			elementProperties.erase(elementProperties.begin() + i);

			// Remove the property from the main element file
			mainFile.open("./Elements/Z0-Element.txt", ios::out | ios::trunc);
			for (string& propertyInVector : elementProperties) {
				if (propertyInVector == elementProperties[0]) {
					mainFile << propertyInVector;
				}
				else {
					mainFile << endl << propertyInVector;
				}
			}
			mainFile.close();

			system("cls");
			cout << "The property \"" + propertyName + "\" has been removed from the list of properties." << endl;
			AskForEnter();
			return;
		}
	}

	system("cls");
	cout << "The property \"" + propertyName + "\" was not found in the list of properties." << endl;
	AskForEnter();
}

// Add a new element file to the directory
void AddElement() {
	short protons = AskInput("Please give the amount of protons of the element.", 1, 118);

	system("cls");

	// Check if the element file already exists
	ifstream testElement("./Elements/Z" + to_string(protons) + ".txt");
	if (testElement.is_open()) {
		cout << "This element is already in the database." << endl;
		testElement.close();
		AskForEnter();
	}
	else {
		// Create and fill a file for the new element by asking a value for every property in the main file
		ofstream newElement("./Elements/Z" + to_string(protons) + ".txt");
		string valueForProperty;

		for (string& property : elementProperties) {
			if (property == elementProperties[0]) {
				continue;
			}
			else if (property == "Protons") {
				newElement << endl << protons;
				continue;
			}
			else if (property == "Positive Charges") {
				newElement << endl << ChargeInput(1);
				continue;
			}
			else if (property == "Negative Charges") {
				newElement << endl << ChargeInput(0);
				continue;
			}

			cout << "Please give the " + property + " of the element." << endl << endl << property + ": ";
			cin >> valueForProperty;
			if (property == "Name") {
				newElement << valueForProperty;
			}
			else {
				newElement << endl << valueForProperty;
			}

			system("cls");
		}

		newElement.close();
	}

	// Update the amount of element files in the main element file
	elementProperties[0] = to_string(stoi(elementProperties[0]) + 1);
	replaceValue("./Elements/Z0-Element.txt", 0, elementProperties[0]);
}

// Remove a specific element file
void RemoveElement() {
	int elementNum = AskInput("What is the elemental number of the element you wish to remove?", 1, 118);
	string fileName = "./Elements/Z" + to_string(elementNum) + ".txt";

	fstream elementFile(fileName);

	if (elementFile.is_open()) {
		elementFile.close();
		remove(fileName.c_str());
		elementProperties[0] = to_string(stoi(elementProperties[0]) - 1);
		replaceValue("./Elements/Z0-Element.txt", 0, elementProperties[0]);

		cout << "The file has been succesfulle deleted." << endl << endl;
		AskForEnter();
	}
	else {
		cout << "This element has not been defined yet." << endl << endl;
		AskForEnter();
	}
}

// Alter one of the element's values
void ChangeElementValue(string elementFileName) {
	char PropertyToChange[30];
	string property;
	int indexOfProperty = 0;

	// Ask for the property whose value we should alter
	do {
		displayProperties(1, elementFileName);
		cout << endl << "What property would you like to change?" << endl << endl;
		cin.getline(PropertyToChange, 30);
		property = PropertyToChange;

		indexOfProperty = indexInVector(elementProperties, property);

		system("cls");
	} while (indexOfProperty == -1 || indexOfProperty == 0);

	string newValue;

	// Display the name and the property that is about to be changed
	cout << elementProperties[indexOfProperty] << endl << endl;
	cout << "Old value:" << endl << "   " << getLineN(elementFileName, indexOfProperty - 1) << endl << endl;
	cout << "New value:" << endl << "   ";

	cin >> newValue;

	replaceValue(elementFileName, indexOfProperty - 1, newValue);

	system("cls");
	cout << "The value was succesfully updated!" << endl << endl;
	AskForEnter();
}

// View and change the information of an element
void viewElement(short elementNumber) {
	system("cls");

	ifstream elementFile("./Elements/Z" + to_string(elementNumber) + ".txt");
	string propertyValue;

	if (elementFile.is_open()) {
		// Display every property with its corresponding value
		displayProperties(1, "./Elements/Z" + to_string(elementNumber) + ".txt");

		// Show a menu with new options
		cout << endl << "What would you like to do?" << endl << endl;

		cout << "1. Change a value" << endl;
		cout << "2. Add a property" << endl;
		cout << "3. Remove a property" << endl;
		cout << "4. Nothing" << endl;

		string propertyName;
		switch (_getch()) {
		case'1':
			system("cls");
			ChangeElementValue("./Elements/Z" + to_string(elementNumber) + ".txt");
			break;

		case'2':
			system("cls");
			if (elementProperties.size() > 6) {
				cout << "Changeable values:" << endl;
				displayProperties();
				cout << endl;
			}
			cout << "What is the name of the new property?" << endl << endl;
			cin >> propertyName;
			AddProperty(propertyName);
			break;

		case'3':
			system("cls");
			if (elementProperties.size() > 6) {
				cout << "Changeable values:" << endl;
				displayProperties();
				cout << endl << "What is the name of the property to remove?" << endl << endl;
				cin >> propertyName;
				RemoveProperty(propertyName);
			}
			else {
				cout << "There are no properties that can be removed." << endl;
				AskForEnter();
			}
			break;

		case'4':
			break;
		}

		elementFile.close();
	}
	else {
		cout << "This element has not been defined yet." << endl;
		AskForEnter();
		return;
	}
}

// Return type for getAtom including two different data types
template<typename T1, typename T2>
struct DataPair {
	T1 firstValue;
	T2 secondValue;
};

// Get a specific charge of a chosen atom
DataPair<string, short> getAtom(string addition = "Enter 0 to return to the main menu.") {
	DataPair<string, short> atomData;
	string elementFileName = "./Elements/Z" + to_string(AskInput("Please give the elemental number of the atom you wish to use.\n" + addition, 0, 118)) + ".txt";

	if (getLineN(elementFileName, 0) == "--Error--") {
		atomData.firstValue = "--Error--";
		atomData.secondValue = 348;
		return atomData;
	}

	atomData.firstValue = getLineN(elementFileName, 1);

	system("cls");

	string positiveCharges = getLineN(elementFileName, 2);
	string negativeCharges = getLineN(elementFileName, 3);

	vector<short> charges;

	for (int i = 0; i < positiveCharges.length(); i += 2) {
		charges.push_back(positiveCharges[i] - '0');
	}
	for (int i = 0; i < negativeCharges.length(); i += 2) {
		charges.push_back(-1 * (negativeCharges[i] - '0'));
	}

	if (charges.size() == 1) {
		atomData.secondValue = charges[0];
		return atomData;
	}

	cout << "Please select the desired charge from the following:" << endl;
	for (int i = 0; i < charges.size(); i++) {
		cout << i + 1 << ".\t" << charges[i] << endl;
	}
	cout << endl;

	short choice = 0;
	do {
		choice = _getch() - '0';
	} while (choice < 1 || choice > charges.size());

	system("cls");
	atomData.secondValue = charges[choice - 1];
	return atomData;
}

// Create an ion to use in the molecule making process
Ion CreateIon(Ion currentIon, short chargeToMatch = 0, short maxNum = 0, fstream* moleculeFile = nullptr, vector<Ion>* possibleMolecules = nullptr) {
	// Create either a cation or an anion to use in the molecule making process
	if (chargeToMatch == 0 && possibleMolecules == nullptr) {
		DataPair<string, short> newAtom = getAtom();
		if (newAtom.secondValue == 348) {
			currentIon.addAtom(newAtom.firstValue, newAtom.secondValue);
			return currentIon;
		}

		currentIon.addAtom(newAtom.firstValue, newAtom.secondValue);

		do {
			newAtom = getAtom("Enter 0 to stop adding atoms.\nThe charge of the ion may not be 0.\n\n" + currentIon.getAbbreviation() + " | " + to_string(currentIon.getCharge()) + "\n");
			if (newAtom.secondValue != 348) {
				currentIon.addAtom(newAtom.firstValue, newAtom.secondValue);
			}
		} while (newAtom.secondValue != 348 || currentIon.getCharge() == 0);

		return currentIon;
	}

	// Complete previously created anion or cation and save all possibilities in a file
	else {
		for (int i = 0; i < abs(chargeToMatch); i++) {
			if (i < maxNum || int(chargeToMatch / maxNum) == 2) {
				string charges;
				if (chargeToMatch < 0) {
					charges = posChargeElements[_Min_value(chargeToMatch - i, int(maxNum)) - 1];
				}
				else {
					charges = negChargeElements[_Min_value(abs(chargeToMatch) - i, int(maxNum)) - 1];
				}

				string copy = charges;

				for (int x = 0; x < CountOccurrances(copy, ":"); x++) {
					Ion newIon = currentIon;

					if (chargeToMatch < 0) {
						newIon.addAtom(getLineN("./Elements/Z" + charges.substr(0, charges.find(":")) + ".txt", 1), -1 * chargeToMatch - i);
					}
					else {
						newIon.addAtom(getLineN("./Elements/Z" + charges.substr(0, charges.find(":")) + ".txt", 1), -1 * chargeToMatch + i);
					}

					if (charges.find(":") + 1 != charges.length()) {
						charges = charges.substr(charges.find(":") + 1);
					}

					if (newIon.getCharge() == 0) {
						if (indexInVector(*possibleMolecules, newIon) == -1) {
							*moleculeFile << newIon.getAbbreviation() << endl;
							possibleMolecules->push_back(newIon);
						}
					}
					else {
						CreateIon(newIon, newIon.getCharge(), abs(maxNum - i), moleculeFile, possibleMolecules);
					}
				}
			}
		}
		return currentIon;
	}
}

int main()
{
	// Fill the elementProperties vector
	GetElementProperties();

	// Show the menu including all possible operations
	while (true) {
		system("cls");
		cout << "What would you like to do?" << endl << endl;

		cout << "1. Add a property" << endl;
		cout << "2. Remove a property" << endl;
		cout << "3. Add an element" << endl;
		cout << "4. Remove an element" << endl;
		cout << "5. View / Change an element" << endl;
		cout << "6. Find possible molecules" << endl;
		cout << "7. Reset main element file" << endl << endl;

		string propertyName;
		switch (_getch()) {
		case'1':
			system("cls");
			if (elementProperties.size() > 6) {
				cout << "Changeable values:" << endl << endl;
				displayProperties(6);
				cout << endl;
			}
			cout << "What is the name of the new property?" << endl << endl;
			cin >> propertyName;
			AddProperty(propertyName);
			break;

		case'2':
			system("cls");
			if (elementProperties.size() > 6) {
				cout << "Changeable values:" << endl << endl;
				displayProperties(6);
				cout << endl << "What is the name of the property to remove?" << endl << endl;
				cin >> propertyName;
				RemoveProperty(propertyName);
			}
			else {
				cout << "There are no properties that can be removed." << endl;
				AskForEnter();
			}
			break;

		case'3':
			system("cls");
			AddElement();
			break;

		case'4':
			system("cls");
			RemoveElement();
			break;

		case'5':
			system("cls");
			viewElement(AskInput("Please give the atomic number of the element whose properties you want to see.", 1, 118));
			break;

		case'6':
			system("cls");
			{
				// Create the ion that will be completed later on
				Ion firstIon;

				firstIon = CreateIon(firstIon);
				if (firstIon.getCharge() == 348) {
					break;
				}
				cout << "The created ion has the following properties." << endl << endl << "Abbreviation: " << firstIon.getAbbreviation() << endl << "Charge: " << firstIon.getCharge() << endl << endl;
				AskForEnter();

				// Inform the user if this ion has already been used and send them back to the main menu if it is
				string fileName = "./Molecules/" + firstIon.getAbbreviation() + "_" + to_string(firstIon.getCharge()) + ".txt";

				fstream moleculeFile(fileName, ios::in);
				if (moleculeFile.is_open()) {
					moleculeFile.close();
					cout << "A list of possible molecules is already made for " + firstIon.getAbbreviation() + "." << endl;

					AskForEnter();

					break;
				}

				// If this ion has not been completed yet, set up a file for all possibilities and fill it
				else {
					moleculeFile.close();
					moleculeFile.open(fileName, ios::out);

					vector<Ion> possibleMolecules = {};

					CreateIon(firstIon, firstIon.getCharge(), abs(firstIon.getCharge()), &moleculeFile, &possibleMolecules);

					cout << "Every possible molecule using your ion has been saved in \"" + fileName + "\"." << endl << "This file can be found in the \"Molecules\" folder included in this project." << endl;
				}
				AskForEnter();
			}
			break;

		case'7':
			system("cls");
			ResetFiles();
			break;
		}
	}
}