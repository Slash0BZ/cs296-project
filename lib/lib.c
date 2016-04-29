#include <string.h>
#include <stdlib.h>
#include <ftw.h>
#include <unistd.h>

void wrong_usage(){
	printf("Incorrect Usage. Use -h argument to see help\n");
}

void help_usage(){
	printf("./remove [option] [targets] \n");
	printf("option: \n");
	printf("\t-f: force delete. Identical to system rm.\n");
	printf("\t-s: stash delete. Send deleted data to remote server \n");
	printf("\t-c: compress delete. Compress locally.\n");
	
}

int handle_arg(int argc, char **argv, int *args, char*** targets){
	for(int i = 0; i < 3; i++){
		args[i] = 0;
	}
	int id = 1;
	for(int i = 1; i < argc; i++){
		if (strcmp((*(argv + i)), "-f") == 0){
			args[0] = 1;
			if (i > id) id = i;
		}
		if (strcmp((*(argv + i)), "-s") == 0){
			args[1] = 1;
			if (i > id) id = i;
		}
		if (strcmp((*(argv + i)), "-c") == 0){
			args[2] = 1;
			if (i > id) id = i;
		}
		if (strcmp((*(argv + i)), "-h") == 0){
			help_usage();
			exit(0);
		}
	}
	int idc = id;
	int target_size = argc - id - 1;
	if (!target_size) return 0;
	*targets = malloc(sizeof(char*) * target_size);
	for(int i = id + 1; i < argc; i++){
		*((*targets) + i - idc - 1) = *(argv + i);
	}
	return target_size;
}

int handle_force(const char* path, const struct stat *sb, int typeflag){
	if (!sb) return 1;
	if (typeflag == FTW_F){
		remove(path);
		return 0;
	}	
	if (typeflag == FTW_D){
		rmdir(path);
	}
	return 0;
}

int force_delete(char *path){
	int retval = ftw(path, handle_force, 20);
	retval = ftw(path, handle_force, 20);
	rmdir(path);
	return retval;
}
