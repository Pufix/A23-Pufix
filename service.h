#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"repo.h"
#include <alg.h>
#include <cassert>
#define elif else if
#define _CRT_SECURE_NO_WARNINGS 1
struct repository* repo = NULL;
struct undostack* undoStack = NULL;
struct undostack* redoStack = NULL;

void undoStackPush(char* command) {
	//here i add commands to the undo stack
	undostack* temp = (undostack*)malloc(sizeof(undostack));
	temp->command = command;
	temp->next = undoStack;
	undoStack = temp;
}void redoStackPush(char* command) {\
//here i add commands to the redo stack
	undostack* temp = (undostack*)malloc(sizeof(undostack));
	temp->command = command;
	temp->next = redoStack;
	redoStack = temp;
}
void undoStackPop() {
	//here i pop commands from the undo stack
	undostack* temp = undoStack;
	undoStack = undoStack->next;
	free(temp->command);
	free(temp);
}
void redoStackPop() {
	//here i pop commands from the redo stack
	undostack* temp = redoStack;
	redoStack = redoStack->next;
	free(temp->command);
	free(temp);
}
void clearRedo() {
	// here i clear the redo stack
	while (redoStack != NULL) 
		redoStackPop();
}
void makeRepo() {
	repo = createRepo();
}
int comp(const void* elem1, const void* elem2) {
	med f = *((med*)elem1);
	med s = *((med*)elem2);
	return (strcmp(f.name, s.name));
}
const char* servAdd(char command[],int opStyle) {
	// here i add a medicine to the repository
	int indexStartFloat = 0;
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
	name = (char*)malloc(sizeof(char) * (i - 3));
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
	indexStartFloat= i;
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
	bool alreadyExisting =repoaddMedicine(repo, name, price, quantity, concentration);
	if (alreadyExisting) {
		char* undoCommand=(char*)malloc(sizeof(char)*1000);
		strcpy(undoCommand, "upd ");
		med* medadded=getMedicine(repo, name, concentration);
		strcat(undoCommand, medadded->name);
		strcat(undoCommand, " ");
		strcat(undoCommand, command + indexStartFloat);
		strcat(undoCommand, " ");
		char buffer[100];
		itoa(medadded->price, buffer, 10);
		strcat(undoCommand, buffer);
		strcat(undoCommand, " ");
		itoa(medadded->quantity-quantity, buffer, 10);
		strcat(undoCommand, buffer);
		if (opStyle == 0)
			undoStackPush(undoCommand),
			clearRedo();
		else if (opStyle == 1)
			redoStackPush(undoCommand);
		else
			undoStackPush(undoCommand);
	}
	else {
		char* undoCommand=(char*)malloc(sizeof(char)*1000);
		strcpy(undoCommand, "del ");
		strcat(undoCommand, name);
		strcat(undoCommand, command + indexStartFloat);
		if (opStyle == normal)
			undoStackPush(undoCommand),
			clearRedo();
		else if (opStyle == undoAction)
			redoStackPush(undoCommand);
		else
			undoStackPush(undoCommand);
	}
	free(name);
	return "\n";
}
const char* commandHandling(char command[], int opStyle);
const char* servFilter(char command[]) {
	// here i filter the medicines by stock
	for (int i = 7; i < strlen(command); i++) {
		if (command[i]<'0'||command[i]>'9')
			return "filter - filter medicines by low stock: filter <stock>\n";
	}
	int* temp = (int*)malloc(sizeof(int));
	med* meds = getAllMedicines(repo, temp);
	int len = temp[0];
	free(temp);
	char res[11111];
	res[0] = '\0';
	if (len == 0) {return "No medicines!\n";}
	qsort(meds, len, sizeof(med), comp);
	int i = 0;
	if(command[6]!=' ')
		return "filter - filter medicines by low stock: filter <stock>\n";
	int j = 7;
	while (command[j] != '\0' && (command[j] <= '9' && command[j] >= '0')) {
		j++;
	}
	if (command[j] != NULL) {
		return "ERROR: Invalid command syntax!\n";
	}
	int quantity = 0;
	for (int i=7;i<j;i++)
		quantity = quantity * 10 + command[i] - '0';
	for (int i = 0; i < len; i++) {
		if (meds[i].quantity>quantity)
			continue;
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
	return res;
}
const char* servRedo(char command[]) {
	// here i redo the last operation
	if (redoStack == NULL) {
		return "No more redos!\n";
	}
	commandHandling(redoStack->command, redoAction);
	undostack* temp = redoStack;
	redoStack = redoStack->next;
	free(temp->command);
	free(temp);
	return "\n";
}
const char* servUndo(char command[]) { 
	// here i undo the last operation
	if (undoStack == NULL) {
		return "No more undos!\n";
	}
	commandHandling(undoStack->command,undoAction);
	undostack* temp = undoStack;
	undoStack = undoStack->next;
	free(temp->command);
	free(temp);
	return "\n";
}
const char* servList(char command[]) {
	// here i list all the medicines
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
		name = (char*)malloc(sizeof(char) * (i - 4));
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

		free(name);
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
const char* servDel(char command[], int opType) {
	// here i delete a medicine
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
	med* m = getMedicine(repo, name, concentration);
	if (m == NULL) {
		return "Medicine not found!\n";
	}
	char* cmd = (char*)malloc(sizeof(char) * 1111);
	cmd[0] = '\0';
	strcat(cmd, "add ");
	strcat(cmd, m->name);
	strcat(cmd, " ");
	char buffer[100];
	itoa (m->price, buffer, 10);
	strcat(cmd, buffer);
	strcat(cmd, " ");
	itoa(m->quantity, buffer, 10);
	strcat(cmd, buffer);
	strcat(cmd, " ");
	strcat(cmd, command + start + 1);
	if (opType == 1)
		undoStackPush(cmd),
		clearRedo();
	else if (opType == 2) 
		redoStackPush(cmd);
	else
		undoStackPush(cmd);
	repoDeleteMedicine(repo, name, concentration);
	return "\n";
}
const char* servUpd(char command[],int opType) {
	// here i update a medicine
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
	int startfloat = i + 1;
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
	int endfloat = i;
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
	med*medicine = getMedicine(repo, name, concentration);
	if (medicine == NULL)
		return "Medicine not found!\n";

	char* cmd = (char*)malloc(sizeof(char) * 1111);
	cmd[0] = '\0';
	strcat(cmd, "upd ");
	strcat(cmd, medicine->name);
	strcat(cmd, " ");
	command[endfloat-1]= '\0';
	strcat(cmd, command + startfloat);
	strcat(cmd, " ");
	char buffer[100];
	itoa(medicine->price, buffer, 10);
	strcat(cmd, buffer);
	strcat(cmd, " ");
	itoa(medicine->quantity, buffer, 10);
	strcat(cmd, buffer);
	if (opType == 1)
		undoStackPush(cmd),
		clearRedo();
	else if (opType == 2)
		redoStackPush(cmd);
	else
		undoStackPush(cmd);
	repoUpdateMedicine(repo, name, price, quantity, concentration);
	return "Medicine updated!\n";
}
void clearing() {
	// here i clear the allocated memory
	deleteRepo(repo);
	while (undoStack != NULL)
		undoStackPop();
	while (redoStack != NULL)
		redoStackPop();
}
const char* commandHandling(char command[],int opStyle) {
	// here i handle the commands
	if (strchr(command, '-') != NULL)
		return "Invalid command syntax!\n";
	if (command[0] == 'a' && command[1] == 'd' && command[2] == 'd') 
		return servAdd(command, opStyle);
	else if (command[0] == 'l' && command[1] == 'i' && command[2] == 's' && command[3] == 't')
		return servList(command);
	else if (command[0] == 'd' && command[1] == 'e' && command[2] == 'l')
		return servDel(command, opStyle);
	else if (command[0] == 'u' && command[1] == 'p' && command[2] == 'd')
		return servUpd(command, opStyle);
	else if (command[0] == 'r' && command[1] == 'e' && command[2] == 'd'&& command[3] == 'o')
		return servRedo(command);
	else if (command[0] =='u' && command[1] =='n'&& command[2] =='d'&& command[3] =='o')
		return servUndo(command);
	else if (command[0] =='f' && command[1] =='i'&& command[2] =='l'&& command[3] =='t'&& command[4] =='e'&& command[5] =='r') 
		return servFilter(command);
	else return "No command found!\n";

}
// test all the functions


void testAdd() {
	// here i test the add function
	repo = createRepo();
	commandHandling("add paracetamol 10 10", normal);
	assert (repo->length == 0);
	commandHandling("add paracetamol 10 10 10", normal);
	assert (repo->length == 1);
	commandHandling("add paracetamol 10 10 10", normal);
	assert (repo->length == 1);
	commandHandling("add paracetamol 10 10 12", normal);
	assert (repo->length == 2);
}
void testDelete() {
	repo = createRepo();
	commandHandling("add paracetamol 10 10", normal);
	commandHandling("add paracetamol 10 10 10", normal);
	commandHandling("add paracetamol 10 10 10", normal);
	commandHandling("add paracetamol 10 10 12", normal);
	commandHandling("del paracetamol 10", normal);
	assert (repo->length == 1);
	commandHandling("del paracetamol 10", normal);
	assert (repo->length == 1);
}
void testUpdate() {
	// here i test the update function
	char command[100];
	repo = createRepo();
	commandHandling("add paracetamol 10 10 10", normal);
	commandHandling("add paracetamol 10 10 10", normal);
	commandHandling("add paracetamol 10 10 12", normal);
	strcpy(command,"upd paracetamol 12 34 56");
	commandHandling(command, normal);
	assert (repo->medicine[0].quantity == 20);
	strcpy(command, "upd paracetamol 10 30 30");
	commandHandling(command, normal);
	assert (repo->medicine[0].quantity == 30);
}
void testUndo() {
	repo = createRepo();
	commandHandling("add paracetamol 10 10 10", normal);
	commandHandling("undo", normal);
	assert(repo->length == 0);
}
void testRedo() {
	repo = createRepo();
	commandHandling("add paracetamol 10 10 10", normal);
	commandHandling("undo", normal);
	commandHandling("redo", normal);
	assert(repo->length == 1);

}
void testAll() {
	// here i test all the functions
	testAdd();
	testDelete();
	testUpdate();
	testUndo();
	testRedo();
}