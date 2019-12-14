#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <conio.h>

using namespace std;
#define NUM_CHAR 256
#define NUM_BIT 8

//Cau truc cay
typedef struct treenode
{
	unsigned int freq;//Trong so
	unsigned char ch;//Ky tu
	treenode* left;//Cay con trai
	treenode* right;//Cay con phai

}treenode;
typedef struct PQ
{
	int heap_size;
	treenode* A[NUM_CHAR];
}PQ;

typedef struct bit_buffer
{
	char content[NUM_BIT];
	int post;
}bit_buffer;

class Compress_File
{
public:
	char* tenfilebandau;
protected:
	unsigned int freq[NUM_CHAR];
	unsigned char ch;
	int nbits;
	int current_byte;
public:
	Compress_File();//Ham tao
	~Compress_File();//Ham huy
public:
	int parent(int i);
	int left_child(int i);
	int right_child(int i);
	void create_pq(PQ* p);

	unsigned int get_frequencies(char* NameFileIn);//Lay tan so
	void heapify(PQ* p, int i);//Ham hieu chinh
	void insert_pq(PQ* p, treenode* r);//Chen vao cay node moi 
	treenode* extract_min_pq(PQ* p);//Trich xuat node nho nhat trong cay
	treenode* build_huffman();//Xay dung cay huffman
	void traverse(treenode* r, int level, char code_so_far[], char* codes[]);
	void bitout(fstream& FileOut, char bit);
	void encode_file(char* NameFileIn, char* NameFileOut, char* codes[]);//Thuc hien nen file
};
