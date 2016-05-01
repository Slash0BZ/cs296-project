#include <stdio.h>
#include <assert.h>
#include "lib/lib.c"
#include "lib/libnetwork.c"
#include <time.h>
#include <pthread.h>

char *log_file_name;
char *log_zip_name;

int main(int argc, char** argv){
	int args[5];
	char **targets = NULL;
	int target_count = handle_arg(argc, argv, args, &targets);	
	if (target_count <= 0 && !args[3] && !args[1]){
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
		stash();
		free(targets);
		return 0;	
	}
	if (args[2]){
		mkdir(".trash", 0755);
		log_file_name = get_log_file_name();
		log_zip_name = get_log_zip_name(log_file_name);
		pthread_t cpt[target_count];
		for(int i = 0; i < target_count; i++){
			pthread_create(&cpt[i], NULL, compress_delete, *(targets + i)); 
		}	
		for(int i = 0; i < target_count; i++){
			pthread_join(cpt[i], NULL);
		}
		free(targets);
		free(log_file_name);
		free(log_zip_name);
		return 0;
	}
	if (args[3]){
		list_trash();
		free(targets);
		return 0;
	}
	if (args[4]){
		mkdir(".trash_temp", 0755);
		for(int i = 0; i < target_count; i++){
			recover_file(*targets + i);
		}
		free(targets);
		return 0;
	}
}
