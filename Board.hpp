#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class Board{
    public: 
        Board(); //6 bins, each holds at most 4 balls
        Board(int numBins, int capacity); //numBins, each holds at most capacity
        void display() const;
        int add(int player); //given a player, return which bin the player is added to
        int winInHorizontal(int bin);
        int winInVertical(int bin);
        int winInDiagonal(int bin);
        int win(int bin); //column must be the most recent ball in that bin
        // bool isFull() const; //checks if the grid is full (at capacity)
        void play();
        //Make public for the main file to be able to access it
        int numBins; //number of bins / LENGTH
        int capacity; //maximum number of shapes held in each bin / HEIGHT
        vector<vector<int>> grid;
    
    //COMMENT OUT PRIVATE FOR GRADESCOPE
    // private: 
        // int numBins; //number of bins / LENGTH
        // int capacity; //maximum number of shapes held in each bin / HEIGHT
        // vector<vector<int>> grid;
};

void print(int numBins);
#endif