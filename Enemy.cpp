#include "Enemy.h"
#include <iostream>
#include <random>

Enemy::Enemy(AnimationManager _anim, const std::string& _name, Level &_level):anim(_anim), name(_name), level(_level) {
    if (level.GetObjects(name).size() != 0) {///походу из-за этого сегфал
    Rect = level.GetObject(name).rect;///name///objects
    //id = level.GetObject(name).id;
    level.Delete_object(level.GetObject(name).id);
    timer = 0;
    if (name == "fire") {
        dx = 0.05;
    }

    else if (name == "spring") {
        dx = 0.07;
        dy = -0.04;
    }
    else if (name == "s_fire") {
        dx = -0.05;
    }
    else {dx = 0;
    dy = 0;}
    onGround = false;
    onLadder = false;
    down = false;
    }

}

sf::FloatRect Enemy::getRect() {
    return Rect;
}

void Enemy::update(float time) {
    if (!onGround && (name == "barrel")) dy += 0.005 * time;
    onGround = false;
    if (!deleted){
        Rect.left += dx * time;
        Rect.top += dy * time;
    }
    if (name == "barrel") path();
    else if (name == "fire") logic();
    else if (name == "spring" && !down) {
        anim.set("spring_open");

        jumping();
        timer += time;
        if (timer>900) {dy *= -1;timer = 0;}

    }
    else if(name == "s_fire") {
        timer += time;
        simple_logic();
        anim.flip(false);
        if (dx>0) anim.flip(true);
    }
    anim.tick(time);
}
///добавить конга и функцию,которая будет раз в какое-то время выбрасывать бочку. Добавить несколько бочек
void Enemy::path() {
    auto solid_state = level.GetObjects("solid");
    auto path = level.GetObjects("path");

    if (!f) {
        dx = 0.05;
        dy = 0.5;
    }
    for (int i = 0; i < path.size();++i) {
        if (getRect().intersects(path[i].rect)) {
            dx *= -1;
            f = true;
        }
    }
    for (int i = 0; i < solid_state.size(); ++i) {
        if (getRect().intersects(solid_state[i].rect)) {
            if (dy>0) {Rect.top = solid_state[i].rect.top - Rect.height+2; dy = 0; onGround = true;}
            if (dy<0) {Rect.top = solid_state[i].rect.top + solid_state[i].rect.height; dy = 0;}
        }
        else {onGround = false;}
    }
}

void Enemy::logic() {
    auto solid_state = level.GetObjects("solid");
    auto ladder = level.GetObjects("Ladder");


    for (int i = 0; i < solid_state.size(); ++i) {
        if (getRect().intersects(solid_state[i].rect) && (!onLadder or down))  {
            if (down) { dx = -0.05; onLadder = false; down = false;}

            if (dy>0) {Rect.top = solid_state[i].rect.top - Rect.height+2; dy = 0; onGround = true; onLadder = false;}
            if (dy<0) {Rect.top = solid_state[i].rect.top + solid_state[i].rect.height; dy = 0;}

            if (dx<0 && Rect.left <= solid_state[i].rect.left) { dx = 0.05;}
            //if (dx>0 && Rect.left >= solid_state[i].rect.left+solid_state[i].rect.width) { std::cout<<"!";dx = -0.05;}
            if (dx>0 && Rect.left >= 580) {dx = -0.05;}
        }
        else{onGround = false;}
    }

    for (int i = 0; i < ladder.size(); ++i) {
        if (getRect().intersects(ladder[i].rect)) {
            if (Rect.top >= ladder[i].rect.top && !down && dx >= 0) {Rect.left = ladder[i].rect.left;dy = -0.05; dx = 0; onLadder = true; onGround = false;}
            if (Rect.top <= ladder[i].rect.top) {dy = 0; down = true;}
            if (Rect.top < ladder[i].rect.top && down) { dy = 0.05;}
        }
    }

    auto objects = level.GetAllObjects();
}

void Enemy::jumping() {
    auto fall_pos = level.GetObject("fall");
    auto solid_state = level.GetObjects("solid");

    for (int i = 0; i < solid_state.size(); ++i) {
        if (getRect().intersects(solid_state[i].rect) && (!down)) {
            anim.set("spring_close");
        }
    }

    if (getRect().intersects(fall_pos.rect)) {
        dy = 0.5;
        down = true;
    }
}

void Enemy::simple_logic() {
    auto solid = level.GetObjects("solid");
    /*int id;
    for (int i = 0 ; i < solid.size(); ++i) {
        if (getRect().intersects(solid[i].rect)) {
            id = i;
        }

    }*/

    /*if (Rect.left <= solid[id].rect.left) {dx *= -1;}
    if (Rect.left + 32 >= solid[id].rect.left+solid[id].rect.width) {dx *= -1;}*/
    std::random_device rd;   // non-deterministic generator
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(2000,3700);
    //if (id == 66 && timer > 2000){dx *= -1; timer = 0;}
    if (timer >dist(gen) ) {dx *= -1; timer = 0;}
}

const std::string& Enemy::getName() {
    return name;
}