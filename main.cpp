#include <iostream>
#include <fstream>
#include <cstring>

int main(int argc, char *argv[])
{
	if(argc != 2)
		return 1;
	
	std::fstream file;
	
	file.open(argv[1], std::ios::in | std::ios::ate | std::ios::binary);
	
	if(not file)
	{
		std::cout << "Failed to open file " << argv[1] << ".\n";
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
	
	for(int i = 0; i < rows; i++)
	{
		memset(buffer, 0, 0x10);
		if(i == (length - length % 0x10) / 0x10) // We're dealing with the last row
			file.read((char*)buffer, length % 0x10);
		else
			file.read((char*)buffer, 0x10);

		std::cout << std::hex << i * 0x10 << "\t";

		for(int j = 0; j < 0x10; j++)
		{
			if(buffer[j] < 0x10)
				std::cout << 0;
			std::cout << std::hex << (int)buffer[j] << " ";
			if(j == 7) //We start at zero
				std::cout << " ";
		}
		
		std::cout << "\t|";
		
		for(int j = 0; j < 0x10; j++)
		{
			if( buffer[j] > 127) //Backspace
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
