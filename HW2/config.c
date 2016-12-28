#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"




void game_config_free(GameConfig *config) {
	free(config);
}

size_t game_config_get_generations(GameConfig *config) {
	return config->generations;
}

int get_config_get_debug(GameConfig *config) {
	return config->debug;
}

int get_config_get_silent(GameConfig *config) {
	return config->silent;
}


GameConfig *game_config_new_from_cli(int argc, char *argv[]) {
	int debug = 0;
	int silent = 0;
	size_t generations = 20;

	int c = 0;
	while((c = getopt(argc, argv, "dn:s")) != -1) {
		switch(c){
			case 'd':
				debug = 1;
				break;

			case 'n':
				generations = atoi(optarg)+1;
				break;

			case 's':
				silent = 1;
				break;
		}
	}

	GameConfig *config;
	config = (GameConfig*)malloc(sizeof(GameConfig));

	config->debug = debug;
	config->silent = silent;
	config->generations = generations;
	config->input_file = fopen(argv[argc-1], "r");


	return config;
}
