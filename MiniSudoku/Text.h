#pragma once
#include "SFML/Graphics.hpp"
class Text
{
	sf::Font font;

	int x = 0, y = 0;
public:
	sf::Text text;
	Text(sf::Font& tempfont,std::string &str, float x=0, float y=0);
};
