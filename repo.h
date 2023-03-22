#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<cstring>
#define _CRT_SECURE_NO_WARNINGS 1
#include "domain.h"
#define normal 1
#define undoAction 2
#define redoAction 3
struct repository {
	med* medicine;
	int length;
	int capacity;
};

repository* createRepo() {
	// here we allocate memory for the repository
	repository* repo = (repository*)malloc(sizeof(struct repository));
	repo->medicine = (struct med*)malloc(sizeof(struct med) * 10);
	repo->length = 0;
	repo->capacity = 10;
	return repo;
}

void deleteRepo(struct repository* repo) {
	// here we free the memory allocated for the repository
	for (int i = 0; i < repo->length; i++) {
		free(repo->medicine[i].name);
	}
	free(repo->medicine);
	free(repo);
}

bool repoaddMedicine(repository* repo, char* name, int price, int quantity, float concentration) {
	// here we add a medicine to the repository
	if (repo->length == repo->capacity) {
		repo->capacity *= 2;
		repo->medicine = (struct med*)realloc(repo->medicine, sizeof(struct med) * repo->capacity);
	}
	bool found = false;
	for (int i = 0; i < repo->length; i++) {
		if (strcmp(repo->medicine[i].name, name) == 0 && repo->medicine[i].concentration == concentration) {
			found = true;
			repo->medicine[i].quantity += quantity;
			break;
		}
	}
	if (!found) {
		repo->medicine[repo->length].name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
		strcpy_s(repo->medicine[repo->length].name, sizeof(char) * (strlen(name) + 1), name);
		repo->medicine[repo->length].price = price;
		repo->medicine[repo->length].quantity = quantity;
		repo->medicine[repo->length].concentration = concentration;
		repo->length++;
	}
	return found;
}

med* getMedicine(struct repository* repo, char* name, float concentration) {\
	// here we get a medicine from the repository
	for (int i = 0; i < repo->length; i++) {
		if (strcmp(repo->medicine[i].name, name) == 0 && repo->medicine[i].concentration == concentration) {
			return &repo->medicine[i];
		}
	}
	return NULL;
}

med* getAllMedicines(struct repository* repo, int* len) {
	// here we get all the medicines from the repository
	*len = repo->length;
	return repo->medicine;
}

bool repoDeleteMedicine(struct repository* repo, char* name, float concentration) {
	// here we delete a medicine from the repository
	for (int i = 0; i < repo->length; i++) {
		if (strcmp(repo->medicine[i].name, name) == 0 && repo->medicine[i].concentration == concentration) {
			free(repo->medicine[i].name);

			for (int j = i; j < repo->length - 1; j++) {
				repo->medicine[j] = repo->medicine[j + 1];
			}
			repo->length--;
			return true;
		}
	}
	return false;
}

bool repoUpdateMedicine(struct repository* repo, char* name, int price, int quantity, float concentration) {
	// here we update a medicine from the repository
	for (int i = 0; i < repo->length; i++) {
		if (strcmp(repo->medicine[i].name, name) == 0 && concentration == repo->medicine[i].concentration) {
			repo->medicine[i].price = price;
			repo->medicine[i].quantity = quantity;
			repo->medicine[i].concentration = concentration;
			return true;
		}
	}
	return false;
}
