#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <cilk/cilk.h>
#include "game.h"

void game_free(Game *game){
	free(game);
}

int game_cell_is_alive(Game *game, size_t row, size_t col){

	if(game->board[col + (game->cols * row)] == '1'){
		return 1;
	}

	return 0;
}

int game_cell_is_dead(Game *game, size_t row, size_t col){

	if(game->board[col + (game->cols * row)] == '0') {
		return 1;
	}

	return 0;

}

Game *game_new(void){
	return (Game*) malloc(sizeof(Game));
}


int game_parse_board(Game *game, GameConfig *config) {

	FILE *f = config->input_file;


	int rows = 0;
	int cols = 0;
	char buffer[256*sizeof(char)];
	int i;
	int j = 0;

	for(i = 0; i < 2; i++){
		fgets(buffer, 256, f);

		if(strstr(buffer, "Rows:") != NULL){
			sscanf(buffer, "Rows:%d", &rows);

		} else if(strstr(buffer, "Cols:") != NULL) {
			sscanf(buffer, "Cols:%d", &cols);
		}
	}

	char buffer2[2*cols*sizeof(char)];
	char* board = (char*)malloc(cols*rows*sizeof(char*));

	while(fgets(buffer2, 2*cols, f) != NULL) {

			int i;

			for(i = 0; i < cols; i++){
				if(buffer2[i] == '.')
					board[j] = '0' ;

				else if(buffer2[i] == '#')
					board[j] = '1';

				j++;


			}
		}



	game->rows = rows;
	game->cols = cols;
	game->board = board;



	return 0;
}
void game_print_board(Game* game){
	int it = game->cols * game->rows;
	char* board = game->board;

	int i;
	for(i = 0; i < it; i++){


			if(board[i] == '1')
				printf("#");

			else if(board[i] == '0')
				printf(".");

			if((i+1)%game->cols == 0)
				printf("\n");

	}
}

void game_cell_set_alive(Game *game, size_t row, size_t col){
	game->board[col + (game->cols * row)] = '1';
}

void game_cell_set_dead(Game *game, size_t row, size_t col){
	game->board[col + (game->cols * row)] = '0';
}

/*
*    Any live cell with fewer than two live neighbours dies, as if caused by under-population.
*    Any live cell with two or three live neighbours lives on to the next generation.
*    Any live cell with more than three live neighbours dies, as if by over-population.
*    Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/

void life(size_t cell_row, size_t cell_col, int cols, int cell_is_alive, int neighbour_living_cells, char *board){

		if(cell_is_alive == 1){
			if(neighbour_living_cells < 2 || neighbour_living_cells > 3){
				board[cell_col + (cols * cell_row)] = '0';
			}else{
				board[cell_col + (cols * cell_row)] = '1';
			}
		} else {
			if(neighbour_living_cells == 3){
				board[cell_col + (cols * cell_row)] = '1';
			}else{
				board[cell_col + (cols * cell_row)] = '0';
			}
		}
}

void treat_center_cell(Game *game,char *new_board, size_t row, size_t col){
	int neighbour_living_cells = 0;
	int cell_is_alive = game_cell_is_alive(game, row, col);
	/*Top left cell*/
	if(game_cell_is_alive(game, row-1, col-1) == 1)
		neighbour_living_cells++;
	/*Top cell*/
	if(game_cell_is_alive(game, row-1, col) == 1)
		neighbour_living_cells++;
	/*Top right cell*/
	if(game_cell_is_alive(game, row-1, col+1) == 1)
		neighbour_living_cells++;
	/*Left cell*/
	if(game_cell_is_alive(game, row, col-1) == 1)
		neighbour_living_cells++;
	/*Right cell*/
	if(game_cell_is_alive(game, row, col+1) == 1)
		neighbour_living_cells++;
	/*Bottom left cell*/
	if(game_cell_is_alive(game, row+1, col-1) == 1)
		neighbour_living_cells++;
	/*Bottom cell*/
	if(game_cell_is_alive(game, row+1, col) == 1)
		neighbour_living_cells++;
	/*Bottom right cell*/
	if(game_cell_is_alive(game, row+1, col+1) == 1)
		neighbour_living_cells++;

	life(row, col,game->cols, cell_is_alive, neighbour_living_cells, new_board);
}
void treat_top_left_cell(Game *game,char *new_board, size_t row, size_t col){
	int neighbour_living_cells = 0;
	int cell_is_alive = game_cell_is_alive(game, row, col);
	/*Top left cell*/
	if(game_cell_is_alive(game, game->rows-1, game->cols-1) == 1)
		neighbour_living_cells++;
	/*Top cell*/
	if(game_cell_is_alive(game, game->rows-1, col) == 1)
		neighbour_living_cells++;
	/*Top right cell*/
	if(game_cell_is_alive(game, game->rows-1, col+1) == 1)
		neighbour_living_cells++;
	/*Left cell*/
	if(game_cell_is_alive(game, row, game->cols-1) == 1)
		neighbour_living_cells++;
	/*Right cell*/
	if(game_cell_is_alive(game, row, col+1) == 1)
		neighbour_living_cells++;
	/*Bottom left cell*/
	if(game_cell_is_alive(game, row+1, game->cols-1) == 1)
		neighbour_living_cells++;
	/*Bottom cell*/
	if(game_cell_is_alive(game, row+1, col) == 1)
		neighbour_living_cells++;
	/*Bottom right cell*/
	if(game_cell_is_alive(game, row+1, col+1) == 1)
		neighbour_living_cells++;

	life(row, col, game->cols, cell_is_alive, neighbour_living_cells, new_board);
}

void treat_top_cell(Game *game,char *new_board, size_t row, size_t col){
	int neighbour_living_cells = 0;
	int cell_is_alive = game_cell_is_alive(game, row, col);
	/*Top left cell*/
	if(game_cell_is_alive(game, game->rows-1, col-1) == 1)
		neighbour_living_cells++;
	/*Top cell*/
	if(game_cell_is_alive(game, game->rows-1, col) == 1)
		neighbour_living_cells++;
	/*Top right cell*/
	if(game_cell_is_alive(game, game->rows-1, col+1) == 1)
		neighbour_living_cells++;
	/*Left cell*/
	if(game_cell_is_alive(game, row, col-1) == 1)
		neighbour_living_cells++;
	/*Right cell*/
	if(game_cell_is_alive(game, row, col+1) == 1)
		neighbour_living_cells++;
	/*Bottom left cell*/
	if(game_cell_is_alive(game, row+1, col-1) == 1)
		neighbour_living_cells++;
	/*Bottom cell*/
	if(game_cell_is_alive(game, row+1, col) == 1)
		neighbour_living_cells++;
	/*Bottom right cell*/
	if(game_cell_is_alive(game, row+1, col+1) == 1)
		neighbour_living_cells++;

	life(row, col, game->cols, cell_is_alive, neighbour_living_cells, new_board);
}

void treat_top_right_cell(Game *game,char *new_board, size_t row, size_t col){
	int neighbour_living_cells = 0;
	int cell_is_alive = game_cell_is_alive(game, row, col);
	/*Top left cell*/
	if(game_cell_is_alive(game, game->rows-1, col-1) == 1)
		neighbour_living_cells++;
	/*Top cell*/
	if(game_cell_is_alive(game, game->rows-1, col) == 1)
		neighbour_living_cells++;
	/*Top right cell*/
	if(game_cell_is_alive(game, game->rows-1, 0) == 1)
		neighbour_living_cells++;
	/*Left cell*/
	if(game_cell_is_alive(game, row, col-1) == 1)
		neighbour_living_cells++;
	/*Right cell*/
	if(game_cell_is_alive(game, row, 0) == 1)
		neighbour_living_cells++;
	/*Bottom left cell*/
	if(game_cell_is_alive(game, row+1, col-1) == 1)
		neighbour_living_cells++;
	/*Bottom cell*/
	if(game_cell_is_alive(game, row+1, col) == 1)
		neighbour_living_cells++;
	/*Bottom right cell*/
	if(game_cell_is_alive(game, row+1, 0) == 1)
		neighbour_living_cells++;

	life(row, col, game->cols, cell_is_alive, neighbour_living_cells, new_board);
}
void treat_left_cell(Game *game,char *new_board, size_t row, size_t col){
	int neighbour_living_cells = 0;
	int cell_is_alive = game_cell_is_alive(game, row, col);
	/*Top left cell*/
	if(game_cell_is_alive(game, row-1, game->cols-1) == 1)
		neighbour_living_cells++;
	/*Top cell*/
	if(game_cell_is_alive(game, row-1, col) == 1)
		neighbour_living_cells++;
	/*Top right cell*/
	if(game_cell_is_alive(game, row-1, col+1) == 1)
		neighbour_living_cells++;
	/*Left cell*/
	if(game_cell_is_alive(game, row, game->cols-1) == 1)
		neighbour_living_cells++;
	/*Right cell*/
	if(game_cell_is_alive(game, row, col+1) == 1)
		neighbour_living_cells++;
	/*Bottom left cell*/
	if(game_cell_is_alive(game, row+1, game->cols-1) == 1)
		neighbour_living_cells++;
	/*Bottom cell*/
	if(game_cell_is_alive(game, row+1, col) == 1)
		neighbour_living_cells++;
	/*Bottom right cell*/
	if(game_cell_is_alive(game, row+1, col+1) == 1)
		neighbour_living_cells++;

	life(row, col, game->cols, cell_is_alive, neighbour_living_cells, new_board);
}

void treat_right_cell(Game *game,char *new_board, size_t row, size_t col){
	int neighbour_living_cells = 0;
	int cell_is_alive = game_cell_is_alive(game, row, col);
	/*Top left cell*/
	if(game_cell_is_alive(game, row-1, col-1) == 1)
		neighbour_living_cells++;
	/*Top cell*/
	if(game_cell_is_alive(game, row-1, col) == 1)
		neighbour_living_cells++;
	/*Top right cell*/
	if(game_cell_is_alive(game, row-1, 0) == 1)
		neighbour_living_cells++;
	/*Left cell*/
	if(game_cell_is_alive(game, row, col-1) == 1)
		neighbour_living_cells++;
	/*Right cell*/
	if(game_cell_is_alive(game, row, 0) == 1)
		neighbour_living_cells++;
	/*Bottom left cell*/
	if(game_cell_is_alive(game, row+1, col-1) == 1)
		neighbour_living_cells++;
	/*Bottom cell*/
	if(game_cell_is_alive(game, row+1, col) == 1)
		neighbour_living_cells++;
	/*Bottom right cell*/
	if(game_cell_is_alive(game, row+1, 0) == 1)
		neighbour_living_cells++;

	life(row, col, game->cols, cell_is_alive, neighbour_living_cells, new_board);
}

void treat_bottom_left_cell(Game *game,char *new_board, size_t row, size_t col){
	int neighbour_living_cells = 0;
	int cell_is_alive = game_cell_is_alive(game, row, col);
	/*Top left cell*/
	if(game_cell_is_alive(game, row-1, game->cols-1) == 1)
		neighbour_living_cells++;
	/*Top cell*/
	if(game_cell_is_alive(game, row-1, col) == 1)
		neighbour_living_cells++;
	/*Top right cell*/
	if(game_cell_is_alive(game, row-1, col+1) == 1)
		neighbour_living_cells++;
	/*Left cell*/
	if(game_cell_is_alive(game, row, game->cols-1) == 1)
		neighbour_living_cells++;
	/*Right cell*/
	if(game_cell_is_alive(game, row, col+1) == 1)
		neighbour_living_cells++;
	/*Bottom left cell*/
	if(game_cell_is_alive(game, 0, game->cols-1) == 1)
		neighbour_living_cells++;
	/*Bottom cell*/
	if(game_cell_is_alive(game, 0, col) == 1)
		neighbour_living_cells++;
	/*Bottom right cell*/
	if(game_cell_is_alive(game, 0, col+1) == 1)
		neighbour_living_cells++;

	life(row, col, game->cols,cell_is_alive, neighbour_living_cells, new_board);
}

void treat_bottom_cell(Game *game,char* new_board, size_t row, size_t col){
	int neighbour_living_cells = 0;
	int cell_is_alive = game_cell_is_alive(game, row, col);
	/*Top left cell*/
	if(game_cell_is_alive(game, row-1, col-1) == 1)
		neighbour_living_cells++;
	/*Top cell*/
	if(game_cell_is_alive(game, row-1, col) == 1)
		neighbour_living_cells++;
	/*Top right cell*/
	if(game_cell_is_alive(game, row-1, col+1) == 1)
		neighbour_living_cells++;
	/*Left cell*/
	if(game_cell_is_alive(game, row, col-1) == 1)
		neighbour_living_cells++;
	/*Right cell*/
	if(game_cell_is_alive(game, row, col+1) == 1)
		neighbour_living_cells++;
	/*Bottom left cell*/
	if(game_cell_is_alive(game, 0, col-1) == 1)
		neighbour_living_cells++;
	/*Bottom cell*/
	if(game_cell_is_alive(game, 0, col) == 1)
		neighbour_living_cells++;
	/*Bottom right cell*/
	if(game_cell_is_alive(game, 0, col+1) == 1)
		neighbour_living_cells++;

	life(row, col, game->cols,cell_is_alive, neighbour_living_cells, new_board);

}

void treat_bottom_right_cell(Game *game,char* new_board, size_t row, size_t col){
	int neighbour_living_cells = 0;
	int cell_is_alive = game_cell_is_alive(game, row, col);
	/*Top left cell*/
	if(game_cell_is_alive(game, row-1, col-1) == 1)
		neighbour_living_cells++;
	/*Top cell*/
	if(game_cell_is_alive(game, row-1, col) == 1)
		neighbour_living_cells++;
	/*Top right cell*/
	if(game_cell_is_alive(game, row-1, 0) == 1)
		neighbour_living_cells++;
	/*Left cell*/
	if(game_cell_is_alive(game, row, col-1) == 1)
		neighbour_living_cells++;
	/*Right cell*/
	if(game_cell_is_alive(game, row, 0) == 1)
		neighbour_living_cells++;
	/*Bottom left cell*/
	if(game_cell_is_alive(game, 0, col-1) == 1)
		neighbour_living_cells++;
	/*Bottom cell*/
	if(game_cell_is_alive(game, 0, col) == 1)
		neighbour_living_cells++;
	/*Bottom right cell*/
	if(game_cell_is_alive(game, 0, 0) == 1)
		neighbour_living_cells++;

	life(row, col, game->cols,cell_is_alive, neighbour_living_cells, new_board);
}

void change(char** a, char** b){
    char *temp = *a;
    *a = *b;
    *b = temp;
}

int game_tick(Game *game){
	int i, j;
	int cols = game->cols;
	int rows = game->rows;

	char* new_board;

	new_board = (char*)malloc(cols*rows*sizeof(char));

	cilk_for(i = 0; i < rows; i++) {
		cilk_for(j = 0; j < cols; j++) {
			//Top Left cell*/
			if(i == 0 && j == 0){
			  treat_top_left_cell(game, new_board, i, j);
			}
			/*Top cell*/
			if(i == 0 && j < cols-1 && j > 0) {
			  treat_top_cell(game, new_board, i, j);
			}
			/*Top right cell*/
			if(i == 0 && j == cols-1) {
			  treat_top_right_cell(game, new_board, i, j);
			}
			/*Left cell*/
			if(i > 0 && i < rows-1 && j == 0){
			  treat_left_cell(game, new_board, i, j);
			}
			/*Center cell*/
			if(i > 0 && i < rows-1 && j > 0 && j < cols-1) {
			  treat_center_cell(game, new_board, i, j);
			}
			/*Right cell*/
			if(i > 0 && i < rows-1 && j == cols-1){
			  treat_right_cell(game, new_board, i, j);
			}
			/*Bottom left cell*/
			if(i == rows-1 && j == 0 ){
			  treat_bottom_left_cell(game, new_board, i, j);
			}
			/*Bottom cell*/
			if(i == rows-1 && j < cols-1 && j > 0){
			  treat_bottom_cell(game, new_board, i, j);
			}
			/*Bottom Right cell*/
			if(i == rows-1 && j == cols-1){
			  treat_bottom_right_cell(game, new_board, i, j);
			}
		}
	}

	free(game->board);
	game->board = new_board;

	return 1;
}
