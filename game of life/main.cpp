#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include <iostream>

//global variables
#define MAXWIDTH 10
#define MAXHEIGHT 10
#define MAXBOX 20
#define WINWIDTH 1200
#define WINHEIGHT 1200


const int boxwidth = WINWIDTH/MAXWIDTH;
const int boxheight = WINHEIGHT/MAXHEIGHT;



class Cell
{
public:
    sf::RectangleShape react;
    sf::Color gridbackground;
    sf::RenderWindow *Window;
    
    float posx;
    float posy;
    bool isAlive = false;
    
    Cell(float posx,float posy,sf::RenderWindow *window){
        this->Window = window;
        this->posx = posx;
        this->posy = posy;
        
    }
    
    void display(){
        react.setSize(sf::Vector2f(MAXHEIGHT, MAXWIDTH));
        react.setPosition(posx*react.getSize().x, posy*react.getSize().y);
        react.setOutlineThickness(1);
        react.setOutlineColor(sf::Color(0, 60, 12));
        Window->draw(react);
        
    }
    
    void dead(){
        react.setFillColor(sf::Color::Black);
    }
    void alive(){
//        int r = rand()%10;
//        int g = rand()%10;
//        int b = rand()%10;
        react.setFillColor(sf::Color::White);
    }
};

void check_neighbor(int array[boxwidth][boxheight],int oldarray[boxwidth][boxheight],int pos_x,int pos_y);
void copycell(int array[boxwidth][boxheight],int oldarray[boxwidth][boxheight]);



int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINWIDTH, WINHEIGHT), "game of life");
    window.setFramerateLimit(60);
    
    bool is_paused = false;
    
    //cell buffers
    int cells[boxwidth][boxheight];
    //temp buffers
    int buffercell[boxwidth][boxheight];

    //seed for random values
    
    srand(time(NULL));
    for (int i = 0;i < boxwidth;i++){
        for (int j = 0;j < boxheight;j++){
            cells[i][j] = rand()%2;
        }
           
    }
    
    //game
    while (window.isOpen())
    {
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                cells[pos.x/(WINWIDTH / boxwidth)][pos.y/(WINHEIGHT/ boxheight)] = 1;
            }
            switch (event.type) {
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::X:
                            window.close();
                            break;
                        case sf::Keyboard::Space:
                            is_paused ? is_paused = false : is_paused = true;
                            break;
                        default:
                            break;
                    }
                    break;
                    
                default:
                    break;
            }
        }
        
        
        window.clear();
        copycell(cells,buffercell);
    
        for (int i = 0;i < boxwidth;i++){
            for (int j = 0;j < boxheight;j++){
                Cell Cell(i, j,&window);
                
                if (cells[i][j] == 1) {
                    Cell.alive();
                } else {
                    Cell.dead();
                }
                // draw
                Cell.display();
                if (!is_paused){
                    check_neighbor(cells,buffercell, i, j);
                }
                
            }
        }
        copycell(buffercell,cells);
        window.display();
    }
    
    return EXIT_SUCCESS;
}


void check_neighbor(int array[boxwidth][boxheight],int oldarray[boxwidth][boxheight],int pos_x,int pos_y){
  
    int sum = 0;
    
    if ( (pos_y+1) < boxheight){
        sum += array[pos_x][pos_y+1];
    }
    if ((pos_y-1) >= 0){
        sum +=  array[pos_x][pos_y-1];
    }
    if ((pos_x+1) < boxwidth){
        sum += array[pos_x+1][pos_y];
    }
    if ((pos_x-1) >= 0){
        sum += array[pos_x-1][pos_y];
    };
    if ((pos_x + 1) < boxwidth && (pos_y + 1) < boxheight){
        sum += array[pos_x+1][pos_y+1];
    }
    if ((pos_x - 1) >= 0 && (pos_y - 1) >=0){
        sum += array[pos_x-1][pos_y-1];
    }
    if ((pos_x + 1) < boxwidth && (pos_y - 1) >=0){
        sum += array[pos_x+1][pos_y-1];
    }
    if ((pos_x - 1) >= 0 && (pos_y + 1) < boxheight){
        sum += array[pos_x-1][pos_y+1];
    }
    
    /*
    Any live cell with two or three live neighbours survives.
    Any dead cell with three live neighbours becomes a live cell.
    All other live cells die in the next generation. Similarly, all other dead cells stay dead.
    */
    if (sum < 2){
        oldarray[pos_x][pos_y] = 0;
    }else if (sum >=2 && sum <=3 && (array[pos_x][pos_y] == 1)  ){
        oldarray[pos_x][pos_y] = 1;
    }else if (sum == 3 &&  (array[pos_x][pos_y] == 0)){
        oldarray[pos_x][pos_y] = 1;
    }else if (sum > 3 &&  (array[pos_x][pos_y] == 1)){
        oldarray[pos_x][pos_y] = 0;
    };
 
};
void copycell(int array[boxwidth][boxheight],int oldarray[boxwidth][boxheight]){
    for (int i = 0;i < boxwidth;i++){
        for (int j = 0;j < boxheight;j++){
            oldarray[i][j] = array[i][j];
        }
    }
};
