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

const int ROWS = 10;//600;
const int COLUMNS = 10; //600;
const int NUM_FISH = -1;
const int NUM_SHARK = -1;
const int FISH_BREED = 3;
const int SHARK_BREED = 6;
const int SHARK_STARVE = 2;
const int NUM_THREADS = 1;
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

sf::Color getFillColor(int x, int y) { return cells[x][y].color; }
bool isOcean(int x, int y) { return cells[x][y].celltype == CellType::Ocean; };
bool isFish(int x, int y) { return cells[x][y].celltype == CellType::Fish; };
bool isShark(int x, int y) { return cells[x][y].celltype == CellType::Shark; };

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
  return cells[x][y];
}

void setCell(int x, int y, Cell cell) {
  cell.x = x;
  cell.y = y;
  cells[x][y] = cell;
}

  void moveFish(int x, int y) { 
      Cell fish = cells[x][y];
      int north = ((y - 1) + ROWS) % ROWS;
      int south = y + 1 % ROWS;
      int east = x + 1 % COLUMNS;
      int west = ((x - 1) + COLUMNS) % COLUMNS;
      Cell neighbours[4] = {getCell(x, north), getCell(east, y), getCell(x, south), getCell(west, y)};
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

void moveShark(int x, int y) {
      Cell shark = cells[x][y];
      int north = (y - 1 + ROWS) % ROWS;
      int south = y + 1 % ROWS;
      int east = x + 1 % COLUMNS;
      int west = (x - 1 + COLUMNS) % COLUMNS;
      Cell neighbours[4] = {getCell(x, north), getCell(east, y), getCell(x, south), getCell(west, y)};
      int location = rand() % 4;
    
      shark.energy--;
      shark.turn++;
      for(int i = 0; i < 4; i++) {
        int x2 = neighbours[location].x;
        int y2 = neighbours[location].y;
        if(isFish(x2, y2)) {
          shark.hasMoved = true;
          shark.energy += SHARK_ENERGY_GAIN;
          setOcean(x, y);
          if(shark.turn >= SHARK_BREED) {
            shark.turn = 0;
            setShark(x, y);
          }
          if(shark.energy < SHARK_STARVE) {
            setOcean(x2, y2);
          }
          break;
        }
        if(!shark.hasMoved) { location = (location + 1) % 4;}
      }
      if(shark.hasMoved) { return; }
      for(int i = 0; i < 4; ++i) {
        int x2 = neighbours[location].x;
        int y2 = neighbours[location].y;
        if(isOcean(x2, y2)) {
          shark.hasMoved = true;
          setOcean(x, y);
          if(shark.turn >= SHARK_BREED) {
            shark.turn = 0;
            setShark(x, y);
          }
          if(shark.energy < SHARK_STARVE) {
            setOcean(x2, y2);
          }
          break;
        }
      if(!shark.hasMoved) { location = (location + 1) % 4;}
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
  for(int i = 0; i < ROWS; ++i){
    for(int k = 0; k < COLUMNS; ++k){
      window.draw(display[i][k]);
    }
  }
  sf::sleep(sf::seconds(1));
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
      display[x][y].setFillColor(getFillColor(x, y));
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
  for(int i=0;i<ROWS;++i){
    for(int k=0;k<COLUMNS;++k){
      display[i][k].setSize(sf::Vector2f(cellXSize,cellYSize));
      display[i][k].setPosition(i*cellXSize,k*cellYSize);
      int id=i*1-+k;
      if(id%9==0) {
        setShark(i, k);
      }
      else if (id%6==0) { 
        setFish(i, k);
      }
      else {
        setOcean(i, k);
      }
    }
  }
}

int main()
{
  srand(0);
  omp_set_num_threads(NUM_THREADS);
  initialize();
  setColors();
  draw();
  while (window.isOpen())
  {
    poll();
    
    
    auto start = std::chrono::steady_clock::now();
  //   //Move Fish
  //   #pragma omp parallel for
    for(int y = 0; y < ROWS - 1; ++y) {
      for(int x = 0; x < COLUMNS - 1; ++x) {
        if(isFish(x, y) && !cells[x][y].hasMoved) {
          //cells[x][y].moveFish(x, y);
          moveFish(x, y);
        }
      }
    }
    
    // Move Shark
 //   #pragma omp parallel for
    // for(int y = 0; y < ROWS - 1; ++y) {
    //   for(int x = 0; x < COLUMNS - 1; ++x) {
    //     if(isShark(x, y) && !cells[x][y].hasMoved) {
    //       moveShark(x, y);
    //     }
    //   }
    // }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    printf("Duration: %ld\n", duration);

    clearMoves();
    setColors();
    draw();
    countFish();
  }
    
    return 0;
}