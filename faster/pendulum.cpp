#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include <string>
#include "button.hpp"
// #define DEBUG


struct Pendulum{
    sf::RenderWindow *parentWindow;
    sf::RectangleShape stick;
    sf::CircleShape anchor, ball;
    sf::Vector2f centrePoint;
    sf::Transform ballTransform;
    sf::Clock clock;
    static constexpr double g = 9.8;
    float theta;
    float thetaDot;
    float thetaDoubleDot;
    float deltaTheta;
    float stickRadius;
    const double PI = std::acos(0) * 2;

    template<typename T>
    inline double degToRad(T const& deg){return deg * (PI/180);}

    template<typename T> 
    inline double radToDeg(T const& rad) {return rad* (180/PI); }

    std::unordered_map<std::string, float> fluidDensity {{"air", 1.1839}};
    std::unordered_map<std::string, float> coeffDrag {{"smooth_ball", 0.5}};

    Pendulum(sf::RenderWindow *parent_, double const& len, int const& ballRadius, int const& startAngle) : 
        parentWindow(parent_), ballTransform(sf::Transform::Identity) {
        centrePoint.x = (float)parentWindow->getSize().x/2;
        centrePoint.y = (float)parentWindow->getSize().y/2;
        /// Set size
        stick.setSize(sf::Vector2f(10, parentWindow->getSize().y*len*0.5));
        anchor.setRadius(10);
        ball.setRadius(ballRadius);
        /// Set origins
        stick.setOrigin(sf::Vector2f(stick.getSize().x/2, 0));
        anchor.setOrigin(anchor.getRadius(), anchor.getRadius());
        ball.setOrigin(sf::Vector2f(ball.getRadius(), ball.getRadius()));
        /// set Position
        stick.setPosition(centrePoint);
        anchor.setPosition(centrePoint);
        ball.setPosition(centrePoint.x, centrePoint.y+parentWindow->getSize().y * 0.5*len);
        /// rotate
        rotation(startAngle, 1);
        /// colours
        sf::Color ballColour(15, 139, 141);
        sf::Color stickColour(20, 54, 66);
        sf::Color anchorColour(168, 32, 26);
        ball.setFillColor(ballColour);
        stick.setFillColor(stickColour);
        anchor.setFillColor(anchorColour);
        /// radius considered metres
        stickRadius = (parentWindow->getSize().y * len) / 1000;
        theta = degToRad(startAngle);
        thetaDot = 0;
        thetaDoubleDot = 0;
    }
    template <typename T, typename F>
    inline void rotation(T const& rotationRate, F const& time){
        auto rotationVal = rotationRate * time;
        stick.rotate(rotationVal);
        ballTransform.rotate(rotationVal, centrePoint);
    }
    inline void draw(){
        parentWindow->draw(stick);
        parentWindow->draw(anchor);
        parentWindow->draw(ball, ballTransform);
    }
    template <typename T>
    void update(T const& time, bool debug){
        // std::cout << -g/stickRadius << "\n";
        float tempTheta = theta;
        thetaDoubleDot = -g/stickRadius * std::sin(theta);
        thetaDot = thetaDot + thetaDoubleDot * time;
        theta = theta + thetaDot * time;
        deltaTheta = radToDeg(theta-tempTheta);
        stick.rotate(deltaTheta);
        if (debug){
            std::cout << "----------" << "\n";
            std::cout << "Theta: " << theta << "\n";
            std::cout << "Thetadot: " << thetaDot << "\n";
            std::cout <<  "ThetaDoubleDot: " << thetaDoubleDot << "\n"; 
        }
        ballTransform.rotate(deltaTheta, centrePoint);
    }
    static void screenShot(){
        sf::RenderWindow window(sf::VideoMode(1920, 1080), "Screenshot Test");
        Pendulum pendulum = Pendulum(&window, 0.75, 30, -30);
        sf::Texture texture;
        pendulum.draw();
        window.display();
        texture.create(window.getSize().x, window.getSize().y);
        texture.update(window);
        texture.copyToImage().saveToFile("test2.jpeg");
    }
};



int main(){
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Pendulum");
    window.setFramerateLimit(60);
    sf::View view = window.getDefaultView();
    sf::Clock clock;
    #ifdef DEBUG
    int debugTicker = 0;
    #endif
    Pendulum pendulum(&window, 0.75, 30, -170);
    bool pause = false;

    while (window.isOpen()){
        sf::Event event;
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
            } 
            // else if  (event.type == sf::Event::LostFocus){
            //     pause = true;
            // } else if (event.type == sf::Event::GainedFocus){
            //     pause = false;
            // }
        }
        if (not pause){
            window.clear(sf::Color::Black); 
            #ifdef DEBUG
            if (debugTicker % 24 == 0){
                pendulum.update(time, true);
                debugTicker = 0;
            } else{
                pendulum.update(time, false);
            }
            debugTicker ++;
            #elifndef DEBUG
            pendulum.update(time, false);
            #endif
            pendulum.draw();
        }
        window.display();

    }
    return 0;
}
