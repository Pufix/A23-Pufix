struct med {
	char* name;
	int price;
	int quantity;
	float concentration;
};
struct undostack {
	char* command;
	undostack* next;
};