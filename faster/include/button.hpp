#include <SFML/Graphics.hpp>
#include <string>
#include <array>
#include <iostream>
#include <functional>
#include <cmath>
#pragma once
#ifndef _BUTTON_H_
#define  _BUTTON_H_



class Interactive{
    static sf::Color hexColor(std::string hex);
    static bool inCircle(sf::CircleShape const& circle, sf::Vector2i const& point);
    template <typename T> 
    static float angle(sf::Vector2<T> const& lhs, sf::Vector2<T> const& rhs){
        double lhsMag = std::sqrt(lhs.x*lhs.x + lhs.y*lhs.y);
        double rhsMag = std::sqrt(rhs.x*rhs.x + rhs.y*rhs.y);
        return (lhs.x * rhs.x + lhs.y * rhs.y) / (lhsMag * rhsMag);
    }
public:
    struct Style{
        template<typename T>
        Style(std::array<std::string, 4> const& colors, T width, T height, sf::Font font_, int fontSize_){
            color1 = hexColor(colors[0]);
            color2 = hexColor(colors[1]);
            color3 = hexColor(colors[2]);
            textColor = hexColor(colors[3]);
            size = sf::Vector2f(width, height);
            font = font_;
            fontSize = fontSize_;
        }
        sf::Color color1;
        sf::Color color2;
        sf::Color color3;
        sf::Color textColor;
        sf::Vector2f size;
        int fontSize;
        sf::Font font;
    };
    enum buttonState{
        LAZY,
        PRESSED,
        HOVER,
    };  

    struct Button{
        buttonState state;
        sf::Text text;
        Style style;
        std::function<void(Button*)> actionTarget;
        Button(Style const& style, std::string const& text_, std::function<void(Button*)>& actionTarget_);
        virtual void rotate(float const& theta) = 0;
        virtual void setPosition(sf::Vector2f const& position) = 0;
        virtual void draw(sf::RenderWindow &renderTarget) = 0;
        virtual bool mouseIsOver(sf::RenderWindow &window) = 0;
        virtual void setColor(sf::Color const& color) = 0;
        void setColor(std::string const& colorHex);
        void setTextColor(std::string const& colorHex);
        void centreText();
        void update(sf::RenderWindow &window, sf::Event &event);
    };
    struct RoundedButton : public Button{
        sf::RectangleShape rect;
        sf::CircleShape lhs, rhs;
        sf::Transform lhsTransform, rhsTransform;
        RoundedButton(Style const& style, std::string const& text_, std::function<void(Button*)>& actionTarget_);
        void setPosition(sf::Vector2f const& position);
        void rotate(float const& theta);
        void draw(sf::RenderWindow &renderTarget);
        bool mouseIsOver(sf::RenderWindow &window);
        void setColor(sf::Color const& color);
    };
    struct CircleButton : public Button{
        sf::CircleShape circ;
        CircleButton(Style const& style, std::string const& text_, std::function<void(Button*)>& actionTarget_);
        void rotate(float const& theta);
        void draw(sf::RenderWindow &renderTarget);
        bool mouseIsOver(sf::RenderWindow &window);
        void setColor(sf::Color const& color);
        void setPosition(sf::Vector2f const& position);
    };
    struct PendulumPoint : public Button{
        
    };
};

#endif