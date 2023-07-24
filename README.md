# Rush Hour
An implementation of the Rush Hour boardgame using C
---

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
`right`, `left`, `down`, `up`: controls the movement of the cars
* requires the desired car's id
`bfs`, `dfs`: executes a solver algorithm

### Functions
* `display(void)`
  * works simple & intuitively
  * prints ids of cars (`+` when 0) from 0 to BOARD_SIZE of the 2D array
* `free_alloc(void)`
  * frees allocated memory to `cars`
  * acts as a wrapper function for extendibility
