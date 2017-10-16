//
// Created by Srihita Vatsavaya on 10/8/17.
//

#ifndef INC_2_1_CELL_H
#define INC_2_1_CELL_H

#include <vector>
#include <string>
//WRITE COPY CONSTRUCTOR


class Cell {
public:
    Cell();
    Cell(std::string type_, char color_);
    Cell(const Cell &other);
    bool operator == (const Cell & Ref) const;
    bool empty;
    char color;
    std::string type;
    int x,y;
   std::vector<char> valid;
    std::vector<std::tuple<int,int> > neighbors;
    int shape = 0; //shape zero means unassigned mid or just reg src too!

};



// Shapes
//1	─	left-right
//2	│	top-bottom

//3	┘	top-left
//4	└	top-right
//5	┐	bottom-left
//6	┌	bottom-right

#endif //INC_2_1_CELL_H
