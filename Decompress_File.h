#pragma once
#include "Compress_File.h"

class Decompress_File : public Compress_File
{
public:
	char* TenFileInFile;
private:
	bit_buffer buff;
public:
	Decompress_File();//Ham tao
	~Decompress_File();//Ham huy
public:
	void convert_bin(unsigned char c);
	void decode_file(char* FileIn);//Ham giai nen file
};
