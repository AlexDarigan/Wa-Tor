// wator.cpp --- 
// 
// Filename: wator.cpp
// Description: 
// Author: David Darigan
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

#include <chrono>
#include <SFML/Graphics.hpp>
#include <cstdlib> 
#include <iostream>
#include <omp.h>

const int ROWS = 10;// 600;//600;
const int COLUMNS = 10;//600; //600;
const int NUM_FISH = -1;
const int NUM_SHARK = -1;
const int FISH_BREED = 3;
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

sf::Color getFillColor(int x, int y) { return cells[y][x].color; }
bool isOcean(int x, int y) { return cells[y][x].celltype == CellType::Ocean; };
bool isFish(int x, int y) { return cells[y][x].celltype == CellType::Fish; };
bool isShark(int x, int y) { return cells[y][x].celltype == CellType::Shark; };
bool hasMoved(int x, int y) { return cells[y][x].hasMoved; }

// Spawn Methods
void setOcean(int x, int y) { 
  Cell cell;
  cell.celltype = CellType::Ocean;
  cell.color = sf::Color::Blue;
  setCell(x, y, cell);
}

void setFish(int x, int y) { 
  Cell cell;
  cell.celltype = CellType::Fish;
  cell.color = sf::Color::Green;
  cell.turn = 0;
  cell.hasMoved = false;
  setCell(x, y, cell);
}

void setShark(int x, int y) { 
  Cell cell;
  cell.celltype = CellType::Shark;
  cell.color = sf::Color::Red;
  cell.energy = SHARK_STARVE;
  cell.turn = 0;
  cell.hasMoved = false;
  setCell(x, y, cell);
}

Cell getCell(int x, int y) {
  return cells[y][x];
}

void setCell(int x, int y, Cell cell) {
  cell.x = x;
  cell.y = y;
  cells[y][x] = cell;
}

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

void moveFish(int x, int y) { 
  Cell fish = getCell(x, y);
  Cell neighbours[4];
  setNeighbours(x, y, neighbours);
  int location = rand() % 4;
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

// Sharks should starve even if they don't move
void moveShark(int x, int y) {
      Cell shark = getCell(x, y);
      Cell neighbours[4];
      setNeighbours(x, y, neighbours);
      int location = rand() % 4;
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


void countFish() {
    int fishes = 0;
    int shark = 0;
    for(int y = 0; y < ROWS; ++y) {
      for(int x = 0; x < COLUMNS; ++x) {
        if(isFish(x, y)) {
          fishes++;
        } else if(isShark(x, y)) {
          shark++;
        }
      }
    }
    printf("Fishes: %d\nSharks: %d\n", fishes, shark);
  }


void draw() {
  window.clear(sf::Color::Black);
  for(int y = 0; y < ROWS; ++y){
    for(int x = 0; x < COLUMNS; ++x){
      window.draw(display[y][x]);
    }
  }
  sf::sleep(sf::seconds(2));
  window.display();
}

void clearMoves() {
  for(int y = 0; y < ROWS; ++y) {
    for(int x = 0; x < COLUMNS; ++x) {
      cells[x][y].hasMoved = false;
    }
  }
}

void setColors() {
  for(int y = 0; y < ROWS; ++y) {
    for(int x = 0; x < COLUMNS; ++x) {
      display[y][x].setFillColor(getFillColor(x, y));
    }
  }
}

void poll() {
  sf::Event event;
  while (window.pollEvent(event))
  {
      if (event.type == sf::Event::Closed)
          window.close();
  }
}

void initialize() {
  for(int y = 0; y < ROWS; ++y){
    for(int x = 0; x < COLUMNS; ++x){
      display[y][x].setSize(sf::Vector2f(cellXSize,cellYSize));
      display[y][x].setPosition(x * cellXSize,y * cellYSize);
      setOcean(x, y);
      int id=y * 1 -+ x;
      // if(id%9==0) {
      //   setShark(y, x);
      // }
      // else if ( id % 6 == 0) { 
      //   setFish(y, x);
      // }
      if(y == 5 && x == 9) {
        setFish(x, y);
      }
    }
  }
}

const int NUM_THREADS = 3;


int main()
{
  srand(0);
  omp_set_num_threads(NUM_THREADS);
  initialize();
  setColors();
  draw();
  int generation = 0;
  while (window.isOpen())
  {
    poll();
    auto start = std::chrono::steady_clock::now();
    #pragma omp parallel
    {
      int chunkSize = ROWS / omp_get_num_threads();
      int start = chunkSize * omp_get_thread_num();
      int end = start + chunkSize;
      // #pragma omp parallel for collapse(2)
      // for (int y = start + 1; y < end - 1; ++y) {
      //   for (int x = y; x >= 0; --x) { 
      //        if(isFish(x, y) && !hasMoved(x,y)) {
      //         moveFish(x, y);
      //       }
      //       else if(isShark(x, y) && !hasMoved(x, y)) {
      //         moveShark(x, y);
      //       }
      //     }
      //   }


      for(int y = start ;y < end ; ++y) {
        for(int x = 0; x < COLUMNS; ++x) {
            if(isFish(x, y) && !hasMoved(x,y)) {
              moveFish(x, y);
            }
            else if(isShark(x, y) && !hasMoved(x, y)) {
              moveShark(x, y);
            }
        }
      }
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printf("Generation %d Duration: %ld\n", generation, duration);
    generation++;

    clearMoves();
    setColors();
    draw();
  //  countFish();
  }
    
    return 0;
}