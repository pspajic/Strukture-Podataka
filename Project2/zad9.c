/*
Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u cvorove binarnog stabla.
a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vraca
pokazivac na korijen stabla.
b) Napisati funkciju replace koja ce svaki element stabla zamijeniti sumom elemenata u
njegovom lijevom i desnom podstablu (tj. sumom svih potomaka prije zamjene
vrijednosti u tim potomcima). Npr. stablo sa slike Slika 1 transformirat æe se u stablo na
slici Slika 2.
c) Prepraviti program na nacin da umjesto predefiniranog cjelobrojnog polja koristenjem
funkcije rand() generira slucajne brojeve u rasponu <10, 90>. Takodjer, potrebno je
upisati u datoteku sve brojeve u inorder prolasku nakon koristenja funkcije iz a), zatim b)
dijela zadatka.
*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2

struct node;
typedef struct node* Position;
typedef struct node {
	int data;
	Position left;
	Position right;
} Node;

Position insert(Position root, int x);
int replace(Position root);
int writeInorderToFile(Position root, int level, FILE* filePointer);
int deleteTree(Position root);

int main()
{
	srand(time(NULL));
	FILE* filePointer = fopen("inorder.txt", "w");
	if (filePointer == NULL) {
		printf("Datoteka nije mogla biti otvorena.\n");
		return FILE_OPEN_ERROR;
	}

	int i = 0;
	Position root = NULL;

	//Generiranje 10 slucajnih brojeva i umetanje u stablo
	for (i = 0;i < 10;i++) {
		root = insert(root, rand() % (90 - 10 + 1) + 10);
		if (root == NULL) {
			printf("Greska pri alokaciji memorije!\n");
			return MALLOC_ERROR;
		}
	}

	//Upisivanje stabla u datoteku prije i nakon promjene
	fprintf(filePointer, "Orginalno stablo:\n");
	writeInorderToFile(root, 0, filePointer);

	//zamjena vrijednosti cvorova
	replace(root);

	fprintf(filePointer, "\nStablo nakon zamjene:\n");
	writeInorderToFile(root, 0, filePointer);

	//Zatvaranje datoteke
	fclose(filePointer);

	deleteTree(root);

	return 0;
}

Position insert(Position root, int x)
{
	if (root == NULL) {
		root = (Position)malloc(sizeof(Node)); //novi cvor

		if (root == NULL) return NULL; //ako ne alocira memoriju vraca NULL

		root->data = x;
		root->left = NULL;
		root->right = NULL;
	}

	else if (x < root->data) {
		root->left = insert(root->left, x); //umetanje u lijevo podstablo
	}

	else {
		root->right = insert(root->right, x);
	}

	return root;
}

int replace(Position root)
{
	if (root == NULL) return 0;
	int data = root->data;
	root->data = replace(root->left) + replace(root->right); //rekurzivna zamjena
	return data + root->data; //vraca staru vrijednost cvora plus sumu
}

int writeInorderToFile(Position root, int level, FILE* filePointer)
{
	if (root != NULL) {
		writeInorderToFile(root->right, level + 1, filePointer); //desno podstablo

		//trenutni cvor s uvlakom
		for (int i = 0; i < level;i++) {
			fprintf(filePointer, "  ");
		}
		fprintf(filePointer, "%d\n", root->data);

		writeInorderToFile(root->left, level + 1, filePointer);
	}
	return 0;
}

int deleteTree(Position root)
{
	if (root == NULL) return EXIT_SUCCESS;

	deleteTree(root->left);
	deleteTree(root->right);

	free(root);

	return EXIT_SUCCESS;
}