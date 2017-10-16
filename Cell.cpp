//
// Created by Srihita Vatsavaya on 10/8/17.
//

#include <curses.h>
#include "Cell.h"

Cell::Cell() {
}

Cell::Cell(const Cell &other) {
    this->x = other.x;
    this->y = other.y;
    this->empty = other.empty;
    this->type = other.type;
    this->color = other.color;
    this->valid = other.valid;
}

bool Cell::operator == (const Cell & other) const {
    return (this->x == other.x &&
            this->y == other.y &&
            this->empty == other.empty &&
            this->type == other.type &&
            this->color == other.color &&
            this->valid == other.valid);
}


Cell::Cell(std::string type_, char color_) { //constructor
    type = type_;

    if(type == "src"){ //SHOULD BE UNCHANGABLE
        empty = false;
        color = color_;
    }

    if(type == "mid") {
        empty = true;
        color = ' ';
    }
    shape = 0;
}

