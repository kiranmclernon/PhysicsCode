#include "button.hpp"

struct GoButton : public Interactive::RoundedButton {
    std::function<void(Interactive::Button*)> doNothing = [](Interactive::Button* button){
        std::cout << button->style.size.x << "\n";};
    GoButton(Interactive::Style const& goStyle_, Interactive::Style const& stopStyle_) : 
        RoundedButton(goStyle_, "Go", doNothing), goStyle(goStyle_), stopStyle(stopStyle_), going(true) {}
    Interactive::Style goStyle;
    Interactive::Style stopStyle;
    bool going;
    void onAction(){
        if (!going){
            going = true;
            text.setString("Go");
            style = goStyle;
        } else if (going) {
            going = false;
            text.setString("Stop");
            style = stopStyle;
        }
        setColor(style.color2);
        centreText();
    }
};

sf::Font pendFont;

std::array<std::string, 4> pendColours = {"#BFD7EA", "#91AEC1", "#508CA4", "#000000"};
Interactive::Style pendStyle = Interactive::Style(pendColours, 400, 20, pendFont, 10);
Interactive::Pendulum pend = Interactive::Pendulum(pendStyle, "#0A8754", "#0A8754");

int main(){
    sf::Font font;
    font.loadFromFile("/Users/kiranmclernon/Documents/Projects/PhysicsCode/faster/resources/Fonts/sf-pro-display-cufonfonts/SFPRODISPLAYBOLD.OTF");
    std::array<std::string, 4> goColours = {"#248232", "#2BA84A", "#9CFC97", "#FFFFFF"};
    std::array<std::string, 4> stopColors = {"#F25C54", "#F27059", "#FF5E5B", "#FFFFFF"};
    Interactive::Style goStyle = Interactive::Style(goColours, 200, 40, font, 40);
    Interactive::Style stopStyle = Interactive::Style(stopColors, 200, 40, font, 0);
    GoButton button = GoButton(goStyle, stopStyle);

    bool &configured = button.going;
    sf::Clock clock;
    bool thetaConfigured = false;

   sf::RenderWindow window(sf::VideoMode(1920, 1080), "Pend test");
    window.setFramerateLimit(60);
    button.setPosition(sf::Vector2f(window.getSize().x/2, window.getSize().y/15));
    pend.setPosition(sf::Vector2f(window.getSize().x/2, window.getSize().y/2));
    while (window.isOpen()){
        sf::Event event;
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
            button.update(window, event);
            
        } 
    if (!configured){
            if (!thetaConfigured) {
                pend.setTheta();
                thetaConfigured = true;
            };
        pend.physicsUpdate(time);
    } else if (configured)
    {
        if (thetaConfigured) thetaConfigured = false;
        pend.updatePendulum(window, event, false);
    }
    

    window.clear(sf::Color::Black);
    button.draw(window);
    pend.draw(window);
    window.display();
    }
    
    return 0;
}
