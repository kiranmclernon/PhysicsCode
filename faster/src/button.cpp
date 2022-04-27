#include "button.hpp"
//****************
// RGB definitions
//****************

sf::Color Interactive::hexColor(std::string hex){
    if(hex.length() == 7 && hex[0] == '#'){
        hex.erase(0,1);
        std::array<std::string, 3> rgb;
        for (int i = 0; i < 3; i++)
            rgb[i] = hex.substr(i*2,2);
        return sf::Color(
            std::stoi(rgb[0], 0, 16),
            std::stoi(rgb[1], 0, 16),
            std::stoi(rgb[2], 0, 16));
    }  else {
        throw("Not a hex color");
    }
}

bool Interactive::inCircle(sf::CircleShape const& circle, sf::Vector2i const& point){
    sf::Vector2f delta = circle.getPosition() - sf::Vector2f(point);
    return std::pow(delta.x,2) + std::pow(delta.y, 2) <= std::pow(circle.getRadius(), 2);
}



//*******************
// Button definitions
//********************
Interactive::Button::Button(Style const& style_, std::string const& text_, std::function<void(Button*)>& actionTarget_)
    : style(style_), actionTarget(actionTarget_){
        text.setFont(style.font);
        text.setString(text_);
        text.setCharacterSize(style.fontSize);
        text.setFillColor(style.textColor);
}

void Interactive::Button::setColor(std::string const& colorHex){setColor(hexColor(colorHex));}

void Interactive::Button::centreText(){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top  + textRect.height/2.0f);
}

void Interactive::Button::update(sf::RenderWindow &window, sf::Event &event){
    if (mouseIsOver(window)) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
            state = PRESSED;
            actionTarget(this);
        } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
            state = HOVER;
        } else if (state != PRESSED)
            state = HOVER;
    }
    else
        if (state != PRESSED) state = LAZY;
    switch (state)
    {
    case PRESSED :
        setColor(style.color3);
        break;
    case HOVER :
        setColor(style.color2);
        break;
    case LAZY :
        setColor(style.color1);
        break;
    default:
        break;
    }
}


// void Interactive::Button::update(sf::RenderWindow &window, sf::Event &event){
//     if (mouseIsOver(window) && event.type == sf::Event::MouseButtonPressed){
//         if (event.mouseButton.button == sf::Mouse::Left)
//             if (!pressed) pressed = true;
//             actionTarget(this);
//     } else if (event.type == sf::Event::MouseButtonReleased)
//         if (pressed) pressed = false;
    
// }

void Interactive::Button::setTextColor(std::string const& colorHex){text.setFillColor(hexColor(colorHex));}

//***************************
// Rounded Button definitions
//****************************
Interactive::RoundedButton::RoundedButton(Style const& style, std::string const& text_, std::function<void(Button*)>& actionTarget_)
    : Button(style, text_, actionTarget_){
        /// Sort out the circles
        lhs.setRadius(style.size.y/2);
        rhs.setRadius(style.size.y/2);
        lhs.setOrigin(lhs.getRadius(), lhs.getRadius());
        rhs.setOrigin(rhs.getRadius(), rhs.getRadius());
        lhs.setFillColor(style.color1);
        rhs.setFillColor(style.color1);
        /// Sort out the rectangle
        sf::Vector2f rectSize = {style.size.x-style.size.y, style.size.y};
        rect.setSize(rectSize);
        rect.setOrigin(rectSize.x/2, rectSize.y/2);
        rect.setFillColor(style.color1);
        /// Sort out text
        centreText();

    }
void Interactive::RoundedButton::setPosition(sf::Vector2f const& position){
    text.setPosition(position);
    rect.setPosition(position);
    lhs.setPosition(sf::Vector2f(position.x-rect.getSize().x/2, position.y));
    rhs.setPosition(sf::Vector2f(position.x+rect.getSize().x/2, position.y));
}
void Interactive::RoundedButton::rotate(float const& theta){
    /// Theta is in degrees
    text.rotate(theta);
    rect.rotate(theta);
    lhsTransform.rotate(theta, rect.getPosition().x, rect.getPosition().y);
    rhsTransform.rotate(theta, rect.getPosition().x, rect.getPosition().y);
}
void Interactive::RoundedButton::draw(sf::RenderWindow &renderTarget){
    renderTarget.draw(rect);
    renderTarget.draw(lhs, lhsTransform);
    renderTarget.draw(rhs, rhsTransform);
    renderTarget.draw(text);
}


bool Interactive::RoundedButton::mouseIsOver(sf::RenderWindow &window){
    sf::Vector2i mousePos(sf::Mouse::getPosition(window).x, 
                        sf::Mouse::getPosition(window).y);
    return (inCircle(lhs, mousePos) || inCircle(rhs, mousePos) 
        || rect.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y));
    
}

void Interactive::RoundedButton::setColor(sf::Color const& color){
    rect.setFillColor(color);
    lhs.setFillColor(color);
    rhs.setFillColor(color);
}

Interactive::CircleButton::CircleButton(Style const& style, std::string const& text_, std::function<void(Button*)>& actionTarget_)
    : Button(style, text_, actionTarget_) {
        circ.setRadius(style.size.x);
        circ.setOrigin(circ.getRadius(), circ.getRadius());   
        circ.setFillColor(style.color1);
        centreText();
    }

void Interactive::CircleButton::rotate(float const& theta){
    circ.rotate(theta);
    text.rotate(theta);
}

void Interactive::CircleButton::draw(sf::RenderWindow &window){
    window.draw(circ);
    window.draw(text);
}
bool Interactive::CircleButton::mouseIsOver(sf::RenderWindow &window){
    sf::Vector2i mousePos(sf::Mouse::getPosition(window).x, 
                    sf::Mouse::getPosition(window).y);
    return inCircle(circ, mousePos);
}  

void Interactive::CircleButton::setColor(sf::Color const& color){circ.setFillColor(color);}

void Interactive::CircleButton::setPosition(sf::Vector2f const& position){
    circ.setPosition(position);
    text.setPosition(position);
}