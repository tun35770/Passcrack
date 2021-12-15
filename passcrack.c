#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>

//default output filename
char *DEFAULT_OUTPUT_FILENAME = "pcout.txt";

time_t start;

//array to contain passwords read from password file provided by user
char *passwords[100];
int passwords_size = 0;

//starting/ending length of passwords to check for
int startLen = 1, endLen = 24;

//holds current password being checked
char *password;

//output file from user (if provided)
FILE *outFile;

//flags
bool p = false;	//print to console
bool o = false; //output file provided
bool s = false;	//start length provided
bool e = false;	//end length provided

//functions
int comparePassToList();
char *nextPass();
void signal_handler(int signal);
void initPassword();

int main(int argc, char *argv[]){

	//too few arguments
	if(argc < 2){
		printf("passcrack.c ERROR: Too few arguments provided.\n./passcrack.c <list file> [output file] [flag(s)]\n");
		exit(1);
	}

	//start time
	start = clock();

	char* listFile = argv[1];

	FILE *f = fopen(listFile, "r");
	if(f == NULL){
		printf("passcrack.c ERROR: Failed to open %s for reading.\n", listFile);
		exit(1);
	}

	//output file name
	char *outFileName = DEFAULT_OUTPUT_FILENAME;

	//check for flags
	if(argc > 2){
		for(int i = 2; i < argc; i++){
			if(strcmp(argv[i], "-p") == 0){
				p = true;
			}
			if(strcmp(argv[i], "-o") == 0 && argc >= i+1){
				outFileName = argv[i+1];
				i++;
			}
			if(strcmp(argv[i], "-s") == 0 && argc >= i+1){
				startLen = atoi(argv[i+1]);
				i++;
			}
			if(strcmp(argv[i], "-e") == 0 && argc >= i+1){
				endLen = atoi(argv[i+1]);
				i++;
			}

		}
	}

	//initialize password with "startLen" a's
	initPassword();

	bool useDefaultOutputFile = (strcmp(outFileName, DEFAULT_OUTPUT_FILENAME) == 0);

	if(!useDefaultOutputFile){
		outFile = fopen(outFileName, "w");
		if(outFile == NULL){
			printf("passcrack.c ERROR: Failed to open %s, using default output file pcout.txt\n", outFileName);
			useDefaultOutputFile = true;
		}
	}

	//use default output file
	if(useDefaultOutputFile){
		outFile = fopen(outFileName, "w");
		if(outFile == NULL){
			printf("passcrack.c ERROR: Failed to open default output file\n");
			exit(1);
		}
	}


	char* buffer;
	size_t buffer_size = 0;

	//read passwords from passwrod file, saving to passwords[]
	while(getline(&buffer, &buffer_size, f) != -1){
		buffer[strcspn(buffer, "\n")] = 0; //remove \n
		passwords[passwords_size] = malloc(strlen(buffer));
		strcpy(passwords[passwords_size], buffer); //copy word into passwords array
		passwords_size++;
	}

	//holds index of password in pass file if a password is found
	int index;

	//set SIGINT handler
	if(signal(SIGINT, signal_handler) == SIG_ERR){
		printf("passcrack.c ERROR: Failed to set signal handler\n");
		exit(1);
	}


	/* ----- MAIN LOOP ----- */
	while(strlen(password) < endLen){
		index = comparePassToList();	//see if password is in pass file

		if(index != -1){		//password found!
			time_t time = clock();

			fprintf(outFile, "TIME: %ld	%s at index %d\n", time, password, index);
			if(p)
				printf("TIME: %ld	%s at index %d\n", time, password, index);
		}

		nextPass();	//increment password
	}
}

//looks in password list for this password
int comparePassToList(){

	int passIndex = -1;

	for(int i = 0; i < passwords_size; i++){
		if(strcmp(passwords[i], password) == 0){
			passIndex = i;
			break;
		}
	}

	return passIndex;
}

//increments the password by a single letter
char *nextPass(){

	int numZ = 0;
	int length = strlen(password);

	//find out how many z's are at the end, thus which letter to increment
	if(password[length - 1] == 'z'){
		for(int i = length - 1; i >= 0; i--){
			if(password[i] == 'z'){
				numZ++;
			}else break;
		}
	}

	char *temp;	//build the new password and save it here
	char letter;	//the letter to be incremented will be here


	//all letters are z, so make new password all a's (with one extra)
	if(numZ == length){
		temp = malloc(length+1);
		char a = 'a';

		for(int i = 0; i < numZ + 1; i++){
			strcat(temp, &a);
		}
	}

	//typical case, simply increment one letter
	else{
		temp = malloc(length);
		strncat(temp, &password[0], length-numZ-1);//get substring from password

		memcpy(&letter, &password[length-numZ-1], 1);//save letter to increment
		letter++;				//increment letter

		strncat(temp, &letter, 1);		//append incremented letter to temp


		char a = 'a';
		for(int i = 0; i < numZ; i++){
			strncat(temp, &a, 1);	//add 'a' to end of temp
		}
	}

	//set new password to check
	password = temp;

	return password;
}

//signal handler...self explanatory
//just for printing stuff after SIGINT
void signal_handler(int signal){

	if(signal == SIGINT){
		time_t time = clock();

		fprintf(outFile, "TOTAL TIME: %ld\n", time - start);
		if(p)
			printf("\nTOTAL TIME: %ld\n", time - start);
		exit(0);
	}
}

void initPassword(){
	password = malloc(startLen);
	char a = 'a';

	for(int i = 0; i < startLen; i++){
		strncat(password, &a, 1);
	}
}
