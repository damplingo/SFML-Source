#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class GameMenu {
private:
    float menu_X;
    float menu_Y;
    int menu_Step;
    int max_menu;
    int size_font;
    int mainMenuSelected;
    sf::Font font;
    std::vector<sf::Text> mainMenu;

    sf::Color menu_text_color = sf::Color::White;
    sf::Color chose_text_color = sf::Color::Yellow;
    sf::Color border_color = sf::Color::Black;

    void setInitText(sf::Text& text, const sf::String& str, float xpos, float ypos) const;

public:
    sf::RenderWindow& mywindow;
    GameMenu(sf::RenderWindow& window, float menux, float menuy, int sizeFont, int step, std::vector<sf::String>& name);
    void draw();
    void MoveUp();
    void MoveDown();
    void setColorTextMenu(sf::Color menColor, sf::Color ChoColor, sf::Color BordColor);
    int getSelectedMenuNumber() const{
        return mainMenuSelected;
    }
};


