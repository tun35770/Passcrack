#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *passwords[100];
int passwords_size = 0;

char *password;

int comparePassToList();
char *nextPass();

int main(int argc, char *argv[]){

	//start time
	time_t start = clock();

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
		index = comparePassToList(password);
		if(index != -1){		//password found!
			printf("%s at index %d\n", password, index);
		}

		printf("%s\n", password);
		nextPass(password);
//	strcpy(pass, nextPass(pass));
		printf("NEW PASS: %s\n", password);
		break;
	}


	printf("TIME: %ld", start);
}

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

char *nextPass(){

	int numZ = 0;
	int length = strlen(password);

	for(int i = length - 1; i >= 0; i--){
		if(password[i] == 'z'){
			numZ++;
		}
		else{
			break;
		}
	}

	char *temp;
	char *substring = malloc(length-numZ);
	char letter;

	temp = malloc(length-numZ+1);		//allocate only length-numZ chars in temp
	memcpy(substring, &password[0], length-numZ-1);//get substring from password
	strcpy(temp, substring);		//put substring in temp
	memcpy(&letter, &password[length-numZ-1], 1);//save letter to increment
	letter++;				//increment letter
	strncat(temp, &letter, 1);		//append letter to temp
	password = malloc(length+1);		//put everything into password
	memcpy(password, &temp[0], strlen(temp));

	char a = 'a';
	for(int i = 0; i < numZ; i++){
		strncat(password, &a, 1);	//add 'a' to end password
	}

	free(temp);
	free(substring);

	return password;
}
