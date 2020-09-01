#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"

typedef struct {
	char *firstName;
	char *lastName;
	char gender;
	int acNumber;
	double deposits;
} Holder;

typedef struct {
	Holder **holdersList; // Since it's a pointer to a list of Holders
	int maxHolders; // To use as a flag for appending the list
	int numOfHolders;
} BankReg;

// Constructor for bank register object
BankReg *BankReg_create();

// Constructor for account holder object
Holder *Holder_create(char *first, char *last, char gender, double money);

// Print full name of an account holder
void printFullName(Holder *curHolder);

// Open a new account in the bank
void createAccount(BankReg *registry);

// Accept valid gender input from the applicant
char genderInput();

// Main menu of the program's interface
void landingUI(BankReg *registry);

// Profile management page for account holders
void signIn(BankReg *registry);

// Check whether account number (the query param) exists in the bank register
int accountInReg(int query, BankReg *registry);

// Return total deposits in an account
double showAmount(Holder *curHolder);

// Print account information of a holder
void viewProfile(Holder *curHolder);

// Update account information of a holder
void editProfile(Holder *curHolder);

// Increase memory allocated to the holder list object in the register
void HoldersList_append(BankReg *registry);

// Delete an account
void deleteAccount(BankReg *registry);

// Decrease memory allocated to the holder list object in the register
void HoldersList_reduce(BankReg *registry);

// Print a separtor line
void separator();

// Make sure that kill methods return something so
// they don't move on to the error snippet

// Release memory allocated to a holder object
int Holder_kill(Holder *curHolder);

// Release memory allocated to the bank register object
int BankReg_kill(BankReg *registry);

int main(int argc, char *argv[]) {
	BankReg *registry = BankReg_create();

	printf("\t\tWelcome to Random Bank\n\n");

	landingUI(registry);

	return 0;
}

BankReg *BankReg_create() {
	BankReg *newReg = malloc(sizeof(BankReg));
	check_mem(newReg);

	newReg->maxHolders = 5;

	newReg->holdersList = malloc(sizeof(Holder) * newReg->maxHolders);
	check_mem(newReg->holdersList);

	newReg->numOfHolders = 0;

	return newReg;

error:
	exit(1);
}

Holder *Holder_create(char *first, char *last, char gender, double money) {
	Holder *newHolder = malloc(sizeof(Holder));
	check_mem(newHolder);

	newHolder->firstName = strdup(first);
	check_mem(newHolder->firstName);

	newHolder->lastName = strdup(last);
	check_mem(newHolder->lastName);

	newHolder->gender = gender;

	newHolder->acNumber = 1000;

	newHolder->deposits = money;

	return newHolder;

error:
	exit(1);
}

void printFullName(Holder *curHolder) {
	printf("%s %s\n", curHolder->firstName, curHolder->lastName);
}

void createAccount(BankReg *registry) {
	char firstName[20];
	char lastName[20];
	char gender;
	double amount;

	int flag = 1;
	char *prefix;

	printf("Enter first name (Max 20 characters)\n> ");
	scanf("%s", firstName);

	printf("\nEnter last name (Max 20 characters)\n> ");
	scanf("%s", lastName);

	while (flag != 0) {
		printf("\nPlease enter M or F.\n");
		gender = genderInput();

		if (gender == 'M' || gender == 'F')
			flag = 0;
	}

	if (gender == 'M')
		prefix = strdup("Mr");
	else
		prefix = strdup("Mrs/Ms");

	printf("\nEnter amount to be deposited\n> ");
	scanf("%lf", &amount);


	if (registry->maxHolders == registry->numOfHolders) {
		HoldersList_append(registry);
	}

	registry->holdersList[registry->numOfHolders] = Holder_create(firstName, lastName, gender, amount);
	registry->holdersList[registry->numOfHolders]->acNumber += registry->numOfHolders;

	registry->numOfHolders++;

	printf("Account successfully created, %s %s %s.\n\n", prefix, firstName, lastName);

	separator();

	free(prefix);
}

char genderInput() {
	char gender;
	printf("Enter your gender\n> ");
	scanf(" %c", &gender);

	return gender;
}

void landingUI(BankReg *registry) {
	int flag = 1;
	
	while(flag != 0) {
		int choice;

		printf("1. Create a new account\n");
		printf("2. Sign in to exisiting account\n");
		printf("3. Delete your account\n");
		printf("0. Exit\n");

		printf("Enter your choice: ");
		scanf("%d", &choice);

		separator();

		switch(choice) {
			case 1:
				createAccount(registry);
				break;
			case 2:
				signIn(registry);
				break;
			case 3:
				deleteAccount(registry);
				break;
			default:
				flag = 0;
				printf("Goodbye!\n");
		}
	}
	BankReg_kill(registry);
	exit(0);
}

void signIn(BankReg *registry) {
	system("clear");
	int query;

	printf("Enter your account number\n> ");
	scanf("%d", &query);

	if (accountInReg(query, registry) == 0) {
		printf("Please enter valid credentials.\n");
		separator();
		landingUI(registry);
	}

	separator();

	int flag = 1;
	int key = query - 1000;

	while (flag != 0) {
		printf("1. Check balance\n");
		printf("2. View personal information\n");
		printf("3. Edit personal information\n");
		printf("4. Sign out\n");

		int choice;

		printf("Enter your choice:\n> ");
		scanf("%d", &choice);

		switch(choice) {
			case 1:
				printf("Current balance: %.2f\n", showAmount(registry->holdersList[key]));
				separator();
				break;
			case 2:
				viewProfile(registry->holdersList[key]);
				break;
			case 3:
				editProfile(registry->holdersList[key]);
				break;
			default:
				flag = 0;
				printf("You have successfully signed out.\n");
		}
	}

	separator();

	landingUI(registry);
}

int accountInReg(int query, BankReg *registry) {
	int i;
	int len = registry->numOfHolders;

	for (i = 0; i < len; i++) {
		if (query == registry->holdersList[i]->acNumber)
			return 1;
	}
	
	return 0;
}

double showAmount(Holder *curHolder) {
	return curHolder->deposits;
}

void viewProfile(Holder *curHolder) {
	system("clear");

	char *prefix;

	if (curHolder->gender == 'M')
		prefix = strdup("Mr");
	else
		prefix = strdup("Mrs/Ms");

	printf("Full name: %s %s %s\n", prefix, curHolder->firstName, curHolder->lastName);
	printf("Account number: %d\n", curHolder->acNumber);
	printf("Total deposits: %.2f\n", curHolder->deposits);

	separator();

	free(prefix);
}

void editProfile(Holder *curHolder) {
	system("clear");
	char newFirst[20];
	char newLast[20];

	printf("Update first name\n> ");
	scanf("%s", newFirst);

	curHolder->firstName = strdup(newFirst);
	printf("First name updated!\n");

	printf("Update last name\n> ");
	scanf("%s", newLast);

	curHolder->lastName = strdup(newLast);
	printf("Last name updated!\n");

	separator();
}

void HoldersList_append(BankReg *registry) {
	registry->maxHolders *= 2;

	registry->holdersList = realloc(registry->holdersList, sizeof(Holder) * registry->maxHolders);
	check_mem(registry->holdersList);

	return;

error:
	exit(1);
}

void deleteAccount(BankReg *registry) {
	return;
}

void HoldersList_reduce(BankReg *registry) {
	registry->maxHolders /= 2;

	registry->holdersList = realloc(registry->holdersList, sizeof(Holder) * registry->maxHolders);
	check_mem(registry->holdersList);

	return;

error:
	exit(1);
}

void separator() {
	int i;
	for (i = 0; i < 50; i++)
		printf("=");
	printf("\n\n");
}

int Holder_kill(Holder *curHolder) {
	check_mem(curHolder);
	
	free(curHolder->firstName);
	free(curHolder->lastName);

	free(curHolder);

	// printf("Memory allocated to holder returned!\n");

	return 0;

error:
	exit(1);
}

int BankReg_kill(BankReg *registry) {
	check_mem(registry);

	int i;
	for (i = 0; i < registry->numOfHolders; i++) {
		// Free every Holder element before freeing the HolderList
		Holder_kill(registry->holdersList[i]);
	}

	free(registry->holdersList);

	free(registry);

	// printf("Memory allocated to registry returned!\n");

	return 0;

error:
	exit(1);
}

