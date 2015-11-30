#include <iostream>
#include <fstream>
#include <cstring>

void usage(bool help, char *programName)
{
	std::cout << "Usage: [-n/--names][-h/--help] files..";
	if(not help)
		std::cout <<"\nRun " << programName << " -h for more information.";
	std::cout << std::endl;
}

int main(int argc, char *argv[])
{
	if(argc < 2) {
		std::cerr << "Error: No file specified!\n";
		usage(false, argv[0]);
		return 1;
	}
	
	int skipVar = 0;
	bool outputFileNames = false;

	for(int i = 0; i < argc; i++) {
		if(std::strcmp(argv[i], "-n") == 0 or std::strcmp(argv[i], "--names") == 0) {
			outputFileNames = true;
			skipVar = i;
		}
		else if(std::strcmp(argv[i], "-h") == 0 or std::strcmp(argv[i], "--help") == 0) {
			usage(true, argv[0]);
			std::cout << argv[0] << ": Look at binary data formatted in hexadecimal.\nFlags:\n\t-n: Display name of file before dumping data\n\t-h: Display this help and exit" << std::endl;
			return 2;
		}
	}
			
	std::fstream file;

	for(int fileIndex = 1; fileIndex < argc; fileIndex++) {
		
		if(fileIndex == skipVar)
			continue;

		if(outputFileNames)
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
	
		if(length % 0x10 == 0)
			rows = length / 0x10;
		else
			rows = ((length - length % 0x10) / 0x10) + 1;
	
		for(int i = 0; i < rows; i++) {
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
		
			for(int j = 0; j < 0x10; j++) {
				if( buffer[j] > 126) //Backspace or some non-drawable character
					std::cout << ".";
				else if( buffer[j] > 32) //32 is space, all other ascii characters come after 31, everything before are status codes and the like 
					std::cout << buffer[j];
				else
					std::cout << ".";
			}
		
			std::cout << "|" << std::endl;
		}

		file.close();
	}
}
