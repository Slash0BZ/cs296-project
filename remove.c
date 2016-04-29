#include <stdio.h>
#include <assert.h>
#include "lib/lib.c"
#include "lib/libnetwork.c"
#include <time.h>
char *log_file_name;
char *log_zip_name;

int main(int argc, char** argv){
	int args[4];
	char **targets = NULL;
	int target_count = handle_arg(argc, argv, args, &targets);	
	if (target_count <= 0){
		wrong_usage();
		exit(0);
	}
	if (args[0]){	
		for(int i = 0; i < target_count; i++){
			force_delete(*targets + i);
		}
		free(targets);
		return 0;
	}	
	if (args[1]){
		
	}
	if (args[2]){
		mkdir(".trash", 0755);
		log_file_name = get_log_file_name();
		log_zip_name = get_log_zip_name(log_file_name);
		for(int i = 0; i < target_count; i++){
			compress_delete(*targets + i);
		}	
		free(targets);
		free(log_file_name);
		free(log_zip_name);
		return 0;
	}
}
