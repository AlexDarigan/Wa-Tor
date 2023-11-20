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

  bool isOcean() { return celltype == CellType::Ocean; };
  bool isFish() { return celltype == CellType::Fish; };
  bool isShark() { return celltype == CellType::Shark; };
 // void setCellType(CellType _celltype) { celltype = _celltype; };

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
    for(int y = 0; y < DIMENSIONS; ++y) {
      for(int x = 0; x < DIMENSIONS; ++x) {
        if(cells[x][y].celltype == CellType::Fish) {
          fishes++;
         // printf("Fish: (%d, %d)\n", x, y);
        }
      }
    }
    printf("Fishes: %d\n", fishes);
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
      // if(id%10==0) {
      //   recArray[i][k].setFillColor(sf::Color::Red);
      //   cells[i][k].celltype = CellType::Shark;
      // }
      //else 
      //if (id%2==0) { 
      if(i == 5 && k == 6) {  
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
 // omp_set_num_threads(6);
  while (window.isOpen())
  {
    poll();

    // Move Fish
    for(int y = 0; y < DIMENSIONS; ++y) {
      for(int x = 0; x < DIMENSIONS; ++x) {
        // if(cells[x][y].celltype == CellType::Fish && !cells[x][y].hasMoved) { // } && !cells[x][y].hasMoved) {
        //   if(cells[(x + 1) % DIMENSIONS][y].celltype == CellType::Ocean) {
        //     cells[(x + 1) % DIMENSIONS][y] = cells[x][y];
        //     cells[(x + 1) % DIMENSIONS][y].hasMoved = true;
        //     cells[x][y].celltype = CellType::Ocean;
        //     //hasMoved = true;
        //   }
        // }

        if(cells[x][y].isFish() && !cells[x][y].hasMoved) {
        bool hasMoved = false;
        int direction = rand() % 4;
          for(int i = 0; i < 4; ++i) {
            switch (direction)
            {
              case North: {
                int north = (y - 1 + DIMENSIONS) % DIMENSIONS;
                if(cells[x][north].celltype == CellType::Ocean) {
                    cells[x][north] = cells[x][y];
                    cells[x][north].hasMoved = true;
                    cells[x][y].celltype = CellType::Ocean;
                    hasMoved = true;
                  }
                }
                break;
              case East: {
                int east = (x + 1) % DIMENSIONS;
                if(cells[east][y].celltype == CellType::Ocean) {
                    cells[east][y] = cells[x][y];
                    cells[east][y].hasMoved = true;
                    cells[x][y].celltype = CellType::Ocean;
                    hasMoved = true;
                  }
                }
                break;
              case South: {
                int south = (y + 1) % DIMENSIONS;
                if(cells[x][south].celltype == CellType::Ocean) {
                    cells[x][south] = cells[x][y];
                    cells[x][south].hasMoved = true;
                    cells[x][y].celltype = CellType::Ocean;
                    hasMoved = true;
                  }
                }
                break;
              case West: {
                int west = (x - 1 + DIMENSIONS) % DIMENSIONS;
                if(cells[west][y].celltype == CellType::Ocean) {
                    cells[west][y] = cells[x][y];
                    cells[west][y].hasMoved = true;
                    cells[x][y].celltype = CellType::Ocean;
                    hasMoved = true;
                  }
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
  //}

    for(int y = 0; y < DIMENSIONS; ++y) {
      for(int x = 0; x < DIMENSIONS; ++x) {
        cells[x][y].hasMoved = false;
      }
    }


        // bool hasMoved = false;
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
        // }
    //   }
    // }

    setColors();
    //countFish();
    draw();
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