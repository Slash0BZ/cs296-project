#include <stdio.h>
#include <assert.h>
#include "lib/lib.c"

int main(int argc, char** argv){
	int args[3];
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
}
