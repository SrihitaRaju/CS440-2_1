#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include "Cell.h"
#include <random>
#include <algorithm>
#include <cstdlib>

std::vector< std::tuple<int,int> > getNeighbors(Cell curr, std::vector<std::vector<Cell> > & grid) {
    int xlim = grid.size();
    int ylim = grid[0].size();
    int x = curr.x;
    int y = curr.y;
    std::vector< std::tuple<int,int> > toret;
    if(y > 0)
        toret.push_back(std::tuple<int,int>(x,y-1));
    if(x > 0)
        toret.push_back(std::tuple<int,int>(x-1,y));
    if(y < ylim - 1)
        toret.push_back(std::tuple<int,int>(x,y+1));
    if(x < xlim - 1)
        toret.push_back(std::tuple<int,int>(x+1,y));

    //std::cout << "in neighbors" << curr.x << "," << curr.y << std::endl;
    return toret;
}



bool assignment(std::vector<std::vector<Cell> > grid) {
    for (auto& line : grid) {
        for (auto &curr : line) {
            if (curr.empty)
                return false;
        }
    }

    //all colored now
    int count,x,y;
    for (auto& line : grid) {
        for (auto &curr : line) {

            count = 0;
            std::vector< std::tuple<int,int> > neighbors = getNeighbors(curr, grid);
            if(curr.type == "mid") {
                for (auto &neigh : neighbors) {
                    std::tie (x, y) = neigh;
                    if (grid[x][y].color == curr.color)
                        count++;
                }
                if(count != 2)
                    return false;
            }

            count = 0;
            if(curr.type == "src") {
                for (auto &neigh : neighbors) {
                    std::tie (x, y) = neigh;
                    if (grid[x][y].color == curr.color)
                        count++;
                }
                if(count != 1)
                    return false;
            }
        }
    }

    //all constraints satisfied
    return true;
}

//
//Cell getVariable(std::vector<Cell> curr) {
//    int size_ = curr.size();
//    std::random_device rd; // obtain a random number from hardware
//    std::mt19937 eng(rd()); // seed the generator
//    std::uniform_int_distribution<> distr(0, size_ - 1); // define the range
//    int n = distr(eng);
//
//    //std::cout << "vec size" << curr.size() << "," << n << std::endl;
//    return curr[n];

//}

int myrandom (int i) { return std::rand()%i;}

std::vector<char> getValues(Cell x, std::vector<std::vector<Cell> > & grid, std::vector<char> curr) {
    std::random_shuffle ( curr.begin(), curr.end() , myrandom);
    return curr;
}

std::vector<Cell> getVars(std::vector<std::vector<Cell> > & grid, std::vector<Cell> curr) {
    std::random_shuffle ( curr.begin(), curr.end() , myrandom);
    return curr;
}


bool consistent(Cell curr, char color, std::vector<std::vector<Cell> > & grid) {
    std::vector<std::tuple<int,int>> neighbors = getNeighbors(curr, grid);
    int count = 0,x,y;
    for(auto& nei : neighbors) {
        std::tie (x, y) = nei;
        if(!grid[x][y].empty && grid[x][y].color == color)
            count++;
    }
    //std::cout << "count is " << count << std::endl;
    return (count <= 2);
}


//std::tuple<std::vector<std::vector<Cell> >, bool> cspSolver(std::vector<std::vector<Cell> > grid, std::vector<Cell> unassigned, std::vector<char> colors, int level) {
//    if (assignment(grid))
//         return std::tuple<std::vector<std::vector<Cell> >, bool>(grid, true);
//    if(unassigned.size() == 0)
//        return std::tuple<std::vector<std::vector<Cell> >, bool>(grid, false);
//
//    Cell var = getVariable(unassigned);
//    std::vector<Cell>::iterator newEnd = std::remove(unassigned.begin(), unassigned.end(), var);
//    unassigned.erase(newEnd, unassigned.end());
//    std::cout << var.x << "," << var.y << ", " << "level" << level << std::endl;
//    for(auto& curr : grid) {
//        for (auto &c : curr)
//            std::cout << c.color;
//
//        std::cout << std::endl;
//    }
//    std::vector<char> values = getValues(var, grid, colors);
//    for(auto& val : values) {
//        if (consistent(var, val, grid)) {
//            //std::cout << var.x << "," << var.y << "," << val << std::endl;
//            grid[var.x][var.y].color = val;
//            grid[var.x][var.y].empty = false;
//            std::tuple<std::vector<std::vector<Cell> >, bool> backtrack = cspSolver( grid, unassigned, colors, level+1);
//            //std::cout << std::get<1>(backtrack) << std::endl;
//            if(std::get<1>(backtrack))
//                return backtrack;
//            grid[var.x][var.y].color = ' ';
//            grid[var.x][var.y].empty = true;
//        }
//    }
//    return std::tuple<std::vector<std::vector<Cell> >, bool>(grid, false);
//}



std::tuple<std::vector<std::vector<Cell> >, bool> cspSolver(std::vector<std::vector<Cell> > grid, std::vector<Cell> unassigned, std::vector<char> colors, int level) {
    if (assignment(grid))
        return std::tuple<std::vector<std::vector<Cell> >, bool>(grid, true);
    if(unassigned.size() == 0)
        return std::tuple<std::vector<std::vector<Cell> >, bool>(grid, false);

    Cell var = unassigned[0];
    unassigned.erase(unassigned.begin());
    std::cout << var.x << "," << var.y << ", " << "level" << level << std::endl;
    for(auto& curr : grid) {
        for (auto &c : curr)
            std::cout << c.color;

        std::cout << std::endl;
    }
    std::vector<char> values = getValues(var, grid, colors);
    for(auto& val : values) {
        if (consistent(var, val, grid)) {
            //std::cout << var.x << "," << var.y << "," << val << std::endl;
            grid[var.x][var.y].color = val;
            grid[var.x][var.y].empty = false;
            std::tuple<std::vector<std::vector<Cell> >, bool> backtrack = cspSolver( grid, unassigned, colors, level+1);
            //std::cout << std::get<1>(backtrack) << std::endl;
            if(std::get<1>(backtrack))
                return backtrack;
            grid[var.x][var.y].color = ' ';
            grid[var.x][var.y].empty = true;
        }
    }
    return std::tuple<std::vector<std::vector<Cell> >, bool>(grid, false);
}



int main() {
    std::ifstream infile("inp.txt");
    std::string line;
    std::vector< std::vector<Cell> > grid;
    std::vector<Cell> unassign;
    std::set<char> colors;
    int i =0, j = 0;
    while (std::getline(infile, line))
    {
        std::vector<Cell> curr;
        j = 0;
        for(auto& c: line) {
            Cell elem;
            elem.x = i;
            elem.y = j;
            if(c == '_') {
                elem.type = "mid";
                elem.color = '_';
                elem.empty = true;
                unassign.push_back(elem);
            }
            else {
                elem.type = "src";
                elem.color = c;
                elem.empty = false;
                colors.insert(c);
            }
            curr.push_back(elem);
            j++;
        }
        grid.push_back(curr);
        i++;
    }
    std::vector<char> cols;
    for(auto& in: colors) {
        cols.push_back(in);
    }
    std::cout << "size of grid" << i << "," << j << std::endl;
    unassign = getVars(grid, unassign);
    std::tuple<std::vector<std::vector<Cell> >, bool> newgridparts = cspSolver(grid, unassign, cols, 0);

    std::cout << "herre2" << std::endl;
    bool res;
    std::tie (grid, res) = newgridparts;
    if(!res)
        std::cout << "FAIL" << std::endl;
    for(auto& curr : grid) {
        for(auto& c : curr)
            std::cout << c.color ;

        std::cout << std::endl;
    }


}