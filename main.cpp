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
#include <queue>
#include <stack>
#include <chrono>

typedef std::pair<int,int> indexPair;
typedef std::vector< std::pair<int,int> > indexPairVector;
typedef std::vector<std::vector<Cell> > gridType;

bool unConstrained(std::pair<int, int> &pair, const gridType &vector);

indexPairVector getNeighbors(indexPair curr, const gridType & grid) {
    int x = curr.first, y = curr.second;
    std::vector< indexPair > toret;
    if(y > 0)
        toret.emplace_back(x,y-1);
    if(x > 0)
        toret.emplace_back(x-1,y);
    if(y < grid[0].size() - 1)
        toret.emplace_back(x,y+1);
    if(x < grid.size() - 1)
        toret.emplace_back(x+1,y);

    return toret;
}

indexPairVector getDiagonalNeighbors(indexPair curr, const gridType & grid) {
    int x = curr.first, y = curr.second;
    std::vector< indexPair > toret;
    if(x > 0 && y > 0)
        toret.emplace_back(x-1, y-1);
    if(x > 0 && y < grid[0].size() - 1)
        toret.emplace_back(x-1, y+1);
    if(x < grid.size() - 1 && y > 0)
        toret.emplace_back(x+1, y-1);
    if(x < grid.size() - 1 && y < grid[0].size() - 1)
        toret.emplace_back(x+1, y+1);

    return toret;
}

bool assignment(const gridType & grid) {
    for (auto& line : grid) {
        for (auto &curr : line) {
            if (curr.empty)
                return false;
        }
    }

    //all colored now
    int count;
    for (auto& line : grid) {
        for (auto &curr : line) {

            count = 0;
            indexPairVector neighbors = getNeighbors({curr.x,curr.y}, grid);
            if(curr.type == "mid") {
                for (auto &neigh : neighbors) {
                    if (grid[neigh.first][neigh.second].color == curr.color)
                        count++;
                }
                if(count != 2)
                    return false;
            }

            count = 0;
            if(curr.type == "src") {
                for (auto &neigh : neighbors) {
                    if (grid[neigh.first][neigh.second].color == curr.color)
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

//Do you think checking for full grid consistency makes more sense at each step?
bool stillConsistent(indexPair & curr, const gridType & grid) {

    //we know curr is not empty
    int count = 0;
    Cell currCell = grid[curr.first][curr.second];
    indexPairVector neighbors = getNeighbors(curr, grid);

    for(auto& nei : neighbors) {
        if(!grid[nei.first][nei.second].empty && grid[nei.first][nei.second].color == currCell.color)
            count++;
    }

    if (currCell.type == "mid" && count > 2)
        return false;
    else if (currCell.type == "src" && count > 1)
        return false;

    return true;

}



//Check if all neigbhors are also consistent
bool consistent(indexPair & curr, char color, gridType  grid) {
    int count = 0;
    Cell currCell = grid[curr.first][curr.second];
    indexPairVector neighbors = getNeighbors(curr, grid);

    for(auto& nei : neighbors) {
        if(!grid[nei.first][nei.second].empty && grid[nei.first][nei.second].color == color)
            count++;
    }


    if (currCell.type == "mid" && count > 2)
        return false;
    else if (currCell.type == "src" && count > 1)
        return false;

    //self cond satisfied, check for neighbors
    grid[curr.first][curr.second].color = color;
    grid[curr.first][curr.second].empty = false;
    for(auto& nei : neighbors) {
        if (!grid[nei.first][nei.second].empty && !stillConsistent(nei, grid))
            return false;
    }

    return true;
}


bool gridConsistent(indexPair & current, char color, const gridType&  grid) {


    int count;


    for (auto& line : grid) {
        for (auto &curr : line) {

            count = 0;
            bool allColor = true;
            indexPairVector neighbors = getNeighbors({curr.x, curr.y}, grid);
            for (auto &neigh : neighbors) {

                if (grid[neigh.first][neigh.second].color == curr.color)
                    count++;
                if (grid[neigh.first][neigh.second].empty)
                    allColor = false;

            }

//            std::cout << "consistency checker" << count << allColor <<std::endl;

            if (curr.type == "mid" && !curr.empty) {
                if ((allColor && count != 2) || count > 2)
                    return false;
            }
            else if (curr.type == "src") {
                if ((allColor && count != 1) || count > 1)
                    return false;
            }
        }
    }
    int x,y;
    //check for diagonal shit
    for (auto& line : grid) {
        for (auto &curr : line) {

            count = 0;
            x = curr.x;
            y = curr.y;
            if (curr.type == "mid" && !curr.empty) {
                if(x > 0 && y > 0) {
                    if (!grid[x - 1][y].empty && !grid[x][y - 1].empty) {
                        if(grid[x - 1][y].color != curr.color && grid[x][y-1].color != curr.color) {
                            if(grid[x - 1][y - 1].color == curr.color)
                                return false;
                        }
                    }
                }
                if(x > 0 && y < grid[0].size() - 1) {
                    if (!grid[x - 1][y].empty && !grid[x][y + 1].empty) {
                        if(grid[x - 1][y].color != curr.color && grid[x][y+1].color != curr.color) {
                            if(grid[x - 1][y + 1].color == curr.color)
                                return false;
                        }
                    }
                }

                if(x < grid.size() - 1 && y > 0) {
                    if (!grid[x + 1][y].empty && !grid[x][y - 1].empty) {
                        if(grid[x + 1][y].color != curr.color && grid[x][y - 1].color != curr.color) {
                            if(grid[x + 1][y - 1].color == curr.color)
                                return false;
                        }
                    }
                }


                if(x < grid.size() - 1 && y < grid[0].size() - 1) {
                    if (!grid[x + 1][y].empty && !grid[x][y + 1].empty) {
                        if(grid[x + 1][y].color != curr.color && grid[x][y + 1].color != curr.color) {
                            if(grid[x + 1][y + 1].color == curr.color)
                                return false;
                        }
                    }
                }

            }
        }
    }




    //all constraints satisfied
    return true;


}






int isSatisfied(indexPair curr, const gridType & grid) {
    Cell currCell = grid[curr.first][curr.second];
    auto neighbors = getNeighbors(curr, grid);
    int count = 0;
    for(auto nei : neighbors) {
        if(currCell.color == grid[nei.first][nei.second].color)
            count++;
    }
    if(currCell.type == "src" && count  == 1)
        return true;
    else if(currCell.type == "mid" && count  == 2)
        return true;
    else
        return false;

}

std::vector<char> maxSatisfy(indexPair & param, gridType & grid) {


    int count;
    Cell neighColor;
    std::vector<char> toret;
    auto neighbors = getNeighbors(param, grid);
    Cell currCell = grid[param.first][param.second];
    std::vector<std::pair<char, int> > sortedValues;

    for(char col : currCell.valid) {

        count = 0;
        grid[param.first][param.second].empty = false;
        grid[param.first][param.second].color = col;


        bool check = true, isSame = false;
        for (auto &nei : neighbors) {
            if(grid[nei.first][nei.second].empty) {
                check = false;
            }
        }

        for (auto &nei : neighbors) {
            if(col == grid[nei.first][nei.second].color) {
                isSame = true;
                count += isSatisfied(nei, grid);
            }
        }




        if(check && !isSame) {
           // std::cout << col << "fails" << std::endl;
            continue;
        }

        sortedValues.emplace_back(col, count);
        grid[param.first][param.second].empty = true;
        grid[param.first][param.second].color = '_';
    }



    sort( sortedValues.begin(), sortedValues.end( ), [ ]( const std::pair<char, int>& lhs, const std::pair<char, int>& rhs )
    {
        return lhs.second < rhs.second; //if queue > , is stack <
    }); //since last one - best case - needs to be popped out first


    for(auto& i : sortedValues) {
        toret.push_back(i.first);
    }

    return toret;
}










indexPair MRV(indexPairVector & unassigned, const gridType & grid) {

    auto it = std::min_element(begin(unassigned), end(unassigned),
                               [&grid] (indexPair  const& s1, indexPair  const& s2)
                               {
                                   return grid[s1.first][s1.second].valid.size() < grid[s2.first][s2.second].valid.size();
                               });

    indexPair toret = *it;
    unassigned.erase(it);
    return toret;
}








//changes valid of neighbors and checks if okay
bool forwardCheck(indexPair curr, gridType & grid) {


    Cell *neighCell;
    auto neighbors = getNeighbors(curr, grid);
    Cell currCell = grid[curr.first][curr.second];

    for (auto nei : neighbors) {
        neighCell = &(grid[nei.first][nei.second]);
        if (!neighCell->empty)
            continue;

        for (auto col : neighCell->valid) {
            if (neighCell->type == "mid" && !gridConsistent(nei, col, grid)) {
                neighCell->valid.erase(std::remove(neighCell->valid.begin(), neighCell->valid.end(), col),
                                       neighCell->valid.end());
            }
        }
        if (neighCell->valid.empty())
            return false;
    }

    return true;

}



inline indexPairVector addNeighbors(indexPair curr, const indexPairVector unassign, const gridType & grid) {
    indexPairVector toret = unassign;
    auto neighbors = getNeighbors(curr, grid);
    for(auto nei : neighbors) {
        if(grid[nei.first][nei.second].empty && std::find(unassign.begin(), unassign.end(), indexPair(nei.first, nei.second)) == unassign.end())
            toret.emplace_back(nei.first, nei.second);
    }
    return toret;
}





std::pair<gridType, bool> cspSolver(gridType iniGrid, indexPairVector totalunassigned, std::vector<char> colors) {

    int pos;
    gridType grid;
    indexPair var;
    std::vector<char> values;
    indexPairVector unassign, newunassign;
    std::stack< std::pair <gridType, indexPairVector> > que;

    que.push({iniGrid, totalunassigned});

    while(!que.empty()) {

        //pop current state from stack
        grid = que.top().first;
        unassign = que.top().second;
        que.pop();


        //remove it from list
        var = MRV(unassign, grid);
//
//
//        std::cout << var.first << "," << var.second << std::endl;
//        //printing
//        for(auto& curr : grid) {
//            for(auto& c : curr)
//                std::cout << c.color ;
//
//            std::cout << std::endl;
//        }
//        std::cout << std::endl;


        //if at bottom of tree, just check if valid



        //loop through ordered color list
        values = maxSatisfy(var, grid);


        for (auto &val : values) {
            grid[var.first][var.second].color = val;
            grid[var.first][var.second].empty = false;
            if (gridConsistent(var, val, grid)) {

                if (unassign.empty()) {
                    if (assignment(grid))
                        return {grid, true};
                    else {
                        continue;
                    }
                }

                if(!forwardCheck(var, grid))
                    continue;

                newunassign = addNeighbors(var, unassign, grid);
                que.push({grid, newunassign});
            }
        }
    }

    std::cout << "queue empty" << std::endl;
    return {grid, false};
}




int main() {

    bool res;
    gridType grid;
    int i =0, j = 0;
    std::string line;
    std::set<char> colors;
    indexPairVector unassign;


    //parsing input
    std::ifstream infile("inp.txt");
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


    //assigning valid colors
    std::vector<char> cols;
    for(auto& in: colors) {
        cols.push_back(in);
    }

    for(auto& curr : grid) {
        for (auto &c : curr) {
            if(c.type == "mid") {
                c.valid = cols;
            }
        }
    }

    for(auto& curr : grid) {
        for (auto &c : curr) {
            if (c.type == "src") {
                auto neighbors = getNeighbors({c.x, c.y}, grid);
                for (auto nei : neighbors) {
                    if (grid[nei.first][nei.second].empty && std::find(unassign.begin(), unassign.end(), indexPair(nei.first, nei.second)) == unassign.end())
                        unassign.emplace_back(nei.first, nei.second);
                }
            }
        }
    }

    //solving problem
    auto newgridparts = cspSolver(grid, unassign, cols);

    grid = newgridparts.first;
    res = newgridparts.second;

    if(!res)
        std::cout << "FAIL" << std::endl;


    //printing result
    for(auto& curr : grid) {
        for(auto& c : curr)
            std::cout << c.color ;

        std::cout << std::endl;
    }


}


//go back to only checking cell and neighbor consistency instead of grid consistency