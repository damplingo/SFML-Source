#include "Animation.h"
#include <iostream>

Animation::Animation(sf::Texture &t, int x, int y, int w, int h,
                     int count, float Speed, int step):
                     speed(Speed), currentFrame(0), isPlaying(true), flip(false){
    Sprite.setTexture(t);

    for (int i = 0; i < count; ++i) {
        frames.push_back(sf::IntRect(x+i*step, y, w, h));
        frames_flip.push_back(sf::IntRect(x+i*step+w, y, -w, h));
    }
}

void Animation::tick(float time) {
    if (!isPlaying) {
        return;
    }
    currentFrame += speed*time;
    if (currentFrame > frames.size()) currentFrame -= frames.size();
    if (!flip) Sprite.setTextureRect(frames[currentFrame]);
    if (flip) Sprite.setTextureRect(frames_flip[currentFrame]);
}

void AnimationManager::create(std::string Name, Animation animation) {
    CurrentAnimation = Name;
    AnimList.insert({CurrentAnimation, animation});
    //AnimList.insert({Name, animation});
}

void AnimationManager::draw(sf::RenderWindow &window, int x_, int y_) {
    auto it = AnimList.find(CurrentAnimation);
    it->second.Sprite.setPosition(x_, y_);
    window.draw(it->second.Sprite);
}

void AnimationManager::set(std::string Name) {
    CurrentAnimation = Name;
}
void AnimationManager::flip(bool isFlip) {
    auto it = AnimList.find(CurrentAnimation);
    it->second.flip = isFlip;
}
void AnimationManager::tick(float time) {
    auto it = AnimList.find(CurrentAnimation);
    it->second.tick(time);

}

int AnimationManager::getFrame() {
    auto it = AnimList.find(CurrentAnimation);
    return static_cast<int>(it->second.currentFrame);
}


void AnimationManager::pause() {
    auto it = AnimList.find(CurrentAnimation);
    it->second.isPlaying = false;
}

void AnimationManager::play() {
    auto it = AnimList.find(CurrentAnimation);
    it->second.isPlaying = true;
}

void AnimationManager::play(std::string name) {
    auto it = AnimList.find(name);
    it->second.isPlaying = true;
}

std::string AnimationManager::getName() {
    return CurrentAnimation;
}