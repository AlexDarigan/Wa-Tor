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


// Defining structs because we need more information (like "moved this round")

enum Direction { North = 0, East = 1, South = 2, West = 3 };
enum CellType { Ocean, Fish, Shark };

struct Cell {
  CellType celltype = CellType::Ocean;
  bool hasMoved = false;
  int birthRate = 3;
  int starvation = 3;
  int energy = 0;
  int turn = 0;

  bool isOcean() { return celltype == CellType::Ocean; };
  bool isFish() { return celltype == CellType::Fish; };
  bool isShark() { return celltype == CellType::Shark; };
  

};

// Constants required by following arrays
const int DIMENSIONS = 10;  // (600 max) Shape is a square so x and y arrays are the same dimensions
const int UP = -1;
const int DOWN = 1;
const int RIGHT = 1;
const int LEFT = -1;
const int STANDSTILL = 0;
const bool OVERRIDING_FREEWILL = true;

// Allocating arrays on the heap by moving them to the global scope
sf::RectangleShape recArray[DIMENSIONS][DIMENSIONS];
Cell cells[DIMENSIONS][DIMENSIONS];

void countFish() {
    // Not working properly, too many fishes
    int fishes = 0;
    int shark = 0;
    for(int y = 0; y < DIMENSIONS; ++y) {
      for(int x = 0; x < DIMENSIONS; ++x) {
        if(cells[x][y].celltype == CellType::Fish) {
          fishes++;
        } else if(cells[x][y].celltype == CellType::Shark) {
          shark++;
        }
      }
    }
    printf("Fishes: %d\nSharks: %d\n", fishes, shark);
  }

const int WindowXSize=800;
const int WindowYSize=600;
const int cellXSize=WindowXSize/DIMENSIONS;
const int cellYSize=WindowYSize/DIMENSIONS;
sf::RenderWindow window(sf::VideoMode(WindowXSize,WindowYSize), "SFML Wa-Tor world");

void draw() {
   window.clear(sf::Color::Black);
    for(int i=0;i<DIMENSIONS;++i){
      for(int k=0;k<DIMENSIONS;++k){
        window.draw(recArray[i][k]);
      }
    }
      sf::sleep(sf::seconds(2));
      window.display();
}

void setColors() {
  for(int y = 0; y < DIMENSIONS; ++y) {
    for(int x = 0; x < DIMENSIONS; ++x) {
      switch(cells[x][y].celltype) {
        case CellType::Ocean:
          recArray[x][y].setFillColor(sf::Color::Blue);
          break;
        case CellType::Fish:
          recArray[x][y].setFillColor(sf::Color::Green);
          break;
        case CellType::Shark:
          recArray[x][y].setFillColor(sf::Color::Red);
          break;
        default:
          break;
        
      }
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
  for(int i=0;i<DIMENSIONS;++i){
    for(int k=0;k<DIMENSIONS;++k){
      recArray[i][k].setSize(sf::Vector2f(cellXSize,cellYSize));
      recArray[i][k].setPosition(i*cellXSize,k*cellYSize);
      int id=i*1-+k;
      //if(false)
      //if(id%5==0) {
      if(i == 7 && k == 7) {
        recArray[i][k].setFillColor(sf::Color::Red);
        cells[i][k].celltype = CellType::Shark;
      }
      //else if (id%2==0) { 
      else if(i == 5 && k == 6) {  
        recArray[i][k].setFillColor(sf::Color::Green);
        cells[i][k].celltype = CellType::Fish;
      }
      else {
        recArray[i][k].setFillColor(sf::Color::Green);
        cells[i][k].celltype = CellType::Ocean;
      }
    }
  }
}

bool moveFish(int x, int y, int x2, int y2) {
  if(cells[x2][y2].celltype == CellType::Ocean) {
    cells[x][y].turn++;
    cells[x2][y2] = cells[x][y];
    cells[x2][y2].hasMoved = true;
    if(cells[x2][y2].turn == cells[x2][y2].birthRate) {
      cells[x][y].celltype = CellType::Fish;
      cells[x][y].birthRate = 3;
      cells[x][y].turn = 0;
      cells[x2][y2].turn = 0;
    } else {
      cells[x][y].celltype = CellType::Ocean;
    }
    return true;
  }
  return false;
}

bool moveShark(int x, int y, int x2, int y2) {
  if(cells[x2][y2].isFish()) {
    cells[x2][y2] = cells[x][y];
    cells[x2][y2].hasMoved = true;
    cells[x2][y2].energy--;
    cells[x][y].celltype = CellType::Ocean;
    if(cells[x2][y2].turn == cells[x2][y2].birthRate) {
      cells[x][y].celltype = CellType::Shark;
      cells[x][y].birthRate = 3;
      cells[x][y].turn = 0;
      cells[x2][y2].turn = 0;
    } else {
      cells[x][y].celltype = CellType::Ocean;
    }
    return true;
  }
  return false;
}


void count() {
  for(int i=0;i<DIMENSIONS;++i) {
    for(int k=0;k<DIMENSIONS;++k){
      printf("%d\n", cells[i][k].celltype);
    }
  }
}

int main()
{
  srand(0);
  initialize();
  draw();
 // omp_set_num_threads(6);
  while (window.isOpen())
  {
    poll();

    // Move Fish
    for(int y = 0; y < DIMENSIONS; ++y) {
      for(int x = 0; x < DIMENSIONS; ++x) {
        if(cells[x][y].isFish() && !cells[x][y].hasMoved) {
        bool hasMoved = false;
        int direction = rand() % 4;
        for(int i = 0; i < 4; ++i) {
          switch (direction)
          {
            case North: {
              int north = (y - 1 + DIMENSIONS) % DIMENSIONS;
              hasMoved = moveFish(x, y, x, north);
            }
              break;
            case East: {
              int east = (x + 1) % DIMENSIONS;
              hasMoved = moveFish(x, y, east, y);
            }
              break;
            case South: {
              int south = (y + 1) % DIMENSIONS;
              hasMoved = moveFish(x, y, x, south);
            }
              break;
            case West: {
              int west = (x - 1 + DIMENSIONS) % DIMENSIONS;
              hasMoved = moveFish(x, y, west, y);
            }
              break;
            default:
              break;
          }
        }
        direction = (direction + 1) % 4;
        if(hasMoved) { break; }
      }
    }
  }

    // Move Shark
    for(int y = 0; y < DIMENSIONS; ++y) {
      for(int x = 0; x < DIMENSIONS; ++x) {
        if(cells[x][y].isShark() && !cells[x][y].hasMoved) {
          bool hasMoved = false;
          int lookDirection = rand() % 4;
          cells[x][y].energy++;
          for(int i = 0; i < 4; ++i) {
            switch (lookDirection) {
              case North: {
                  int north = (y - 1 + DIMENSIONS) % DIMENSIONS;
                  hasMoved = moveShark(x, y, x, north);
                }
                break;
              case East: {
                  int east = (x + 1) % DIMENSIONS;
                  hasMoved = moveShark(x, y, east, y);
                }
                break;
              case South: {
                  int south = (y + 1) % DIMENSIONS;
                  hasMoved = moveShark(x, y, x, south);                
                }
                break;
              case West: {
                int west = (x - 1 + DIMENSIONS) % DIMENSIONS;
                hasMoved = moveShark(x, y, west, y);
                }
                break;
              default:
                break;
            }
            lookDirection = (lookDirection + 1) % 4;
            if(hasMoved) { 
              break; 
            }
          }
          if(cells[x][y].isShark() && cells[x][y].energy == cells[x][y].starvation) {
            cells[x][y].celltype = Ocean;
          }
      }
    }
  }

    for(int y = 0; y < DIMENSIONS; ++y) {
      for(int x = 0; x < DIMENSIONS; ++x) {
        cells[x][y].hasMoved = false;
      }
    }

    setColors();
    draw();
    countFish();
  }
    
    return 0;
}
      
    ///setColors();
       

    // for(int y = 0; y < DIMENSIONS; ++y) {
    //   for(int x = 0; x < DIMENSIONS; ++x) {
    //     switch(cells[x][y].celltype) {
    //       case CellType::Ocean:
    //         recArray[x][y].setFillColor(sf::Color::Blue);
    //         break;
    //       case CellType::Fish:
    //         recArray[x][y].setFillColor(sf::Color::Green);
    //         break;
    //       case CellType::Shark:
    //         recArray[x][y].setFillColor(sf::Color::Red);
    //         break;
    //       default:
    //         break;
          
    //     }
    //   }
//   }
// }


//     auto end = std::chrono::steady_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//     setColors();
//     countFish();
//     draw();
//   }
    
//     return 0;
// }
// wator.cpp ends here


        //bool hasMoved = false;
        // int direction = rand() % 4;
        // for(int i = 0; i < 4; ++i) {
        //   switch (direction)
        //   {
        //     case North:
        //       if(cells[x][y + 1].isOcean()) {
        //         cells[x][(y + 1) % DIMENSIONS] = cells[x][y];
        //         cells[x][(y + 1) % DIMENSIONS].hasMoved = true;
        //         cells[x][y].celltype = CellType::Ocean;
        //         hasMoved = true;
        //       }
        //       break;
        //     case East:
        //       if(cells[(x + 1) % DIMENSIONS][y].isOcean()) {
        //         cells[(x + 1) % DIMENSIONS][y] = cells[x][y];
        //         cells[(x + 1) % DIMENSIONS][y].hasMoved = true;
        //         cells[x][y].celltype = CellType::Ocean;
        //         hasMoved = true;
        //       }
        //       break;
        //     case South:
        //       if(cells[x][(y - 1 + DIMENSIONS) % DIMENSIONS].isOcean()) {
        //         cells[x][(y - 1 + DIMENSIONS) % DIMENSIONS] = cells[x][y];
        //         cells[x][(y - 1 + DIMENSIONS) % DIMENSIONS].hasMoved = true;
        //         cells[x][y].celltype = CellType::Ocean;
        //         hasMoved = true;
        //       }
        //       break;
        //     case West:
        //       if(cells[(x - 1 + DIMENSIONS) % DIMENSIONS][y].isOcean()) {
        //         cells[(x - 1 + DIMENSIONS) % DIMENSIONS][y] = cells[x][y];
        //         cells[(x - 1 + DIMENSIONS) % DIMENSIONS][y].hasMoved = true;
        //         cells[x][y].celltype = CellType::Ocean;
        //         hasMoved = true;
        //       }
        //       break;
        //   }
        //   direction = (direction + 1) % 4;
        //   if(hasMoved) { continue; }
    //     }
    //   }
    // }