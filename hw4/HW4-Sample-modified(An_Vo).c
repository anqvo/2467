/*	I, An Vo, am going for extra credit */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <pwd.h>	/*	modified */
#include <grp.h>	/*	modified */

#include <string.h>	/*	modified for extra credit */

/*	see chapter4/readdir_r-v1.c for extra comments */

int main(int argc, char **argv) {

	/*	Always list the current directory */
	char *d_to_open = ".";

	DIR *dstream = opendir(d_to_open);
	if(dstream == NULL) {
		perror("opening directory failed");
	}

	struct dirent d;
	struct dirent *result;

	for(;;) {
		int ret = readdir_r(dstream, &d, &result);

		if(ret != 0) {
			perror("read error");
			exit(3);
		}
		if(!result) {
			break;
		}

		struct stat mystat;	/*	must declare the struct itself not just
										a pointer to it so the memory gets
										allocated for the whole struct */

		if((stat(d.d_name, &mystat) == -1)) {	/*	but stat wants a pointer to
																the struct, so use & */
			perror("stat");
			exit(4);
		}

		uid_t p = mystat.st_uid;	/*	use stat to get the current uid */
		gid_t g = mystat.st_gid;	/*	use stat to get the current gid */
		struct passwd *pw = getpwuid(p);	/*	get the uid and store it into a passwd structure
														getpwuid returns a pointer to the passwd structure */
		struct group *gr = getgrgid(g);	/*	get the gid and store it into a group structure
														getgrgid returns a pointer to the group structure */
		if(pw == NULL) {	/*	check for no match/error */
			perror("getpwuid error");
			exit(5);
		}
		if(gr == NULL) {	/*	check for no match/error */
			perror("getgrgid error");
			exit(6);
		}

		char *dir = "";	/*	variable dir points to an empty string */

		if(S_ISDIR(mystat.st_mode)) {	/*	if it's a directory, change dir */

			/*	the following are modifications for extra credit */
			char *curr = ".";	/*	current directory (dot) */
			char *prev = "..";	/*	previous directory (dot dot) */
			int cmp = strcmp(d.d_name, curr);	/* compare directory */
			int cmp2 = strcmp(d.d_name, prev);	/* compare directory */
			if(cmp == 0 || cmp2 == 0) {	/*	if either are equal, do nothing (skip print) */
			}
			else {
				dir = "(Directory)";			/*	to point to the string "(Directory)" */
				/*	modified to print user name and group name as well */
				printf("user: %s, group: %s, mode: %llo, name: %s %s\n", pw->pw_name, gr->gr_name, (long long) mystat.st_mode, d.d_name, dir);
			}
		}
		else {
			/*	now we can print the mode in octal using %llo, the name, and
			 *	the string dir which may either be "" or (Directory)"
			 *	modified to print user name and group name as well */
			printf("user: %s, group: %s, mode: %llo, name: %s %s\n", pw->pw_name, gr->gr_name, (long long) mystat.st_mode, d.d_name, dir);
		}
	}
	return 0;
}
