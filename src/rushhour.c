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
    if (!fp) {
        perror("fopen");
        return 1;
    }

    char *buf = (char *) malloc(1024);
    size_t n_buf = 1024;

    fscanf(fp, "%d", &n_cars); // first line, number of cars
    cars = (car_t *) malloc(sizeof(car_t) * (n_cars + 1)); // cars in list

    getline(&buf, &n_buf, fp); // skip first line 

    int len;
    for (int i = 1; i <= n_cars; i++) {
        char *sep = ":\n";
        char *token;

        len = getline(&buf, &n_buf, fp);
        cars[i].id = i; // id

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

    printf("\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (-1 == colored_cells[i][j]) {
                printf("+");
            } else {
                printf("%d", colored_cells[i][j]);
            }

            printf(" ");
        }

        printf("\n");
    }

    printf("\n\n");
}

int 
update_cells (void)
{
	memset(cells, 0, sizeof(int) * 36) ; // clear cells before the write.

	//FIXME
	// return 0 for sucess
	// return 1 if the given car information (cars) has a problem
    for (int i = 1; i <= n_cars; i++) {
        int x = cars[i].x1;
        int y = cars[i].y1;

        for (int j = 0; j < cars[i].span; j++) {
            if (0 != cells[y][x]) {
                return 1;
            }

            cells[y][x] = cars[i].id;
            colored_cells[y][x] = -1;

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
    if (id <= 0 || n_cars < id) {
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
free_alloc (void)
{
    free(cars);
}

int
bfs_solver (void)
{
    // return 0 for SUCCESS
    // return 1 for FAILURE
    return 0;
}

int
dfs_solver (void)
{
    // return 0 for SUCCESS
    // return 1 for FAILURE
    return 0;
}

int
find_movable_car (void)
{
    // returns car id with movable cell
    // either white or has another color
    // if only its color, return -1
    for (int i = 1; i <= n_cars; i++) {
        if (vertical == cars[i].dir) {
            if (0 < cars[i].y1) { // check up
                if (cars[i].id != colored_cells[cars[i].y1 - 1][cars[i].x1] &&
                    -1 != colored_cells[cars[i].y1 - 1][cars[i].x1])
                {
                    return i;
                }
            }
            
            if (cars[i].y2 < BOARD_SIZE - 1) { // check down
                if (cars[i].id != colored_cells[cars[i].y2 + 1][cars[i].x1] &&
                    -1 != colored_cells[cars[i].y2 + 1][cars[i].x1])
                {
                    return i;
                }
            }
        } else { // horizontal
            if (0 < cars[i].x1) { // check left
                if (cars[i].id != colored_cells[cars[i].y1][cars[i].x1 - 1] &&
                    -1 != colored_cells[cars[i].y1][cars[i].x1 - 1])
                {
                    return i;
                }
            }

            if (cars[i].x2 < BOARD_SIZE - 1) { // check right
                if (cars[i].id != colored_cells[cars[i].y1][cars[i].x2 + 1] &&
                    -1 != colored_cells[cars[i].y1][cars[i].x2 + 1])
                {
                    return i;
                }
            }
        }
    }

    return -1; // no cases found
}

int
color_cell (int car_id, int y, int x, commands dir)
{
    colored_cells[y][x] = car_id; // the prev state
    move(car_id, dir);
    update_cells();

    return 0;
}

// check_adjacent()

int
move_to_white (int car_id)
{
    // return 0 for SUCCESS
    // return 1 for FAILURE
    //
    // check both sides of car's direction
    // move to first found white
    // color the prev state
    // if adjacent cells of prev state are a different color
    // clear them to white
    int flag1 = 0;
    int flag2 = 0;

    if (vertical == cars[car_id].dir) {
        if (0 < cars[car_id].y1) { // check up
            if (0 == colored_cells[cars[car_id].y1 - 1][cars[car_id].x1] &&
                -1 != colored_cells[cars[car_id].y1 - 1][cars[car_id].x1])
            {
                return color_cell(car_id, cars[car_id].y2, cars[car_id].x1, up); // prev state
            } else if (cars[car_id].id != colored_cells[cars[car_id].y1 - 1][cars[car_id].x1] &&
                        -1 != colored_cells[cars[car_id].y1 - 1][cars[car_id].x1])
            {
                flag1 = 1;
            }
        }
        
        if (cars[car_id].y2 < BOARD_SIZE - 1) { // check down
            if (0 == colored_cells[cars[car_id].y2 + 1][cars[car_id].x1] &&
                -1 != colored_cells[cars[car_id].y2 + 1][cars[car_id].x1])
            {
                return color_cell(car_id, cars[car_id].y1, cars[car_id].x1, down); // prev state
            } else if (cars[car_id].id != colored_cells[cars[car_id].y2 + 1][cars[car_id].x1] &&
                        -1 != colored_cells[cars[car_id].y2 + 1][cars[car_id].x1])
            {
                flag2 = 1;
            }
        }

        if (1 == flag1) {
            return color_cell(car_id, cars[car_id].y2, cars[car_id].x1, up);
        } else if (1 == flag2) {
            return color_cell(car_id, cars[car_id].y1, cars[car_id].x1, up);
        }
    } else { // horizontal
        if (0 < cars[car_id].x1) { // check left
            if (0 == colored_cells[cars[car_id].y1][cars[car_id].x1 - 1] &&
                -1 != colored_cells[cars[car_id].y1][cars[car_id].x1 - 1])
            {
                return color_cell(car_id, cars[car_id].y1, cars[car_id].x2, left); // prev state
            } else if (cars[car_id].id != colored_cells[cars[car_id].y1][cars[car_id].x1 - 1] &&
                        -1 != colored_cells[cars[car_id].y1][cars[car_id].x1 - 1])
            {
                flag1 = 1;
            }
        }
        
        if (cars[car_id].y2 < BOARD_SIZE - 1) { // check right
            if (0 == colored_cells[cars[car_id].y1][cars[car_id].x2 + 1] &&
                -1 != colored_cells[cars[car_id].y1][cars[car_id].x2 + 1])
            {
                return color_cell(car_id, cars[car_id].y1, cars[car_id].x1, right); // prev state
            } else if (cars[car_id].id != colored_cells[cars[car_id].y1][cars[car_id].x2 + 1] &&
                        -1 != colored_cells[cars[car_id].y1][cars[car_id].x2 + 1])
            {
                flag2 = 1;
            }
        }

        if (1 == flag1) {
            return color_cell(car_id, cars[car_id].y1, cars[car_id].x2, left);
        } else if (1 == flag2) {
            return color_cell(car_id, cars[car_id].y1, cars[car_id].x1, right);
        }
    }

    return 1;
}

int
color_solver (void)
{
    // return 0 for SUCCESS
    // return 1 for FAILURE
    while (1 != cells[(BOARD_SIZE / 2) - 1][BOARD_SIZE - 1]) { // while game not over
        int move_car_id = find_movable_car();
        if (-1 == move_car_id) { // all moves exhausted
            fprintf(stderr, "exhausted\n");
            return 1;
        }

        int move_status = move_to_white(move_car_id);
        if (1 == move_status) { // all moves exhausted
            fprintf(stderr, "exhausted\n");
            return 1;
        }

        display();
    }

    return 0;
}

int (* solver) (void);

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
                if (0x0 == cars) {
                    fprintf(stderr, "init game\n");
                    break;
                }

				scanf("%d", &id) ;

				move(id, op) ;
				update_cells() ;
				display() ;

                if (1 == cells[(BOARD_SIZE / 2) - 1][BOARD_SIZE - 1]) {
                    printf("Game Over\n");
                    free_alloc();

                    return EXIT_SUCCESS;
                }

                break;

            case bfs:
                solver = &bfs_solver;
                goto solving;
            case dfs:
                solver = &dfs_solver;
                goto solving;
            case color:
                solver = &color_solver;

            solving: // goto
                if (0x0 == cars) {
                    fprintf(stderr, "init game\n");
                    break;
                }

                int solved = solver();

                if (0 == solved) {
                    printf("Game Over\n");
                    free_alloc();

                    return EXIT_SUCCESS;
                } else {
                    // in case fails to solve program
                    // exhausts all options
                    fprintf(stderr, "exhausted\n");
                    free_alloc();

                    return EXIT_FAILURE;
                }

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
