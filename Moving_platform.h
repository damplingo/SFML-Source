#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "Animation.h"
#include "Level.h"

class Moving_platform {
private:

    float dy;
    int timer;
    Level& level;
    sf::FloatRect Rect;
public:
    AnimationManager& Anim;
    Moving_platform(Level& level_, AnimationManager& anim_);
    void update(float time);
    sf::FloatRect getRect();
};



