#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>

int alllayers,layer = 1;
int playerposx, playerposy;
int warplen;
std::vector <int> width;
std::vector <int> height;
std::vector < std::vector < std::vector <int> > > field;
std::vector < std::vector < std::vector <int> > > warps;

int aomaps = 1;
int aoworlds = 2;
//std::vector < std::vector <std::string> > maps(aoworlds, std::vector<std::string> (aomaps));
std::vector < std::vector <std::string> > maps = {{"textmap.txt"},
                                                {"testmap.txt","1-1.txt"}};

bool game = true; //true - run game
bool save = true;
int playerid = 10; //player
bool jumpinprogress = false, inair = false,jump = false; //is plauer jumping is player in air?, jump key pressed
int jumphight = 3; //it does what it says
int jumplenght = 0; // cant think of anything better 
char direction = 'i'; //i - idle, l - left, r - right
bool gravity = true; //turning on and off gravity

int cursorposx = 0, cursorposy = 0;
bool pipeswitch = true; //true first felse second

const int all = 19;
int blockid = -1;
int coinid = 1;
int airid = 0;
int goalid = 2;
int goaltid = 3;
int brickid = -2;
int coinbid = -3, coinbeid = -4;
int pipeid = -5, pipelid = -6, piperid = -7, pipehid = -13;
int pipe2id = -8, pipe3id = -9, pipebrid = -10, pipeblid = -11;
int barid = -12;
int lavaid = 4;

int items[all] = {airid,blockid,brickid,coinbid,barid,pipeid,pipelid,piperid,pipe2id,pipe3id,pipeblid,pipebrid,pipehid,lavaid,coinbeid,coinid,goalid,goaltid,playerid};
int sitem = 0;
int swarp = 0;

int screenx = 100, screeny = 0, screenlenght = 30, scrollback = 3, scrollforward = 15, screenheight = 15, scrollup = 2, scrolldown = 5;

std::string filename;

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


void display(){
    //display
    std::cout << "\ncursor: " << cursorposx << " " << cursorposy << " " << layer << "\n";
    std::cout << "screen: " << screenx << " " << screeny << "\n";
    std::cout << "item selected: ";
    switch (items[sitem]){
        case -13: // horizontal pipe
            std::cout << "═\n";
            break;
        case -12: // bar
            std::cout << "=\n";
            break;
        case -11: // pipe otton left
            std::cout << "╚\n";
            break;
        case -10: // pipe bottom right
            std::cout << "╝\n";
            break;
        case -9: // pipe3
            std::cout << "╠\n";
            break;
        case -8: // pipe2
            std::cout << "╣\n";
            break;
        case -7: // pipe right
            std::cout << "╗\n";
            break;
        case -6: // pipe left
            std::cout << "╔\n";
            break;
        case -5: // pipe
            std::cout << "║\n";
            break;
        case -4: // cloin block (empty)
            std::cout << "⛝\n";
            break;
        case -3: // coin block 
            std::cout << "⛋\n";
            break;
        case -2: // brick 
            std::cout << "▦\n";
            break;
        case -1: // block
            std::cout << "■\n";
            break;
        case 0: // nothing
            std::cout << "·\n";
            break;
        case 1: // coin
            std::cout << "Ø\n";
            break;
        case 10: // player
            std::cout << "ඞ\n";
            break;
        case 2: // pole
            std::cout << "|\n";
            break;
        case 3: // top
            std::cout << "°\n";
            break;
        case 4: //lava
            std::cout << "^\n";
            break;
        default: // errors
            std::cout << sitem << "\n";
            break;
            } 
    std::cout << "Current warp selected: x1: ";
    std::cout << warps[swarp][0][0] << " y1: " << warps[swarp][0][1] << " x2: " << warps[swarp][1][0] << " y2: " << warps[swarp][1][1] << " " << pipeswitch << "\n";      

    for (int x = 0; x < screenlenght + 2; x++ ){std::cout << "- ";}
    for (int b = screeny; b < (screenheight + screeny); b++){
        std::cout << "\n| ";
        for (int a = screenx; a < (screenlenght + screenx); a++ ){
            if (a == cursorposx  && b == cursorposy){
                std::cout << "|";
            }
            else if (a == cursorposx + 1 && b == cursorposy){
                std::cout << "|";
            } 
            else{
                std::cout << " ";
            }
            
            switch (field[layer][a][b]){
                case - 13: // horizontal pipe
                    std::cout << "═";
                    break;
                case -12: // bar
                    std::cout << "=";
                    break;
                case -11: // pipe otton left
                    std::cout << "╚";
                    break;
                case -10: // pipe bottom right
                    std::cout << "╝";
                    break;
                case -9: // pipe3
                    std::cout << "╠";
                    break;
                case -8: // pipe2
                    std::cout << "╣";
                    break;
                case -7: // pipe right
                    std::cout << "╗";
                    break;
                case -6: // pipe left
                    std::cout << "╔";
                    break;
                case -5: // pipe
                    std::cout << "║";
                    break;
                case -4: // cloin block (empty)
                    std::cout << "⛝";
                    break;
                case -3: // coin block 
                    std::cout << "⛋";
                    break;
                case -2: // brick 
                    std::cout << "▦";
                    break;
                case -1: // block
                    std::cout << "■";
                    break;
                case 0: // nothing
                    std::cout << "·";
                    break;
                case 1: // coin
                    std::cout << "Ø";
                    break;
                case 10: // player
                    std::cout << "ඞ";
                    break;
                case 2: // pole
                    std::cout << "|";
                    break;
                case 3: // top
                    std::cout << "°";
                    break;
                case 4: //lava
                    std::cout << "^";
                    break;
                default: // errors
                    std::cout << field[layer][a][b] << " ";
                    break;
            }               
        }
        std::cout << "|";
    }
    std::cout << "\n";
    for (int x = 0; x < screenlenght + 2; x++ ){std::cout << "- ";}
}


void playerinput(){
    switch (_getch())
        {
        case 'w':
            if (cursorposy > 0){
                cursorposy--;
                if (cursorposy < (screeny + scrollup) && screeny > 0){screeny = (cursorposy - scrollup);}
            }
            break;
        case 'a':
            if (cursorposx > 0){
                cursorposx--;
                if (cursorposx < (scrollback + screenx) && screenx > 0){screenx = (cursorposx - scrollback);} //scroll screen back
            }
            break;
        case 's':
            if (cursorposy < height[layer] - 1){
                cursorposy++;
                if (cursorposy > (screeny + scrolldown) && screeny < (height[layer] - screenheight)){screeny = (cursorposy - scrolldown);}
            }
            break;
        case 'd':
            if (cursorposx < width[layer] - 1){
                cursorposx++;
                if (cursorposx > (scrollforward + screenx) && screenx < (width[layer] - screenlenght)){screenx = (cursorposx - scrollforward);} // scroll screen forward
            }
            break;
        case 'q':
            if (sitem > 0) {sitem--;}
            break;
        case 'e':
            if (sitem < all - 1) {sitem++;}
            break;
        case 32:
            if (items[sitem] == playerid){
                field[layer][playerposx][playerposy] = 0;
                playerposx = cursorposx;
                playerposy = cursorposy;
            }
            field[layer][cursorposx][cursorposy] = items[sitem];
            break;
        case VK_ESCAPE:
            game = false;
            break;
        case 'z':
            if(swarp > 0){swarp--;}
            break;
        case 'c':
            if(swarp < warplen - 1){swarp++;}
            break;
        case VK_RETURN:
            if(pipeswitch){
                warps[swarp][0][0] = cursorposx;
                warps[swarp][0][1] = cursorposy;
                warps[swarp][2][0] = -4;
                warps[swarp][2][1] = -4;
                warps[swarp][2][2] = layer;
            }
            else{
                warps[swarp][1][0] = cursorposx;
                warps[swarp][1][1] = cursorposy;
                warps[swarp][3][0] = -4;
                warps[swarp][3][1] = -4;
                warps[swarp][3][2] = layer;
            }
            break;
        case VK_TAB:
            if (pipeswitch){pipeswitch = false;} else{pipeswitch = true;}
            break;
        case VK_BACK:
            save = false;
            game = false;
            break;
        case 'l': // L
            if (layer == 0){layer = 1;} else{layer = 0;} 
            cursorposx = 0; cursorposy = 0;
            break;
        default:
            break;
        }
}

int main(){  
    /*list of ids: <0 - solid >0 - not  
    -12 = bar = 
    -11 = pipebl ╚  
    -10 = pipebr ╝ 
    -9 = pipe3 ╠ 
    -8 = pipe2 ╣
    -7 = piper ╗
    -6 = pipel ╔ 
    -5 = pipe ║
    -4 = coin block (empty)⛝
    -3 = coin block ⛋
    -2 = brick ▦
    -1 = block ◼
    0 = nothing ·
    1 = coins Ø
    2 = goal |
    3 = goal top °
    4 = lava ^
    10 = player ඞ

    error 
    */
    std::cin >> filename;
    loadmap(filename,true);
    while(game){
        playerinput();
        display();
    }
    if(save){
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
}