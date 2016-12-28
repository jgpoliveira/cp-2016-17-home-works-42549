
#include "game.h"

int main(int argc, char *argv[]){

  Game *game = game_new();
  GameConfig *config = game_config_new_from_cli(argc, argv);

  game_parse_board(game, config);
  printf("Generations:%zu\n", config->generations);
  printf("Rows:%zu\n", game->rows);
  printf("Cols:%zu\n", game->cols);
  printf("\n\n" );

  //inicial state
  if(config->silent != 1){
    game_print_board(game);
    printf("\n\n");
  }

   size_t i;

  for(i = 0; i < config->generations; i++){
    game_tick(game);

    if(config->debug == 1){
      game_print_board(game);
      printf("\n\n");
    }

  }

  if(config->debug != 1 && config->silent != 1){
    game_print_board(game);
  }


  return 0;
}
