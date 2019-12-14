//Chuong trinh nen file chay bang tham so dong lenh
#include "Menu.h"
#define _CRT_SECURE_NO_WARNINGS

int main(int argc, char* argv[])
{
	if (argc == 4)
	{
		char* doituong = new char[(int)strlen(argv[1]) + 1];
		strcpy(doituong, argv[1]);
		if (!strcmp(doituong, "File"))
		{
			Compress_File C;
			Decompress_File D;
			char* luachon = new char[(int)strlen(argv[2]) + 1];
			char* duongdanfile = new char[strlen(argv[3]) + 1];
			strcpy(luachon, argv[2]);
			strcpy(duongdanfile, argv[3]);
			char* codes[NUM_CHAR];
			char code[NUM_CHAR];
			if (!strcmp(luachon, "-e"))//Chon chuc nang nen file
			{
				C.get_frequencies(duongdanfile);
				treenode* r = C.build_huffman();
				C.traverse(r, 0, code, codes);
				C.tenfilebandau = new char[strlen(argv[3]) + 1];// Lay ten file
				strcpy(C.tenfilebandau, argv[3]);
				int NumberNameTrue = 0;
				for (int i = 0; i < strlen(duongdanfile); i++)
				{
					if (duongdanfile[i] == '.')
					{
						break;
					}
					NumberNameTrue++;
				}
				char* outputfile = new char[NumberNameTrue];
				strncpy(outputfile, duongdanfile, NumberNameTrue);
				outputfile[NumberNameTrue] = '\0';
				strcat(outputfile, ".huf");
				//Xuat ra file nen	
				C.encode_file(duongdanfile, outputfile, codes);
			}
			if (!strcmp(luachon, "-d"))//Lua chon chuc nang giai nen file
			{
				D.decode_file(duongdanfile);
			}
		}

	}
	else
	{
		cout << endl << "!!!BAN CHAY SAI THAM SO DONG LENH!!!" << endl;
		UserGuide();
	}
	return 0;
}