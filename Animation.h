#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>

class Animation {
private:
    sf::Sprite Sprite;
    std::vector<sf::IntRect> frames, frames_flip;
    float currentFrame, speed;
    bool flip, isPlaying;
    friend class AnimationManager;
public:
    Animation(sf::Texture &t, int x, int y, int w, int h, int count, float Speed, int step);
    void tick(float time);
};


class AnimationManager {
public:
    std::string CurrentAnimation;
    std::map<std::string, Animation> AnimList;

    AnimationManager() = default;
    void create(std::string Name, Animation animation);
    void draw(sf::RenderWindow &window, int x_ = 0, int y_ = 0);
    void set(std::string Name);
    void flip(bool isFlip);
    void tick(float time);
    void pause();
    void play();
    void play(std::string name);
    int getFrame();
    std::string getName();
};