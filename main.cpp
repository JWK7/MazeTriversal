#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include "MazeTriversal.h"
using namespace std;

int main(int argc, char *argv[])
{
    float gamma=1;
    int episodes=10000;
    float epsilon=0.01;
    if (argc == 4)
    {
        gamma = atof(argv[1]);
        episodes = atoi(argv[2]);
        epsilon = atof(argv[3]);
    }
    int** Maze = createMaze(GRID_SIZE);
    showMaze(Maze);
    float*** Q = q_learning(gamma,episodes,epsilon,Maze);
    int** policy = generatePolicy(Q);
    showPolicy(policy);
    return 0;
}