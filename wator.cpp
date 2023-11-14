// wator.cpp --- 
// 
// Filename: wator.cpp
// Description: 
// Author: David Darigan
// Maintainer: David Darigan
// Created: Fri Nov  9
// Last-Updated: Fri Nov  9
//           By: David
//     Update #: 1
// 
// 

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

#include <SFML/Graphics.hpp>
#include <cstdlib> 

// Defining structs because we need more information (like "moved this round")

enum Direction { North = 0, East = 1, South = 2, West = 3 };
enum CellType { Ocean, Fish, PastFish, FutureFish, Shark, PastShark, FutureShark };

struct Cell {
  CellType celltype = Ocean;
};

// Constants required by following arrays
const int xdim = 10;
const int ydim= 10;

// Allocating arrays on the heap by moving them to the global scope
sf::RectangleShape recArray[xdim][ydim];
sf::RectangleShape recArray2[xdim][ydim];
Cell cells[xdim][ydim];

int main()
{
  srand(0);
  int WindowXSize=800;
  int WindowYSize=600;
  int cellXSize=WindowXSize/xdim;
  int cellYSize=WindowYSize/ydim;

  for(int i=0;i<xdim;++i){
    recArray[xdim][ydim].setFillColor(sf::Color::Blue);
    for(int k=0;k<ydim;++k){//give each one a size, position and color
      recArray2[i][k].setFillColor(sf::Color::Blue);
      recArray[i][k].setSize(sf::Vector2f(80.f,80.f));
      recArray[i][k].setPosition(i*cellXSize,k*cellYSize);//position is top left corner!
      int id=i*1-+k;
      if(id%10==0) {
        recArray[i][k].setFillColor(sf::Color::Red);
        cells[i][k].celltype = CellType::Shark;
      }
      else if (id%2==0) { 
      //if(i == 0 && k == 0) {  
        recArray[i][k].setFillColor(sf::Color::Green);
        cells[i][k].celltype = CellType::Fish;
      }
      else {
        recArray[i][k].setFillColor(sf::Color::Blue);
        cells[i][k].celltype = CellType::Ocean;
      }
    }
  }

    sf::RenderWindow window(sf::VideoMode(WindowXSize,WindowYSize), "SFML Wa-Tor world");
   

    while (window.isOpen())
    {
 
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

      // MoveFishRight + New Fish
      for(int y = 0; y < ydim; ++y) {
        for(int x = 0; x < xdim; ++x) {
          int destination = (x + 1) % xdim;
          bool isFish = cells[x][y].celltype == CellType::Fish;
          bool isNotBlocked = cells[destination][y].celltype == CellType::Ocean; 
          bool willMoveNow = (rand() % 2) == 1; // 1 - Yes / 0 - No
          if(isFish && willMoveNow && isNotBlocked) {
            cells[destination][y].celltype = CellType::FutureFish;
            cells[x][y].celltype = CellType::PastFish;
          }
        }
      }

      // // MoveFishLeft + New Fish
      for(int y = 0; y < ydim; ++y) {
        for(int x = xdim - 1; x > 0; --x) {
          int destination = (x - 1) % xdim;
          bool isFish = cells[x][y].celltype == CellType::Fish;
          bool isNotBlocked = cells[destination][y].celltype == CellType::Ocean;
          bool willMoveNow = (rand() % 2) == 1; // 1 - Yes / 0 - No
          if(isFish && willMoveNow && isNotBlocked) {
            cells[destination][y].celltype = CellType::FutureFish;
            cells[x][y].celltype = CellType::PastFish;
          }
        }
      }

      
      // MoveFishUp + New Fish
      for(int y = ydim - 1; y > -1; --y) {
        for(int x = 0; x < xdim; ++x) {
          int destination = (y - 1 + ydim) % ydim;
          bool isFish = cells[x][y].celltype == CellType::Fish;
          bool isNotBlocked = cells[x][destination].celltype == CellType::Ocean;
          bool willMoveNow = (rand() % 2) == 1; // 1 - Yes / 0 - No
          if(isFish && willMoveNow && isNotBlocked) {
            cells[x][destination].celltype = CellType::FutureFish;
            cells[x][y].celltype = CellType::PastFish;
          }
        }
      }
      
      //MoveFishDown + New Fish
      for(int y = 0; y < ydim; ++y) {
        for(int x = 0; x < xdim; ++x) {
          int destination = (y + 1) % ydim;
          bool isFish = cells[x][y].celltype == CellType::Fish;
          bool isNotBlocked = cells[x][destination].celltype == CellType::Ocean;
          bool willMoveNow = (rand() % 2) == 1; // 1 - Yes / 0 - No
          if(isFish && willMoveNow && isNotBlocked) {
            cells[x][destination].celltype = CellType::FutureFish;
            cells[x][y].celltype = CellType::PastFish;
          }
        }
      }

      // Handle all the fish who have not yet moved
      for(int y = 0; y < ydim; ++y) {
        for(int x = 0; x < xdim; ++x) {
          //if(cells[x][y].celltype != CellType::Fish) { continue; }
          int direction = rand() % 4;
          int options = 4;
          while((cells[x][y].celltype == CellType::Fish) && (options > 0)) {
            switch(direction) {
              case Direction::North: {
                  int destination = ((y - 1) + ydim) % ydim;
                  bool isNotBlocked = cells[x][destination].celltype == CellType::Ocean; 
                  if(isNotBlocked) {
                    cells[x][destination].celltype = CellType::FutureFish;
                    cells[x][y].celltype = CellType::PastFish;
                  }
                  break;
                }  
                case Direction::East: {
                    int destination = (x + 1) % xdim;
                    bool isNotBlocked = cells[destination][y].celltype == CellType::Ocean; 
                    if(isNotBlocked) {
                      cells[destination][y].celltype = CellType::FutureFish;
                      cells[x][y].celltype = CellType::PastFish;
                    }
                    break;
                } 
                case Direction::South: {
                    int destination = ((y + 1)) % ydim;
                    bool isNotBlocked = cells[x][destination].celltype == CellType::Ocean; 
                    if(isNotBlocked) {
                      cells[x][destination].celltype = CellType::FutureFish;
                      cells[x][y].celltype = CellType::PastFish;
                    }
                    break;
                }
                case Direction::West: {
                    int destination = ((x - 1) + xdim) % xdim;
                    bool isNotBlocked = cells[destination][y].celltype == CellType::Ocean; 
                    if(isNotBlocked) {
                      cells[destination][y].celltype = CellType::FutureFish;
                      cells[x][y].celltype = CellType::PastFish;
                    }
                    break;
                }
            }
              --options;
              // Move to the next direction, wrap if near boundaries with options left
              printf("\nDirection: %d/n", direction);
              direction = (direction + 1) % 4;
            }
            // if(cells[x][y].celltype == CellType::Fish) {
            //   // If we're still a fish (ie have not moved, then set as future fish)
            //   // ..this is unnecessary but useful for debugging new fish
            //   cells[x][y].celltype = CellType::FutureFish;
            // }
          }
        }
    //   }
    // }

      // MoveSharksLeft + EatFish
      // -- We can't just move everything in a direction at once in this case
      // -- We need to get decisions per direction, then act on those directions
      // -- We could check all fish to the left, then decided if the shark wants to eat them
      // -- but maybe it decides no, but also there is no other fish
      // ..alternatively, we could make a decision and then seperate them into four lists of eat direction
      // ..
      // MoveSharksRight + EatFish
      // MoveSharksUp + EatFish
      // MoveSharksDown + EatFish

    // Pass buffer and clear


    for(int y = 0; y < ydim; ++y) {
      for(int x = 0; x < xdim; ++x) {
        switch(cells[x][y].celltype) {
          case CellType::Ocean: {
            recArray[x][y].setFillColor(sf::Color::Blue);
            break;
          }
          case CellType::Fish: {
            recArray[x][y].setFillColor(sf::Color::Green);
            // No Cells should be fish unless surrounded
            break;
          }
          case CellType::PastFish: {
            recArray[x][y].setFillColor(sf::Color::Blue);
            cells[x][y].celltype = CellType::Ocean;
            break;
          }
          case CellType::FutureFish: {
            recArray[x][y].setFillColor(sf::Color::Green);
            cells[x][y].celltype = CellType::Fish;
            break;
          }
        }
      }
    }

    // Not working properly, too many fishes
    int fishes = 0;
    for(int x = 0; x < xdim; ++x) {
      for(int y = 0; y < ydim; ++y) {
        if(cells[x][y].celltype == CellType::Fish) {
          fishes++;
        }
      }
    }
    printf("Fishes: %d\n", fishes);


	//loop these three lines to draw frames
    window.clear(sf::Color::Black);
    for(int i=0;i<xdim;++i){
      for(int k=0;k<ydim;++k){
        window.draw(recArray[i][k]);
      }
    }
      sf::sleep(sf::seconds(1));
      window.display();
      
  }
    
    return 0;
}

// 
// wator.cpp ends here
