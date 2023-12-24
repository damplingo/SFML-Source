#include <SFML/Graphics.hpp>
#include <iostream>
#include "Jumpman.h"
#include "Animation.h"
#include "Level.h"
#include <memory>
#include "Enemy.h"
#include <list>
#include <unistd.h>
#include "GameMenu.h"
#include "Moving_platform.h"


///положение персонажа в пространстве устанавливается в методе drow

bool StartGame(int& numberLevel, sf::RenderWindow& window, int record, int score);

void win_func(Level& level, AnimationManager& Kong_Anim, sf::RenderWindow& window, bool win, Object& Kong, float& time, sf::Sprite& Paulina) {

    if (win) {
        Kong_Anim.set("Kong_krash");
        static float dy = 0.05;
        if (Kong_Anim.getFrame() == 2) {
            Kong_Anim.pause();
            auto platform = level.GetObject("Kong_platform");
            sf::RectangleShape black_rect;
            black_rect.setSize(sf::Vector2f(platform.rect.width, platform.rect.height));
            black_rect.setPosition(platform.rect.left, platform.rect.top);
            black_rect.setFillColor(sf::Color::Black);
            window.draw(black_rect);
            auto solid = level.GetObjects("solid");
            Kong.rect.top += dy * time;
            for (auto& i:solid) {
                if (Kong.rect.intersects(i.rect)) {
                    dy = 0;
                    time = 0;
                    auto paulina_2 = level.GetObject("Paulina_2");
                    Paulina.setPosition(sf::Vector2(paulina_2.rect.left, paulina_2.rect.top));

                    sf::Sprite heart;
                    sf::Image heart_i;
                    heart_i.loadFromFile("T/heart.png");
                    heart_i.createMaskFromColor(sf::Color(0,91,127));
                    sf::Texture heart_t;
                    heart_t.loadFromImage(heart_i);
                    heart.setTexture(heart_t);
                    heart.setPosition(level.GetObject("heart").rect.left, level.GetObject("heart").rect.top);
                    window.draw(heart);
                }
            }
        }
    }

}

void print_bonus(int bonus, sf::RenderWindow& window, int record, int score) {
    sf::Font font;
    font.loadFromFile("T/BungeeSpice-Regular.ttf");
    sf::Text text_bonus;
    text_bonus.setFont(font);
    text_bonus.setFillColor(sf::Color::White);
    text_bonus.setCharacterSize(20);
    text_bonus.setPosition(10,10);
    std::string str_2 = std::to_string(bonus);
    std::string str_1 = "bonus\n";

    sf::Text text_score;
    text_score.setFont(font);
    text_score.setFillColor(sf::Color::White);
    text_score.setCharacterSize(20);
    text_score.setPosition(550,10);
    std::string s_str_1 = "S\n";
    std::string s_str_2 = std::to_string(score);
    text_score.setString(s_str_1+s_str_2);

    sf::Text text_record;
    text_record.setFont(font);
    text_record.setFillColor(sf::Color::White);
    text_record.setCharacterSize(20);
    text_record.setPosition(100,10);
    std::string string_1 = "record\n";
    std::string string_2 = std::to_string(record);
    text_record.setString(string_1+string_2);
    text_bonus.setString(str_1+str_2);
    window.draw(text_score);
    window.draw(text_bonus);
    window.draw(text_record);
}

void changeLevel(Level &lvl,int &numberLevel) {
    if (numberLevel == 1) { lvl.LoadFromFile("Map/Level_1_ladder2_new_map_xml.tmx"); }
    if (numberLevel == 2) { lvl.LoadFromFile("Map/Level_2_map.tmx");}
    if (numberLevel == 3) { lvl.LoadFromFile("Map/Level_3_map.tmx");}
    //if (numberLevel>3) {numberLevel = 1;lvl.LoadFromFile("Level_1_ladder2_new_map_xml.tmx");}
}

void Play_Menu(int& numberLevel, sf::RenderWindow& window, int record, int score) {
    std::vector<sf::String> name_menu{L"Play", L"Exit"};
    GameMenu mymenu(window, 140, 80, 40, 100, name_menu);
    mymenu.setColorTextMenu(sf::Color::White, sf::Color::Red, sf::Color::Black);

    window.clear(sf::Color::Black);
    sf::Texture texture;
    texture.loadFromFile("T/loose_fone.jpeg");
    sf::Sprite sprite;
    sprite.setTexture(texture);
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Up) { mymenu.MoveUp();}
                if (event.key.code == sf::Keyboard::Down) { mymenu.MoveDown();}
                if (event.key.code == sf::Keyboard::Enter) {
                    switch (mymenu.getSelectedMenuNumber()) {
                        case 0:
                            StartGame(numberLevel, window, record, score);
                            break;
                        case 1: window.close();break;
                        default: break;
                    }
                }
            }


        }

        window.clear();
        window.draw(sprite);
        mymenu.draw();
        window.display();

    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(612, 700), "SFML Application");
    int numberLevel = 1;
    int record = 0;
    int score = 0;
    Play_Menu(numberLevel, window, record, score);
    return 0;
}

bool StartGame(int& numberLevel, sf::RenderWindow& window, int record, int score) {///добавление спрайта неплохо бы вынести в отдельную функцию
    Level level;
    changeLevel(level, numberLevel);
    if (numberLevel == 1) {
        score = 0;
    }
    sf::Texture jumpman_texture;
    sf::Image Image;
    Image.loadFromFile("T/im.png");
    Image.createMaskFromColor(sf::Color(112, 146, 190));
    jumpman_texture.loadFromImage(Image);
    auto win = level.GetObject("win");
    sf::Clock clock;
    sf::Clock clock_2;


    ///Jumpman set
    AnimationManager Anim;
    Anim.create("run", Animation(jumpman_texture, 59, 14, 30, 50, 4, 0.01, 30));
    Anim.create("stay", Animation(jumpman_texture, 29, 14, 30, 50, 1, 0, 0));
    Anim.create("jump", Animation(jumpman_texture, 185, 15, 30, 47, 1, 0, 0));
    Anim.create("climb", Animation(jumpman_texture, 182, 185, 30, 47, 3, 0.01, 30));
    Anim.create("died", Animation(jumpman_texture, 515, 244, 40, 50, 1, 0, 0));
    Anim.create("hammer", Animation(jumpman_texture, 445, 171, 53, 67, 2, 0.005, 53));
    Jumpman Jumpman(jumpman_texture, level, Anim, window,record, score);

    ///Kong set
    sf::Image Kong_Image;
    Kong_Image.loadFromFile("T/Kong_details.png");
    Kong_Image.createMaskFromColor(sf::Color(0, 91, 127));
    sf::Texture Kong_texture;
    Kong_texture.loadFromImage(Kong_Image);
    AnimationManager Kong_Anim;
    sf::Image Kong_i;
    Kong_i.loadFromFile("T/Kong_krash.png");
    Kong_i.createMaskFromColor(sf::Color(0, 91, 127));
    sf::Texture Kong_t;
    Kong_t.loadFromImage(Kong_i);
    Kong_Anim.create("Kong_krash", Animation(Kong_t, 0, 0, 88, 70, 3, 0.005, 88));
    Kong_Anim.create("Kong", Animation(Kong_texture, 0, 0, 104, 70, 4, 0.005, 104));
    Object Kong = level.GetObject("Kong");

    ///Paulina set

    sf::Image Paulina_Image;
    Paulina_Image.loadFromFile("T/Paulina.png");
    Paulina_Image.createMaskFromColor(sf::Color(0, 91, 127));
    sf::Texture Paulina_Texture;
    Paulina_Texture.loadFromImage(Paulina_Image);
    Object Paulina = level.GetObject("Paulina");
    sf::Sprite Paulina_Sprite;
    Paulina_Sprite.setTexture(Paulina_Texture);
    Paulina_Sprite.setPosition(Paulina.rect.left, Paulina.rect.top);

    ///bonus_set
    sf::Image bonus_image;
    bonus_image.loadFromFile("T/bonus.png");
    bonus_image.createMaskFromColor(sf::Color(0, 91, 127));
    sf::Texture bonus_texture;
    bonus_texture.loadFromImage(bonus_image);
    sf::Sprite bonus_sprite;
    bonus_sprite.setTexture(bonus_texture);

    ///barrel set

    AnimationManager barrel_anim;
    sf::Image Barrel;
    Barrel.loadFromFile("T/barrel.png");
    Barrel.createMaskFromColor(sf::Color(0, 105, 105));

    sf::Image Barrel_crash;
    Barrel_crash.loadFromFile("T/barrel_crash.png");
    Barrel_crash.createMaskFromColor(sf::Color(0, 105, 105));

    sf::Texture Barrel_texture;
    sf::Texture Barrel_crash_texture;
    Barrel_crash_texture.loadFromImage(Barrel_crash);
    Barrel_texture.loadFromImage(Barrel);

    barrel_anim.create("barrel", Animation(Barrel_texture, 0, 0, 32, 28, 4, 0.002, 32));
    barrel_anim.create("barrel_crash", Animation(Barrel_crash_texture, 0, 0, 32, 28, 4, 0.003, 32));
    barrel_anim.set("barrel");

    Enemy barrel_enemy(barrel_anim, "barrel", level);
    barrel_anim.set("barrel");
    std::list<Enemy> barrel_list;
    sf::Sprite Barrel_heap_sprite;
    sf::Texture Barrel_heap_texture;
    sf::Image Barrel_heap_image;

    if (numberLevel == 1) {

        Barrel_heap_image.loadFromFile("T/barrel_heap.png");
        Barrel_heap_image.createMaskFromColor(sf::Color(0, 105, 105));
        Barrel_heap_texture.loadFromImage(Barrel_heap_image);

        Barrel_heap_sprite.setTexture(Barrel_heap_texture);
        Barrel_heap_sprite.setPosition(level.GetObject("barrel_heap").rect.left,
                                       level.GetObject("barrel_heap").rect.top);
    }
    ///set spring
    sf::Image spring_image;
    AnimationManager spring_animation;
    spring_image.loadFromFile("T/spring.png");
    spring_image.createMaskFromColor(sf::Color(0, 105, 105));
    sf::Texture spring_texture;
    spring_texture.loadFromImage(spring_image);
    std::string str_1 = "spring_open";
    std::string str_2 = "spring_close";
    spring_animation.create(str_1, Animation(spring_texture, 0,0,28, 32, 1, 0, 0));
    spring_animation.create(str_2, Animation(spring_texture, 28,0,28, 32, 1, 0, 0));
    Enemy spring_enemy(spring_animation, "spring", level);
    std::list<Enemy> spring_list;
    int l = 1;///last time of barrel or spring

    ///hammer
    sf::Texture hammer_texture;
    sf::Image hammer_image;
    hammer_image.loadFromFile("T/hammer_2.png");
    hammer_image.createMaskFromColor(sf::Color(112, 146, 190));
    hammer_texture.loadFromImage(hammer_image);
    std::map<int, sf::Sprite> hammers;

    auto hammer_coord = level.GetObjects("hammer");

    for (int i = 0; i < hammer_coord.size(); ++i) {
        sf::Sprite Sprite(hammer_texture);
        Sprite.setPosition(hammer_coord[i].rect.left, hammer_coord[i].rect.top);
        hammers.insert({hammer_coord[i].id, Sprite});
    }


    ///fire

    sf::Image fire_image;
    sf::Texture fire_texture;
    fire_image.loadFromFile("T/fire.png");
    fire_image.createMaskFromColor(sf::Color(0, 105, 105));
    fire_texture.loadFromImage(fire_image);
    AnimationManager fire_anim;
    fire_anim.create("fire", Animation(fire_texture, 0, 0, 27, 30, 2, 0.01, 27));
    std::list<Enemy> fire_list;
    while (level.GetObjects("fire").size() != 0) {
        fire_list.push_back(Enemy(fire_anim, "fire", level));
    }

    sf::Texture s_fire_texture;
    s_fire_texture.loadFromFile("T/s_fire.png");
    sf::Texture s_fire_crash_texture;
    s_fire_crash_texture.loadFromFile("T/s_fire_crash.png");
    AnimationManager s_fire_anim;
    s_fire_anim.create("s_fire", Animation(s_fire_texture, 0,0,37.5, 33, 2, 0.01, 37.5));
    s_fire_anim.create("s_fire_crash", Animation(s_fire_crash_texture, 5, 0, 40, 32, 3, 0.005, 40));
    s_fire_anim.set("s_fire");
    while (level.GetObjects("s_fire").size()!=0) {
        fire_list.push_back(Enemy(s_fire_anim, "s_fire", level));
    }

    sf::Image platform_image;
    sf::Texture platform_texture;
    platform_image.loadFromFile("T/Moving_platform.png");
    platform_image.createMaskFromColor(sf::Color::White);
    platform_texture.loadFromImage(platform_image);
    AnimationManager platform_anim;
    platform_anim.create("Moving_platform", Animation(platform_texture, 0,0,64, 31, 1, 0, 64));

    std::vector<Moving_platform> moving_platforms;
    while (level.GetObjects("Moving_platform").size() != 0) {
        moving_platforms.push_back(Moving_platform(level,platform_anim));
    }

    ///grit
    sf::Sprite grit_sprite;
    sf::Texture grit_texture;
    grit_texture.loadFromFile("T/grit.png");
    grit_sprite.setTexture(grit_texture);
    std::vector<sf::Sprite> grit_vector;


    bool pause = false;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asMicroseconds();

        int time_all = clock_2.getElapsedTime().asSeconds();///barrel circle

        if (time_all % 10 == 0 && l != time_all && numberLevel == 1) {
            l = time_all;
            barrel_list.push_back((barrel_enemy));
        }

        if (time_all % 5 == 0 && l != time_all && numberLevel == 2) {
            l = time_all;
            spring_list.push_back(spring_enemy);
            std::cout << spring_list.size() << " ";
        }
        auto get_grit = level.GetObjects("grit");
        clock.restart();
        if (pause) { time = 0; }
        time /= 800;
        sf::Event event;
        Jumpman.anim.set("stay");

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::G) {
                    if (pause) pause = false;
                    else if (!pause) pause = true;
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    Jumpman.getScore()-=Jumpman.getBonus();
                    Play_Menu(numberLevel, window, Jumpman.getRecord(), Jumpman.getScore());
                }
            }
        }

        Jumpman.anim.set("stay");
        window.clear(sf::Color::Black);
        level.Draw(window);
        win_func(level, Kong_Anim, window, Jumpman.win, Kong, time, Paulina_Sprite);
        Jumpman.update(time);

        ///HammerCheck(пока не красиво - подумать как переделать
        if (Jumpman.hammer) {
            if (Jumpman.HammerCheck()) {
                hammers.erase(Jumpman.CurrentHammerid);
                level.Delete_object(Jumpman.CurrentHammerid);
            }

        }

        Kong_Anim.tick(time);
        if (numberLevel != 3) {
            for (auto& it:fire_list) {
                it.update(time);
                it.anim.draw(window, it.getRect().left, it.getRect().top);
                Jumpman.Collision_with_enemy(it.getRect());
            }
        }
        else {
            Jumpman.Hammer_collision(fire_list, time);
        }

        for (auto &i: moving_platforms) {
            i.update(time);
            Jumpman.Collision_with_platform(i.getRect());
            i.Anim.draw(window, i.getRect().left, i.getRect().top);
        }

        for (auto &j:get_grit) {
            grit_sprite.setPosition(j.rect.left, j.rect.top);
            window.draw(grit_sprite);
            if (Jumpman.Collision_with_grit(j.rect)) {level.Delete_object(j.id);}
        }

        Jumpman.anim.draw(window, Jumpman.Rect.left, Jumpman.Rect.top);
        Kong_Anim.draw(window, Kong.rect.left, Kong.rect.top);
        window.draw(Paulina_Sprite);
        window.draw(Barrel_heap_sprite);
        for (int i = 0; i < level.GetObjects("bonus").size(); ++i) {
            bonus_sprite.setPosition(level.GetObjects("bonus")[i].rect.left,level.GetObjects("bonus")[i].rect.top);
            window.draw(bonus_sprite);
        }

        for (auto &i: hammers) {
            window.draw(i.second);
        }

        for (auto &i: spring_list) {
            i.update(time);
            i.anim.draw(window, i.getRect().left, i.getRect().top);
            Jumpman.Collision_with_enemy(i.getRect());
            if (i.getRect().top > window.getSize().y) spring_list.pop_front();
        }


        ///проверка столкновения с бочками

        Jumpman.Hammer_collision(barrel_list,time);

        print_bonus(Jumpman.getBonus(), window, Jumpman.getRecord(), Jumpman.getScore());
        window.display();
        if (Jumpman.getRect().intersects(win.rect) and Jumpman.anim.CurrentAnimation == "stay") {/*return true;*/;
            if (numberLevel == 3 && get_grit.size() == 0) {
                //return true;
                //win_func(level, Kong_Anim, window);
                Jumpman.win = true;
            }

            else if (numberLevel != 3){
                numberLevel++;
                StartGame(numberLevel, window, Jumpman.getRecord(), Jumpman.getScore());
            }
        }///добавить окно выигрыша
        if (Jumpman.anim.CurrentAnimation == "died") { sleep(2);Jumpman.getScore()-=Jumpman.getBonus();Play_Menu(numberLevel, window, Jumpman.getRecord(), Jumpman.getScore());}
        if (!window.isOpen()) {return false;}
    }
    return true;
}
