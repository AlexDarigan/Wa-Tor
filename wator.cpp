// wator.cpp --- 
// 
// Filename: wator.cpp
// Description: 
// Author: Joseph Kehoe
// Maintainer: David Darigan
// Created: Fri Nov  9

// Commentary: 
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
// 
// Code:

/*! \file Wa-Tor File
    \brief Code to implement the Wa-Tor (https://en.wikipedia.org/wiki/Wa-Tor) simulation concurrently using
    SFML for graphics and OpenMP for concurrency
*/

/*! \fn Wa-Tor File
    \brief A Function
*/



#include <chrono>
#include <SFML/Graphics.hpp>
#include <cstdlib> 
#include <iostream>
#include <omp.h>
#include <iostream>
#include <fstream>
#include "Semaphore.h"

const int MAX_LOCK = 20;
const int MAX_DURATIONS = 1000;
const int MAX_THREADS = 8;
const int NUM_THREADS = 4;
const int ROWS = 600;
const int COLUMNS = 600;
const int NUM_FISH = -1;
const int NUM_SHARK = -1;
const int FISH_BREED = 1;
const int SHARK_BREED = 6;
const int SHARK_STARVE = 2;
const int SHARK_ENERGY_GAIN = 1;
const int WindowXSize = 800;
const int WindowYSize = 600;
const int cellXSize = WindowXSize / ROWS;
const int cellYSize = WindowYSize / COLUMNS;
enum Direction { North = 0, East = 1, South = 2, West = 3 };
enum CellType { Ocean, Fish, Shark };
struct Cell;
Cell getCell(int x, int y);
void setCell(int x, int y, Cell cell);

struct Cell {
    CellType celltype;
    sf::Color color;
    int energy = 0;
    int turn = 0;
    int x;
    int y;
    bool hasMoved = false;
};

sf::RenderWindow window(sf::VideoMode(WindowXSize,WindowYSize), "SFML Wa-Tor world");
sf::RectangleShape display[ROWS][COLUMNS];
Cell cells[ROWS][COLUMNS];
Semaphore locks[MAX_LOCK];
drand48_data DecisionData[MAX_THREADS];
int durations[MAX_DURATIONS];


/*! \fn getFillColor
    \param x - The x co-ordinate of the cell
    \param y - The y co-ordinate of the cell
    \brief Returns the fill color for the cell type (Ocean - Blue, Fish - Green & Shark - Red) at the x, y coordinates
*/
sf::Color getFillColor(int x, int y) { return cells[y][x].color; }


/*! \fn isOcean
    \param x - The x co-ordinate of the cell
    \param y - The y co-ordinate of the cell
    \return bool
    \brief Returns true if the cell at the x,y co-ordinates is an Ocean
*/
bool isOcean(int x, int y) { return cells[y][x].celltype == CellType::Ocean; };


/*! \fn isFish
    \param x - The x co-ordinate of the cell
    \param y - The y co-ordinate of the cell
    \return bool
    \brief Returns true if the cell at the x,y co-ordinates is a Fish
    \brief A Function
*/
bool isFish(int x, int y) { return cells[y][x].celltype == CellType::Fish; };


/*! \fn isShark
    \param x - The x co-ordinate of the cell
    \param y - The y co-ordinate of the cell
    \return bool
    \brief Returns true if the cell at the x,y co-ordinates is a Shark
*/
bool isShark(int x, int y) { return cells[y][x].celltype == CellType::Shark; };


/*! \fn hasMoved
    \param x - The x co-ordinate of the cell
    \param y - The y co-ordinate of the cell
    \return bool
    \brief Returns true if the cell has already move during the current game loop
*/
bool hasMoved(int x, int y) { return cells[y][x].hasMoved; }

/*! \fn setOcean
    \param x - The x co-ordinate of the cell
    \param y - The y co-ordinate of the cell
    \brief Sets the cell at the x, y co-ordinates as an Ocean Cell
*/
void setOcean(int x, int y) { 
  Cell cell;
  cell.celltype = CellType::Ocean;
  cell.color = sf::Color::Blue;
  setCell(x, y, cell);
}


/*! \fn setFish
    \param x - The x co-ordinate of the cell
    \param y - The y co-ordinate of the cell
    \brief Sets the cell at the x, y co-ordinates as a Fish Cell
*/
void setFish(int x, int y) { 
  Cell cell;
  cell.celltype = CellType::Fish;
  cell.color = sf::Color::Green;
  cell.turn = 0;
  cell.hasMoved = false;
  setCell(x, y, cell);
}

/*! \fn setShark
    \param x - The x co-ordinate of the cell
    \param y - The y co-ordinate of the cell
    \brief Set's the cell at the x, y co-ordinates as a Shark Cell
*/
void setShark(int x, int y) { 
  Cell cell;
  cell.celltype = CellType::Shark;
  cell.color = sf::Color::Red;
  cell.energy = SHARK_STARVE;
  cell.turn = 0;
  cell.hasMoved = false;
  setCell(x, y, cell);
}


/*! \fn getCell
    \param x - The x co-ordinate of the cell
    \param y - The y co-ordinate of the cell
    \return Cell
    \brief Returns a value-copy of the Cell located at the x, y coordinates
*/
Cell getCell(int x, int y) {
  return cells[y][x];
}


/*! \fn setCell
    \param x - The x co-ordinate of the cell
    \param y - The y co-ordinate of the cell
    \param Cell
    \brief Sets the cell at the x, y co-ordinates as a value copy of the Cell cell parameter
*/
void setCell(int x, int y, Cell cell) {
  cell.x = x;
  cell.y = y;
  cells[y][x] = cell;
}


/*! \fn setNeighbours
    \param x - The x co-ordinate of the center cell
    \param y - The y co-ordinate of the center cell
    \param [out] Cell[] - A list to be populated with neighbours
    \brief Populates Cell[] with the north, south, east and west neighbours of the Cell at the x, y co-ordinates
*/
void setNeighbours(int x, int y, Cell list[]) {
  int north = ((y - 1) + ROWS) % ROWS;
  int south = (y + 1) % ROWS; // no braces cause segfault
  int east = (x + 1) % COLUMNS; // no braces cause segfault
  int west = ((x - 1) + COLUMNS) % COLUMNS;
  list[0] = getCell(x, north);
  list[1] = getCell(east, y);
  list[2] = getCell(x, south);
  list[3] = getCell(west, y);
}


/*! \fn moveFish
    \param x - The x co-ordinate of the cell
    \param y - The y co-ordinate of the cell
    \brief The function dedicated to moving the fish at the x, y co-ordinates
*/
void moveFish(int x, int y) { 
  Cell fish = getCell(x, y);
  Cell neighbours[4];
  setNeighbours(x, y, neighbours);
  long location;
  lrand48_r(&DecisionData[omp_get_thread_num()], &location);
  location = location % 4;
  for(int i = 0; i < 4; i++) {
    int x2 = neighbours[location].x;
    int y2 = neighbours[location].y;
    if(isOcean(x2, y2)) {
      fish.turn = fish.turn + 1;
      fish.hasMoved = true;
      setOcean(x, y);
      if(fish.turn == FISH_BREED) {
        fish.turn = 0;
        setFish(x, y);
      }
      setCell(x2, y2, fish);
      break;
    }
    if(!fish.hasMoved) { location = (location + 1) % 4; }
  }
}


/*! \fn moveShark
    \param x - The x co-ordinate of the cell
    \param y - The y co-ordinate of the cell
    \brief The function dedicated to moving the Shark at the x, y co-ordinates
*/
void moveShark(int x, int y) {
      Cell shark = getCell(x, y);
      Cell neighbours[4];
      setNeighbours(x, y, neighbours);
      long location;
      lrand48_r(&DecisionData[omp_get_thread_num()], &location);
      location = location % 4;
      shark.energy--;
      shark.turn++;
      int x2, y2;
      // Hunting Fish
      for(int i = 0; i < 4; i++) {
        x2 = neighbours[location].x;
        y2 = neighbours[location].y;
        if(isFish(x2, y2)) {
          shark.hasMoved = true;
          shark.energy += SHARK_ENERGY_GAIN;
          setOcean(x, y);
          break;
        }
        if(!shark.hasMoved) { location = (location + 1) % 4;}
      }

      // No fish found
      // Finding free space to move
      if(!shark.hasMoved) {
          for(int i = 0; i < 4; ++i) {
            int x2 = neighbours[location].x;
            int y2 = neighbours[location].y;
            if(isOcean(x2, y2)) {
              shark.hasMoved = true;
              setOcean(x, y);
              break;
            }
          if(!shark.hasMoved) { location = (location + 1) % 4;}
        }
      }

      if(shark.hasMoved) {
        if(shark.turn >= SHARK_BREED) {
          shark.turn = 0;
          setShark(x, y);
        }

        if(shark.energy < 0) {
          setOcean(x2, y2);
        } else {
          setCell(x2, y2, shark);
        }
      } else {
          // Sharks still starve if they don't move
          if(shark.energy < 0) {
            setOcean(x, y);
          } else {
            setCell(x2, y2, shark);
          }
      }
}


/*! \fn poll
    \brief Wrapper function around SFML's poll event
*/
void poll() {
  sf::Event event;
  while (window.pollEvent(event))
  {
      if (event.type == sf::Event::Closed)
          window.close();
  }
}


/*! \fn draw
    \brief Wrapper function around SFML's drawing algorithm
*/
void draw() {
  window.clear(sf::Color::Black);
  for(int y = 0; y < ROWS; ++y) {
    for(int x = 0; x < COLUMNS; ++x) {
      window.draw(display[y][x]);
    }
  }
  window.display();
}


/*! \fn initialize
    \brief Function that initializes the game state, including seeding the random number generations, setting the
    total number of threads, preparing the mutex locks and thread-safe random buffers, as well as building and drawing
    the initial grid. It does a lot but it only does it once.
*/
void initialize() {
  srand48(0);
  omp_set_num_threads(NUM_THREADS);
  for(int i = 0; i < MAX_LOCK; ++i) { locks[i].Signal(); }
  for(int i = 0; i < MAX_THREADS; ++i) { srand48_r(0, &DecisionData[i]); }
  for(int y = 0; y < ROWS; ++y){
    for(int x = 0; x < COLUMNS; ++x){
      display[y][x].setSize(sf::Vector2f(cellXSize,cellYSize));
      display[y][x].setPosition(x * cellXSize,y * cellYSize);
      setOcean(x, y);
      int id=y * 1 -+ x;
      if(id%18==0) {
        setShark(x, y);
      }
      else if ( id % 10 == 0) { 
        setFish(x, y);
      }
      display[y][x].setFillColor(getFillColor(x, y));
    }
  }
  draw();
}


/*! \fn move
    \param x - The x co-ordinate of the cell
    \param y - The y co-ordinate of the cell
    \brief Checks that the Cell at the x, y co-ordinate is a movable fish or shark (that hasn't moved this game loop)
    and calls into their respective move functions
*/
void move(int x, int y) {
    if(isFish(x, y) && !hasMoved(x,y)) {
     moveFish(x, y);
    } 
    else if(isShark(x, y) && !hasMoved(x, y)) {
      moveShark(x, y);
  }
}


/*! \fn move
    \brief Thread-safe move function that loops over the entire grid (split by the current number of threads) and calls into
    the single-cell move function per cell.
*/
void move() {
    int id = omp_get_thread_num();
    int CHUNK_SIZE = ROWS / omp_get_num_threads();
    int start = CHUNK_SIZE * omp_get_thread_num();
    int end = start + CHUNK_SIZE;
    if(end > ROWS) {
      end = ROWS;
    }
    for(int y = start; y < end; ++y) {
      if(y == start) {
        locks[id].Wait();
        for(int x = 0; x < COLUMNS; ++x) {
          move(x, y);
          display[y][x].setFillColor(getFillColor(x, y));
        }
        locks[id].Signal();
      } else if(y == (end - 1)) {
        locks[(id + 1) % omp_get_num_threads()].Wait();
        for(int x = 0; x < COLUMNS; ++x) {
          move(x, y);
          display[y][x].setFillColor(getFillColor(x, y));
        }
        locks[(id + 1) % omp_get_num_threads()].Signal();
      } else {
        for(int x = 0; x < COLUMNS; ++x) {
          move(x, y);
          display[y][x].setFillColor(getFillColor(x, y));
        }
      }
  }
}


/*! \fn writeToFile
    \brief A debugging function used to write duration per generations to a text file
*/
void writeToFile() {
  std::ofstream file;
  file.open("threads.txt");
  for(int i = 0; i < MAX_DURATIONS; ++i) {
    file << durations[i] << ",";
  }
  file << "\n";
  file.close();
}


/*! \fn main
    \brief The main application entry point
*/
int main()
{
  initialize();
  int generation = 0;
  while (window.isOpen() && generation < MAX_DURATIONS)
  {

    poll();    
    auto start = std::chrono::steady_clock::now();
    #pragma omp parallel 
    {
      move();

      #pragma omp for collapse(2)
      for(int y = 0; y < ROWS; ++y) {
        for(int x = 0; x < COLUMNS; ++x) {
          cells[x][y].hasMoved = false;
        }
      }

      
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    // Draw takes about 5x more than anything else and can't be called from multiple threads consistently
    draw();
    printf("Generation %d Duration: %ld\n", generation, duration);
    durations[generation] = duration;
    generation++;
  }

  writeToFile();
  return 0;
}
