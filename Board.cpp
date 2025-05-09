/*
Author: Nava Karine Nizard
Course: CSCI-135
Instructor: Professor Tong Yi
Date: April 20, 2025
Assignment: Project 3 (Tasks A-C)

This program simulates a two-player Connect4 game from the command line.
*/

#include "Board.hpp"
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

struct Coord {
    int bin;
    int idx;
};

//Default constructor
Board::Board(){
    numBins = 6; //length or columns
    capacity = 4; //height or rows
    //push (empty) vertical vectors (bins)
    for (int i = 0; i < numBins; i++){
        vector<int> v1;
        grid.push_back(v1);
    } 
    
}

//Parametized constructor
Board::Board(int numBins, int capacity){
    if (numBins < 5){
        numBins = 5;
    }
    if (capacity < 4){
        capacity = 4;
    }
    this->numBins = numBins; //length or columns
    this->capacity = capacity; //height or rows
    //push (empty) vertical vectors (bins)
    for (int numRows = 0; numRows < numBins; numRows++){
        vector<int> v1;
        grid.push_back(v1);
    }
}

//Make the board line shape
void print(int numBins){
    cout << "+";
    for(int i = 0; i < numBins; i++){
        cout << "--+";
    }
    cout << endl;
}

//Display the current board
void Board::display() const {
    string mapping[] = {
        "\033[31m\u2b24\033[0m", // 0 = red circle (🔴)
        "\033[34m\u2b1f\033[0m", // 1 = blue pentagon (🔷)
        "\033[31m\u25c9\033[0m", // 2 = red double circle (⭕)
        "\033[34m\u2b54\033[0m"  // 3 = blue empty pentagon 
    };

    print(numBins);
    for (int j = capacity - 1; j >= 0; j--){ //Loop through each horizontal row
        cout << "|";
        for (int i = 0; i < numBins; i++){ //Move across the bins
            //Check if there's a ball in the bin and display it
            if (j < grid[i].size()){ //if there are elements to display
                cout << mapping[grid[i][j]] << " |";
            }
            //If there're no balls, display a space
            else{
                cout << "  |";
            }
        }
        cout << endl;
        print(numBins);
    }

    //Print out the labels of the column numbers
    for (int i = 0; i < numBins; i++){
        cout << " " << i << " ";
    }
}

int Board::add(int player){
    //Player ID is either 0 or 1
    int binIdx;
    cout << "Enter a bin index in [0, " << numBins << ") that is not full: ";
    cin >> binIdx;
    while ((binIdx < 0 || binIdx >= numBins) || grid[binIdx].size() >= capacity){
        if ((binIdx < 0 || binIdx >= numBins)){
            cout << "Invalid bin entered. Must be in [0, " << numBins << "). Reenter a bin index: ";
        }
        else{
            cout << "The bin entered is full. Reenter bin index: ";
        }
        cin >> binIdx;
    }

    grid[binIdx].push_back(player);
    return binIdx;
}

int Board::winInHorizontal(int bin){
    int idx = grid[bin].size() - 1; //idx of last element added to the bin
    int player = grid[bin][idx]; //gets the ID of player who last played
    vector<Coord> candidates; //saves the bin and idx of each shape sharing player along the anti-diagonal path

    //the first possible winning candidate
    candidates.push_back({bin, idx});

    //find all right neighbors
    int currBin = bin + 1;
    
    while (currBin < numBins && idx < grid[currBin].size() && grid[currBin][idx] == player){
        candidates.push_back({currBin, idx});
        currBin++;
    }

    //find all left neighbors
    currBin = bin - 1;

    while (currBin >= 0 && idx < grid[currBin].size() && grid[currBin][idx] == player){
        candidates.push_back({currBin, idx});
        currBin--;
    }

    //check if there are four in a row (horizontally) to win
    if (candidates.size() >= 4){
        //change the winning sequence to a diff color to diffrentiate
        for (int i = 0; i < 4; i++){
            grid[candidates[i].bin][idx] = player + 2;
        }
        return player;
    }

    return -1; ///If no winner, return -1
}

int Board::winInVertical(int bin){
    int idx = grid[bin].size() - 1;
    int player = grid[bin][idx];
    vector<Coord> candidates;

    //first possible candidate
    candidates.push_back({bin, idx});

    //find all lower plays
    int currIdx = idx - 1;

    while (currIdx >= 0 && grid[bin][currIdx] == player){
        candidates.push_back({bin, currIdx});
        currIdx--;
    }

    //check if there are four in a row (vertically)
    if (candidates.size() >= 4){
        //change the winning sequence to a diff color to diffrentiate
        for (int i = 0; i < 4; i++){
            grid[bin][candidates[i].idx] = player + 2;
        }
        return player;
    }

    return -1; //if no winner is found, return -1
}

int Board::winInDiagonal(int bin){
    int idx = grid[bin].size() - 1; //idx of last element added to the bin
    int player = grid[bin][idx]; //gets the ID of player who last played
    vector<Coord> antiDiagonalCandidates; //saves the bin and idx of each shape sharing player along the anti-diagonal path
    vector<Coord> diagonalCandidates;

    //the first possible winning candidate
    antiDiagonalCandidates.push_back({bin, idx});
    diagonalCandidates.push_back({bin, idx});

    //ANTI-DIAGONAL:
    //find top left neighbors
    int currBin = bin - 1;
    int currIdx = idx + 1;

    //verifies that the pairs are consecutive and the indeces are valid
    while(currBin >= 0 && (currIdx < grid[currBin].size()) && grid[currBin][currIdx] == player){
        antiDiagonalCandidates.push_back({currBin, currIdx}); //add it if its a match. save in case it wins
        //Update variables
        currBin--;
        currIdx++;
    }

    //find bottom right neighbors
    currBin = bin + 1;
    currIdx = idx - 1;

    //currBin < numBins or grid.size() (amount of elements in grid//vectors)
    while(currBin < numBins && currIdx >= 0 && (currIdx < grid[currBin].size()) && grid[currBin][currIdx] == player){
        antiDiagonalCandidates.push_back({currBin, currIdx});
        currBin++;
        currIdx--;
    }

    //DIAGONAL:
    //find top right neighbor
    currBin = bin + 1;
    currIdx = idx + 1;

    while (currBin < numBins && (currIdx < grid[currBin].size()) && grid[currBin][currIdx] == player){
        diagonalCandidates.push_back({currBin, currIdx});
        currBin++;
        currIdx++;
    }

    //find bottom left neighbor
    currBin = bin - 1;
    currIdx = idx - 1;

    while (currBin >= 0 && currIdx >= 0 && (currIdx < grid[currBin].size()) && grid[currBin][currIdx] == player){
        diagonalCandidates.push_back({currBin, currIdx});
        currBin--;
        currIdx--;
    }

    //check if there are 4 in a row (antidiagonally)
    if (antiDiagonalCandidates.size() >= 4){
        //change the winning sequence to a diff color to diffrentiate
        for (int i = 0; i < 4; i++){
            grid[antiDiagonalCandidates[i].bin][antiDiagonalCandidates[i].idx] = player + 2;
        }
        return player;
    }

    //check if there are 4 in a row (diagonally)
    else if (diagonalCandidates.size() >= 4){
        //change the winning sequence to a diff color to diffrentiate
        for (int i = 0; i < 4; i++){
            grid[diagonalCandidates[i].bin][diagonalCandidates[i].idx] = player + 2;
        }
        return player;
    }

    return -1;
}

int Board::win(int bin){
    int winningPlayer = grid[bin][grid[bin].size() - 1]; //last person who played

    if (winInHorizontal(bin) == winningPlayer){
        return winningPlayer;
    }

    if (winInVertical(bin) == winningPlayer){
        return winningPlayer;
    }

    if (winInDiagonal(bin) == winningPlayer){
        return winningPlayer;
    }

    return -1;
}

void Board::play(){
    bool gameOver = false;
    int ballsDropped = 0;
    //Display an empty grid to start with
    display();
    cout << endl;

    int player1 = 0;
    int player2 = 1;

    while(ballsDropped < (numBins * capacity)){ //while there's still room on the grid and no winner found yet, continue playing
        cout << "Player: Red" << endl;
        int move1 = add(player1);
        ballsDropped++;
        if (win(move1) == player1){
            gameOver = true;
            cout << "Winner: Red!" << endl;
            break;
        }
        display();
        cout << endl;

        if (ballsDropped == (numBins * capacity)){ //in the case of ___, check if board is full after red player,
            break;                                // so as not to let blue play if there's no more space 
        }

        cout << "Player: Blue" << endl;
        int move2 = add(player2);
        ballsDropped++;
        if (win(move2) == player2){
            gameOver = true;
            cout << "Winner: Blue!" << endl;
            break;
        }
        display();
        cout << endl;


    }

    if (ballsDropped == (numBins * capacity) && !gameOver){ // if full and no one won, it's a tie
        cout << "Tie" << endl;
    }

    display();
    cout << endl;

}