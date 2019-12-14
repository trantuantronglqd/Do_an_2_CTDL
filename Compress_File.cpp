#include "Menu.h"
#include "Compress_File.h"
#define _CRT_SECURE_NO_WARNINGS

//Ham tao
Compress_File::Compress_File()
{
	for (int i = 0; i < NUM_CHAR; ++i)
		freq[i] = 0;

	this->tenfilebandau = new char;
}
//Ham huy
Compress_File::~Compress_File()
{
}
//Tim cha
int Compress_File::parent(int i)
{
	return (i - 1) / 2;
}
//Tim con trai
int Compress_File::left_child(int i)
{
	return i * 2 + 1;
}
//Tim con phai
int Compress_File::right_child(int i)
{
	return i * 2 + 2;
}
//Tao node moi
void Compress_File::create_pq(PQ* p)
{
	p->heap_size = 0;
}
//Ham lay tan so
unsigned int Compress_File::get_frequencies(char* NameFileIn)
{
	unsigned int n;
	fstream    FileIn(NameFileIn, ios::in | ios::binary);

	for (n = 0;; n++)
	{
		ch = FileIn.get();
		if (FileIn.eof())
			break;

		freq[ch]++;
	}
	FileIn.close();
	return n;
}
//Ham hieu chinh vi tri node trong cay
void Compress_File::heapify(PQ* p, int i)
{
	int l, r, smallest;
	treenode* t;

	l = left_child(i);
	r = right_child(i);
	if (l < p->heap_size && p->A[l]->freq < p->A[i]->freq)
		smallest = l;
	else
		smallest = i;
	if (r < p->heap_size && p->A[r]->freq < p->A[smallest]->freq)
		smallest = r;

	if (smallest != i)
	{
		t = p->A[i];
		p->A[i] = p->A[smallest];
		p->A[smallest] = t;
		this->heapify(p, smallest);
	}
}
//Ham them node vao cay huffman
void Compress_File::insert_pq(PQ* p, treenode* r)
{
	int i;

	p->heap_size++;
	i = p->heap_size - 1;

	while ((i > 0) && (p->A[parent(i)]->freq > r->freq))
	{
		p->A[i] = p->A[parent(i)];
		i = parent(i);
	}
	p->A[i] = r;
}
//Ham trich xuat node nho nhat trong cay
treenode* Compress_File::extract_min_pq(PQ* p)
{
	treenode* r;
	if (p->heap_size == 0)
	{
		cout << "hang doi rong!\n";
		exit(1);
	}
	r = p->A[0];
	p->A[0] = p->A[p->heap_size - 1];
	p->heap_size--;
	this->heapify(p, 0);
	return r;
}
//Ham tao cay huffman
treenode* Compress_File::build_huffman()
{
	int i, n;
	treenode* x = NULL, * y = NULL, * z = NULL;
	PQ p;

	create_pq(&p);

	for (i = 0; i < NUM_CHAR; i++)
	{
		x = (treenode*)malloc(sizeof(treenode));
		x->left = NULL;
		x->right = NULL;
		x->freq = freq[i];
		x->ch = (char)i;
		this->insert_pq(&p, x);
	}
	n = p.heap_size - 1;
	for (i = 0; i < n; i++)
	{
		z = (treenode*)malloc(sizeof(treenode));
		x = this->extract_min_pq(&p);
		y = this->extract_min_pq(&p);
		z->left = x;
		z->right = y;
		z->freq = x->freq + y->freq;
		this->insert_pq(&p, z);
	}

	return this->extract_min_pq(&p);
}

void Compress_File::traverse(treenode* r, int level, char code_so_far[], char* codes[])
{
	if ((r->left == NULL) && (r->right == NULL))
	{
		code_so_far[level] = 0;

		codes[r->ch] = _strdup(code_so_far);
	}
	else
	{
		code_so_far[level] = '0';
		this->traverse(r->left, level + 1, code_so_far, codes);

		code_so_far[level] = '1';
		this->traverse(r->right, level + 1, code_so_far, codes);
	}
}

void Compress_File::bitout(fstream& FileOut, char bit)
{
	current_byte <<= 1;

	if (bit == '1')
		current_byte |= 1;
	nbits++;

	if (nbits == NUM_BIT)
	{
		FileOut.put((unsigned char)current_byte);
		nbits = 0;
		current_byte = 0;
	}
}

//Ham thuc hien nen file
void Compress_File::encode_file(char* NameFileIn, char* NameFileOut, char* codes[])
{
	int n = 0;
	char* s;
	current_byte = 0;
	nbits = 0;
	for (int i = 0; i < NUM_CHAR; ++i)
		if (freq[i])
			++n;

	fstream FileIn(NameFileIn, ios::in | ios::binary);
	fstream FileOut(NameFileOut, ios::out | ios::binary);

	FileOut << "18120595" << "\n";  //ghi lai MSSV cua coder
	FileOut << this->tenfilebandau << "\n"; //// ghi lai ten file ban dau
	FileOut.write((char*)(&n), sizeof(n));  /// ghi so ky tu co trong file
	for (int i = 0; i < NUM_CHAR; ++i)
		if (freq[i])
		{
			FileOut.put(unsigned char(i));
			FileOut.write((char*)(&freq[i]), sizeof(int));
		}
	for (;;)
	{
		ch = FileIn.get();
		if (FileIn.eof())
			break;
		for (s = codes[ch]; *s; s++)
			this->bitout(FileOut, *s);
	}
	n = nbits;
	while (nbits)
		this->bitout(FileOut, '0');
	FileOut.put(char(n));
	FileIn.close();
	FileOut.close();

	cout << "NEN THANH CONG!" << endl;
	UserGuide();
}