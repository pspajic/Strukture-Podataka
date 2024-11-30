/*
5. Napisati program koji iz datoteke èita postfiks izraz i zatim korištenjem stoga raèuna
rezultat. Stog je potrebno realizirati preko vezane liste.
*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct CvorStoga {
	double broj;
	struct CvorStoga* next;
}CvorStoga;
typedef CvorStoga* Pozicija;

Pozicija StvoriElement(double broj);
int DodajNaVrh(Pozicija head, double broj);
int SkiniSVrha(Pozicija head, double* rezultat);
int IzvrsiOperaciju(Pozicija head, char operacija);
int IzracunajPostfixIzDatoteke(double* rezultat, const char* nazivDatoteke);

int main() {
	double rezultat = 0;
	const char* nazivDatoteke = "Postfix.txt";

	if (IzracunajPostfixIzDatoteke(&rezultat, nazivDatoteke) == EXIT_SUCCESS) {
		printf("Rezultat postfix izraza: %.2f\n", rezultat);
	}
	else {
		printf("Dogodila se greska pri racunanju postfix izraza!\n");
	}
	
	return EXIT_SUCCESS;
}

Pozicija StvoriElement(double broj)
{
	Pozicija noviElement = (Pozicija)malloc(sizeof(CvorStoga));

	if (!noviElement) {
		printf("Greska pri alokaciji memorije!");
		return NULL;
	}

	noviElement->broj = broj;
	noviElement->next = NULL;

	return noviElement;
}

int DodajNaVrh(Pozicija head, double broj)
{
	Pozicija noviElement = StvoriElement(broj);

	if (!noviElement) {
		return -1;
	}

	noviElement->next = head->next;
	head->next = noviElement;

	return EXIT_SUCCESS;
}

int SkiniSVrha(Pozicija head, double* rezultat)
{
	if (!head->next) {
		printf("Stog je prazan! Postfix izraz nije valjan!\n");
		return -1;
	}

	Pozicija temp = head->next;
	*rezultat = temp->broj;

	head->next = temp->next;
	free(temp);

	return EXIT_SUCCESS;
}

int IzvrsiOperaciju(Pozicija head, char operacija)
{
	double operand1 = 0, operand2 = 0, rezultat = 0;

	if (SkiniSVrha(head, &operand2) != EXIT_SUCCESS || SkiniSVrha(head, &operand1) != EXIT_SUCCESS) {
		return -1;
	}

	switch (operacija) {
	case '+':
		rezultat = operand1 + operand2;
		break;

	case '-':
		rezultat = operand1 - operand2;
		break;

	case '*':
		rezultat = operand1 * operand2;
		break;

	case '/':
		if (operand2 == 0) {
			printf("Dijeljenje s nulom nije dopusteno!\n");
			return -1;
		}
		rezultat = operand1 / operand2;
		break;

	default:
		printf("Nepoznata operacija: %c\n", operacija);
		return -1;
	}

	return DodajNaVrh(head, rezultat);
}

int IzracunajPostfixIzDatoteke(double* rezultat, const char* nazivDatoteke)
{
	FILE* datoteka = fopen(nazivDatoteke, "r");
	if (!datoteka) {
		printf("Ne mogu otvoriti datoteku!");
		return -1;
	}

	char buffer[1024];
	fgets(buffer, sizeof(buffer), datoteka);
	fclose(datoteka);

	Pozicija head = (Pozicija)malloc(sizeof(CvorStoga));
	if (!head) {
		printf("Greska pri alokaciji memorije!");
		return -1;
	}
	head->next = NULL;

	char* trenutni = buffer;
	while (*trenutni != '\0') {

		if (isdigit(*trenutni) || (*trenutni == '-' && isdigit(*(trenutni + 1)))) {
			double broj = 0;
			sscanf(trenutni, "%lf", &broj);
			DodajNaVrh(head, broj);

			while (isdigit(*trenutni) || *trenutni == '.' || *trenutni == '-') {
				trenutni++;
			}
		}

		else if (*trenutni == '+' || *trenutni == '-' || *trenutni == '*' || *trenutni == '/') {
			if (IzvrsiOperaciju(head, *trenutni) != EXIT_SUCCESS) {
				free(head);
				return -1;
			}
			trenutni++;
		}
		else {
			trenutni++;
		}
	}

	if (SkiniSVrha(head, rezultat) != EXIT_SUCCESS || head->next != NULL) {
		printf("Postfix izraz nije valjan!\n");
		free(head);

		return -1;
	}

	free(head);

	return EXIT_SUCCESS;
}