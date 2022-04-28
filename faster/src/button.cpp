#include "button.hpp"
//****************
// RGB definitions
//****************

const float Interactive::PI = std::acos(0) * 2;
const float Interactive::DEGRADCONST = PI/180;
const float Interactive::RADDEGCONST = 180/PI;

float Interactive::degToRad(float const& deg) {return deg * DEGRADCONST;}

float Interactive::radToDeg(float const& rad) {return rad * RADDEGCONST;}

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

bool Interactive::inCircle(sf::CircleShape const& circle, sf::Vector2f const& point){
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
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            if (state != PRESSED) state = PRESSED;
        } else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (state==PRESSED){onAction();}
            if (state != HOVER) state = HOVER;
        }
    } else{
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            if (state ==PRESSED) onAction();
            state = LAZY;
        }
    }
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

void Interactive::RoundedButton::onAction(){actionTarget(this);}


bool Interactive::RoundedButton::mouseIsOver(sf::RenderWindow &window){
    sf::Vector2i mousePos(sf::Mouse::getPosition(window).x, 
                        sf::Mouse::getPosition(window).y);
    sf::Vector2f envPos = window.mapPixelToCoords(mousePos);
    return (inCircle(lhs, envPos) || inCircle(rhs, envPos) 
        || rect.getGlobalBounds().contains(envPos));
    
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

void Interactive::CircleButton::onAction(){actionTarget(this);}


bool Interactive::CircleButton::mouseIsOver(sf::RenderWindow &window){
    sf::Vector2i mousePos(sf::Mouse::getPosition(window).x, 
                    sf::Mouse::getPosition(window).y);
    sf::Vector2f envPos = window.mapPixelToCoords(mousePos);
    return inCircle(circ, envPos);
}  

void Interactive::CircleButton::setColor(sf::Color const& color){circ.setFillColor(color);}

void Interactive::CircleButton::setPosition(sf::Vector2f const& position){
    circ.setPosition(position);
    text.setPosition(position);
}

//*********************
// Pendulum definitions
//*********************

std::function<void(Interactive::Button*)> Interactive::Pendulum::actionTarget = [](Interactive::Button *button){button->style.fontSize=0;};

Interactive::Pendulum::Pendulum(Style const& style, std::string const& stickCol_, std::string const& anchorCol_) 
    : Button(style, "", actionTarget), stickCol(hexColor(stickCol_)), anchorCol(hexColor(anchorCol_)){
    /// Anchor
    anchor.setRadius(10);
    anchor.setOrigin(anchor.getRadius(), anchor.getRadius());
    anchor.setFillColor(anchorCol);
    /// Stick
    stick.setSize(sf::Vector2f(10, style.size.x));
    stick.setOrigin(5,0);
    stick.setFillColor(stickCol);
    /// Anchor
    ball.setRadius(style.size.y);
    ball.setOrigin(ball.getRadius(), ball.getRadius());
    ball.setFillColor(style.color1);
    /// Angles
    theta = 0;
    thetaDot = 0;
    thetaDoubleDot = 0;
    stickRadius = style.size.x/150;
    }

void Interactive::Pendulum::rotate(float const& angle){
    stick.rotate(angle);
    float stickRotation = stick.getRotation();
    sf::Vector2f anchorToBall = sf::Vector2f(style.size.x * std::sin(degToRad(-stickRotation)),
                                            style.size.x * std::cos(degToRad(-stickRotation)));
    ball.setPosition(stick.getPosition()+anchorToBall);
}


void Interactive::Pendulum::setTheta(){
    if (stick.getRotation()>180) theta=  degToRad(stick.getRotation() - 360);
    else theta =  degToRad(stick.getRotation());
    thetaDot = 0;
    thetaDoubleDot = 0;
}

float Interactive::Pendulum::getTheta() const {return theta;}

void Interactive::Pendulum::setRotation(float const& angle){
    stick.setRotation(angle);
    float stickRotation = stick.getRotation();
    sf::Vector2f anchorToBall = sf::Vector2f(style.size.x * std::sin(degToRad(-stickRotation)),
                                            style.size.x * std::cos(degToRad(-stickRotation)));
    ball.setPosition(stick.getPosition()+anchorToBall);
}

void Interactive::Pendulum::setPosition(sf::Vector2f const& position){
    anchor.setPosition(position);
    stick.setPosition(position);
    float stickRotation = 90-stick.getRotation();
    sf::Vector2f anchorToBall = sf::Vector2f(style.size.x * std::cos(degToRad(stickRotation)),
                                            style.size.x * std::sin(degToRad(stickRotation)));
    ball.setPosition(position+anchorToBall);
}

void Interactive::Pendulum::setColor(sf::Color const& color){
    ball.setFillColor(color);
}

bool Interactive::Pendulum::mouseIsOver(sf::RenderWindow &window){
    sf::Vector2i mousePos(sf::Mouse::getPosition(window).x, 
                    sf::Mouse::getPosition(window).y);
    sf::Vector2f envPos = window.mapPixelToCoords(mousePos);
    return inCircle(ball, envPos);
}

void Interactive::Pendulum::draw(sf::RenderWindow &window){
    window.draw(stick);
    window.draw(anchor);
    window.draw(ball);
}

void Interactive::Pendulum::mouseAngleTransform(sf::RenderWindow const& window){
    sf::Vector2i mousePos(sf::Mouse::getPosition(window).x, 
                    sf::Mouse::getPosition(window).y);
    sf::Vector2f envPos = window.mapPixelToCoords(mousePos);
    envPos = sf::Vector2f(envPos.x, -envPos.y);
    sf::Vector2f anchorPos = sf::Vector2f(anchor.getPosition().x, -anchor.getPosition().y);
    envPos = envPos - anchorPos;
    float theta = angle(envPos, downVector);
    float realAngle;
    if (envPos.x >= 0){
        realAngle = 360 -  theta;
    } else if (envPos.x <= 0){
        realAngle = theta;
    } 
    rotate(realAngle-stick.getRotation());
}

void Interactive::Pendulum::updatePendulum(sf::RenderWindow& window, sf::Event &event, bool debug){
    tMinsOne = state;
    update(window, event);
    if (state == PRESSED){
        mouseAngleTransform(window);
    }
    if (debug) std::cout << state << "\n";
}
void Interactive::Pendulum::onAction(){int a= 0;}  

void Interactive::Pendulum::physicsUpdate(float const& time){
    float tempTheta = theta;
    thetaDoubleDot = -g/stickRadius * std::sin(theta);
    thetaDot = thetaDot + thetaDoubleDot * time;
    theta = theta + thetaDot * time;
    setRotation(radToDeg(theta));
}



std::ostream& operator<<(std::ostream& out, sf::Color const& target){
    out << "Red : " << (int)target.r << " Green : " << (int)target.g << " Blue : " << (int)target.b;
    return out;
}