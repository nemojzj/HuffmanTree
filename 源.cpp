#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
#include "priority_queue.h"
#include "queue.h"
typedef struct HTNode{
	unsigned char key;
	unsigned int weight;
	char* code;
	HTNode* parent;
	HTNode* lchild;
	HTNode* rchild;
}HTNode, *HuffmanTree;

typedef struct {
	unsigned int weight;
	unsigned char key;
	char* code;
}VCode, *Code;

int cmp(const HuffmanTree &a, const HuffmanTree &b) {
	if (a->weight > b->weight) return 1;
	else if (a->weight < b->weight) return -1;
	else return 0;
}

void PreOrderTraverse(HuffmanTree T, FILE* fnew) {
	unsigned char ch;
	if (T == NULL) {
		return;
	}
	if (T->lchild == NULL && T->rchild == NULL) {
		ch = '1';
		fwrite(&ch, sizeof(unsigned char), 1, fnew);
		ch = T->key;
		fwrite(&ch, sizeof(unsigned char), 1, fnew);
		return;
	}
	ch = T->key;
	fwrite(&ch, sizeof(unsigned char), 1, fnew);
	PreOrderTraverse(T->lchild, fnew);
	PreOrderTraverse(T->rchild, fnew);
}

bool CharGet(Code &TV,char *s,int &n) {
	FILE *fp = NULL;
	fopen_s(&fp, s, "rb");
	if (fp == NULL) {
		printf("Fire Opening failed");
		return 1;
	}
	unsigned char ch;
	fread(&ch, sizeof(unsigned char), 1, fp);
	Code temp;
	TV = (Code)malloc(sizeof(VCode));
	if (!TV) return 1;
	TV[0].key = ch;
	TV[0].weight = 1;
	TV[0].code = "";
	int i = 0;
	int count = 1;
	while (!feof(fp)) {
		fread(&ch, sizeof(unsigned char), 1, fp);
		for (i = 0; i < count; i++) 
			if (TV[i].key == ch) 
				break;
		if (i == count) {
			count++;
			temp = (Code)realloc(TV, count*sizeof(VCode));
			if (temp == NULL) return 1;
			TV = temp;
			if (!TV) return 1;
			TV[count - 1].key = ch;
			TV[count - 1].weight = 1;
		}
		else {
			TV[i].weight++;
		}
	}
	n = count;
	fclose(fp);
	return 0;
}

bool HTBuild(HuffmanTree &HT, Code TV,int n) {
	struct Priority_Queue<HuffmanTree> q;
	Priority_Queue<HuffmanTree>::init(q, n, cmp);
	int i;
	HTNode* T;
	HTNode* ltemp;
	HTNode* parent;
	HTNode* rtemp;
	for (i = 0; i < n; i++) {
		T = (HTNode*)malloc(sizeof(HTNode));
		if (!T) return 1;
		T->key = TV[i].key;
		T->lchild = NULL;
		T->rchild = NULL;
		T->weight = TV[i].weight;
		Priority_Queue<HuffmanTree>::enqueue(q, T);
		T = NULL;
	}
	while (!Priority_Queue<HuffmanTree>::empty(q)) {
		Priority_Queue<HuffmanTree>::dequeue(q, ltemp);
		if (Priority_Queue<HuffmanTree>::empty(q)) {
			HT->key = '@';
			HT->lchild = ltemp->lchild;
			HT->rchild = ltemp->rchild;
			HT->weight = ltemp->weight;
			break;
		}
		Priority_Queue<HuffmanTree>::dequeue(q, rtemp);
		parent = (HTNode*)malloc(sizeof(HTNode));
		if (!parent)return 1;
		parent->weight = ltemp->weight + rtemp->weight;
		parent->lchild = ltemp;
		parent->rchild = rtemp;
		parent->key = '@';
		ltemp->parent = rtemp->parent = parent;
		Priority_Queue<HuffmanTree>::enqueue(q, parent);
		parent = NULL;
	}
	return 0;
}

bool CodeBuild(HuffmanTree HT, Code &TV, int i, int &j, char k) {
	if (HT == NULL) return 1;
	if(i > 0) {
		HT->code = (char*)malloc((i + 1) * sizeof(char));
		if (!HT->code) return 1;
		int t = 0;
		for (; t < i - 1; t++) {
			HT->code[t] = HT->parent->code[t];
		}
		HT->code[i - 1] = k;
		HT->code[i] = '\0';
	}
	if (!HT->lchild && !HT->rchild) {
		TV[j].key = HT->key;
		TV[j].weight = HT->weight;
		TV[j].code = HT->code;
		j++;
	}
	CodeBuild(HT->rchild, TV, i + 1, j, '1');
	CodeBuild(HT->lchild, TV, i + 1, j, '0');
	return 0;
}

bool HuffmanZip(HuffmanTree T, Code TV, char* s, char* t, int n) {
	FILE* fp = NULL;
	FILE* fnew = NULL;
	char filename[100] = "C:\\Users\\nemoj\\Desktop\\";
	char* back = ".huff";
	int i, j, k;
	i = j = k = 0;
	strcat_s(filename, 100, t);
	strcat_s(filename, 100, back);
	fopen_s(&fnew, filename, "wb");//创建一个新文件储存二进制压缩文件
	fopen_s(&fp, s, "rb");
	if (fnew == NULL) {
		printf("Fire Opening failed1");
		return 1;
	}
	if (fp == NULL) {
		printf("Fire Opening failed2");
		return 1;
	}
	fseek(fp, 0L, SEEK_END);//将文件指针移到末尾
	unsigned long long FileSize = ftell(fp);//记录文件总字节数
	fseek(fp, 0L, SEEK_SET);//将指针移到开头
	PreOrderTraverse(T, fnew);
	while (s[k] != '\0') k++;
	fwrite(s, sizeof(unsigned char), k, fnew);
	char temp = '`';
	fwrite(&temp, sizeof(unsigned char), 1, fnew);
	int offset = 8;
    unsigned char ReadByte;//读取的一个字节
    unsigned char TempByte = 0;//暂时存储要写入的编码
    unsigned long long BitSize = 0;//记录位数，方便进行解压
	for(unsigned long long l = 0; l < FileSize; l++) {
		fread(&ReadByte, sizeof(unsigned char), 1, fp);
		for (j = 0; j < n; j++) {
			if (TV[j].key == ReadByte) break;
		}
		for (int i = 0; TV[j].code[i]; i++) {
			TempByte = (TempByte << 1) | (TV[j].code[i] - '0');//利用位或TempByte左移一位并写入一位
			BitSize++;
			offset--;
			if (offset == 0) {
				//字节8位已填满
				offset = 8;//重置为8位
				fwrite(&TempByte, sizeof(unsigned char), 1, fnew);
				TempByte = 0;
			}
		}
	}
	if (offset != 8) {
		TempByte <<= offset;
		fwrite(&TempByte, sizeof(unsigned char), 1, fnew);
	}
	fwrite(&BitSize, sizeof(unsigned long long), 1, fnew);//将位数写在文件的最后
	fclose(fp);
	fclose(fnew);
	return 0;
}

void HTRebuild(HuffmanTree &T, FILE* fp, unsigned char &temp) {//不妨以先序遍历二叉树的顺序来创建二叉树//
	temp = fgetc(fp);
	if (temp == '1') {//1表示叶子结点//
		T = (HTNode*)malloc(sizeof(HTNode));
		if (!T) return;
		T->lchild = NULL;
		T->rchild = NULL;
		temp = fgetc(fp);
		T->key = temp;
		return;
	}
	else {
		T = (HTNode*)malloc(sizeof(HTNode));
		if (!T) return;
		T->key = temp;
		HTRebuild(T->lchild, fp, temp);
		HTRebuild(T->rchild, fp, temp);
	}
}

bool Zip(int argc, char* argv[], int m){
	HuffmanTree HT;
	HT = (HTNode*)malloc(sizeof(HTNode));
	if (!HT) return 1;
	Code TV;
	int n, j;
	j = 0;
	CharGet(TV, argv[2], n);
	HTBuild(HT, TV, n);
	CodeBuild(HT, TV, 0, j, '0');
	HuffmanZip(HT, TV, argv[2], argv[m], n);
	return 0;
}

bool UnZip(int argc, char* argv[]) {
	HTNode* p;
	HuffmanTree T;
	T = (HTNode*)malloc(sizeof(HTNode));
	if (!T) return 1;
	unsigned char temp, ch;
	int i, n;
	int offset = 8;
	unsigned char TempByte = 0;
	unsigned char ReadByte = 0;
	unsigned long long BitSize;
	i = 0;
	temp = NULL;
	FILE* fp = NULL;
	FILE* fnew = NULL;	
	char filename[100];
	fopen_s(&fp, argv[2], "rb+");
	if (fp == NULL) {
		printf("Fire Opening failed");
		return 1;
	}
	fseek(fp, -1L * (long long int)(sizeof(unsigned long long)), SEEK_END);//跳到末尾
	fread(&BitSize, sizeof(unsigned long long), 1, fp);//读取位数
	fseek(fp, 0L, SEEK_SET);//跳到文件头
	HTRebuild(T, fp, temp);//解压缩时通过先序遍历序列创建二叉树以解码
	fread(&ReadByte, sizeof(unsigned char), 1, fp);
	while (ReadByte != '`') {
		filename[i++] = ReadByte;
		fread(&ReadByte, sizeof(unsigned char), 1, fp);
	}
	filename[i] = '\0';
	char front[100] = "C:\\Users\\nemoj\\Desktop\\";
	strcat_s(front, 100, filename);
	fopen_s(&fnew, front, "wb");//创建一个新文件储存原文件
	if (fnew == NULL) {
		printf("Fire Opening failed");
		return 1;
	}
	p = T;
	fread(&ReadByte, sizeof(unsigned char), 1, fp);
	for (unsigned long long a = 0; a < BitSize; a++) {
		TempByte = 1 & (ReadByte >> 7);//位与判断ReadByte的位为1还是0
		if (!p) return 1;
		else if (TempByte) p = p->rchild;
		else p = p->lchild;
		if (!p) return 1;
		else if ((!p->lchild) && (!p->rchild)) {
			ch = p->key;
			fwrite(&ch, sizeof(unsigned char), 1, fnew);
			p = T;
		}
		ReadByte = ReadByte << 1;//舍弃第一位
		offset--;
		if (offset == 0) {
			ReadByte = 0;
			fread(&ReadByte, sizeof(unsigned char), 1, fp);
			offset = 8;
		}
	}
	fclose(fp);
	fclose(fnew);
	return 0;
}

int main(int argc, char *argv[]) {
	if (argc == 3)
		if (!strcmp(argv[1], "-z")) Zip(argc, argv, 2);
		else if (!strcmp(argv[1], "-u")) UnZip(argc, argv);
		else printf("ERROR\n");
	else if (argc == 4)
		if (!strcmp(argv[1], "-zr")) Zip(argc, argv, 3);
		else printf("ERROR\n");
	else printf("ERROR\n");
}

