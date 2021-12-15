#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

time_t start;

char *passwords[100];
int passwords_size = 0;

char *password;

int comparePassToList();
char *nextPass();
void signal_handler(int signal);

int main(int argc, char *argv[]){

	//start time
	start = clock();

	char* listFile = argv[1];

	FILE *f = fopen(listFile, "r");
	if(f == NULL){
		printf("passcrack.c ERROR: Failed to open %s for reading.\n", listFile);
		exit(1);
	}

	char* buffer;
	size_t buffer_size = 0;

	while(getline(&buffer, &buffer_size, f) != -1){
		buffer[strcspn(buffer, "\n")] = 0; //remove \n
		passwords[passwords_size] = malloc(strlen(buffer));
		strcpy(passwords[passwords_size], buffer); //copy word into passwords array
		passwords_size++;
	}

	password = "aaaaaa";
	int index;

	while(strcmp(password, "zzzzzz") != 0){
		index = comparePassToList();
		if(index != -1){		//password found!
			printf("%s at index %d\n", password, index);
		}

		printf("PASS: %s\n", password);
		nextPass();
	}


	printf("TIME: %ld", start);
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

	temp = malloc(length);		//allocate only length-numZ chars in temp
	strncat(temp, &password[0], length-numZ-1);//get substring from password

	memcpy(&letter, &password[length-numZ-1], 1);//save letter to increment
	letter++;				//increment letter

	strncat(temp, &letter, 1);		//append incremented letter to temp


	char a = 'a';
	for(int i = 0; i < numZ; i++){
		strncat(temp, &a, 1);	//add 'a' to end of temp
	}


	//set new password to check
	password = temp;

	return password;
}

void signal_handler(int signal){
	if(signal == SIGTERM){
		printf("TOTAL TIME: %ld\n", clock() - start);
		exit(0);
	}
}
