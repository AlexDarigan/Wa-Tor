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

const int ROWS = 10;
const int COLUMNS = 10;
const int NUM_FISH = -1;
const int NUM_SHARK = -1;
const int FISH_BREED = 3;
const int SHARK_BREED = 3;
const int SHARK_STARVE = 3;
const int NUM_THREADS = 1;
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

  private:
    CellType celltype = CellType::Ocean;
    int energy = SHARK_STARVE;
    int turn = 0;
    sf::Color color = sf::Color::Blue;
    int x;
    int y;

  public:

    bool hasMoved = false;
    bool isOcean() { return celltype == CellType::Ocean; };
    bool isFish() { return celltype == CellType::Fish; };
    bool isShark() { return celltype == CellType::Shark; };
    void setMove(bool _hasMoved) { hasMoved = _hasMoved; }
    int getX() { return x; }
    int getY() { return y; }

    
    void moveFish(int x, int y) {  
      // get neighbours
      int north = (y - 1 + ROWS) % ROWS;
      int south = y + 1 % ROWS;
      int east = x + 1 % COLUMNS;
      int west = (x - 1 + COLUMNS) % COLUMNS;
      Cell neighbours[4] = {getCell(x, north), getCell(east, y), getCell(x, south), getCell(west, y)};
      int location = rand() % 4;
      for(int i = 0; i < 4; i++) {
        int x2 = neighbours[location].getX();
        int y2 = neighbours[location].getY();
        if(getCell(x2, y2).isOcean()) {
          turn = turn + 1;
          hasMoved = true;
          Cell prev;
          prev = prev.toOcean();
          if(turn == FISH_BREED) {
            turn = 0;
            prev = prev.toFish();
          }
          setCell(x2, y2, *this);
          setCell(x, y, prev);
          break;
        }
       if(!hasMoved) { location = (location + 1) % 4; }
     }
    }

    Cell toOcean() {
      celltype = CellType::Ocean;
      color = sf::Color::Blue;
      return *this;
    }

    Cell toFish() {
      celltype = CellType::Fish;
      color = sf::Color::Green;
      turn = 0;
      hasMoved = false;
      return *this;
    }

    Cell toShark() {
      celltype = CellType::Shark;
      color = sf::Color::Red;
      energy = 0;
      turn = 0;
      hasMoved = false;
      return *this;
    }

    void setPosition(int _x, int _y) {
      x = _x;
      y = _y;
    }

    sf::Color getFillColor() { return this->color; }
};

sf::RenderWindow window(sf::VideoMode(WindowXSize,WindowYSize), "SFML Wa-Tor world");
sf::RectangleShape display[ROWS][COLUMNS];
Cell cells[ROWS][COLUMNS];

Cell getCell(int x, int y) {
  return cells[x][y];
}

void setCell(int x, int y, Cell cell) {
  cells[x][y] = cell;
  cells[x][y].setPosition(x, y);
}

void countFish() {
    // Not working properly, too many fishes
    int fishes = 0;
    int shark = 0;
    for(int y = 0; y < ROWS; ++y) {
      for(int x = 0; x < COLUMNS; ++x) {
        if(cells[x][y].isFish()) {
          fishes++;
        } else if(cells[x][y].isShark()) {
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
  sf::sleep(sf::seconds(2));
  window.display();
}

void setColors() {
  for(int y = 0; y < ROWS; ++y) {
    for(int x = 0; x < COLUMNS; ++x) {
      display[x][y].setFillColor(cells[x][y].getFillColor());
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
      //if(false)
      if(id%6==0) {
     // if(i == 7 && k == 7) {
        Cell c;
        setCell(i, k, c.toShark());
      }
      //else if (id%2==0) { 
      else if(i == 5 && k == 6) {  
        Cell c;
        setCell(i,k, c.toFish());
      }
      else {
        Cell c;
        setCell(i, k, c.toOcean());
      }
    }
  }
}

// bool moveFish(int x, int y, int x2, int y2) {
//   if(cells[x2][y2].celltype == CellType::Ocean) {
//     cells[x][y].turn++;
//     cells[x2][y2] = cells[x][y];
//     cells[x2][y2].setMove(true);
//     if(cells[x2][y2].turn == FISH_BREED) {
//       cells[x][y].toFish();
//       cells[x2][y2].turn = 0;
//     } else {
//       cells[x][y].celltype = CellType::Ocean;
//     }
//     return true;
//   }
//   return false;
// }

// bool moveShark(int x, int y, int x2, int y2, bool justMove = false) {
//   if(cells[x2][y2].isFish()) {
//     cells[x2][y2] = cells[x][y];
//     cells[x2][y2].setMove(true);
//     cells[x2][y2].energy--;
//     cells[x][y].celltype = CellType::Ocean;
//     if(cells[x2][y2].turn == SHARK_BREED) {
//       cells[x][y].toShark();
//       cells[x2][y2].turn = 0;
//     } else {
//       cells[x][y].celltype = CellType::Ocean;
//     }
//     return true;
//   }
//   return false;
// }


// void count() {
//   for(int i=0;i<ROWS;++i) {
//     for(int k=0;k<COLUMNS;++k){
//       printf("%d\n", cells[i][k].celltype);
//     }
//   }
// }

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

    
    //Move Fish
    for(int y = 0; y < ROWS; ++y) {
      for(int x = 0; x < COLUMNS; ++x) {
        if(cells[x][y].isFish() && !cells[x][y].hasMoved) {
          cells[x][y].moveFish(x, y);
      }
    }
  }

    // Move Shark
  //   for(int y = 0; y < ROWS; ++y) {
  //     for(int x = 0; x < COLUMNS; ++x) {
  //       if(cells[x][y].isShark() && !cells[x][y].hasMoved) {
  //         bool hasMoved = false;
  //         int lookDirection = rand() % 4;
  //         cells[x][y].energy++;
  //         for(int i = 0; i < 4; ++i) {
  //           switch (lookDirection) {
  //             case North: {
  //                 int north = (y - 1 + ROWS) % ROWS;
  //                 hasMoved = moveShark(x, y, x, north);
  //               }
  //               break;
  //             case East: {
  //                 int east = (x + 1) % COLUMNS;
  //                 hasMoved = moveShark(x, y, east, y);
  //               }
  //               break;
  //             case South: {
  //                 int south = (y + 1) % ROWS;
  //                 hasMoved = moveShark(x, y, x, south);                
  //               }
  //               break;
  //             case West: {
  //               int west = (x - 1 + COLUMNS) % COLUMNS;
  //               hasMoved = moveShark(x, y, west, y);
  //               }
  //               break;
  //             default:
  //               break;
  //           }
  //           lookDirection = (lookDirection + 1) % 4;
  //           if(hasMoved) { 
  //             break; 
  //           }
  //         }
  //         if(cells[x][y].isShark() && cells[x][y].energy == SHARK_STARVE) {
  //           cells[x][y].celltype = Ocean;
  //         }
  //     }
  //   }
  // }

    for(int y = 0; y < ROWS; ++y) {
      for(int x = 0; x < COLUMNS; ++x) {
        cells[x][y].hasMoved = false;
      }
    }

    setColors();
    draw();
    countFish();
  }
    
    return 0;
}