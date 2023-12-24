#include "GameMenu.h"
#include <vector>
#include <SFML/Graphics.hpp>

void GameMenu::setInitText(sf::Text& text, const sf::String& str, float xpos, float ypos) const
{
    text.setFont(font);
    text.setFillColor(menu_text_color);
    text.setString(str);
    text.setCharacterSize(size_font);
    text.setPosition(xpos, ypos);
    text.setOutlineColor(border_color);
}

GameMenu::GameMenu(sf::RenderWindow &window, float  menux, float menuy, int sizeFont, int step,
                   std::vector<sf::String>& name):menu_X(menux), menu_Y(menuy), menu_Step(step), max_menu(static_cast<int>(name.size())),
                   size_font(sizeFont), mainMenu(name.size()), mywindow(window) {

    font.loadFromFile("T/BungeeSpice-Regular.ttf");
    for (int i = 0, ypos = static_cast<int>(menu_Y); i < max_menu; i++, ypos += menu_Step) {
        setInitText(mainMenu[i], name[i], menu_X, static_cast<float>(ypos));
    }

    mainMenuSelected = 0;
    mainMenu[mainMenuSelected].setFillColor(chose_text_color);
}

void GameMenu::MoveUp()
{
    mainMenuSelected--;

    if (mainMenuSelected >= 0) {
        mainMenu[mainMenuSelected].setFillColor(chose_text_color);
        mainMenu[mainMenuSelected + 1].setFillColor(menu_text_color);
    }
    else
    {
        mainMenu[0].setFillColor(menu_text_color);
        mainMenuSelected = max_menu - 1;
        mainMenu[mainMenuSelected].setFillColor(chose_text_color);
    }
}

void GameMenu::MoveDown()
{
    mainMenuSelected++;

    if (mainMenuSelected < max_menu) {
        mainMenu[mainMenuSelected - 1].setFillColor(menu_text_color);
        mainMenu[mainMenuSelected].setFillColor(chose_text_color);
    }
    else
    {
        mainMenu[max_menu - 1].setFillColor(menu_text_color);
        mainMenuSelected = 0;
        mainMenu[mainMenuSelected].setFillColor(chose_text_color);
    }

}
void GameMenu::draw()
{
    for (int i = 0; i < max_menu; i++) mywindow.draw(mainMenu[i]);
}

void GameMenu::setColorTextMenu(sf::Color menColor, sf::Color ChoColor, sf::Color BordColor)
{
    menu_text_color = menColor;
    chose_text_color = ChoColor;
    border_color = BordColor;

    for (int i = 0; i < max_menu; i++) {
        mainMenu[i].setFillColor(menu_text_color);
        mainMenu[i].setOutlineColor(border_color);
    }

    mainMenu[mainMenuSelected].setFillColor(chose_text_color);
}