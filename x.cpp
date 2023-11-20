NumShark Starting population of sharks;
NumFish Starting population of fish;
FishBreed Number of time units that pass before a fish can reproduce;
SharkBreed Number of time units that must pass before a shark can repro-
duce;
Starve Period of time a shark can go without food before dying;
GridSize Dimensions of world;
Threads Number of threads to use.

const int ROWS = 100;
const int COLUMNS = 100;
const int FISH = 10;
const int SHARKS = 10;
const int FISH_BREED = 3;
const int SHARK_BREED = 3;
const int SHARK_STARVATION = 3;
int grid[ROWS][COLUMNS];

enum CellType { OCEAN, FISH, SHARK }

struct Cell {
    CellType celltype;
}

int* getNeighbours() {

}

void moveFish();
void moveShark();
void count();
void color();
void poll();
void draw();