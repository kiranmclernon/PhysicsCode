#include "button.hpp"

struct GoButton : public Interactive::RoundedButton {

    GoButton(Interactive::Style const& goStyle_, Interactive::Style const& stopStyle_)
    : RoundedButton(goStyle_, "Go", doNothing), goStyle(goStyle_), stopStyle(stopStyle_){
        std::cout << "working" << "\n";
    }
    bool running = false;
    Interactive::Style goStyle;
    Interactive::Style stopStyle;

    std::function<void(Interactive::Button*)> doNothing = [](Interactive::Button *button){
        int size = button->style.fontSize;
        button->style.fontSize=0;
        button->style.fontSize = size;};
    
    void onAction(){
        if (!running){
            running = true;
            style = goStyle;
            setColor(style.color2);
            text.setString("Stop");
            centreText();
        } else if (running) {
            running = false;
            style = stopStyle;
            setColor(style.color2);
            text.setString("Go");
            centreText();
        }
    }

};



int main(){
    sf::Font font;
    font.loadFromFile("/Users/kiranmclernon/Documents/Projects/PhysicsCode/faster/resources/Fonts/ProggyTiny.ttf");
    std::array<std::string, 4> goColours = {"#248232", "#2BA84A", "#9CFC97", "#0267C1"};
    std::array<std::string, 4> stopColors = {"#F25C54", "#F27059", "#FF5E5B", "#0267C1"};
    Interactive::Style goStyle = Interactive::Style(goColours, 100, 200, font, 24);
    Interactive::Style stopStyle = Interactive::Style(stopColors, 100, 200, font, 24);
    GoButton button = GoButton(goStyle, stopStyle);
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Pend test");
    window.setFramerateLimit(30);
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