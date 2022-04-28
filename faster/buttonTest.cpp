#include "button.hpp"

int main(){
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "button test");
    window.setFramerateLimit(30);
    sf::Font font;
    font.loadFromFile("/Users/kiranmclernon/Documents/Projects/PhysicsCode/faster/resources/Fonts/ProggyTiny.ttf");
    std::array<std::string, 4> colors = {"#7AE7C7", "#75bba7", "#6c809a",  "#345830"};
    std::cout << "working " << "\n";
    Interactive::Style buttonStyle(colors, 400, 200, font, 50);
    std::array<std::string, 4> colors2 = {"#f59ca9", "#f6828c", "#DF57BC", "#484A47"};
    Interactive::Style buttonStyle2(colors2, 400, 300, font, 50);
    typedef std::function<void(Interactive::Button*)> activationFunc;
    activationFunc a = [](Interactive::Button *button){button -> rotate(-30);};
    activationFunc b = [](Interactive::Button *button){button -> rotate(30);};
    Interactive::CircleButton button2(buttonStyle2, "I'm a circle", a);
    Interactive::RoundedButton button(buttonStyle, "Hello, World!", b);
    button.setPosition(sf::Vector2f(window.getSize().x/4, window.getSize().y/2));
    button2.setPosition(sf::Vector2f(3* window.getSize().x/4, window.getSize().y/2));
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            } 
            button.update(window, event);
            button2.update(window, event);
        }
        window.clear(sf::Color::Black);
        button.draw(window);
        button2.draw(window);
        window.display();
    }
    return 0;
}