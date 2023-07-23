#include "rushhour.h"


commands
get_op_code (char * s)
{
	// return the corresponding number for the command given as s.
	// FIXME
    for (int i = 0; i < N_op; i++) {
        if (0 == strcmp(s, op_str[i])) {
            return i;
        }
    }

    printf("invalid command\n");
    return -1;
}

int
load_game (char * filename)
{
	//FIXME
	// load_game returns 0 for a success, or return 1 for a failure.
	// Use fopen, getline, strtok, atoi, strcmp
	// Note that the last character of a line obtained by getline may be '\n'.
    FILE *fp = fopen(filename, "r");
    char *buf = (char *) malloc(1024);
    size_t n_buf = 1024;

    fscanf(fp, "%d", &n_cars); // first line, number of cars
    cars = (car_t *) malloc(sizeof(car_t) * (n_cars)); // cars in list

    getline(&buf, &n_buf, fp); // skip first line 

    int len;
    for (int i = 0; i < n_cars; i++) {
        char *sep = ":\n";
        char *token;

        len = getline(&buf, &n_buf, fp);
        cars[i].id = i + 1; // id, 1-n_cars

        token = strtok(buf, sep);
        cars[i].x1 = token[0] - 'A'; // x1 col
        cars[i].y1 = BOARD_SIZE - (token[1] - '0'); // y1 row

        token = strtok(0x0, sep);
        direction dir_car;
        if (0 == strcmp(token, "vertical")) {
            dir_car = vertical;
        } else if (0 == strcmp(token, "horizontal")) {
            dir_car = horizontal;
        } else {
            fprintf(stderr, "file data incorrect\n");
            return 1;
        }
        cars[i].dir = dir_car; // dir

        token = strtok(0x0, sep);
        int tmp = atoi(token); // span
        if (0 == tmp) {
            fprintf(stderr, "file data incorrect\n");
            return 1;
        }
        cars[i].span = tmp;

        if (vertical == dir_car) { // y2 row
            cars[i].x2 = cars[i].x1;
            cars[i].y2 = cars[i].y1 + (cars[i].span - 1); // span includes x1/y1

            if (BOARD_SIZE - 1 < cars[i].y2) {
                fprintf(stderr, "file data incorrect\n");
                return 1;
            }
        } else if (horizontal == dir_car) { // x2 col
            cars[i].x2 = cars[i].x1 + (cars[i].span - 1);
            cars[i].y2 = cars[i].y1;

            if (BOARD_SIZE - 1 < cars[i].x2) {
                fprintf(stderr, "file data incorrect\n");
                return 1;
            }
        } else {
            fprintf(stderr, "file data incorrect\n");
            return 1;
        }
    }

    return 0;
}

void
display (void)
{
	/* The beginning state of board1.txt must be shown as follows: 
 	 + + 2 + + +
 	 + + 2 + + +
	 1 1 2 + + +
	 3 3 3 + + 4
	 + + + + + 4
	 + + + + + 4
	*/

	//FIXME
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (0 == cells[i][j]) {
                printf("+");
            } else {
                printf("%d", cells[i][j]);
            }

            printf(" ");
        }

        printf("\n");
    }
}

int 
update_cells (void)
{
	memset(cells, 0, sizeof(int) * 36) ; // clear cells before the write.

	//FIXME
	// return 0 for sucess
	// return 1 if the given car information (cars) has a problem
    for (int i = 0; i < n_cars; i++) {
        int x = cars[i].x1;
        int y = cars[i].y1;

        for (int j = 0; j < cars[i].span; j++) {
            if (0 != cells[y][x]) {
                return 1;
            }

            cells[y][x] = cars[i].id;

            if (vertical == cars[i].dir) {
                y++;
            } else {
                x++;
            }
        }
    }

    return 0;
}

int
move (int id, int op) 
{
	//FIXME
	// move returns 1 when the given input is invalid.
	// or return 0 for a success.

	// Update cars[id].x1, cars[id].x2, cars[id].y1 and cars[id].y2
	//   according to the given command (op) if it is possible.

	// The condition that car_id can move left is when 
	//  (1) car_id is horizontally placed, and
	//  (2) the minimum x value of car_id is greather than 0, and
	//  (3) no car is placed at cells[cars[id].y1][cars[id].x1-1].
	// You can find the condition for moving right, up, down as
	//   a similar fashion.
    id--;

    if (id < 0 || n_cars <= id) {
        fprintf(stderr, "move invalid\n");
        return 1;
    }

    if (vertical == cars[id].dir) {
        if (left == op || right == op) {
            fprintf(stderr, "move invalid\n");
            return 1;
        }
    } else {
        if (up == op || down == op) {
            fprintf(stderr, "move invalid\n");
            return 1;
        }
    }

    if (vertical == cars[id].dir) {
        if (down == op) {
            if ((cars[id].y2 == BOARD_SIZE - 1) ||
                (cells[cars[id].y2 + 1][cars[id].x1] != 0)) {
                fprintf(stderr, "move invalid\n");
                return 2;
            }

            cars[id].y1++;
            cars[id].y2++;
        } else {
            if ((cars[id].y1 == BOARD_SIZE - 1) ||
                (cells[cars[id].y1 - 1][cars[id].x1] != 0)) {
                fprintf(stderr, "move invalid\n");
                return 2;
            }

            cars[id].y1--;
            cars[id].y2--;
        }
    } else {
        if (right == op) {
            if ((cars[id].x2 == BOARD_SIZE - 1) ||
                (cells[cars[id].y1][cars[id].x2 + 1] != 0)) {
                fprintf(stderr, "move invalid\n");
                return 2;
            }

            cars[id].x1++;
            cars[id].x2++;
        } else {
            if ((cars[id].x1 == 0) ||
                (cells[cars[id].y1][cars[id].x1 - 1] != 0)) {
                fprintf(stderr, "move invalid\n");
                return 2;
            }

            cars[id].x1--;
            cars[id].x2--;
        }
    }

    return 0;
}

void
free_alloc(void)
{
    free(cars);
}

int
main (void)
{
	char buf[128] ;
	int op ;
	int id ;

	while (1) {
		scanf("%127s", buf) ;

		switch (op = get_op_code(buf)) {
			case start:
				scanf("%s", buf) ;

				load_game(buf) ;
				update_cells() ;
				display() ;
                break;

			case left:  // fallthrough
			case right: // fallthrough
			case up:    // fallthrough
			case down:
				scanf("%d", &id) ;

                if (0x0 == cars) {
                    fprintf(stderr, "init game\n");
                    break;
                }

				move(id, op) ;
				update_cells() ;
				display() ;

                if (1 == cells[(BOARD_SIZE / 2) - 1][BOARD_SIZE - 1]) {
                    printf("Game Over\n");
                    free_alloc();
                    return EXIT_SUCCESS;
                }

                break;

			//FIXME
            case quit:
                // free allocated memory
                if (cars != 0x0) {
                    free_alloc();
                }

                return EXIT_SUCCESS;
            default:
                break;
		}
	}

    if (cars != 0x0) { // clean up
        free_alloc();
    }
    
    return EXIT_SUCCESS;
}
