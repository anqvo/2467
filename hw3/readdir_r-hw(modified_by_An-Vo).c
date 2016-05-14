/*	readdir_r-hw.c was modified by An Vo */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>

/*	modified */
#include <sys/stat.h>

/*	see chapter4/readdir_r-v1.c for extra comments */

int main(int argc, char **argv) {
	
	/*	Always list the current directory */
	char *d_to_open = ".";

	DIR *dstream = opendir(d_to_open);
	if(dstream == NULL) {
		perror("opening directory failed");
		exit(2);
	}

	struct dirent d;
	struct dirent *result;

	/*	modified */
	struct stat s;	/*	(struct stat *buf) argument in stat(),
							used to stat the files pointed by
							*path */

	for(;;) {
		
		int ret = readdir_r(dstream, &d, &result);
		
		if(ret != 0) {
			perror("readdir_r error");
			exit(3);
		}
		
		/*	modified */
		int set = stat(d.d_name, &s);	/*	initialize an int variable to hold
												return value of stat() */
		if(set != 0) {	/*	check for error */
			perror("stat error");
			exit(4);
		}
		
		if(result == NULL) {
			break;
		}
		
		/*	modified */
		if(S_ISDIR(s.st_mode)) {	/*	is the current entry a Directory?
												if so, specifically state it is
												a Directory and print its mode
												%o for octal output of mode */
			printf("mode: %o, name: %s%s\n", s.st_mode, d.d_name, " (Directory)");
		}
		else {	/* if the current entry is not a Directory,
						print the file name and its mode
						%o for octal output of mode */
			printf("mode: %o, name: %s\n", s.st_mode, d.d_name);
		}
	}

	return 0;
}
