#include "button.hpp"

int main(){
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "button test");
    window.setFramerateLimit(30);
    sf::Font font;
    font.loadFromFile("/Users/kiranmclernon/Documents/Projects/PhysicsCode/faster/resources/sf-pro-display-cufonfonts/SFPRODISPLAYHEAVYITALIC.OTF");
    std::array<std::string, 4> colors = {"#7AE7C7", "#75bba7", "#6c809a",  "#345830"};
    std::cout << "working " << "\n";
    Interactive::Style buttonStyle(colors, 400, 200, font, 50);
    std::function<void(Interactive::Button*)> a =  [](Interactive::Button *button){
        button -> rotate(30);
    };
    Interactive::RoundedButton button(buttonStyle, "Hello, World!", a);
    button.setPosition(sf::Vector2f(window.getSize().x/2, window.getSize().y/2));
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            } 
            button.update(window, event);
        }
        window.clear(sf::Color::Black);
        button.draw(window);
        window.display();
    }
    return 0;
}