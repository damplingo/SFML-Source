#include "Moving_platform.h"

Moving_platform::Moving_platform(Level &level_, AnimationManager &anim_):Anim(anim_), level(level_) {
    if (level.GetObjects("Moving_platform").size() != 0) {///заполнять в цикле
        auto obj = level.GetObject("Moving_platform");
        Rect = obj.rect;
        level.Delete_object(obj.id);
    }
    dy = -0.05;
    timer = 0;
}

void Moving_platform::update(float time) {
    Rect.top += dy*time;
    timer += time;
    if (timer>5000) {dy *= -1;timer = 0;}
    Anim.tick(time);
}

sf::FloatRect Moving_platform::getRect() {
    return Rect;
}

