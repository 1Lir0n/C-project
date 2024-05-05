#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#ifndef LIBRARY_H
#define LIBRARY_H

#define DEBUG_PRINT printf("debug here.")
#define MAX_BOOKS 4
#define ID_SIZE 10
#define ID_BUFF 11
#define NAME_BUFF 100

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
	char Id[ID_SIZE]; //str
	struct Date DateOfBirth; //date
	int nBooks; //int | starts at 0
	struct Book LoanBooks[MAX_BOOKS]; //array of books
};

//capitalize words in string
void capitalize(char str[]);

//checks if string is made from only letters and spaces
bool only_letters_and_spaces(char* text);

//checks if the provided id is valid (9 digits)
bool valid_id(char Id[]);

//for registerd members, asks for id and checks it
bool getId(char id[]);


//checks if given id is used already
bool id_exist(struct LibMember mem[], int n, char id[]);

// Function to swap two library members
void swap_members(struct LibMember* a, struct LibMember* b);

//sorts members by ID
void sort_members(struct LibMember mem[], int n);

//return true if 1 is found and false if not
bool contains_one(const int array[], int size);

//clears lingering text data
void clear();

//if we cannot allocate memmory the program will close.
void memoryError();

#endif //LIBRARY_H