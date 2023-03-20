#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <vector>

int main(){

int alllayers = 3;
int layer;
int playerposx = 0, playerposy = 0;
int warplen = 16;
std::vector <int> width = {190,190,17};
std::vector <int> height = {15,15,13};
std::vector < std::vector < std::vector <int> > > field = {{{0}}};
std::vector < std::vector < std::vector <int> > > warps = {{{0}}};

    field.resize(alllayers);
    for(int b = 0; b < alllayers; b++){
        field[b].resize(width[b]);
        for(int c = 0; c < width[b]; c++){
            field[b][c].resize(height[b]);
        }
    }

    warps.resize(warplen);
    for(int a = 0; a < warplen; a++){
        warps[a].resize(4);
        warps[a][0].resize(2);
        warps[a][1].resize(2);
        warps[a][2].resize(3);
        warps[a][3].resize(3);
    }

    std::string filename = "1-2.txt";
    std::ofstream world(filename);

        world << alllayers << "\n";
        for(int a = 0; a < alllayers; a++){world << width[a] << " ";}
        for(int a = 0; a < alllayers; a++){world << height[a] << " ";}
        world << "\n" << playerposx << " " << playerposy << "\n";

        for(int a = 0; a < alllayers; a++){
            for (int b = 0; b < width[a]; b++){
                for(int c = 0; c < height[a]; c++){
                    world << field[a][b][c] << " ";
                }
                world << "\n";
            }
            world << "\n";
        }

        world << warplen << "\n";
        for(int a = 0; a < warplen; a++){
            world << warps[a][0][0] << " " << warps[a][0][1] << " " << warps[a][1][0] << " " << warps[a][1][1] << "\n";
            world << warps[a][2][0] << " " << warps[a][2][1] << " " << warps[a][2][2] << " ";
            world << warps[a][3][0] << " " << warps[a][3][1] << " " << warps[a][3][2] << "\n";
        }
        world.close();

}