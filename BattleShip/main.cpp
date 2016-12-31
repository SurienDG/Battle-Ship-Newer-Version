
/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, standard IO, and, strings
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <SDL_image.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <time.h>
#include <stdlib.h>
#include <vector>

using namespace std;

typedef vector <int> VectorInt;
std::string IntToString (int integer);

enum colour
{
    red=0,green, orange,purple,black
};

int locationCentre [2] = {0,0};

//The actual hardware texture
SDL_Texture* Intro[1586];

SDL_Texture* menu;
SDL_Texture* battleshipBoard;
SDL_Texture* ships [5][2];
SDL_Texture* StartGameBtn;
SDL_Texture* BattleShipStart;
SDL_Texture* MiniMap;
SDL_Texture* Miss;
SDL_Texture* Hit;


enum downUp {Down=0,Up};
enum xAxisyAxis {xAxis=0, yAxis};


int IntroW=0;
int IntroH=0;


class buttons
{

public:
    void render (void);
    bool ClickedPause (int xcoordinate, int ycoordinate);


private:





};
//Texture wrapper class
class LTexture
{
public:
    //Initializes variables
    LTexture();

    //Deallocates memory
    ~LTexture();



    bool loadMedia(void);
    SDL_Texture* loadFromFile( std::string path, int width, int height );


    //Deallocates texture
    void free();



    //Renders texture at given point
    void render( int x, int y, SDL_Texture* Texture,int width,int height,  SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );



    //Gets image dimensions
    int getWidth();
    int getHeight();




private:
    //The actual hardware texture
    SDL_Texture* mTexture;


};



//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;
SDL_Surface *screen =NULL;
LTexture gSplashTexture;

//Screen dimensions
SDL_Rect gScreenRect = { 0.0, 0.0, 320.0, 240.0 };






LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;

}
bool IntroRender (int x, int y)
{
    //Event handler
    SDL_Event e;
    LTexture texture;
    std::string counter="";

    for (int i=0; i<1502; i+=2)
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0 );
            SDL_RenderClear( gRenderer );
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                return false;
            }
        }
        bool quit=false;
        counter+="IntroPics/Intro";
        counter+=IntToString(i+1);
        counter+=".jpg";
        Intro [i]=texture.loadFromFile( counter.c_str(),gScreenRect.w, gScreenRect.h);
        gSplashTexture.render(x, y,Intro[i],IntroW, IntroH);
        //Update screen
        SDL_RenderPresent( gRenderer );
        if (Intro [i]!=NULL)

        {
            SDL_DestroyTexture( Intro [i] );
            Intro [i] = NULL;
        };

        printf(counter.c_str());
        counter="";
        SDL_Delay(10);

    }

//    //Render splash
//    for (int i=0 ; i<1410; i++)
//
//    {
//    if (Intro [i]!=NULL)
//
//        {
//            SDL_DestroyTexture( Intro [i] );
//           Intro [i] = NULL;
//        };
//    if (i<910)
//    Intro [i+500]=texture.loadFromFile( counter.c_str(),gScreenRect.w, gScreenRect.h);
//    }
    return true;
}
std::string IntToString (int integer)
{


    std::stringstream convert;   // stream used for the conversion

    convert << integer;      // insert the textual representation of 'Number' in the characters in the stream

    return convert.str();
}
int HitCounter (char map[10][10])
{
    int counter=0;
    for (int cols=0; cols<10; cols++)
    {
        for (int rows=0; rows<10; rows++)
        {
            if (map[cols][rows]=='H')
                counter++;
        }

    }
    return counter;
}
char IntToChar (int integer)
{
    return (integer+48);
}


LTexture::~LTexture()
{
    //Deallocate
    free();
}

SDL_Texture* LTexture::loadFromFile( std::string path, int width, int height )
{


    //The final texture
    SDL_Texture* newTexture = IMG_LoadTexture(gRenderer,path.c_str());




    if( newTexture == NULL )
    {
        printf( "Unable to create texture from %s! SDL Error:%s\n", path.c_str(), SDL_GetError() );
    }




    return newTexture;
}




void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;


    }





}





void LTexture::render( int x, int y, SDL_Texture* Texture,int width,int height, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, width, height };

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx( gRenderer, Texture, clip, &renderQuad, angle, center, flip );
}






bool init()
{
    //Initialization flag
    bool success = true;




    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        SDL_Log( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {

        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            SDL_Log( "Warning: Linear texture filtering not enabled!" );
        }

        //Get device display mode
        SDL_DisplayMode displayMode;
        if( SDL_GetCurrentDisplayMode( 0, &displayMode ) == 0 )
        {


            float viewport_aspect_ratio = 1820.0 / 980.0; // The aspect ratio you want your viewport to have
            gScreenRect.w = displayMode.w-100;
            gScreenRect.h = displayMode.h-100;
            float screen_aspect_ratio = float(gScreenRect.w) / float(gScreenRect.h);

            screen_aspect_ratio = float(gScreenRect.w) / float(gScreenRect.h);
            if (viewport_aspect_ratio!=screen_aspect_ratio)
            {


                // screen_aspect_ratio = float(gScreenRect.w) / float(gScreenRect.h);
                if (viewport_aspect_ratio < screen_aspect_ratio)
                {
                    // Viewport is wider than screen, fit on X
                    //gScreenRect.w = displayMode.w-100;
                    gScreenRect.h = (1/screen_aspect_ratio)*gScreenRect.w;
                }
                else
                {
                    // Screen is wider than viewport, fit on Y
                    //gScreenRect.h = displayMode.h-100;
                    gScreenRect.w = gScreenRect.h*viewport_aspect_ratio;
                }


            }
            while (true)
            {
                if (gScreenRect.w>displayMode.w)
                {
                    gScreenRect.w /= 1.1;
                    gScreenRect.h /= 1.1;
                }
                else break;
            }

            locationCentre [xAxis] = displayMode.w/2 - gScreenRect.w/2;
            locationCentre [yAxis] = displayMode.h/2 - gScreenRect.h/2;

            // gScreenRect.h = gScreenRect.h*(980/gScreenRect.h);
            screen_aspect_ratio = float(gScreenRect.w) / float(gScreenRect.h);
            std::cout<</*gScreenRect.w<<*/screen_aspect_ratio/*<<gScreenRect.h*/<<std::endl;
            std::cout<</*gScreenRect.w<<*/viewport_aspect_ratio/*<<gScreenRect.h*/<<std::endl;

        }




        //Create window
        gWindow = SDL_CreateWindow( "Battle Ship", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, displayMode.w, displayMode.h, SDL_WINDOW_SHOWN);
        if( gWindow == NULL )
        {
            SDL_Log( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL )
            {
                SDL_Log( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            }
        }
    }

    return success;
}

bool LTexture::loadMedia(void)
{
//Get rid of preexisting texture
    free();
    std::string counter="";




    IntroW=gScreenRect.w;
    IntroH=gScreenRect.h;

    menu=loadFromFile( "Menu.png", gScreenRect.w, gScreenRect.w);
    battleshipBoard=loadFromFile( "BattleShipBoard.png", gScreenRect.w, gScreenRect.w);
    ships [red][Up]= loadFromFile( "redShipUp.png", gScreenRect.w, gScreenRect.w);
    ships [red][Down]= loadFromFile( "redShipDown.png", gScreenRect.w, gScreenRect.w);
    ships [green][Up]= loadFromFile( "greenShipUp.png", gScreenRect.w, gScreenRect.w);
    ships [green][Down]= loadFromFile( "greenShipDown.png", gScreenRect.w, gScreenRect.w);
    ships [orange][Up]= loadFromFile( "orangeShipUp.png", gScreenRect.w, gScreenRect.w);
    ships [orange][Down]= loadFromFile( "orangeShipDown.png", gScreenRect.w, gScreenRect.w);
    ships [purple][Up]= loadFromFile( "purpleShipUp.png", gScreenRect.w, gScreenRect.w);
    ships [purple][Down]= loadFromFile( "purpleShipDown.png", gScreenRect.w, gScreenRect.w);
    ships [black][Up]= loadFromFile( "blackShipUp.png", gScreenRect.w, gScreenRect.w);
    ships [black][Down]= loadFromFile( "blackShipDown.png", gScreenRect.w, gScreenRect.w);
    StartGameBtn= loadFromFile( "StartGameButton.png", gScreenRect.w, gScreenRect.w);
    BattleShipStart= loadFromFile( "BattleShipStart.png", gScreenRect.w, gScreenRect.w);
    MiniMap= loadFromFile( "Map.png", gScreenRect.w, gScreenRect.w);
    Hit= loadFromFile( "explosion.png", gScreenRect.w, gScreenRect.w);
    Miss= loadFromFile( "miss.png", gScreenRect.w, gScreenRect.w);



    return true;




}


void close()
{
    //Free loaded images
    gSplashTexture.free();

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

bool overlapFunc (int colour, float motionx, float motiony,  float ShipSize[5][2],float ShipsPotions[5][2])
{
    enum coordinates {x=0,y};


    for(int i=0; i<5; i++)
    {
        if (((motionx>ShipsPotions[i][x] or motionx + ShipSize[colour][x]>ShipsPotions[i][x]) and (motionx<ShipsPotions[i][x]+ShipSize[i][x] or motionx + ShipSize[colour][x]<ShipsPotions[i][x]+ShipSize[i][x]))
                and ((motiony>ShipsPotions[i][y] or motiony + ShipSize[colour][y]>ShipsPotions[i][y]) and (motiony<ShipsPotions[i][y]+ShipSize[i][y] )))
        {
            if (i!=colour)
            {
                return true;
            }
        }

    }

}



void PlayersMapSetter (float ShipSize[5][2],float ShipsPotions[5][2], char map[10][10])
{

//check to see if in the on the grid properly

    enum colour
    {
        red=0,green, orange,purple,black
    };
    int xchange = ceil(gScreenRect.w/23.9473684211);
    int ychange = ceil(gScreenRect.h/11.9512195122);
    enum xy {x=0,y};

    int sizes [5]= {4,3,5,3,2};
    char markers [5]= {'R','G','O','P','B'};

//if (ShipsPotions[colour][x]<644)
    // return false;
    for (int i=0; i<5; i++)
    {
        int rows=0;

        for (int ys=gScreenRect.h/5.76470588235 + locationCentre[yAxis]; ys<gScreenRect.h/1.00926879506 + locationCentre[yAxis]; ys+=ychange)
        {
            int cols=0;
            for (int xs=gScreenRect.w/2.82608695652 + locationCentre[xAxis]; xs<gScreenRect.w/1.2962962963 + locationCentre[xAxis]; xs+=xchange)
            {

                if (((ShipsPotions[i][x]>xs or ShipsPotions[i][x] + ShipSize[i][x]>xs) and (ShipsPotions[i][x]<xs+xchange or ShipsPotions[i][x] + ShipSize[i][x]<xs+xchange))
                        and ((ShipsPotions[i][y]>ys or ShipsPotions[i][y] + ShipSize[i][y]>ys) and (ShipsPotions[i][y]<ys+ychange)))
                {
                    map[cols][rows]=markers[i];
                }
                cols++;
            }
            rows++;

        }

    }
}
int randNum(int mini,int maxi)
{

    return rand() % (maxi - mini+1) + mini;

}
bool isShip (int row1, int col1, int row2, int col2, char map [10][10])
{
    //Declare variable
    int shipDifference=0;

    //Switch Rows and Columns
    int rows1 = row1;
    int rows2 = row2;
    int cols1 = col1;
    int cols2 = col2;

    if (row2<row1 && col1==col2)
    {
        rows1=row2;
        rows2=row1;
    }
    else if (row2==row1 && col2<col1)
    {
        cols1=col2;
        cols2=col1;
    }

    //Create Ship
    for (int rows=rows1; rows<=rows2; rows++)
    {
        for (int cols=cols1; cols<=cols2; cols++)
        {
            if (map[cols][rows] != '0')
            {
                return true;
            }


        }
    }
    return false;



}
void randomMap (char map[10][10])
{

//check to see if in the on the grid properly

    enum colour
    {
        red=0,green, orange,purple,black
    };
    int success=0;
    int sizes [5]= {4,3,5,3,2};
    int type = 0;
    int location=0;
    int maxLocation=0;
    int otherLocation=0;
    char markers [5]= {'R','G','O','P','B'};
    enum directions {horz=0, vert};
    int direction=randNum(0,1);
    bool SameType=false;
    bool types[]= {false,false,false,false,false};
    srand (time(NULL));
    while (true)
    {

        direction=randNum(0,1);
        type = randNum(0,4);
        maxLocation = 9-sizes[type];
        location = randNum(0,maxLocation);

        otherLocation = randNum(0,9);
        if (types[type]==false)
        {

            if (direction==horz)
            {
                if (!isShip(location,otherLocation,location+sizes[type]-1,otherLocation, map))
                {

                    for (int cols=otherLocation; cols<=otherLocation; cols++)

                    {
                        for (int rows=location; rows<=location+sizes[type]-1; rows++)
                        {
                            map[cols][rows] = markers[type];
                        }

                    }
                    types[type]=true;
                    success++;

                }
            }
            else
            {
                if (!isShip(otherLocation,location,otherLocation,location+sizes[type]-1, map))
                {

                    for (int rows=otherLocation; rows<=otherLocation; rows++)

                    {
                        for (int cols=location; cols<=location+sizes[type]-1; cols++)
                        {
                            map[cols][rows] = markers[type];
                        }

                    }
                    types[type]=true;
                    success++;

                }
            }
        }
        if (success==5)
        {
            break;
        }

    }




}




void BlankMap (char map[10][10])
{
    for (int i=0; i<10; i++)
    {
        for (int t=0; t<10; t++)
        {
            map[t][i]='0';
        }
    }
}

void DisplayPlayersMap (char map[10][10], float ShipsPotions[5][2], float ShipSize[5][2])
{
    //check to see if in the on the grid properly


    int xchange = gScreenRect.w/56.875;
    int ychange = gScreenRect.h/30.625;
    enum xy {x=0,y};
    int counter=0;

    bool colours [5]= {false,false, false,false, false};


    int rows=0;
    for (int ys=gScreenRect.h/1.73451327434 + locationCentre[yAxis]; ys<gScreenRect.h/1.11111111111 + locationCentre[yAxis]; ys+=ychange)
    {
        int cols=0;

        for (int xs=gScreenRect.w/1.22972972973 + locationCentre[xAxis]; xs<gScreenRect.w/1.01449275362 + locationCentre[xAxis]; xs+=xchange)
        {

            if (map[cols][rows]=='R' and colours[red]==false)
            {
                ShipSize[red][x]/=2.5;
                ShipSize[red][y]/=2.5;
                ShipsPotions[red][x]=xs+2;
                ShipsPotions[red][y]=ys+5;

                colours[red]=true;
            }
            else if (map[cols][rows]=='G' and colours[green]==false)
            {
                ShipSize[green][x]/=2.5;
                ShipSize[green][y]/=2.5;
                ShipsPotions[green][x]=xs+2;
                ShipsPotions[green][y]=ys+2;
                colours[green]=true;
            }
            else if (map[cols][rows]=='B' and colours[black]==false)
            {
                ShipSize[black][x]/=2.5;
                ShipSize[black][y]/=2.5;
                ShipsPotions[black][x]=xs+2;
                ShipsPotions[black][y]=ys+2;
                colours[black]=true;
            }
            else if (map[cols][rows]=='O' and colours[orange]==false)
            {
                ShipSize[orange][x]/=2.5;
                ShipSize[orange][y]/=2.5;
                ShipsPotions[orange][x]=xs+2;
                ShipsPotions[orange][y]=ys+2;
                colours[orange]=true;
            }
            else if (map[cols][rows]=='P' and colours[purple]==false)
            {
                ShipSize[purple][x]/=2.5;
                ShipSize[purple][y]/=2.5;
                ShipsPotions[purple][x]=xs+2;
                ShipsPotions[purple][y]=ys+2;
                colours[purple]=true;
            }





            cols++;
        }
        rows++;


    }





}

void DisplayHitMissPlayerMap(char map [10][10])
{
    int xchange = gScreenRect.w/56.875;
    int ychange = gScreenRect.h/30.625;
    int rows =0;
    for (int ys=gScreenRect.h/1.73451327434 + locationCentre[yAxis]; ys<gScreenRect.h/1.11111111111 + locationCentre[yAxis]; ys+=ychange)
    {
        int cols=0;

        for (int xs=gScreenRect.w/1.22972972973 + locationCentre[xAxis]; xs<gScreenRect.w/1.01449275362 + locationCentre[xAxis]; xs+=xchange)
        {

            if (map[cols][rows]=='H')
            {
                gSplashTexture.render(xs,ys,Hit,xchange, ychange);
            }
            else if (map[cols][rows]=='X')
            {
                gSplashTexture.render(xs,ys,Miss,xchange, ychange);
            }




            cols++;
        }
        rows++;


    }
}



bool OnGrid (float ShipSize[5][2],float ShipsPotions[5][2])
{
//check to see if in the on the grid properly

    int numberOfSquares=0;
    int xchange = ceil(gScreenRect.w/23.9473684211);
    int ychange = gScreenRect.h/11.9512195122;
    enum xy {x=0,y};
    int counter=0;
    int sizes [5]= {4,3,5,3,2};
    for (int i=0; i<5; i++)
    {
        if (ShipsPotions[i][x]+ShipSize[i][x]>gScreenRect.w/1.2962962963 + locationCentre[xAxis] or ShipsPotions[i][y]<gScreenRect.h/5.76470588235 + locationCentre[yAxis]
                or ShipsPotions[i][x]<gScreenRect.w/2.82608695652 + locationCentre[xAxis]  or ShipsPotions[i][y]+ShipSize[i][y]>gScreenRect.h + locationCentre[yAxis])
        {
            return false;
        }

    }
//if (ShipsPotions[colour][x]<644)
    // return false;
    for (int i=0; i<5; i++)
    {
        numberOfSquares=0;
        for (int ys=gScreenRect.h/5.76470588235 + locationCentre[yAxis]; ys<gScreenRect.h/1.00926879506 + locationCentre[yAxis]; ys+=ychange)
        {
            for (int xs=gScreenRect.w/2.82608695652 + locationCentre[xAxis]; xs<gScreenRect.w/1.2962962963 + locationCentre[xAxis]; xs+=xchange)
            {

                if (((ShipsPotions[i][x]>xs or ShipsPotions[i][x] + ShipSize[i][x]>xs) and (ShipsPotions[i][x]<xs+xchange or ShipsPotions[i][x] + ShipSize[i][x]<xs+xchange))
                        and ((ShipsPotions[i][y]>ys or ShipsPotions[i][y] + ShipSize[i][y]>ys) and (ShipsPotions[i][y]<ys+ychange)))
                {
                    numberOfSquares+=1;
                }
            }

        }
        if (numberOfSquares==sizes[i]) counter++;

    }

    if (counter==5)return true;
    else return false;


}

bool PlayerMarkingClick (int Clickx, int Clicky, char AImap [10][10])
{
    int xchange = ceil(gScreenRect.w/23.9473684211);
    int ychange = ceil(gScreenRect.h/11.9512195122);
    int rows=0;

    for (int ys=gScreenRect.h/5.76470588235 + locationCentre[yAxis]; ys<gScreenRect.h/1.00926879506 + locationCentre[yAxis]; ys+=ychange)
    {
        int cols=0;
        for (int xs=gScreenRect.w/2.82608695652 + locationCentre[xAxis]; xs<gScreenRect.w/1.2962962963 + locationCentre[xAxis]; xs+=xchange)
        {

            if (Clickx >xs and Clickx<xs+xchange and Clicky>ys and Clicky<ys+ychange)
            {
                if (AImap[cols][rows]=='0')
                {
                    AImap[cols][rows]='X';
                    return true;
                }
                else if (AImap[cols][rows]!='X' && AImap[cols][rows]!='H')
                {
                    AImap[cols][rows]='H';
                    return true;
                }

            }
            cols++;
        }
        rows++;

    }
    return false;


}

void AITurn (char PlayerMap[10][10])
{
    static int horz=0;
    static int vert=0;
    static bool PreviousHit=false;
    static bool searchingH= false;
    static bool searchingV= false;
    static int HorizontalMovement=0;
    static int VerticalMovement=0;
    static bool HorizontalCheckRight = false;
    static bool HorizontalCheckLeft = false;
    static bool VerticalCheckBottom = false;
    static bool VerticalCheckTop = false;
    static int originalhorz=0;
    static int originalvert=0;
    //static int hits =0;
    if (VerticalCheckTop==true && VerticalCheckBottom==true)
    {
        //cout<<"hello";
        horz=0;
        vert=0;
        PreviousHit=false;
        searchingV= false;
        searchingH= false;

        VerticalMovement=0;
        VerticalCheckTop = false;
        VerticalCheckBottom = false;

    }
    if (HorizontalCheckRight==true && HorizontalCheckLeft==true)
    {
        //cout<<"hello";
        horz=originalhorz;
        if (originalvert==0)
            vert=originalvert+1;
        else
            vert=originalvert-1;
        PreviousHit=false;
        searchingH= false;
        searchingV= true;
        HorizontalMovement=0;
        HorizontalCheckRight = false;
        HorizontalCheckLeft = false;

    }
    //trying to avoid pointer errors but still debugging
    if (!(horz<0 || horz>9 || vert<0 || vert>9))
    {


//searching= false;
        while ((PlayerMap[horz][vert]=='H' || PlayerMap[horz][vert]=='X')&& (PreviousHit==true ||searchingH==true) && searchingV==false /*&& hits<=5*/)
        {
            if (searchingH ==false)
            {
                originalhorz=horz;
                originalvert=vert;
            }

            searchingH= true;

            if (horz==9)
            {
                horz=  originalhorz;
                HorizontalCheckRight = true;
                // horz+=  - HorizontalMovement-1;
                //HorizontalMovement=0;
            }
            else if (horz==0)
            {
                horz=  originalhorz;
                HorizontalCheckLeft = true;
                //horz= horz + HorizontalMovement;
                //HorizontalMovement=0;
            }
            if (HorizontalCheckRight==true && HorizontalCheckLeft==true)
            {
                horz=originalhorz;
                if (originalvert==0)
                {

                    originalvert+=1;
                    vert=originalvert;
                }
                else
                {
                    originalvert-=1;
                    vert=originalvert-1;
                }

                PreviousHit=false;
                searchingH= false;
                searchingV= true;
                HorizontalMovement=0;
                HorizontalCheckRight = false;
                HorizontalCheckLeft = false;

                break;
            }
            if (PreviousHit==true  && HorizontalCheckRight==false)
            {
                horz+=1;
                HorizontalMovement+=1;

            }
            else if (PreviousHit==true && HorizontalCheckLeft==false  )
            {
                horz-=1;
                HorizontalMovement+=1;
            }
            else if (PreviousHit==false && HorizontalCheckRight==false)
            {
                horz=  originalhorz-1;
                if (horz<0)
                {
                    horz=  originalhorz;
                    HorizontalCheckLeft = true;
                }
                HorizontalMovement=1;
                HorizontalCheckRight = true;
                //PreviousHit=true;
            }

            else if (PreviousHit==false  && HorizontalCheckLeft==false/*&& !((horz + HorizontalMovement+1)>9)*/ )
            {
                horz= originalhorz+1;
                if (horz>9)
                {
                    horz= originalhorz;
                    HorizontalCheckRight = true;
                }
                HorizontalMovement=1;
                // PreviousHit=true;
                HorizontalCheckLeft = true;
            }



        }
        if (!(horz<0 || horz>9 || vert<0 || vert>9))
        {


            if (PlayerMap[horz][vert]!='0' && PlayerMap[horz][vert]!='H' && PlayerMap[horz][vert]!='X')
            {
                PlayerMap[horz][vert]='H';

                PreviousHit=true;
                return;
            }
            else if (PlayerMap[horz][vert]=='0')
            {
                PlayerMap[horz][vert]='X';
                PreviousHit=false;
                return;
            }
        }

//starts vertical search
        while ((PlayerMap[horz][vert]=='H' || PlayerMap[horz][vert]=='X')&& (PreviousHit==true ||searchingV==true) && searchingH==false   /*&& hits<=5*/)
        {
            searchingV= true;

            if (vert==9)
            {
                VerticalCheckBottom = true;
                // horz+=  - HorizontalMovement-1;
                //HorizontalMovement=0;
            }
            else if (vert==0)
            {
                VerticalCheckTop = true;
                //horz= horz + HorizontalMovement;
                //HorizontalMovement=0;
            }
            if (VerticalCheckTop==true && VerticalCheckBottom==true)
            {
                horz=0;
                vert=0;
                VerticalCheckBottom=false;
                VerticalCheckTop=false;
                VerticalMovement=0;
                PreviousHit=false;
                searchingV=false;


                break;
            }
            if (PreviousHit==true  && VerticalCheckBottom==false)
            {
                vert+=1;
                VerticalMovement+=1;

            }
            else if (PreviousHit==true && VerticalCheckTop==false  )
            {
                vert-=1;
                VerticalMovement+=1;
            }

            else if (PreviousHit==false && VerticalCheckBottom==false)
            {
                vert= originalvert-1;
                if (vert<0)
                {
                    vert= originalvert;
                    VerticalCheckTop=true;
                }
                VerticalMovement=1;
                VerticalCheckBottom= true;
                // PreviousHit=true;
            }
            else if (PreviousHit==false  && VerticalCheckTop==false/*&& !((horz + HorizontalMovement+1)>9)*/ )
            {
                //vert= originalvert+1;
                if (vert>9)
                {
                    vert= originalvert;
                    VerticalCheckBottom=true;
                }
                VerticalMovement=1;
                //PreviousHit=true;
                VerticalCheckTop = true;
            }





        }

        if (!(horz<0 || horz>9 || vert<0 || vert>9))
        {


            if (PlayerMap[horz][vert]!='0' && PlayerMap[horz][vert]!='H' && PlayerMap[horz][vert]!='X')
            {
                PlayerMap[horz][vert]='H';

                PreviousHit=true;
                return;
            }
            else if (PlayerMap[horz][vert]=='0')
            {
                PlayerMap[horz][vert]='X';
                PreviousHit=false;
                return;
            }
        }
    }

    while (true)
    {


        horz=randNum(0,9);
        vert= randNum(0,9);

        if (PlayerMap[horz][vert]!='0' && PlayerMap[horz][vert]!='H' && PlayerMap[horz][vert]!='X')
        {
            PlayerMap[horz][vert]='H';

            PreviousHit=true;

            return;
        }
        else if (PlayerMap[horz][vert]=='0' )
        {
            PlayerMap[horz][vert]='X';
            PreviousHit=false;
            return;
        }

    }
}



void PlayerDisplayingHitsAndMiss (char AImap [10][10])
{
    int xchange = gScreenRect.w/23.9473684211;
    int ychange = ceil(gScreenRect.h/11.9512195122);
    int rows=0;

    for (int ys=gScreenRect.h/5.76470588235 + locationCentre[yAxis]; ys<gScreenRect.h/1.00926879506 + locationCentre[yAxis]; ys+=ychange)
    {
        int cols=0;
        for (int xs=ceil(gScreenRect.w/2.82608695652)+10 +locationCentre[xAxis]; xs<gScreenRect.w/1.2962962963 + locationCentre[xAxis]; xs+=xchange)
        {

            if (AImap[cols][rows]=='X')
            {
                gSplashTexture.render(xs,ys,Miss,xchange, ychange);
            }
            else if (AImap[cols][rows]=='H')
            {
                gSplashTexture.render(xs,ys,Hit,xchange, ychange);
            }
            cols++;
        }
        rows++;

    }


}




int main( int argc, char* args[] )
{
    bool Menu=true;
    int  Clickx=0;
    int Clicky=0;
    bool trackingGeneral = false;
    int motionx = 0;
    int motiony = 0;
    bool tracking[5]= {false,false,false,false,false};
    bool overlap =false;
    enum coordinates {x=0,y};
    int directions[5]= {Down,Down,Down,Down,Down};
    bool LockShips=false;
    bool StartGame=false;
    char PlayerMap [10][10];
    char AIMap [10][10];
    BlankMap(PlayerMap);
    BlankMap(AIMap);
    randomMap(AIMap);
    bool PlayerTurn = true;
    srand (time(NULL));

    bool WindowOpen = true;


    if( !init())
    {
        SDL_Log( "Failed to initialize!\n" );
    }
    else
    {

        //Main loop flag
        bool quit = false;

        //Event handler
        SDL_Event e;

        gSplashTexture.loadMedia();

        //While application is running
        while( !quit )
        {

            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }

                else if( e.type == SDL_WINDOWEVENT )
                {
                    //Window resize/orientation change
                    if( e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
                    {

                        //Get screen dimensions
                        gScreenRect.w = e.window.data1;
                        gScreenRect.h = e.window.data2;

                        gSplashTexture.loadMedia();





                        //Update screenRocketY-gScreenRect.h/5
                        SDL_RenderPresent( gRenderer );
                    }

                }

            }


            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0 );
            SDL_RenderClear( gRenderer );

            float ShipsPotions[5][2] = {{gScreenRect.w/20 + locationCentre[xAxis],gScreenRect.h/10 + locationCentre[yAxis]},{gScreenRect.w/20 + locationCentre[xAxis],gScreenRect.h/5 + locationCentre[yAxis]},
                {gScreenRect.w/20 + locationCentre[xAxis],gScreenRect.h/4 + locationCentre[yAxis]},{gScreenRect.w/20 + locationCentre[xAxis],gScreenRect.h/3 + locationCentre[yAxis]},{gScreenRect.w/20+ locationCentre[xAxis],gScreenRect.h/2.5 + locationCentre[yAxis]}
            };
            float ShipSize [5][2]= {{gScreenRect.w/6.5,gScreenRect.h/20},{gScreenRect.w/8.7,gScreenRect.h/20},{gScreenRect.w/5,gScreenRect.h/20},{gScreenRect.w/8.7,gScreenRect.h/20},{gScreenRect.w/14,gScreenRect.h/20}};



            //Maps.collision();



            //Maps.ScrollingMaps();




            while (true)
            {

//Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0);
                SDL_RenderClear( gRenderer );
                Clickx=0;
                Clicky=0;
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        //Free resources and close SDL
                        close();

                        return 0;
                    }
                    else if(e.type == SDL_MOUSEBUTTONDOWN)
                    {
                        //If the left mouse button was pressed
                        if(e.button.button == SDL_BUTTON_LEFT )
                        {
                            Clickx=e.button.x;
                            Clicky=e.button.y;

                            for (int i=0; i<5; i++)
                            {

                                if (tracking[i]==true and trackingGeneral==true and !overlapFunc(i,motionx,motiony,ShipSize,ShipsPotions))
                                {
                                    tracking[i]=false;
                                    trackingGeneral=false;
                                }




                            }
                        }
                        else
                        {
                            if (trackingGeneral==true)
                            {


                                for (int i=0; i<5; i++)
                                {
                                    if (tracking[i]==true)
                                    {
                                        int temp=0;
                                        temp=ShipSize[i][x];
                                        ShipSize[i][x]=ShipSize[i][y];
                                        ShipSize[i][y]=temp;
                                        temp=ShipsPotions[i][x];
                                        ShipsPotions[i][x]=ShipsPotions[i][y];
                                        ShipsPotions[i][y]=temp;
                                        if (directions[i]==Up)
                                            directions[i]=Down;
                                        else
                                            directions[i]=Up;

                                    }


                                }
                            }


                        }
                    }

                    else if( e.type == SDL_MOUSEMOTION )
                    {
                        if (trackingGeneral==true)
                        {

                            //Get the mouse offsets

                            SDL_GetMouseState( &motionx, &motiony );

                        }



                    }
                    //If a key was pressed
                    else if( e.type == SDL_KEYDOWN /*&& e.key.repeat == 0*/ )
                    {
                        //Adjust the velocity
                        if  (e.key.keysym.sym == SDLK_ESCAPE)
                        {
                            //Free resources and close SDL
                            close();

                            return 0;
                        }
                        else if (e.key.keysym.sym == SDLK_KP_MINUS)
                        {
                            SDL_MinimizeWindow(gWindow);

                        }
                    }


                }

                if (trackingGeneral==true)
                {
                    for (int i=0; i<5; i++)
                    {
                        if (tracking[i]==true)
                        {


                            ShipsPotions[i][x]=motionx;
                            ShipsPotions[i][y]=motiony;


                        }
                    }


                }
                if (Clickx!=0 or Clicky!=0)
                {
                    std::cout<<std::endl<<"x="<<Clickx;
                    std::cout<<std::endl<<"y="<<Clicky;
                }

//std::cout<<gScreenRect.w/20+IntroW/5<<std::endl;
                if (trackingGeneral==false and LockShips==false)
                {


                    for (int i=0; i<5; i++)
                    {

                        if (Menu==false and Clicky<ShipsPotions[i][y]+ShipSize[i][y] and Clicky>ShipsPotions[i][y] and Clickx>ShipsPotions[i][x] and Clickx<ShipsPotions[i][x]+ShipSize[i][x] )
                        {
                            trackingGeneral=true;
                            tracking[i]=true;
                            motionx=ShipsPotions[i][x];
                            motiony=ShipsPotions[i][y];
                        }
                    }
                }

                if (Menu==true)
                {

                    gSplashTexture.render(( gScreenRect.w - gScreenRect.w/2 - gScreenRect.w/3 - gScreenRect.w/5) / 2 +gScreenRect.w/60.6666666667 + locationCentre[xAxis],0 + locationCentre[yAxis],menu,IntroW, IntroH);
                    //Update screen
                    SDL_RenderPresent( gRenderer );
                    //Menu=true;
                    if (Clickx>gScreenRect.w/6.97318007663 + locationCentre[xAxis] and Clickx<gScreenRect.w/1.15262824573 + locationCentre[xAxis]
                            and Clicky<gScreenRect.h/1.18932038835 + locationCentre[yAxis] and Clicky>gScreenRect.h/1.48036253776 + locationCentre[yAxis])
                    {
                        Menu=false;
                    }
                }
                else if (StartGame==false and Menu==false)
                {
                    //Clear screen
                    //SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0 );
                    //SDL_RenderClear( gRenderer );
                    gSplashTexture.render(( gScreenRect.w - gScreenRect.w/2 - gScreenRect.w/3 - gScreenRect.w/5) / 2 +gScreenRect.w/60.6666666667 + locationCentre[xAxis],0 + locationCentre[yAxis],battleshipBoard,IntroW, IntroH);
                    int temp=10;
                    for (int i=0; i<5; i++)
                    {

                        gSplashTexture.render(ShipsPotions[i][x],ShipsPotions[i][y],ships[i][directions[i]],ShipSize[i][x], ShipSize[i][y]);
                        if (tracking[i]==true)
                        {
                            temp=i;
                        }
                    }



                    if (temp!=10)
                        gSplashTexture.render(ShipsPotions[temp][x],ShipsPotions[temp][y],ships[temp][directions[temp]],ShipSize[temp][x], ShipSize[temp][y]);

                    if (trackingGeneral==false)
                    {
                        if (OnGrid(ShipSize,ShipsPotions))
                        {
                            gSplashTexture.render(gScreenRect.w/1.27 + locationCentre[xAxis],gScreenRect.h/1.7 + locationCentre[yAxis],StartGameBtn,gScreenRect.w/5, gScreenRect.h/10);
                            if (Clickx>gScreenRect.w/1.27 + locationCentre[xAxis] and Clickx<gScreenRect.w/1.27+gScreenRect.w/5 + locationCentre[xAxis]
                                    and Clicky>gScreenRect.h/1.7 + locationCentre[yAxis] and Clicky<gScreenRect.h/1.7+gScreenRect.h/10 + locationCentre[yAxis])
                            {

                                LockShips=true;
                                StartGame=true;
                                PlayersMapSetter(ShipSize,ShipsPotions,PlayerMap);
                                DisplayPlayersMap(PlayerMap,ShipsPotions,ShipSize);
                            }
                        }


                    }



                }
                else if (StartGame==true )
                {



                    gSplashTexture.render(( gScreenRect.w - gScreenRect.w/2 - gScreenRect.w/3 - gScreenRect.w/5) / 2 +0.01648351648*gScreenRect.w + locationCentre[xAxis],0 + locationCentre[yAxis],BattleShipStart,IntroW, IntroH);

                    gSplashTexture.render(gScreenRect.w/1.25517241379 + locationCentre[xAxis],gScreenRect.h/1.83177570093 + locationCentre[yAxis],MiniMap,gScreenRect.w/5.2, gScreenRect.h/2.8);

                    for (int i=0; i<5; i++)
                    {

                        gSplashTexture.render(ShipsPotions[i][x],ShipsPotions[i][y],ships[i][directions[i]],ShipSize[i][x], ShipSize[i][y]);

                    }




                    PlayerDisplayingHitsAndMiss(AIMap);

                    if (PlayerTurn==true)
                    {
                        if (PlayerMarkingClick(Clickx,Clicky,AIMap)==true)
                        {
                            PlayerTurn=false;
                        }
                    }
                    else
                    {

                        //the AI's turn
                        AITurn(PlayerMap);
                        for (int rows=0; rows<10; rows++)
                        {
                            for (int cols=0; cols<10; cols++)
                            {
                                std::cout<<PlayerMap[cols][rows];
                            }
                            std::cout<<std::endl;
                        }
                        PlayerTurn=true;
                    }
                    DisplayHitMissPlayerMap(PlayerMap);



                    //Update screen
                    // SDL_RenderPresent( gRenderer );
                }



                SDL_RenderPresent( gRenderer );
                if (HitCounter(PlayerMap)==17)
                {
                    std::cout<<"comp win";
                    //return 0;

                }
                else if (HitCounter(AIMap)==17)
                {
                    std::cout<<"player wins";
                    //return 0;

                }


            }


        }
    }





//Free resources and close SDL
    close();

    return 0;
}
