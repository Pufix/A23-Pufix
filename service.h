#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"repo.h"
#include <alg.h>
#define elif else if
#define _CRT_SECURE_NO_WARNINGS 1
struct repository* repo = NULL;

void makeRepo() {
	repo = createRepo();
}
int comp(const void* elem1, const void* elem2) {
	med f = *((med*)elem1);
	med s = *((med*)elem2);
	return (strcmp(f.name, s.name));
}
const char* servAdd(char command[]) {
	int j;
	if (command[3] != ' ')
		return "add - add a medicine: add <name> <price> <quantity> <concentration>\n";
	int i = 4;
	while (command[i] != '\0' && !(command[i] <= '9' && command[i] >= '0')) {
		i++;
	}
	if (command[i] == NULL) {
		return "ERROR: Invalid command syntax!\n";
	}
	char* name;
	name = (char*)malloc(sizeof(char) * (i - 4));
	for (j = 4; j < i; j++) {
		name[j - 4] = command[j];
	}
	name[i - 4] = '\0';
	int price = 0;
	int start = i;
	while (command[i] != NULL && command[i] <= '9' && command[i] >= '0') {
		i++;
	}
	if (command[i] == NULL) {
		return "ERROR: Invalid command syntax!\n";
	}
	for (j = start; j < i; j++) {
		price = price * 10 + command[j] - '0';
	}
	i++;
	int quantity = 0;
	start = i;
	while (command[i] != NULL && command[i] <= '9' && command[i] >= '0') {
		i++;
	}
	if (command[i] == NULL) {
		return "ERROR: Invalid command syntax!\n";
	}
	for (j = start; j < i; j++) {
		quantity = quantity * 10 + command[j] - '0';
	}
	float concentration = 0;
	i++;
	start = i;
	bool dot = false;
	while (command[i] != NULL && (command[i] <= '9' && command[i] >= '0' || (dot == false && command[i] == '.'))) {
		if (command[i] == '.') {
			dot = true;
		}
		i++;
	}
	if (i == start) {
		return "ERROR: Invalid command syntax!\n";
	}
	dot = false;
	int power = 10;
	for (j = start; j < i; j++) {
		if (command[j] == '.') {
			dot = true;
		}
		else {
			if (dot == false) {
				concentration = concentration * 10 + command[j] - '0';
			}
			else {
				concentration = concentration + (float)(command[j] - '0') / power;
				power *= 10;
			}
		}
	}
	repoaddMedicine(repo, name, price, quantity, concentration);
	return "\n";
}
const char* servList(char command[]) {
	//printf("%s", "list - list all medicines: list\n");
	int* temp = (int*)malloc(sizeof(int));
	med* meds = getAllMedicines(repo, temp);
	int len = temp[0];
	free(temp);
	char res[11111];
	res[0] = '\0';
	if (len == 0) {
		return "No medicines!\n";
	}
	qsort(meds, len, sizeof(med), comp);
	if (command[4] == ' ' && command[5] != '\0') {
		int i = 5;
		int j;
		while (command[i] != '\0' && !(command[i] <= '9' && command[i] >= '0')) {
			i++;
		}
		char* name;
		name = (char*)malloc(sizeof(char) * (i - 5));
		for (j = 5; j < i; j++) {
			name[j - 5] = command[j];
		}
		name[i - 5] = '\0';
		for (i = 0; i < len; i++) {
			if (strstr(meds[i].name, name) != NULL) {
				strcat(res, meds[i].name);
				strcat(res, ": price ");
				char buffer[100];
				itoa(meds[i].price, buffer, 10);
				strcat(res, buffer);
				strcat(res, " quantity ");
				itoa(meds[i].quantity, buffer, 10);
				strcat(res, buffer);
				strcat(res, " concentration ");
				gcvt(meds[i].concentration, 10, buffer);
				strcat(res, buffer);
				strcat(res, "0\n");
			}

		}
	}
	else {
		for (int i = 0; i < len; i++) {
			strcat(res, meds[i].name);
			strcat(res, ": price ");
			char buffer[100];
			itoa(meds[i].price, buffer, 10);
			strcat(res, buffer);
			strcat(res, " quantity ");
			itoa(meds[i].quantity, buffer, 10);
			strcat(res, buffer);
			strcat(res, " concentration ");
			gcvt(meds[i].concentration, 10, buffer);
			strcat(res, buffer);
			strcat(res, "0\n");
		}
	}
	return res;
}
const char* servDel(char command[]) {
	if (command[3] == '\0')
		return "del - delete a medicine: del <name> <concentration>\n";
	int i = 3, start = 4, j;
	char name[111];
	while (command[i] != '\0' && !(command[i] >= '0' && command[i] <= '9')) {
		i++;
	}
	if (command[i] == '\0')
		return "ERROR: Invalid command syntax!\n";
	for (j = start; j < i; j++)
		name[j - 4] = command[j];
	name[i - 4] = '\0';
	float concentration = 0;
	start = i;
	bool dot = false;
	while (command[i] != NULL && (command[i] <= '9' && command[i] >= '0' || (dot == false && command[i] == '.'))) {
		if (command[i] == '.') {
			dot = true;
		}
		i++;
	}
	if (i == start)
		return "ERROR: Invalid command syntax!\n";
	if (command[i] != '\0')
		return "ERROR: Invalid command syntax!\n";
	dot = false;
	int power = 10;
	for (j = start; j < i; j++) {
		if (command[j] == '.') {
			dot = true;
		}
		else {
			if (dot == false) {
				concentration = concentration * 10 + command[j] - '0';
			}
			else {
				concentration = concentration + (float)(command[j] - '0') / power;
				power *= 10;
			}
		}
	}
	if (repoDeleteMedicine(repo, name, concentration)) {
		return "Medicine removed!\n";
	}
	else {
		return "Medicine not found\n";
	}
}
const char* servUpd(char command[]) {
	if (command[3] == '\0')
		return "update - update a medicine: upd <name> <concentration> <new price> <new quantity> \n";

	int i = 3, start = 4, j;
	char name[111];
	while (command[i] != '\0' && !(command[i] >= '0' && command[i] <= '9')) {
		i++;
	}
	if (command[i] == '\0')
		return "ERROR: Invalid command syntax!\n";
	for (j = start; j < i; j++)
		name[j - 4] = command[j];
	name[i - 4] = '\0';
	float concentration = 0;
	start = i;
	bool dot = false;
	while (command[i] != NULL && (command[i] <= '9' && command[i] >= '0' || (dot == false && command[i] == '.'))) {
		if (command[i] == '.') {
			dot = true;
		}
		i++;
	}
	if (command[i] == '\0')
		return "ERROR: Invalid command syntax!\n";
	dot = false;
	int power = 10;
	for (j = start; j < i; j++) {
		if (command[j] == '.') {
			dot = true;
		}
		else {
			if (dot == false) {
				concentration = concentration * 10 + command[j] - '0';
			}
			else {
				concentration = concentration + (float)(command[j] - '0') / power;
				power *= 10;
			}
		}
	}
	i++;
	int price = 0;
	start = i;
	while (command[i] != NULL && command[i] <= '9' && command[i] >= '0') {
		i++;
	}
	if (command[i] == NULL) {
		return "ERROR: Invalid command syntax!\n";
	}
	for (j = start; j < i; j++) {
		price = price * 10 + command[j] - '0';
	}
	i++;
	int quantity = 0;
	start = i;
	while (command[i] != NULL && command[i] <= '9' && command[i] >= '0') {
		i++;
	}
	for (j = start; j < i; j++) {
		quantity = quantity * 10 + command[j] - '0';
	}
	if (repoUpdateMedicine(repo, name, price, quantity, concentration))
		return "Medicine updated!\n";
	else
		return "Medicine not found!\n";
	return "\0";
}
const char* commandHandling(char command[]) {
	if (command[0] == 'a' && command[1] == 'd' && command[2] == 'd')return servAdd(command);
	else if (command[0] == 'l' && command[1] == 'i' && command[2] == 's' && command[3] == 't')return servList(command);
	else if (command[0] == 'd' && command[1] == 'e' && command[2] == 'l')return servDel(command);
	else if (command[0] == 'u' && command[1] == 'p' && command[2] == 'd')return servUpd(command);
	else return "No command found!\n";
}