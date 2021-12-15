#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *passwords[100];
int passwords_size = 0;

int comparePassToList(char *password);
char *nextPass(char *password);

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

	char *pass = "aaaaaa";
	int index;
	while(strcmp(pass, "zzzzzz") != 0){
		index = comparePassToList(pass);
		if(index != -1){		//password found!
			printf("%s at index %d\n", pass, index);
		}

		pass = nextPass(pass);
	}


	printf("TIME: %ld", start);
}

int comparePassToList(char *password){

	int passIndex = -1;

	for(int i = 0; i < passwords_size; i++){
		if(strcmp(passwords[i], password) == 0){
			passIndex = i;
			break;
		}
	}

	return passIndex;
}

char *nextPass(char *password){

	return "";
}
