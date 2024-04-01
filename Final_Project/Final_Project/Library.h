#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

//Date object| dd/mm/yyyy
struct Date
{
	int Day; //max 30 assume
	int Month; //1-12
	int Year; //4 digits
};

//Book object| name, author name, return date 
struct Book
{
	char* BookName; //str
	char* AuthorName; //str
	struct Date ReturnDate; //date
};

//Library member object| name, id, date of birth,number of books(max 4), books data
struct LibMember
{
	char* Name; //str
	char Id[10]; //str
	struct Date DateOfBirth; //date
	int nBooks; //int | starts at 0
	struct Book LoanBooks[4]; //array of books
};

//capitalize words in string
void capitalize(char str[]) {
	for (int i = 0; str[i] != '\0'; i++) {
		//check first character is lowercase alphabet
		if (i == 0) {
			if ((str[i] >= 'a' && str[i] <= 'z'))
				str[i] = str[i] - 32; //subtract 32 to make it capital
			continue; //continue to the loop
		}
		if (str[i] == ' ') //check space
		{
			//if space is found, check next character
			++i;
			//check next character is lowercase alphabet
			if (str[i] >= 'a' && str[i] <= 'z') {
				str[i] = str[i] - 32; //subtract 32 to make it capital
				continue; //continue to the loop
			}
		}
		else {
			//all other uppercase characters should be in lowercase
			if (str[i] >= 'A' && str[i] <= 'Z')
				str[i] = str[i] + 32; //subtract 32 to make it small/lowercase
		}
	}
}

//checks if string is madee of only letters and spaces
bool only_letters_and_spaces(const char* text) {
	while (*text != '\0') {
		// Check if the character is an uppercase letter (A-Z)
		if ((*text >= 'A' && *text <= 'Z') ||
			// Check if the character is a lowercase letter (a-z)
			(*text >= 'a' && *text <= 'z') ||
			// Check if the character is a space
			(*text == ' ')) {
			text++;
		}
		else {
			return false;
		}
	}
	return true;
}

//checks if the provided id is valid (9 digits)
bool valid_id(char Id[]) {
	if (strlen(Id) > 9) {
		return false;
	}
	//checks if the char is comprised of only digits
	for (int i = 0; i < 9; i++)
	{
		if (!isdigit(Id[i]))
			return false;
	}
	return true;
}

//checks if given id is used already
bool id_exist(struct LibMember mem[], int n, char id[]) {
	for (int i = 0; i < n; i++) {
		if (strcmp(mem[i].Id, id) == 0) {
			return true;
		}
	}
	return false;
}

// Function to swap two library members
void swap_members(struct LibMember* a, struct LibMember* b) {
	struct LibMember temp = *a;
	*a = *b;
	*b = temp;
}

//sorts members by ID
void sort_members(struct LibMember mem[], int n) {
	int i, j;
	for (i = 0; i < n - 1; i++) {
		// Last i elements are already in place
		for (j = 0; j < n - i - 1; j++) {
			if (strcmp(mem[j].Id, mem[j + 1].Id) > 0 || mem[j].Id == 0) {
				// Swap if the current element is greater than the next
				swap_members(&mem[j], &mem[j + 1]);
			}
		}
	}
}

bool contains_one(const int array[], int size) {
	for (int i = 0; i < size; i++) {
		if (array[i] == 1) {
			return true; // If 1 is found, return true
		}
	}
	return false; // If no 1 is found, return false
}

//clears lingering text data
void clear() {
	int c = 0;
	while ((c = getchar()) != '\n' && c != EOF);//remove any lingering data in line
}