#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>

int alllayers,layer = 0;
int playerposx, playerposy;
int warplen;
std::vector <int> width;
std::vector <int> height;
std::vector < std::vector < std::vector <int> > > field;
std::vector < std::vector < std::vector <int> > > warps;

std::string output,debug;

int aomaps = 1;
int aoworlds = 2;
//std::vector < std::vector <std::string> > maps(aoworlds, std::vector<std::string> (aomaps));
std::vector < std::vector <std::string> > maps = {{"textmap.txt"},
                                                {"testmap.txt","1-1.txt","1-2.txt"}};

bool game = true; //true - run game
int playerid = 10; //player
bool jumpinprogress = false, inair = false,jump = false; //is plauer jumping is player in air?, jump key pressed
int jumphight = 3; //it does what it says
int jumplenght = 0; // cant think of anything better 
char direction = 'i'; //i - idle, l - left, r - right
bool gravity = true; //turning on and off gravity
int blockid = -1; //solid platform
int brickid = -2; // bricks
int coinbid = -3, coinbeid = -4; //coinblock and empty coinblock
int pipeid = -5, pipelid = -6, piperid = -7, pipehid = -13;
int pipe2id = -8, pipe3id = -9, pipebrid = -10, pipeblid = -11;
int barid = -12;
int lavaid = 4;
int score = 0, coinq = 1, coinid = 1; //score, how much coin gives you and its id
int goalid = 2, goaltid = 3; // goal
int screenx = 0, screeny = 0, initialscreenlenght = 30, scrollback = 3, scrollforward = 8, initialscreenheight = 8, scrollup = 2, scrolldown = 5;
int forcedscreenlenght, forcedscreenheight, screenlenght, screenheight;

std::string filename = "1-2.txt";

void loadmap(std::string filename){
    std::ifstream world(filename);
    world >> alllayers;
    width.resize(alllayers);
    height.resize(alllayers);

    for (int a = 0; a < alllayers; a++){world >> width[a];}
    for (int a = 0; a < alllayers; a++){world >> height[a];}
    world >> playerposx >> playerposy;
    
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

void screensizeupdate(){
    screenx = 0;
    screeny = 0;
    forcedscreenlenght = width[layer];
    forcedscreenheight = height[layer];

    if (initialscreenlenght > forcedscreenlenght){screenlenght = forcedscreenlenght;} else {screenlenght = initialscreenlenght;}
    if (initialscreenheight > forcedscreenheight){screenheight = forcedscreenheight;} else {screenheight = initialscreenheight;}
    scrollforward = screenlenght / 2;
    scrolldown = (screenheight / 2) + 1;
}

void cameraupdate(){
    if (playerposx < (scrollback + screenx) && screenx > 0){screenx = (playerposx - scrollback);} //scroll screen back
    if (playerposx > (scrollforward + screenx) && screenx < (width[layer] - screenlenght)){screenx = (playerposx - scrollforward);} // scroll screen forward
    if (playerposy < (screeny + scrollup) && screeny > 0){screeny = (playerposy - scrollup);} // scroll up
    if (playerposy > (screeny + scrolldown) && screeny < (height[layer] - screenheight)){screeny = (playerposy - scrolldown);} // scroll down
}

void display(){
    //display
    std::cout << "\x1B[2J\x1B[H";
    output = "";
    debug = "";

    //please dont look at here
    debug += "\nplayer: "; debug += std::to_string(playerposx); debug += " "; debug += std::to_string(playerposy); debug += " controls: "; debug += std::to_string(jump); debug += " "; debug += direction; debug += "\n";
    debug += "score: "; debug += std::to_string(score); debug += "\n";
    debug += "screen offset: "; debug += std::to_string(screenx); debug += " "; debug += std::to_string(screeny); debug += "\n";

    output += ("Super Mario SUS\n");

    for (int x = 0; x < screenlenght + 2; x++ ){output += ("- ");}
    for (int b = screeny; b < (screenheight + screeny); b++){
        output += ("\n| ");
        for (int a = screenx; a < (screenlenght + screenx); a++ ){
            switch (field[layer][a][b]){
                case -13: // horizontal pipe
                    output += ("═");
                    break;
                case -12: // bar
                    output += ("=");
                    break;
                case -11: // pipe otton left
                    output += ("╚");
                    break;
                case -10: // pipe bottom right
                    output += ("╝");
                    break;
                case -9: // pipe3
                    output += ("╠");
                    break;
                case -8: // pipe2
                    output += ("╣");
                    break;
                case -7: // pipe right
                    output += ("╗");
                    break;
                case -6: // pipe left
                    output += ("╔");
                    break;
                case -5: // pipe
                    output += ("║");
                    break;
                case -4: // cloin block (empty)
                    output += ("⛝");
                    break;
                case -3: // coin block 
                    output += ("⛋");
                    break;
                case -2: // brick 
                    output += ("▦");
                    break;
                case -1: // block
                    output += ("■");
                    break;
                case 0: // nothing
                    output += (" ");
                    break;
                case 1: // coin
                    output += ("Ø");
                    break;
                case 10: // player
                    output += ("ඞ");
                    break;
                case 2: // pole
                    output += ("|");
                    break;
                case 3: // top
                    output += ("°");
                    break;
                case 4: //lava
                    output += ("^");
                    break;
                default: // errors
                    output += std::to_string(field[layer][a][b]);
                    break;
            }
            output += " ";
        }
        output += ("|");
    }
    output += ("\n");
    for (int x = 0; x < screenlenght + 2; x++ ){output += ( "- ");}
    std::cout << output;
    std::cout << debug;
    Sleep(150);
}

void playerinput(){
    if (GetAsyncKeyState(0x44)){ // d
            direction = 'r';
        }
        else if (GetAsyncKeyState(0x41)){ // a
            direction = 'l';
        }
        else if(GetAsyncKeyState(0x53)){ // s
            for (int a = 0; a < warplen; a++){
                if ((warps[a][0][0] == playerposx) && (warps[a][0][1] == playerposy) && (warps[a][2][2] == layer) && (filename == maps[warps[a][2][0]][warps[a][2][1]])){
                    field[layer][playerposx][playerposy] = 0;
                    filename = maps[warps[a][3][0]][warps[a][3][1]];
                    layer = warps[a][3][2];
                    playerposx = warps[a][1][0];
                    playerposy = warps[a][1][1];
                    if (maps[warps[a][3][0]][warps[a][3][1]] != maps[warps[a][2][0]][warps[a][2][1]]){loadmap(filename);}
                    screensizeupdate();
                    field[layer][playerposx][playerposy] = playerid;
                    break;
                }
                if ((warps[a][1][0] == playerposx) && (warps[a][1][1] == playerposy) && (warps[a][3][2] == layer) && (filename == maps[warps[a][3][0]][warps[a][3][1]])){
                    field[layer][playerposx][playerposy] = 0;
                    filename = maps[warps[a][2][0]][warps[a][2][1]];
                    layer = warps[a][2][2];
                    playerposx = warps[a][0][0];
                    playerposy = warps[a][0][1];
                    if (maps[warps[a][3][0]][warps[a][3][1]] != maps[warps[a][2][0]][warps[a][2][1]]){loadmap(filename);}
                    screensizeupdate();
                    field[layer][playerposx][playerposy] = playerid;
                    break;
                }
            }
        }
        else{ // idle
            direction = 'i';
        }

        if (GetAsyncKeyState(0x20)){ //32 spacebar
            jump = true;
        }
        else{
            jump = false;
        }

}

int main(){  
    /*list of ids: <0 - solid >0 - not
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
    10 = player ඞ

    error 
    */
    std::cin >> filename;
    loadmap(filename);
    screensizeupdate();

    while(game){
        display();

        //exit
        if (GetAsyncKeyState(VK_ESCAPE)) {game=false;break;}

        //win
        if (field[layer][playerposx+1][playerposy] == goalid || field[layer][playerposx-1][playerposy] == goalid || field[layer][playerposx][playerposy+1] == goalid || field[layer][playerposx][playerposy-1] == goalid){
            game = false;
            std::cout << "\nlevel complete!";
            break;
        }
        //checking if player in air
        if (field[layer][playerposx][playerposy+1] < 0){inair = false;} else {inair = true;}
        
        playerinput();

        
        // r l movement
        switch (direction)
        {
        case 'r':
            if (field[layer][playerposx+1][playerposy] == 0 && playerposx < width[layer]){
                field[layer][playerposx][playerposy] = 0;
                field[layer][playerposx+1][playerposy] = playerid;
                playerposx++;
            }
            break;
        case 'l':
            if (field[layer][playerposx-1][playerposy] == 0 && playerposx > screenx){
                field[layer][playerposx][playerposy] = 0;
                field[layer][playerposx-1][playerposy] = playerid;
                playerposx--;
            }
            break; 
        default:  // for idle
            break;
        }

        //jumping
        if (jumpinprogress == true || jump == true){
            if (jumpinprogress == true || inair == false){
                if (jumplenght <= jumphight && field[layer][playerposx][playerposy-1] >= 0){
                    jumpinprogress = true;
                    gravity = false;
                    field[layer][playerposx][playerposy] = 0;
                    field[layer][playerposx][playerposy-1] = playerid;
                    playerposy--;
                    jumplenght++;
                    }
                else if (jumplenght <= jumphight && field[layer][playerposx][playerposy-1] == brickid){
                    jumpinprogress = false;
                    gravity = true;
                    jumplenght = 0;
                    field[layer][playerposx][playerposy-1] = 0;
                }
                else if (jumplenght <= jumphight && field[layer][playerposx][playerposy-1] == coinbid){
                    jumpinprogress = false;
                    gravity = true;
                    jumplenght = 0;
                    field[layer][playerposx][playerposy-1] = coinbeid;
                    field[layer][playerposx][playerposy-2] = coinid;
                }
                else{
                    jumpinprogress = false;
                    gravity = true;
                    jumplenght = 0;
                }
            }
        }

        //gravity
        if (field[layer][playerposx][playerposy+1] >= 0 && gravity == true){
            field[layer][playerposx][playerposy] = 0;
            field[layer][playerposx][playerposy+1] = playerid;
            playerposy++;
            
        }

        cameraupdate();

        //coins, bruh
        if (field[layer][playerposx+1][playerposy] == coinid){
            field[layer][playerposx+1][playerposy] = 0;
            score += coinq;
        }
        if (field[layer][playerposx-1][playerposy] == coinid){
            field[layer][playerposx-1][playerposy] = 0;
            score += coinq;
        }
        if (field[layer][playerposx][playerposy+1] == coinid){
            field[layer][playerposx][playerposy+1] = 0;
            score += coinq;
        }
        if (field[layer][playerposx][playerposy-1] == coinid){
            field[layer][playerposx][playerposy-1] = 0;
            score += coinq;
        }
       
        //death
        if (playerposy >= height[layer] - 1){
            game = false;
            std::cout << "\nGame Over.";
            break;
        }
    }
    std::cout << "\nGame ended";
    return 0;
}