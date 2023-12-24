#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <map>
//#include<tinyxml/tinyxml.h>
#include "TinyXML/tinyxml.h"

struct Object {
    int GetPropertyInt(std::string name);
    float GetPropertyFloat(std::string name);
    std::string GetPropertyString(std::string name);

    std::string name;
    std::string type;
    sf::Rect<float> rect;
    std::map<std::string, std::string> properties;

    int id;///***
    sf::Sprite Sprite;
};

struct Layer {
    int opacity;
    std::vector<sf::Sprite> tiles;
};

class Level {
public:
    bool LoadFromFile(std::string filename);
    Object GetObject(std::string name);
    std::vector<Object> GetObjects(std::string name);
    void Draw(sf::RenderWindow &window);
    sf::Vector2i GetTileSize();
    std::vector<Object> GetAllObjects();
    void Delete_object(int id);

private:
    int width, height, tileWidth, tileHeight;
    int firstTileId;
    sf::Rect<float> drawingBounds;
    sf::Texture tilesetImage;
    std::vector<Object> objects;
    std::vector<Layer> layers;
};



