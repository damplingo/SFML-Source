#include "Jumpman.h"
#include <iostream>
#include <unistd.h>


Jumpman::Jumpman(sf::Texture Texture,  Level& level1, AnimationManager& _anim,sf::RenderWindow& window1, int record, int score):level(level1), anim(_anim), record(record), window(window1), score(score) {
    //Rect = sf::FloatRect (40, 535, 30, 50);
    Rect = level.GetObject("Jumpman").rect;
    dx=dy=0;
    STATE = stay;
    onGround = false;
    dir = false;
    hammer = false;
    bonus = 0;
    win = false;
}


void Jumpman::Keycheck() {
    ///если кнопка зажата

    /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && STATE != climb && onLadder) {
        STATE = jump;
        onLadder = false;
        dy = -0.18;
    }*/

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && STATE != climb && STATE != died && !hammer) {
        dir2 = 1;
        STATE = run;
        dx = -0.1;
        dir = true;
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && STATE == climb && onGround && STATE != died) {///вниз и влево после лестницы
        dir2 = 1;
        STATE = run;
        dx = -0.1;
        dir = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && STATE != climb && !hammer) {
        dir2 = 1;
        dir = false;
        STATE = run;
        dx = 0.1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && STATE == climb && onGround) {
        dir2 = 1;
        STATE = run;
        dx = 0.1;
        dir = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && onGround && !onLadder && !hammer) {
        STATE = jump;
        anim.play("jump");
        dy = -0.18;
        anim.play("jump");
        onGround = false;
    }
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !onGround && !onLadder) {
         STATE = jump;
         anim.play("jump");
     }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && onLadder && (STATE == stay or STATE == climb)) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {STATE = climb;dx = 0;}
        dir2 = 1;
        STATE = climb;
        onGround = false;
        dy = -0.05;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && onLadder) {
            dir2 = 0;
            STATE = climb;
            dy = 0.05;

    }

    if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))) {
        if (STATE == climb) {dy = 0;dx = 0;}
    }


}



void Jumpman::Animation(float time) {///не умирает если hammer поправить
    if (hammer) {anim.set("hammer"); anim.tick(time);return;}
    if (STATE == stay){
        anim.set("stay");
    }
    if (STATE == run) {
        anim.set("run");
    }
    if (STATE == jump) {anim.set("jump");}
    if (STATE == climb) {
        anim.set("climb");
        anim.pause();
        if (dy != 0) anim.play();
    }

    if (STATE == died) {anim.set("died"); anim.flip(!dir);anim.tick(time);
        return;}
    anim.flip(dir);
    anim.tick(time);
}



sf::FloatRect Jumpman::getRect() {
    return sf::FloatRect(Rect.left, Rect.top, Rect.width, Rect.height);
}

void Jumpman::update(float time) {
    Keycheck();
    Animation(time);
    if (STATE == climb){
        if (!onLadder) {
            STATE = stay;
            onGround = true;
        }
    }
    if (STATE != climb && !hammer) dy += 0.0005*time;
    onLadder = false;
    onGround = false;
    Rect.left += dx*time;
    Collision(0);
    Rect.top += dy*time;
    Collision(1);
    dx = 0;
    if (STATE != climb) STATE = stay;
}

void Jumpman::Collision(int num) {
    auto obj = level.GetAllObjects();
    for (int i = 0; i < obj.size(); ++i) {
        if (getRect().intersects(obj[i].rect)) {
            if (obj[i].name == "Ladder") {onLadder = true; if (STATE == climb) Rect.left = obj[i].rect.left/*onGround = false*/; if (obj[i].rect.height == 33 && STATE == climb) { dy = 0.01;}}
            if (obj[i].name == "EndLadder") {
                onLadder = true;
                if (dy > 0.04 && STATE == climb) Rect.left = obj[i].rect.left;//!=run
                if (Rect.top+Rect.height - obj[i].rect.top>=0 && Rect.top+Rect.height - obj[i].rect.top <= 1 && dir2){
                    dy = 0;
                    //dy = -0.02;
                    onGround = true;
                    STATE = stay;///***
                    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))&& sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                        dy = -0.18;
                    }

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) onLadder = false;
                    //onLadder = false;
                }

            }

            if (obj[i].name == "hammer" && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && STATE == stay) {
                hammer = true;
                CurrentHammerid = obj[i].id;
            }

            if (obj[i].name == "solid" && !hammer) {
                onLadder = false;
                if (dy>0 && num == 1) {Rect.top = obj[i].rect.top - 50; dy = 0; onGround = true;}
                if (dy<0 && num == 1) {Rect.top = obj[i].rect.top + obj[i].rect.height; dy = 0;}
                if (dx>0 && num == 0) {Rect.left = obj[i].rect.left - 50; }
                if (dx<0 && num == 0 ) {Rect.left = obj[i].rect.left + obj[i].rect.width; STATE = stay;}

            }

            if (obj[i].name == "solid" && (hammer || anim.getName() == "hammer")) {
                if (dy>0) {Rect.top = obj[i].rect.top - 70; dy = 0; onGround = true; }
                if (dy == 0) {Rect.top = obj[i].rect.top - 70; dy = 0; onGround = true; }
                //if (dy<0 && num == 1) {Rect.top = obj[i].rect.top + obj[i].rect.height; dy = 0; std::cout<<"@";}
            }

            if (obj[i].name == "bonus") {
                add_bonus(100);
                level.Delete_object(obj[i].id);
            }

        }
    }
}

void Jumpman::Collision_with_enemy(sf::FloatRect Enemy_rect) {
    sf::FloatRect Center_Enemy = sf::FloatRect(Enemy_rect.left,Enemy_rect.top, Enemy_rect.width, Enemy_rect.height);
    sf::FloatRect Center_Jumpman = sf::FloatRect(getRect().left+7, getRect().top, getRect().width-7, getRect().height/2);///накрутила, мб надо поправить
    if (Center_Jumpman.intersects(Center_Enemy) && !hammer) { STATE = died; dx = 0;}
}

void Jumpman::Collision_with_platform(sf::FloatRect platform_rect) {
    if (getRect().intersects(platform_rect)) {
        if (dy>0) {Rect.top = platform_rect.top - 50; dy = 0; onGround = true;}
        if (dy<0) {Rect.top = platform_rect.top + platform_rect.height; dy = 0;}
        if (dx>0) {Rect.left = platform_rect.left - 50; }
        if (dx<0) {Rect.left = platform_rect.left + platform_rect.width; STATE = stay;}
    }
}

bool Jumpman::HammerCheck() {
    sf::Clock clock1;

    if (start_hammer == 0 && hammer) {
        hammer_time = clock1;
        start_hammer = hammer_time.getElapsedTime().asSeconds();
        now_hammer = start_hammer;
        return true;
    }

    now_hammer = hammer_time.getElapsedTime().asSeconds();
    if ((now_hammer-start_hammer) >= 2) {
        hammer = false;
        dy = 0;
        start_hammer = 0;
        now_hammer = 0;
        return false;
    }

}

void Jumpman::add_bonus(int bonus) {
    this->bonus += bonus;
    score += bonus;
}

int Jumpman::getBonus() {
    return bonus;
}

/*int Jumpman::getRecord() {
    if (bonus > record) {
        record = bonus;
        return bonus;
    }
    else {
        return record;
    }
}*/

int Jumpman::getRecord() {
    if (score > record) {
        record = score;
    }
    return record;
}

bool Jumpman::Collision_with_grit(sf::FloatRect grit) {
    sf::FloatRect up_rect(grit.left + grit.width/2, grit.top - 20, grit.width/2, grit.height/2);
    if (getRect().intersects(up_rect)) {
        return true;
    }
    return false;
}

void Jumpman::Hammer_collision(std::list<Enemy> &enemy_list, float time) {
    for (auto it = enemy_list.begin(); it != enemy_list.end(); ++it) {
        it->update(time);
        it->anim.draw(window, it->getRect().left, it->getRect().top);

        if ((it->deleted && it->anim.getFrame() == 3 && it->anim.getName() == "barrel_crash") ||
        (it->deleted && it->anim.getFrame() == 2 && it->anim.getName() == "s_fire_crash")) {
            anim.pause();
            it->deleted = false;
            enemy_list.erase(it);
            add_bonus(500);
            anim.play();
        }
        if (!hammer) Collision_with_enemy(it->getRect());

        if (it->getRect().left < 0) enemy_list.pop_front();

        if (hammer) {
            if (getRect().intersects(it->getRect()) && anim.getFrame() == 1) {
                if (it->getName() == "barrel") {it->anim.set("barrel_crash");}
                else if (it->getName() == "s_fire") {
                    it->anim.set("s_fire_crash");
                }
                it->deleted = true;
            }
        }
    }
}

int& Jumpman::getScore() {
    return score;
}
