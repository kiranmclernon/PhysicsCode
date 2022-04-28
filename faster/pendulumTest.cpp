#include "button.hpp"


sf::Font font;
std::string anchorColor = "#bfdbf7";
std::string stickCol = "#D81159";
std::array<std::string, 4> ballColors {"#db222a", "#73D2DE", "#EEE82C", "#002626"};
Interactive::Style pendStyle(ballColors, 300, 30, font, 10);
Interactive::Pendulum pendulum(pendStyle, stickCol, anchorColor);
int counter = 0;

int main(){
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Pend test");
    window.setFramerateLimit(30);
    pendulum.setPosition(sf::Vector2f(window.getSize().x/2, window.getSize().y/2));
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
            pendulum.updatePendulum(window, event, false);

            
        } 
    window.clear(sf::Color::Black);
    pendulum.draw(window);
    window.display();
    counter ++ ;
    }
    
    return 0;
}