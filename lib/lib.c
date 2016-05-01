#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <ftw.h>
#include <unistd.h>
#include <time.h>
#include <libgen.h>

extern char* log_file_name;
extern char* log_zip_name;

void wrong_usage(){
	printf("Incorrect Usage. Use -h argument to see help\n");
}

void help_usage(){
	printf("./remove [option] [targets] \n");
	printf("option: \n");
	printf("\t-f: force delete. Identical to system rm.\n");
	printf("\t-s: stash delete. Send deleted data to remote server \n");
	printf("\t-c: compress delete. Compress locally.\n");
	printf("\t-l: list of all deleted files under current directory \n");
	printf("\t-r: recover file with certain index in the trash list\n");
	printf("\n");
	printf("This remover is Copyright (C) 2016 , and GNU GPL'd , By Xuanyu Zhou \n");
	
	
	
}

int handle_arg(int argc, char **argv, int *args, char*** targets){
	for(int i = 0; i < 5; i++){
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
		if (strcmp((*(argv + i)), "-l") == 0){
			args[3] = 1;
			if (i > id) id = i;
		}
		if (strcmp((*(argv + i)), "-r") == 0){
			args[4] = 1;
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

int handle_compress(const char* path, const struct stat *sb, int typeflag){
	if (!sb) return 1;
	if (typeflag == FTW_F){
		char buffer[200];
		FILE *fp = fopen(".trash/.log", "a+");
		realpath(path, buffer);
		fprintf(fp, "%s\t%s\n", buffer, log_zip_name);
		fclose(fp);
	}
	return 0;
}

void* compress_delete(void *vpath){
	char *path = (char*)vpath;
	printf("Deleting: %s\n", path);
	struct stat pathstat;
	stat(path, &pathstat);
	if (S_ISREG(pathstat.st_mode)){
		char buffer[200];
		FILE *fp = fopen(".trash/.log", "a+");
		realpath(path, buffer);
		fprintf(fp, "%s\t%s\n", buffer, log_zip_name);
		fclose(fp);
	}
	else{
		ftw(path, handle_compress, 20);
	}

	char cmd[200];
	strcpy(cmd, "zip -r ");
	strcat(cmd, log_zip_name);
	strcat(cmd, " ");
	strcat(cmd, path);
	system(cmd);
	force_delete(path);
	return 0;
}

char *get_log_file_name(){
	long timeLong = time(0);
	char buffer[50];
	char buffer_ret[100];
	sprintf(buffer, "%ld", timeLong);	
	strcpy(buffer_ret, ".trash/.");
	strcat(buffer_ret, buffer);
	strcat(buffer_ret, ".log");
	return strdup(buffer_ret);
}

char *get_log_zip_name(char *file_name){
	char *ret = malloc(100);
	strcpy(ret, file_name);
	strcat(ret, ".zip");
	return ret;	
}

void list_trash(){
	FILE *fp = fopen(".trash/.log", "r");
	char *entry = NULL;
	size_t size = 0;
	int count = 0;
	while(getline(&entry, &size, fp) != -1){
		printf("%d: %s", count, entry);	
		count++;
	}	
	fclose(fp);
}

void recover_file(char *target){
	FILE *fp = fopen(".trash/.log", "r");
	int line_num = atoi(target);
	char *entry = NULL;
	size_t size = 0;
	int count = 0;	
	char *target_line = NULL;
	char *cur = NULL;
	while(getline(&entry, &size, fp) != -1){
		if (count == line_num){
			target_line = entry;
			break;	
		}
		count++;
	}
	if (!target_line){
		printf("Cannot locate target file in trash bin\n");
	}
	else{
		cur = strdup(target_line);	
		char *file_path = strtok(cur, "\t");
		char *file_path_dup_1 = strdup(file_path);
		char *file_path_dup_2 = strdup(file_path);
		char *file_folder = dirname(file_path_dup_2);
		mkdir(file_folder, 0755);
		char *file_zip = strtok(NULL, "\t");
		*(file_zip + strlen(file_zip) - 1) = '\0';

		char pathbuffer[128];
		char *curPath = getcwd(pathbuffer, 128);
		char *relativePath = file_path + strlen(curPath) + 1;

		char cmd[128];
		strcpy(cmd, "unzip ");
		strcat(cmd, file_zip);
		strcat(cmd, " -d .trash_temp");
		system(cmd);
		char cmd2[128];
		strcpy(cmd2, "mv .trash_temp/");
		strcat(cmd2, relativePath);
		strcat(cmd2, " ");
		strcat(cmd2, file_path);
		system(cmd2);	
		force_delete(".trash_temp");
		free(file_path_dup_1);
		free(file_path_dup_2);
	}
	fclose(fp);
	free(cur);
}

void stash(){
	char* cmd1 = "git add -A";
	char* cmd2 = "git commit -m 'add'";
	char* cmd3 = "git push";
	system(cmd1);
	system(cmd2);
	system(cmd3);
}

