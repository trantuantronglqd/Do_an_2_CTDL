#include "Menu.h"
#include "Decompress_File.h"
#define _CRT_SECURE_NO_WARNINGS

//Ham tao
Decompress_File::Decompress_File()
{
	for (int i = 0; i < NUM_CHAR; ++i)
		freq[i] = 0;

	this->TenFileInFile = new char;
}
//Ham huy
Decompress_File::~Decompress_File()
{
}
void Decompress_File::convert_bin(unsigned char c)
{
	int i;
	for (i = 0; i < NUM_BIT; ++i)
		buff.content[i] = '0';
	buff.content[NUM_BIT] = '\0';
	i = NUM_BIT - 1;
	while (i >= 0)
	{
		buff.content[i] = (c % 2) + '0';
		c = c / 2;
		i--;
	}
	buff.post = 0;

}

//Ham thuc hien giai nen file
void Decompress_File::decode_file(char* NameFileIn)
{
	char* codes[NUM_CHAR];
	char code[NUM_CHAR];
	char* MaFile = new char;
	unsigned char c;
	int n, i;
	fstream FileIn(NameFileIn, ios::in | ios::binary);
	if (!FileIn)// Kiem tra file co ton tai khong?
	{
		//system("cls");
		cout << "Khong mo duoc file nen!" << endl;
		cout << "GIAI NEN KHONG THANH CONG!" << endl;
		UserGuide();
		return;
	}
	FileIn.getline(MaFile, '\n');
	if (strcmp(MaFile, "18120595") != 0)  // Kiem tra dinh dang file
	{
		//system("cls");
		cout << "Khong Biet Dinh Dang File" << endl;
		cout << "GIAI NEN KHONG THANH CONG!" << endl;
		UserGuide();
		return;
	}
	FileIn.getline(this->TenFileInFile, '\n');
	fstream FileOut(TenFileInFile, ios::out | ios::binary);

	FileIn.read((char*)(&n), sizeof(n));   // So ky tu trong file
	for (int i = 1; i <= n; ++i)
	{
		unsigned char ch = (unsigned char)FileIn.get();
		FileIn.read((char*)(&freq[ch]), sizeof(int));
	}
	int start = FileIn.tellg();
	FileIn.seekp(-1, ios::end);
	int full_bytes = FileIn.tellg();
	full_bytes = full_bytes - start - 1;
	c = FileIn.get();
	FileIn.seekg(-full_bytes - 2, ios::end);
	treenode* r = this->build_huffman();
	this->traverse(r, 0, code, codes);
	treenode* t = r;
	ch = FileIn.get();
	this->convert_bin(ch);
	for (i = 1; i <= full_bytes;)
	{
		if (t->left == NULL && t->right == NULL)
		{
			FileOut.put(t->ch);
			t = r;
		}
		else
		{
			if (buff.post < NUM_BIT && buff.content[buff.post] == '0')
				t = t->left;
			if (buff.post < NUM_BIT && buff.content[buff.post] == '1')
				t = t->right;
			if (buff.post == NUM_BIT)
			{
				ch = FileIn.get();
				this->convert_bin(ch);
				++i;
				buff.post = -1;
			}
			buff.post++;
		}
	}
	int remain = (int(c) == 0) ? NUM_BIT : int(c);
	for (; buff.post <= remain;)
	{
		if (t->left == NULL && t->right == NULL)
		{
			FileOut.put(t->ch);
			t = r;
		}
		else
		{
			if (buff.post < NUM_BIT && buff.content[buff.post] == '0')
				t = t->left;
			if (buff.post < NUM_BIT && buff.content[buff.post] == '1')
				t = t->right;
			buff.post++;

		}

	}
	FileIn.close();
	FileOut.close();
	cout << "GIAI NEN THANH CONG!" << endl;
	UserGuide();
}