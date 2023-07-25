# Rush Hour
An implementation of the Rush Hour boardgame using C<br>

---

### Algorithm
I created an algorithm (coloring-algorithm) that solves basic Rush Hour puzzles. Simply, it keeps record of the previous states (colored shadows).<br>
The most difficult problem when thinking of an algorithm was that it required backtracking conditionally. The car should be kept going in a single direction (to prevent overhead), but sometimes it required backtracking for another car to pass through.<br>
After observing carefully, I realized backtracking was needed when another car passed by its previous path.<br>
So I left car-colored-shadows of previous states of each car, and when another car passes by the colors are overwritten.<br>

### Build
Execute the make file to create a runnable file
```
$ make
gcc -o src/rushhour.o -c src/rushhour.c -I./include
gcc -o rushhour src/rushhour.o
```
Run the created runnable file
```
$ ./rushhour
```

### Options
`start`: starts the game
* requires the filepath for loading data
* must be executed before any other option
* some examples are in `data/`

`right`, `left`, `down`, `up`: controls the movement of the cars
* requires the desired car's id

`bfs`, `dfs`: executes a solver algorithm

### Functions
* `commands get_op_code (char *s)`
  * returns the corresponding number (enum) for the command given as s
* `int load_game (char *filename)`
  * having recieved the data file, loads the game according to the data
  * uses fopen, getline, strtok, atoi, strcmp
* `void display (void)`
  * works simple & intuitively
  * prints ids of cars (`+` when 0) from 0 to BOARD_SIZE of the 2D array
  * also prints ids of car colors (`+` when -1) from 0 to BOARD_SIZE of the 2D array
* `int update_cells (void)`
  * updates `cells[]` and `colored_cells[]` according to the data of `cars[]`
* `int move (int id, int op)`
  * selects a car by its id: `cars[id]`
  * moves the selected car by the given command
* `void free_alloc (void)`
  * frees allocated memory to `cars`
  * acts as a wrapper function for extendibility
* `int bfs_solver (void)`
* `int dfs_solver (void)`
  * TBC (To Be Coded)
* `int find_movable_car (void)`
  * return a car id that has an available moving space
* `void color_adjacent_cells (int car_id, int y, int x)`
  * when colors crossover, color adjacent row/column with newer color
  * if a car is in closer to the newer color than colors, don't overwrite
* `int color_cells (int car_id, int y, int x, commands dir)`
  * colors cells and moves the car
  * also checks adjacent cells by calling, `color_adjacent_cells()`
* `int move_to_white (int car_id)`
  * check both sides of a car's direction
  * if white (`0`) is found, call `color_celss()` toward it
  * if a different car id is found compare with the other sides result
* `int color_solver (void)`
  * acts as the driver, calls coloring-algorithm-related functions
  * loops the process till game over or all options are exhausted