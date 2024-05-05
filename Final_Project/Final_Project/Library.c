#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "Library.h"

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
			if (str[i] == '\0') return;
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

bool only_letters_and_spaces(char* text) {
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

bool valid_id(char Id[]) {
	if (strlen(Id) > ID_SIZE-1) {
		return false;
	}
	//checks if the char is comprised of only digits
	for (int i = 0; i < ID_SIZE-1; i++)
	{
		if (!isdigit(Id[i]))
			return false;
	}
	return true;
}

bool getId(char id[]) {
	printf("Enter member's id:\n");
	scanf("%10s", id);
	if (!valid_id(id)) {
		printf("not a valid id.\n");
		return 0;
	}
	return 1;
}

bool id_exist(struct LibMember mem[], int n, char id[]) {
	for (int i = 0; i < n; i++) {
		if (strcmp(mem[i].Id, id) == 0) {
			return true;
		}
	}
	return false;
}

void swap_members(struct LibMember* a, struct LibMember* b) {
	struct LibMember temp = *a;
	*a = *b;
	*b = temp;
}

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

void clear() {
	int c = 0;
	while ((c = getchar()) != '\n' && c != EOF);//remove any lingering data in line
}

void memoryError() {
	printf("Could not allocate memory \n Have a nice day!");
	exit(1);
}