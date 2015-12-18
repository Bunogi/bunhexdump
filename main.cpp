#include <iostream>
#include <fstream>
#include <string>
#include <cstring> //For memset only
#include <vector>

namespace FlagStates {
	unsigned int help = 1; //0001
	unsigned int names = 2;  //0010

};

void printUsage(bool help, std::string programName) {
	std::cout << "Usage: [-n/--names][-h/--help] files..";
	if(not help)
		std::cout <<"\nRun " << programName << " -h for more information.";
	std::cout << std::endl;
}

void printHelp(std::string programName) {
	printUsage(true, programName);
	std::cout << programName << ": Look at binary data formatted in hexadecimal.\nFlags:\n\t-n: Display name of file before dumping data\n\t-h: Display this help and exit\n";
}

void multipleArgsError(std::string argument) {
	std::cerr << "Error: flag '" << argument << "' inputted more than once.\n";
}

void printUnknownArgError(char arg) {
	std::cerr << "Error: unknown argument '-" << arg << "'\n";
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Error: No file specified!\n";
		printUsage(false, argv[0]);
		return 1;
	}

	std::vector<int> skipvars;
	unsigned int flags = 0;

	for(int i = 1; i < argc; i++) {
		std::string checking = argv[i];
		bool skipThis = false;
		if (checking[0] == '-' and checking[1] != '-') {
			for (unsigned int j = 1; j < checking.size(); j++) {
				switch (checking[j]) {
					case 'h':
						printHelp(argv[0]);
						return 0;
					case 'n':
						if ((FlagStates::names & flags) == FlagStates::names) {
							multipleArgsError("n");
							return 1;
						}
						flags |= FlagStates::names;
						skipThis = true;
						break;
					default:
						printUnknownArgError(checking[j]);
						return 1;
				}
			}
		}
		else {
			if (checking == "--names") {
				if ((FlagStates::names & flags) == FlagStates::names) {
					multipleArgsError("names");
					return 1;
				flags |= FlagStates::names;
				skipThis = true;
				}
				else if (checking == "--help") {
					printHelp(argv[0]);
					return 0;
				}
			}
		} 
		if (skipThis)
			skipvars.push_back(i);
	}
			
	std::fstream file;

	for (int fileIndex = 1; fileIndex < argc; fileIndex++) {

		bool skipThis = false;

		for (auto i : skipvars) {
			if (i == fileIndex) {
				skipThis = true;
				break;
			}
		}

		if (skipThis)
			continue;

		if ((flags & FlagStates::names) == FlagStates::names) //Do we want to print the names?
			std::cout << "File: " << argv[fileIndex] << std::endl;
	
		file.open(argv[fileIndex], std::ios::in | std::ios::ate | std::ios::binary);
	
		if(not file) {
			std::cout << "Failed to open file " << argv[fileIndex] << ".\n";
			return 1;
		}	
	
		int length = file.tellg();
		file.seekg(0, file.beg);
	
		unsigned char buffer[0x10];

		int rows = 0;
	
		if (length % 0x10 == 0)
			rows = length / 0x10;
		else
			rows = ((length - length % 0x10) / 0x10) + 1;
	
		for (int i = 0; i < rows; i++) {
			memset(buffer, 0, 0x10);
			if(i == (length - length % 0x10) / 0x10) // We're dealing with the last row
				file.read((char*)buffer, length % 0x10);
			else
				file.read((char*)buffer, 0x10);

			std::cout << std::hex << i * 0x10 << "\t";

			for(int j = 0; j < 0x10; j++) {
				if(buffer[j] < 0x10)
					std::cout << 0;
				std::cout << std::hex << (int)buffer[j] << " ";
				if(j == 7) //We start at zero
					std::cout << " ";
			}
		
			std::cout << "\t|";
		
			for (int j = 0; j < 0x10; j++) {
				if (buffer[j] > 126) //Backspace or some non-drawable character
					std::cout << ".";
				else if (buffer[j] > 32) //32 is space, all other ascii characters come after 31, everything before are status codes and the like 
					std::cout << buffer[j];
				else
					std::cout << ".";
			}
		
			std::cout << "|" << std::endl;
		}

		file.close();
		std::cout << std::hex << length << std::endl;
	}
}
