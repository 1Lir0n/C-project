#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include "Library.h"


//A.
// definitions and simple functions (Library.h)


//B.

//prints provided member data
void print_member(struct LibMember* member) {
	printf("\nName: %s\n", member->Name);
	printf("Id: %s\n", member->Id);
	printf("Date of birth: %.2d/%.2d/%.4d\n", member->DateOfBirth.Day, member->DateOfBirth.Month, member->DateOfBirth.Year);
	printf("Loaned Books: %d\n", member->nBooks);
	for (int i = 0; i < member->nBooks; i++) {
		printf("Book: %s, by: %s, untill: %d/%d/%d\n", member->LoanBooks[i].BookName, member->LoanBooks[i].AuthorName,
			member->LoanBooks[i].ReturnDate.Day, member->LoanBooks[i].ReturnDate.Month, member->LoanBooks[i].ReturnDate.Year);
	}
	printf("");
}



//C.

//return the index of the member with the provided id | -1 if wasn't found
int search_id(struct LibMember members[], int n, char id[]) {
	for (int i = 0; i < n; i++) {
		if (strcmp(members[i].Id, id) == 0) return i;
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
void add_member(struct LibMember members[], int* n) {

	if (*n == 300) {
		printf("Exceeded the allowed member count.\n");
		return;
	}
	struct Date date = { 0,0,0 };
	char buffer[NAME_BUFF] = { 0 };

	//Name	

	clear();
	do {
		printf("Enter member's full name: \n");
		fgets(buffer, sizeof(buffer), stdin); // Read full name from user input
		buffer[strcspn(buffer, "\n")] = '\0';
		if (!only_letters_and_spaces(buffer)) {
			printf("Please input only letters and spaces.\n\n");
		}
	} while (!only_letters_and_spaces(buffer));

	// Dynamically allocate memory for the full name
	members[*n].Name = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
	if (members[*n].Name == NULL) //malloc check
		memoryError();

	// Copy the name from the buffer to the allocated memory
	strcpy(members[*n].Name, buffer);
	capitalize(members[*n].Name);

	//id
	char id[ID_BUFF] = { 0 };
	do {
		id[0] = '\0';
		printf("Enter ID:\n");
		scanf("%11s", id);
		clear();
		id[strcspn(id, "\n")] = '\0';

		if (!valid_id(id)) {
			printf("Please enter a 9 digit number.\n\n");
		}
	} while (!valid_id(id));

	if (id_exist(members, *n, id)) {
		printf("An account with this Id already exist. Index(starts from 0):%d\n", search_id(members, *n, id));
		return;
	}
	strcpy(members[*n].Id, id);

	//date of birth
	int d = 0, m = 0, y = 0;
	bool validDate = false;
	time_t Time;
	struct tm* localTime;
	time(&Time);
	localTime = localtime(&Time);
	do {
		printf("Enter date of birth. in this format: dd mm yyyy\n");
		if (scanf_s("%d %d %d", &d, &m, &y)) {
			if (!(y > localTime->tm_year + 1900 || y < 1900))
				if (y == localTime->tm_year + 1900) {
					if ((d > 0 && d <= localTime->tm_mday) && (m > 0 && m <= localTime->tm_mon + 1)) validDate = true;
				}
				else if ((d > 0 && d <= 30) && (m > 0 && m <= 12))
					validDate = true;
		}
		if (!validDate) {
			printf("Please enter a vaild date of birth.\n\n");
			clear();
		}
	} while (!validDate);

	date.Day = d;
	date.Month = m;
	date.Year = y;

	members[*n].DateOfBirth = date;

	//default 
	members[*n].nBooks = 0;

	(*n)++;

	//sort when done
	sort_members(members, *n);

}



//F.

//loan book for member via id
void loan_books(struct LibMember members[], int n) {
	char id[ID_SIZE] = { 0 };

	//asks for id and returns to menu if invalid
	if (!getId(id))
		return;

	//checks for member
	if (search_id(members, n, id) == -1) {
		printf("Member wasn't found.\n");
		return;
	}

	//save member to pointer for easy use
	struct LibMember* m = &members[search_id(members, n, id)];

	//print member data
	print_member(m);

	//checks if can loan more books
	if (m->nBooks == 4) {
		printf("Member reached the max loaned books allowed.\n");
		return;
	}

	char strBuffer[NAME_BUFF];

	printf("\nEnter book name: ");
	clear();
	fgets(strBuffer, sizeof(strBuffer), stdin); // Read book name from user input into buffer
	strBuffer[strcspn(strBuffer, "\n")] = '\0';

	// Dynamically allocate memory for the book name
	m->LoanBooks[m->nBooks].BookName = (char*)malloc((strlen(strBuffer) + 1) * sizeof(char));
	if (m->LoanBooks[m->nBooks].BookName == NULL) //malloc check
		memoryError();

	// Copy the name from the buffer to the allocated memory
	strcpy(m->LoanBooks[m->nBooks].BookName, strBuffer);
	capitalize(m->LoanBooks[m->nBooks].BookName);


	printf("Enter author name: ");
	fgets(strBuffer, sizeof(strBuffer), stdin); // Read author name from user input into buffer
	strBuffer[strcspn(strBuffer, "\n")] = '\0';
	// Dynamically allocate memory for the author name
	m->LoanBooks[m->nBooks].AuthorName = (char*)malloc((strlen(strBuffer) + 1) * sizeof(char));
	if (m->LoanBooks[m->nBooks].AuthorName == NULL) //malloc check
		memoryError();

	// Copy the name from the buffer to the allocated memory
	strcpy(m->LoanBooks[m->nBooks].AuthorName, strBuffer);
	capitalize(m->LoanBooks[m->nBooks].AuthorName);


	// Declare a variable to hold the current time
	time_t currentTime;

	// Get the current time
	time(&currentTime);

	// Convert the current time to a struct tm
	struct tm* localTime = localtime(&currentTime);
	int d = localTime->tm_mday;//get the day
	localTime->tm_mday = d + 31;//move 1 month ahead
	mktime(localTime);//make it a true date
	struct Date rDate = { d, localTime->tm_mon + 1, localTime->tm_year + 1900 };
	m->LoanBooks[m->nBooks].ReturnDate = rDate;

	m->nBooks++;//update number of loaned books

}



//G

void return_book(struct LibMember members[], int n) {
	char id[ID_SIZE] = { 0 };
	char strBuffer[NAME_BUFF] = { 0 };
	bool bookFound = false;

	//first finds and show the member:

	//asks for id and returns to menu if invalid
	if (!getId(id))
		return;
	//finds member
	int index = search_id(members, n, id);

	if (index < 0) {
		printf("A member with the ID: %s was not found.\n", id);
		return;
	}
	//prints member info:
	print_member(&members[index]);

	if (members[index].nBooks == 0) {
		printf("The member doesn't have any loaned books.\n");
		return;
	}
	else {
		printf("Enter the name of the book to return:\n");
		clear();
		fgets(strBuffer, sizeof(strBuffer), stdin);
		strBuffer[strcspn(strBuffer, "\n")] = '\0';
		char* name = (char*)malloc((strlen(strBuffer) + 1) * sizeof(char));
		if (name == NULL) //malloc check
			memoryError();

		strcpy(name, strBuffer);
		capitalize(name);

		//move all books to new array(except return) 
		//(it is faster to search for book and create new array together)
		struct Book loan[MAX_BOOKS] = { 0 };
		int j = 0;
		for (int i = 0; i < members[index].nBooks; i++) {
			if (strcmp(members[index].LoanBooks[i].BookName, name) != 0) {
				loan[j] = members[index].LoanBooks[i];
				j++;
			}
			else {
				bookFound = true;
				free(members[index].LoanBooks[i].BookName);
				free(members[index].LoanBooks[i].AuthorName);
			}
		}

		if (bookFound) {
			members[index].nBooks -= 1;
			*members[index].LoanBooks = *loan;
			printf("The book: %s was removed successfully.\n", name);
		}
		else {
			printf("The book: %s is not on loan.\n", name);
		}

		free(name);
	}

}



//H.

//prints all overdue books and its members
void check_book_overdue(struct LibMember members[], int n) {
	struct tm date = { 0 };
	int cnt[MAX_BOOKS] = { 0 };
	bool over = false;
	//check members
	for (int i = 0; i < n; i++) {
		//check book dates
		for (int j = 0; j < members[i].nBooks; j++) {
			date.tm_mday = members[i].LoanBooks[j].ReturnDate.Day;
			date.tm_mon = members[i].LoanBooks[j].ReturnDate.Month - 1;
			date.tm_year = members[i].LoanBooks[j].ReturnDate.Year - 1900;
			time_t time1 = mktime(&date);
			time_t time2 = time(NULL);
			if (time1 < time2) {
				cnt[j] = 1;
				over = true;
			}
		}
		if (contains_one(cnt, 4))
		{
			printf("\n");
			for (int j = 0; j < MAX_BOOKS; j++) {
				if (cnt[j] == 1) {
					printf("Book: %s, by: %s is overdue.\n", members[i].LoanBooks[j].BookName, members[i].LoanBooks[j].AuthorName);
					cnt[j] = 0;
				}
			}
			print_member(&members[i]);
		}
	}
	if (!over) {
		printf("There aren't any overdue books.\n");
	}
}



//I.

//delete a member via ID
void delete_member(struct LibMember members[], int* nMem) {
	char id[ID_SIZE] = { 0 };

	//asks for id and returns to menu if invalid
	if (!getId(id))
		return;

	// Find the index of the member with the provided ID
	int index = search_id(members, *nMem, id);

	if (index == -1) {
		printf("Member with ID %s not found.\n", id);
		return;
	}
	printf("\nThe member:\n ");
	print_member(&members[index]);
	// Free memory for the member to be deleted
	free(members[index].Name);

	// If the member to be deleted is not the last member, move the last member to its position
	if (index < *nMem - 1) {
		members[index] = members[*nMem - 1];
	}

	// Reduce the count of total members
	(*nMem)--;
	sort_members(members, *nMem);
	printf("\nwas deleted successfully.\n");
}



//J.

//prints the array data
void print_members(struct LibMember members[], int n) {
	printf("\nNumber of members: %d\n", n);
	for (int i = 0; i < n; i++) {
		print_member(&members[i]);
	}
}



//K.

//free allocated memory and quit the program
void quit(struct LibMember members[], int nMem) {
	// Free dynamically allocated memory for each member's name
	for (int i = 0; i < nMem; i++) {
		for (int j = 0; j < members[i].nBooks; j++) {
			free(members[i].LoanBooks[j].BookName);
			free(members[i].LoanBooks[j].AuthorName);
		}
		free(members[i].Name);
	}

	printf("Have a nice day!\n");
	printf("Quitting the program...\n");
	exit(0);
}



//L

//program exe
int main() {
	struct LibMember members[300];
	int nMem = 0;    // number of current members can't exceed 300
	int userInput = 0;
	while (true) {
		userInput = menu();
			 if (userInput == 1) add_member(members, &nMem);
		else if (userInput == 2) loan_books(members, nMem);
		else if (userInput == 3) return_book(members, nMem);
		else if (userInput == 4) check_book_overdue(members, nMem);
		else if (userInput == 5) delete_member(members, &nMem);
		else if (userInput == 6) print_members(members, nMem);
		else if (userInput == 7) quit(members, nMem);
	}
	return 0;
}