#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

	char* listFile = argv[1];

	FILE *f = fopen(listFile, "r");
	if(f == NULL){
		printf("passcrack.c ERROR: Failed to open %s for reading.\n", listFile);
		exit(1);
	}

	char* passwords[100];
	int passwords_size = 0;
	char* buffer;
	size_t buffer_size = 0;

	while(getline(&buffer, &buffer_size, f) != -1){
		buffer[strcspn(buffer, "\n")] = 0; //remove \n
		passwords[passwords_size] = malloc(strlen(buffer));
		strcpy(passwords[passwords_size], buffer); //copy word into passwords array
		passwords_size++;
	}
}
