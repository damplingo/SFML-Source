#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "Level.h"
#include "Animation.h"
#include "Enemy.h"



///прибавить 21 к ladder
class Jumpman {
private:
    float start_hammer = 0;
    float now_hammer = 0;
    sf::Clock hammer_time;
    int bonus = 0;
    int record;
    int score;

public:

    AnimationManager& anim;
    sf::RenderWindow& window;
    Jumpman(sf::Texture Texture, Level& level, AnimationManager& A, sf::RenderWindow& window1, int record, int score);
    void update(float time);
    float dx, dy;///speed
    bool onGround, onLadder, hammer, win;
    sf::FloatRect Rect;
    Level& level;
    void Collision(int num);
    enum {run, stay, jump, climb, died} STATE;
    void Keycheck();
    bool dir;
    bool dir2;///1 - вверх, 0 - вниз
    sf::FloatRect getRect();
    void Animation(float time);
    void Collision_with_enemy(sf::FloatRect Enemy_rect);
    bool HammerCheck();
    void add_bonus(int bonus);
    int getBonus();
    int& getScore();
    void Collision_with_platform(sf::FloatRect platform_rect);
    int CurrentHammerid;
    int getRecord();
    bool Collision_with_grit(sf::FloatRect grit);
    void Hammer_collision(std::list<Enemy>& enemy_list, float time);
};


