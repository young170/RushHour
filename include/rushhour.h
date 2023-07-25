#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	start,
	left,
	right,
	up,
	down,
	quit,
    bfs,
    dfs,
    color,
	N_op 
} commands ;

typedef enum {
	vertical,
	horizontal 
} direction ;

typedef struct {
	int id ;
	int y1, y2 ;	// y1: the minimum of y, y2: the maximum of y
	int x1, x2 ;	// x1: the minimum of x, x2: the maximum of x
	int span ;		// the number of cells 
	direction dir ;	// the direction of the car
} car_t ;

char * op_str[N_op] = {
	"start",
	"left",
	"right",
	"up",
	"down",
	"quit",
    "bfs",
    "dfs",
    "color"
} ;

#define BOARD_SIZE 6

int n_cars = 0 ;
car_t * cars = 0x0 ;
int cells[6][6] ; // cells[Y][X]
int colored_cells[6][6]; // colored_cells[Y][X]
// A6 -> cells[5][0]
// F4 -> cells[3][5]
// F1 -> cells[0][5]
