#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <utime.h>



int main(int argc, char* argv[]){

	int opt = 0;
	bool enable_h = false, enable_d = false, enable_m = false, enable_t = false;
        char* arg1;
        

	while ((opt = getopt (argc, argv, ":d:hmt")) != -1){ 
		switch (opt){
			case 'h':
			enable_h = true;
			break;
			case 'd':
                        arg1 = optarg;
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
		printf("The program will accept a file name as an argument.\n");
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

   int wd = inotify_add_watch(fd, "/home/jamesperra/test.txt", IN_MODIFY);
   if (wd == -1){
   	 perror("inotify_add_watch");
   	 return EXIT_FAILURE;
   }
   int input;
   int output;
   ssize_t b_in, b_out;
   char* permissions;
   struct stat fStatI;
   struct stat fStatO;
   if(stat("/home/jamesperra/test.txt",&fStatI) < 0)
	perror("stat");
    
    
   int x;
   char* p;
   const size_t buf_len = 500;
   char buf[buf_len];
   int count = 0;
   char str[100];
   

   while (1) {
      x = read(fd, buf, buf_len);
      if(fd == -1){
      	perror("read");
      	return EXIT_FAILURE;
      }

      if (access("/home/jamesperra/test.txt", F_OK) == -1) {//stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c-cross-platform
            perror("File Deleted");
            return EXIT_FAILURE;
      }

      for(p = buf; p < buf + x;) {
         struct inotify_event* event = (struct inotify_event*)p;

         
        
         if((event->mask & IN_MODIFY) != 0) {
            if (enable_d == true && enable_t == false) {
                 snprintf(str, 100, "%stest_rev%d.txt", arg1, count);
            	
            }
            
            else if (enable_t == true && enable_d == false) {
                 time_t t = time(NULL);
                 struct tm tm = *localtime(&t);
                 char date[100];
                 sprintf(date, "%d%d%d%d%d%d", tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,       tm.tm_min, tm.tm_sec);
                 snprintf(str, 100, "/home/jamesperra/test_%s.txt", date);
            }
 
            else if (enable_t == true && enable_d == true){
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                char date[100];
                sprintf(date, "%d%d%d%d%d%d", tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,       tm.tm_min, tm.tm_sec);
                snprintf(str, 100, "%stest_%s.txt", arg1, date);
            }
  
            else{
                 snprintf(str,100, "/home/jamesperra/test_rev%d.txt", count);
            }
            input = open("/home/jamesperra/test.txt", O_RDONLY);
            output = open(str, O_WRONLY | O_CREAT,0644);
            if (enable_m == false) {
                  if(stat(str,&fStatO) < 0)
	              perror("stat");
                  struct utimbuf t;//stackoverflow.com/questions/2185338/how-to-set-the-modification-time-of-a-file-programmatically
                  t.actime = fStatI.st_atime;
                  t.modtime = fStatI.st_mtime;   
                  utime(str, &t);
                  chown(str, fStatI.st_uid, fStatI.st_gid);              
            }
            printf("file has been modified\n");
            count += 1;
            while ((b_in = read(input, &buf,buf_len)) > 0) {//techytalk.info/linux-system-programming-open-file-read-file-and-write-file/
                b_out = write(output, &buf, (ssize_t) b_in);
            }
            close (input);
            close (output);

         }
         p += sizeof(struct inotify_event) + event->len;

       }
   }
	return EXIT_SUCCESS;
}
