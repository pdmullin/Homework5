#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/inotify.h>


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

	printf("%s spawned this process.\n", argv[0]);

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
		return EXIT_SUCCESS;
	}

	int fd = inotify_init();
   if (fd == -1){
   	perror("inotify_init");
   	return EXIT_FAILURE;
   }

	int wd = inotify_add_watch(fd, "/etc/passwd", IN_MODIFY | IN_ACCESS);
    if (wd == -1){
   	 perror("inotify_add_watch");
   	return EXIT_FAILURE;
   }

   int x;
   char* p;
   const size_t buf_len = 500;
   char buf[buf_len];

   while (1) {
      x = read(fd, buf, buf_len);
      if(fd == -1){
      	perror("read");
      	return EXIT_FAILURE;
      }

      for(p = buf; p < buf + x;) {
         struct inotify_event* event = (struct inotify_event*)p;
        
         if((event->mask & IN_ACCESS) != 0) {
            printf("file has been accessed\n");
         }
         
         if((event->mask & IN_MODIFY) != 0) {
            printf("file has been modified\n");
         }
         p += sizeof(struct inotify_event) + event->len;

       }
   }
	return EXIT_SUCCESS;
}