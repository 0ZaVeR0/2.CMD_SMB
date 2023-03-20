#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>

int alllayers,layer;
int playerposx, playerposy;
int warplen;
std::vector <int> width;
std::vector <int> height;
std::vector < std::vector < std::vector <int> > > field;
std::vector < std::vector < std::vector <int> > > warps;
int screenx,screeny;

void loadmap(std::string filename, bool loadposfromfile){
    int dump;
    screenx = 0;
    screeny = 0;

    std::ifstream world(filename);
    world >> alllayers;
    std::cout << alllayers;
    width.resize(alllayers);
    height.resize(alllayers);
    for (int a = 0; a < alllayers; a++){world >> width[a];}
    for (int a = 0; a < alllayers; a++){world >> height[a];}
    if (loadposfromfile){world >> playerposx >> playerposy;}
    else{world >> dump; world >> dump;}
    field.resize(alllayers);
    for(int b = 0; b < alllayers; b++){
        field[b].resize(width[b]);
        for(int c = 0; c < width[b]; c++){
            field[b][c].resize(height[b]);
        }
    }
    for (int c = 0; c < alllayers; c++){
        for (int a = 0; a < width[c]; a++){
            for (int b = 0; b < height[c]; b++){
                world >> field[c][a][b];
            }  
        }
    }

    world >> warplen;
    warps.resize(warplen);
    for(int a = 0; a < warplen; a++){
        warps[a].resize(4);
        warps[a][0].resize(2);
        warps[a][1].resize(2);
        warps[a][2].resize(3);
        warps[a][3].resize(3);
    }
    for(int a = 0; a < warplen; a++){
        for(int b = 0; b < 2; b++){
            world >> warps[a][b][0];
            world >> warps[a][b][1];
        }
        for(int c = 2; c < 4; c++){
            world >> warps[a][c][0];
            world >> warps[a][c][1];
            world >> warps[a][c][2];
        }
    }
}


int main(){
    loadmap("1-2.txt",true);
    std::cout << alllayers <<"\n";
    std::cout << height.size() << " " << width.size() << width[0] <<"\n";
    for(int layer = 0; layer < alllayers; layer++){
        for (int a = 0; a < width[layer]; a++){
            for (int b = 0; b < height[layer]; b++){
                std::cout << field[layer][a][b] << ", ";
            }
            std::cout << "\n";
        }
    }
    for(int a = 0; a < warplen; a++){
        std::cout << warps[a][0][0] << " " << warps[a][0][1] << " " << warps[a][1][0] << " " << warps[a][1][1] << " " << warps[a][2][0] << " " << warps[a][2][1] << " " << warps[a][2][2] << " " << warps[a][3][0] << " " << warps[a][3][1] << " " << warps[a][3][2] << "\n"; 
    }
    return 0;
}