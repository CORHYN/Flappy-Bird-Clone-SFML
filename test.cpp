#include <SFML/Graphics.hpp> // sf
#include <iostream> // std
#include <string>
#include <cstdlib> // std

#define WINDOW_HIGHT 600
#define WINDOW_WIDTH 800
#define PLAYER_START_POS_X 200
#define PLAYER_START_POS_Y 0

using namespace sf;

class Pillar_Obj{
    private:
    // x and y postion of the pillartop - pt , and pillarbottom - pb
        float pt_x = 800,pt_y = 250,pb_x = 800,pb_y = 350;
    // Pillar offset value 
        float offset = 0;
    // Declare the top and bottom pillers for the game
        RectangleShape pillar_top, pillar_bottom;
    public:
        Pillar_Obj(float x = 0.0, float y = 0.0){
        // set the scales for both the top and bottom piller
            pillar_top.setSize(Vector2f(x, y));
            pillar_bottom.setSize(Vector2f(x, y));
        // default color for both is green
            pillar_top.setFillColor(Color::Green);
            pillar_bottom.setFillColor(Color::Green);
        // set the top pillars point of transformation to be the bottom left corner , this will help later on in the calcualations
        // origin y-axis + hight of pillar -> set this as the y-axis of the new point of origin
            pillar_top.setOrigin(0,y);
        }
        // checks if the pillar is pass the 0 on the axis , to make sure that its comepletely invisible to the player
        // subtract the witdh of the pillar from 0 to get the point on the x-axis at which point the pillar will be completely invisible to the player
        bool is_Visible_Behind_Player(){
            return (pillar_top.getPosition().x < (0 - pillar_top.getSize().x));
        }
        // randomly set the pillar hight in the y axis , while retaning the x axis position
        void Set_Pillar_Position(){
            int random = rand() % 301 + (-150);
            float pos_y = pt_y + random;
            pillar_top.setPosition(pt_x, pos_y);
            pillar_bottom.setPosition(pb_x, pos_y+ 100);
        }
        // check if a shape collides with the pillares
        bool Check_Collison(Shape *Shape){
            if(pillar_top.getGlobalBounds().intersects(Shape->getGlobalBounds()) || pillar_bottom.getGlobalBounds().intersects(Shape->getGlobalBounds())){
                return true;
            }else{
                return false;
            }
        }
        // Move the pillar depending on the velocity set , + forwards , - backwards
        void Move_Pillar(float pillar_velocity){
            pillar_top.move(pillar_velocity,0);
            pillar_bottom.move(pillar_velocity,0);
        }
        // Used to set the pillars to the original postition they started in 
        void Set_Pillar_Back_to_Orgin(){
            pillar_top.setPosition(pt_x, pt_y);
            pillar_bottom.setPosition(pb_x, pb_y);
        }
        // Set the pillar offset from the position its choosen to appear at 
        // (+) - offset to the right of
        // (-) - offset to the left of 
        void Set_Pillar_Offset(float offset){
            this->offset = offset;
            pt_x += offset;
            pb_x += offset;
        }
        // Reset the offset
        void Reset_Pillar_Offset(){
            pt_x -= offset;
            pb_x -= offset;
            offset -= offset;
        }
        // Draw the pillar to the window
        void Draw_pillar(RenderWindow * w){
            w->draw(pillar_top);
            w->draw(pillar_bottom);
        }
};

int main()
{

    // Window 
    int score = 0;
    int h_score = 0;
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HIGHT),"Flappy Bird SFML", Style::Titlebar | Style::Close);
    CircleShape player(20);
    Font font;
    font.loadFromFile("ARCADECLASSIC.TTF");
    Text ScoreCount,HighScore, Controls, GameOver;
    Controls.setFont(font);
    ScoreCount.setFont(font);
    HighScore.setFont(font);
    GameOver.setFont(font);
    Controls.setString("Space Bar  Jump \nEscape  Exit");
    ScoreCount.setString(std::to_string(score));
    HighScore.setString(std::to_string(score));
    GameOver.setString("Game Over");
    GameOver.setCharacterSize(68);
    HighScore.setPosition(750,0);
    Controls.setPosition(0,500);
    GameOver.setPosition(275,245);
    player.setPosition(PLAYER_START_POS_X, PLAYER_START_POS_Y);
    player.setFillColor(Color::Red);
    const float velocity_pillar = -5;
    Pillar_Obj pillars1(50,800);
    Pillar_Obj pillars2(50,800);
    pillars2.Set_Pillar_Offset(425);
    pillars1.Set_Pillar_Position();
    pillars2.Set_Pillar_Position();
    pillars2.Reset_Pillar_Offset();

    std::cout << "Welcome to Flappy Bird Clone made with SFML\n";
    // Game Loop
    float currentTime;
    float player_velocity = 0.1;
    Clock clock;
    window.setFramerateLimit(60);
    // pause condition
    bool pause = true;
    bool game_over = false;
    
    while(window.isOpen()){
        // Event polling
        // Compute framerate
        currentTime = clock.restart().asSeconds();
        float fps = 1.0 / (currentTime);
        Event evnt;
        
        while(window.pollEvent(evnt)){
            switch (evnt.type)
            {
            case Event::Closed:
                window.close();
                break;
            case Event::KeyReleased:
                if(evnt.key.code == Keyboard::Space){
                    std::cout << "Jump" << std::endl;
                    player_velocity = -8;
                }
                break;
            case Event::KeyPressed:
                if(evnt.key.code == Keyboard::Escape){
                    window.close();
                }else if(evnt.key.code == Keyboard::P){
                    pause = !pause;
                    std::cout << "Pause" << std::endl;
                }
                break;
            }
        }  
        
        if(pause){
        if(pillars1.is_Visible_Behind_Player()){
            pillars1.Set_Pillar_Position();
        }else if(pillars2.is_Visible_Behind_Player()){
            pillars2.Set_Pillar_Position();
        }

        if(player.getPosition().y <= 0){
            player.setPosition(player.getPosition().x, 0);
        }

        if(player.getPosition().y >= 600 || pillars1.Check_Collison(&player) ||  pillars2.Check_Collison(&player)){
            if(!game_over){
                game_over = true;
            }
            if(score > h_score){
                h_score = score;
                HighScore.setString(std::to_string(h_score));
            }
            if(Keyboard::isKeyPressed(Keyboard::Space)){
                player.setPosition(PLAYER_START_POS_X, PLAYER_START_POS_Y);
                pillars2.Set_Pillar_Offset(425);
                pillars1.Set_Pillar_Position();
                pillars2.Set_Pillar_Position();
                pillars2.Reset_Pillar_Offset();
                score = 0;
                game_over = false;
            }
        }else{
            player_velocity += 0.5; // Gravity is applied  
            score += 1;
            // std::cout << player_velocity << std::endl;
            player.move(Vector2f(0,player_velocity));
            pillars1.Move_Pillar(velocity_pillar);
            pillars2.Move_Pillar(velocity_pillar);
            ScoreCount.setString(std::to_string(score));
        }
        }

        window.clear();
        window.draw(player);

        
        
        pillars1.Draw_pillar(&window);
        pillars2.Draw_pillar(&window);

        window.draw(Controls);
        window.draw(ScoreCount);
        window.draw(HighScore);
        if(game_over){
            window.draw(GameOver);
        }
        window.display(); 
    }
   
     
    return 0;
}