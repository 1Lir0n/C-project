#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include "Library.h"



//A. Inside of Library.h



//B.

//prints provided member data
void print_member(struct LibMember* mem) {
	printf("\nName: %s\n", mem->Name);
	printf("Id: %s\n", mem->Id);
	printf("Date of birth: %.2d/%.2d/%.4d\n", mem->DateOfBirth.Day, mem->DateOfBirth.Month, mem->DateOfBirth.Year);
	printf("Loaned Books: %d\n", mem->nBooks);
	for (int i = 0; i < mem->nBooks; i++) {
		printf("Book: %s, by: %s, untill: %d/%d/%d\n", mem->LoanBooks[i].BookName, mem->LoanBooks[i].AuthorName,
			mem->LoanBooks[i].ReturnDate.Day, mem->LoanBooks[i].ReturnDate.Month, mem->LoanBooks[i].ReturnDate.Year);
	}
	printf("");
}



//C.

//return the index of the member with the provided id | -1 if didnt found
int search_id(struct LibMember mem[], int n, char id[]) {
	for (int i = 0; i < n; i++) {
		if (strcmp(mem[i].Id, id) == 0) return i;
	}
	return -1;
}



//D. 

//prints a menu and awaits user input
int menu() {
	printf("-------------------------\n");
	printf("\033[4mDatabase System Menu\033[0m\n");
	printf("  1. Add member\n");
	printf("  2. Loan books\n");
	printf("  3. Return books\n");
	printf("  4. Check book overdue\n");
	printf("  5. Delete member\n");
	printf("  6. Display all members\n");
	printf("  7. Quit\n");
	printf("-------------------------\n");

	int response = 0;
	scanf("%d", &response);

	if (!(response < 8 && response>0)) {
		printf("Wrong input please provide a number between 1-7.\n\n");
		clear();
		response = menu();
	}
	return response;
}



//E.

//adds member data to the total members and sorts it
void add_member(struct LibMember mem[], int *n) {

	if (n == 300) {
		printf("Exceeded the allowed member count.\n");
		return;
	}
	struct Date date = {0,0,0};
	char buffer[100] = { 0 };

	//Name
	do {
		printf("Enter member's full name: \n");
		clear();
		fgets(buffer, sizeof(buffer), stdin); // Read full name from user input
		buffer[strcspn(buffer, "\n")] = '\0';
		if(!only_letters_and_spaces(buffer)){
			printf("Please input only letters and spaces.\n\n");
		}
	} while (!only_letters_and_spaces(buffer));
	
	// Dynamically allocate memory for the full name
	mem[*n].Name = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
	
	// Copy the name from the buffer to the allocated memory
	strcpy(mem[*n].Name, buffer);
	capitalize(mem[*n].Name);

	//id
	char id[20];
	do {
		printf("Enter ID:\n");
		fgets(id, sizeof(id), stdin);
		if (!valid_id(id) || strlen(id) > 9) {
			printf("Please enter a 9 digit number.\n\n");
		}
	} while (!valid_id(id));
	clear();
	if (id_exist(mem, *n, id)) {
		printf("An account with this Id already exist. Index(from 0):%d\n", search_id(mem, n, id));
		return;
	}
	strcpy(mem[*n].Id, id);

	//date of birth
	int d=0, m=0, y=0;
	do {
		printf("Enter date of birth. in this format: dd mm yyyy\n");
		scanf_s("%d %d %d", &d, &m, &y);
		if (!((d > 0 && d <= 30) && (m > 0 && m <= 12) && (y >= 1900)))
			printf("Please enter a vaild date of birth.\n\n");
	} while (!((d > 0 && d <= 30) && (m > 0 && m <= 12) && (y >= 1900)));
	date.Day = d;
	date.Month = m;
	date.Year = y;
	mem[*n].DateOfBirth = date;
	
	//default 
	mem[*n].nBooks = 0;

	(*n)++;

	//sort when done
	sort_members(mem, *n);

}



//F.

//loan book for member via id
void loan_books(struct LibMember mem[], int n) {
	char id[10] = { 0 };
	printf("Enter ID: ");
	scanf("%s", &id);
	//checks for member
	if (search_id(mem, n, id) == -1) {
		printf("Member wasn't found.\n");
		return;
	}

	//save member to pointer for easy use
	struct LibMember* m = &mem[search_id(mem, n, id)];

	//print member data
	print_member(m);

	//checks if can loan more books
	if (m->nBooks == 4) {
		printf("Member reached the max loaned books allowed.");
		return;
	}
	char buffer[100];
	printf("Enter book name: ");
	clear();
	fgets(buffer, sizeof(buffer), stdin); // Read full name from user input
	buffer[strcspn(buffer, "\n")] = '\0';
	// Dynamically allocate memory for the full name
	m->LoanBooks[m->nBooks].BookName = (char*)malloc((strlen(buffer) + 1) * sizeof(char));

	// Copy the name from the buffer to the allocated memory
	strcpy(m->LoanBooks[m->nBooks].BookName, buffer);
	capitalize(m->LoanBooks[m->nBooks].BookName);

	printf("Enter author name: ");
	fgets(buffer, sizeof(buffer), stdin); // Read full name from user input
	buffer[strcspn(buffer, "\n")] = '\0';
	// Dynamically allocate memory for the full name
	m->LoanBooks[m->nBooks].AuthorName = (char*)malloc((strlen(buffer) + 1) * sizeof(char));

	// Copy the name from the buffer to the allocated memory
	strcpy(m->LoanBooks[m->nBooks].AuthorName, buffer);
	capitalize(m->LoanBooks[m->nBooks].AuthorName);

	
	// Declare a variable to hold the current time
	time_t currentTime;

	// Get the current time
	time(&currentTime);

	// Convert the current time to a struct tm
	struct tm* localTime = localtime(&currentTime);
	int d = localTime->tm_mday;//get the day
	if (d == 31) d = 30;//normalize to 30
	localTime->tm_mday = d + 30;//move 1 month ahead
	mktime(localTime);//make it a true date
	struct Date rDate = { d, localTime->tm_mon+1, localTime->tm_year + 1900 };
	m->LoanBooks[m->nBooks].ReturnDate = rDate;
	
	m->nBooks++;//update number of loaned books
}



//G

void return_book(struct LibMember mem[], int n) {
	char id[10] = { 0 };
	char buffer[100] = { 0 };

	printf("Enter member's id:\n");
	scanf("%s", id);

	int index = search_id(mem, n, id);

	if (index < 0) {
		printf("A member with the ID: %s was not found.\n", id);
		return;
	}

	print_member(&mem[index]);

	if (mem[index].nBooks == 0) {
		printf("The member doesn't have any loaned books.\n");
		return;
	}
	else {
		printf("Enter the name of the book to return:\n");
		clear();
		fgets(buffer, sizeof(buffer), stdin);
		buffer[strcspn(buffer, "\n")] = '\0';
		char* name = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(name, buffer);
		capitalize(name);

		struct Book loan[4] = { 0 };
		int j = 0;
		for (int i = 0; i < mem[index].nBooks;i++) {
			if (strcmp(mem[index].LoanBooks[i].BookName, name) != 0) {
				loan[j] = mem[index].LoanBooks[i];
				j++;
			}
		}
		
		mem[index].nBooks-=1;
		*mem[index].LoanBooks = *loan;
		printf("The book: %s was removed successfully.\n", name);
	}
}



//H.

//prints all overdue books and its members
void check_book_overdue(struct LibMember mem[],int n) {
	struct tm date = { 0 };
	int cnt[4] = { 0 };
	bool over = false;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < mem[i].nBooks; j++) {
			date.tm_mday = mem[i].LoanBooks[j].ReturnDate.Day;
			date.tm_mon = mem[i].LoanBooks[j].ReturnDate.Month;
			date.tm_year = mem[i].LoanBooks[j].ReturnDate.Year;
			time_t time1 = mktime(&date);
			time_t time2 = mktime(&localtime);
			if (time1 < time2) {
				cnt[j] = 1;
				over = true;
			}
		}
		if (contains_one(cnt, 4))
		{
			for (int j = 0; j < 4; j++) {
				if (cnt[j] == 1) {
					printf("Book: %s, by: %s is overdue.\n", mem[i].LoanBooks[j].BookName, mem[i].LoanBooks[j].AuthorName);
				}
			}
			print_member(&mem[i]);
		}
	}
	if (!over) {
		printf("There aren't any overdue books.\n");
	}
}



//I.

//delete a member via ID
void delete_member(struct LibMember mem[], int* nMem) {
	char id[10] = {0};
	printf("Enter the member's ID you would like to delete: ");
	scanf("%s", id);

	// Find the index of the member with the provided ID
	int index = search_id(mem, *nMem, id);

	if (index == -1) {
		printf("Member with ID %s not found.\n", id);
		return;
	}
	printf("\nThe member:\n ");
	print_member(&mem[index]);
	// Free memory for the member to be deleted
	free(mem[index].Name);

	// If the member to be deleted is not the last member, move the last member to its position
	if (index < *nMem - 1) {
		mem[index] = mem[*nMem - 1];
	}

	// Reduce the count of total members
	(*nMem)--;
	sort_members(mem, *nMem);
	printf("\nwas deleted successfully.\n");
}



//J.

//prints the array data
void print_members(struct LibMember mem[], int n) {
	printf("\nNumber of members: %d\n", n);
	for (int i = 0; i < n; i++) {
		print_member(&mem[i]);
	}
}



//K.

//free allocated memory and quit the program
void quit(struct LibMember mem[], int nMem) {
	// Free dynamically allocated memory for each member's name
	for (int i = 0; i < nMem; i++) {
		free(mem[i].Name);
	}

	printf("Quitting the program...\n");
	exit(0);
}



//L

//program exe
int main() {
	struct LibMember mem[300];
	int nMem = 0;    // number of current members can't exceed 300
	int userInput = 0;
	while (true) {
		userInput = menu();
			 if (userInput == 1) add_member(mem, &nMem);
		else if (userInput == 2) loan_books(mem, nMem);
		else if (userInput == 3) return_book(mem,nMem);
		else if (userInput == 4) check_book_overdue(mem, nMem);
		else if (userInput == 5) delete_member(mem,&nMem);
		else if (userInput == 6) print_members(mem, nMem);
		else if (userInput == 7) quit(mem,nMem);
	}
}