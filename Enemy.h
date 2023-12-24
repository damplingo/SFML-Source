#pragma once

#include "Animation.h"
#include <SFML/Graphics.hpp>
#include "Level.h"
#include <memory>
#include <string>

///бочки 0 105 105

class Enemy {
    float dx, dy;
    const std::string& name;
    sf::FloatRect Rect;
    Level& level;
    bool onGround;
    bool onLadder;
    bool down;
    bool f = 0;
    //int id;
    float timer;
public:
    AnimationManager anim;
    bool deleted = false;
    Enemy(AnimationManager anim, const std::string& name, Level& level);
    void update(float time);
    sf::FloatRect getRect();
    void path();
    void logic();
    void jumping();
    void simple_logic();
    const std::string& getName();
};
