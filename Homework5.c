#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[]){

	int opt = 0;
	bool enable_h = false, enable_d = false, enable_m = false, enable_t = false;

	while ((opt = getopt (argc, argv, ":d:hmt")) != -1){ 
		switch (opt){
			case 'h':
			enable_h = true;
			break;
			case 'd':
			enable_d = true;
			break;
			case 'm':
			enable_m = true;
			break;
			case 't':
			enable_t = true;
			break;
			default:
			printf("Error: Invalid option entered. Enter -h for help information. \n");
			return EXIT_SUCCESS;
			break;
		}
	}

	if (enable_h == true){
		printf("----------------------------------------------------------\n");
		printf("The purpose of this program is to create file backups.\n");
		printf("The program will monitor the file until it is deleted or program terminates.\n");
		printf("Every time the file changes the program will create a perfect backup.\n");
		printf("-h option to print this information.\n");
		printf("-d ARG to customize backup location, ARG is path to desired location.\n");
		printf("If no path is given for -d program will default location to the home directory.\n");
		printf("-m to disable meta-data duplication.\n");
		printf("-t to append duplication time to the backup file name.\n");
		printf("----------------------------------------------------------\n");
	}

	printf("%s spawned this process.\n", argv[0]);
	return EXIT_SUCCESS;
}