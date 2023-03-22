#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <crtdbg.h>
#include <string.h>
#include "service.h"
#include "src.h"
#define _CRT_SECURE_NO_WARNINGS 1

void prtHelp() {
	printf("%s", "add - add a medicine: add <name> <price> <quantity> <concentration>\n");
	printf("%s", "del - delete a medicine: del <name> <concentration>\n");
	printf("%s", "update - update a medicine: upd <name> <concentration> <new price> <new quantity> \n");
	printf("%s", "list - list all medicines: list <subname>\n");
	printf("%s", "filter - filter medicines in low supply: filter <supply>\n");
	printf("%s", "quit/exit - exit the program\n");
}

void loop() {
	bool running = true;
	while (running) {
		char command[100];
		gets_s(command);
		int index = 0;
		while (command[index] != '\0')index++;
		index--;
		while (index > 0 && command[index] == ' ')command[index--] = '\0';
		if (command[0] == 'e' && command[1] == 'x' && command[2] == 'i' && command[3] == 't') {
			running = false;
			clearing();
		}
		else if (command[0] == 'q' && command[1] == 'u' && command[2] == 'i' && command[3] == 't') {
			running = false;
			clearing();
		}
		else if (command[0] == 'h' && command[1] == 'e' && command[2] == 'l' && command[3] == 'p') {
			prtHelp();
		}
		else {
			printf("%s", commandHandling(command,normal));
		}
	}
}
void initialData() {
	commandHandling("add paracetamol 27 100 28.5",normal);
	commandHandling("add ibuprofen 27 100 28.5", normal);
	commandHandling("add aspirin 27 100 28.5", normal);
	commandHandling("add paracetamol 27 100 20", normal);
	commandHandling("add paracetamol 27 100 20", normal);
	commandHandling("add paracetamol 27 100 10", normal);
	commandHandling("add lisinopril 30 10 5.5", normal);
}
int main() {
	makeRepo();
	initialData();
	loop();
	//char *DummyLeak = (char*)malloc(100);
	printf("\n\n\n\nLEAKS: %d \n\n\n\n\n\n",_CrtDumpMemoryLeaks());
	return 0;
}