#include "Game.h"
Game::Game()
{
	gamestate = Gamestates::Main;
	font = makeFont();
	window = makeWindow();
	//vreau sa stiu unde pun textura si unde pun sprite-ul
	//acum primesc doar un index la un sprite
	//vreau sa primesc index la textura
	initTable();
	initTextures();
	initSprites();
	initButtons();
	initSounds();
}


void Game::render(){
	window->clear();

	switch (gamestate)
	{
	case Gamestates::Intro:
			window->draw(other_sprites[eIntroBg]);

		for (size_t i = 0; i < buttons.size(); i++)
		{
			drawInterractable(buttons[i], ePlayID);
			drawInterractable(buttons[i], eSettingsID);
			drawInterractable(buttons[i], eMediaID);

		}
		break;

	case Gamestates::Main:
			window->draw(other_sprites[eBg]);
			window->draw(other_sprites[eBgOverlay]);

			for (size_t i = 0; i < buttons.size(); i++)
			{
				drawInterractable(buttons[i], eGenerateID);
				drawInterractable(buttons[i], eSolveID);
				drawInterractable(buttons[i], eBackID);
			}
		break;

	case Gamestates::Solving:
			window->draw(other_sprites[eBg]);

			for (size_t i = 0; i < buttons.size(); i++)
			{
				drawInterractable(buttons[i], eGenerateID);
				drawInterractable(buttons[i], eSolveID);
				drawInterractable(buttons[i], eBackID);
			}
		break;

	case Gamestates::Generating:
		window->draw(other_sprites[eBg]);

		for (size_t i = 0; i < buttons.size(); i++)
		{
			drawInterractable(buttons[i], eGenerateID);
			drawInterractable(buttons[i], eSolveID);
			drawInterractable(buttons[i], eBackID);
		}
		break;

	case Gamestates::Settings:
			window->draw(other_sprites[eBg]);

			for (size_t i = 0; i < buttons.size(); i++)
			{

				drawInterractable(buttons[i], eBackID);
			}
		break;
		
	default: //DEBUG
		for (size_t i = 0; i < other_sprites.size(); i++)
		{
			other_sprites[i].setPosition(i * 20, i * 20);
			window->draw(other_sprites[i]);
		}
		for (size_t i = 0; i < buttons.size(); i++)
		{
			drawInterractable(buttons[i]);
		}
		break;
	}
	window->display();
}

void Game::drawInterractable(Button& btn)
{
	if (btn.shouldUpdate())btn.setTexture(interractable_textures);
	window->draw(btn.getRect());
}
void Game::drawInterractable(Button& btn, int ID)
{
	if (btn.getId() == ID)
	{
		if (btn.shouldUpdate())btn.setTexture(interractable_textures);
		if (btn.getNoise())
		{
			btnPressSound.play();
			btn.resetNoise();
		}
		window->draw(btn.getRect()); 
		window->draw(btn.getText());
	}
}
void Game::loop()
{
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			KeyboardEvent keyboardEvent;
			switch (event.type)
			{
			case sf::Event::MouseButtonPressed:
				mouseEvent.type = ButtonEventType::Pressed;

				break;
			case sf::Event::MouseButtonReleased:
				mouseEvent.type = ButtonEventType::Released;
				break;

			case sf::Event::MouseMoved:
				mouseEvent.type = ButtonEventType::Moved;
				mouseEvent.mousePos = sf::Mouse::getPosition(*window);
				break;
			default:
				mouseEvent.type = ButtonEventType::None;
			}
			mouseEventHandler(mouseEvent);
		}
		render();
	}
}

void Game::mouseEventHandler(MouseButtonEvent& ev)
{
	//got a custom event. we have to see what type it is, so we can see if there's any button that needs updated
	switch (ev.type)
	{
	case ButtonEventType::None:
		break;

	case ButtonEventType::Pressed:
		buttonMouseHandler(eClicked, ev.mousePos);
		break;

	case ButtonEventType::Moved:
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			buttonMouseHandler(eClicked, ev.mousePos);
			break;
		}
	case ButtonEventType::Released:
		buttonMouseHandler(eHovered, ev.mousePos);
		//make sound
		break;

	}
}

sf::Font Game::makeFont()
{
	sf::Font temp;
	temp.loadFromFile("Resources\\Fonts\\font.ttf");
	return temp;
}
void Game::handleSfmlEvent(sf::Event event)
{

}
void Game::buttonMouseHandler(int newButtonState, sf::Vector2i v2i)
{
	checkButtonColision(buttons, v2i, newButtonState);
}

sf::Texture Game::loadTexture(std::string PATH)
{
	sf::Texture texture;
	texture.loadFromFile("Resources\\Textures\\" + PATH);

	return texture;
}

void Game::initTextures()
{
	std::ifstream read;
	read.open("Resources\\Textures\\textures.txt");
	std::string x;
	//we flag the textures which we know are interractable, and we put those in interractable textures
	std::vector<std::pair<sf::Texture, bool>> flaggedTextures;

	while (getline(read, x))
		if (x[0] != '#')
		{
			if (x[0] == '!') // Flagged as interractable, add bool
			{
				x.erase(0, 1);
				flaggedTextures.push_back(std::make_pair(loadTexture(x + ".jpg"), true));
			}
			else //not flagged as interractable
			{
				flaggedTextures.push_back(std::make_pair(loadTexture(x + ".jpg"), false));
			}
		}
	for (int i = 0; i < flaggedTextures.size(); i++)
	{
		if (flaggedTextures[i].second == true) interractable_textures.push_back(flaggedTextures[i].first);
		else  other_textures.push_back(flaggedTextures[i].first);
	}
	read.close();

}

void Game::initSprites()
{
	for (size_t i = 0; i < other_textures.size(); i++)
	{
		sf::Sprite temp;
		temp.setTexture(other_textures[i]);
		other_sprites.push_back(temp);
	}
	prepareSprites();
}
void Game::prepareSprites()
{

	other_sprites[eBg].setScale(18.75, 18.75);
	other_sprites[eIntroBg].setScale(3.733333333, 4);
}
void Game::initButtons()
{
	std::pair<float, float> wide(150, 50);
	std::pair<float, float> normal(50, 50);
	
	makeButton("PLAY", font, 205 , 400, wide, ePlayID);
	makeButton("Generate", font, 145, 20, wide, eGenerateID);
	makeButton("Solve", font, 300, 20, wide, eSolveID);

	makeButton("", font, 90, 20, normal, eBackID);
	makeButton("", font, 205, 455, normal, eSettingsID);
	makeButton("", font, 305, 455, normal, eMediaID);

}

void  Game::checkButtonColision(std::vector<Button>& btns, sf::Vector2i mousepos, int newButtonState)
{
	for (size_t i = 0; i < btns.size(); i++)
	{
		if (btns[i].checkBounds(mousepos))
		{
			//we know the button contains the cursor. We are hovering/holding
			btns[i].resourcesHandler(newButtonState);
		}
		else
		{
			btns[i].resourcesHandler(0);
			//we are outside. we HAVE to set the texture to none
		}
	}
}

void Game::makeButton(std::string str, const sf::Font& font, float x, float y, std::pair<float, float> size, int id)
{

	Button btn(str, font, x, y, size.first, size.second, id);
	buttons.push_back(btn);
}

sf::RenderWindow* Game::makeWindow()
{
	//given initial values in case we can't open settings
	int vsync, height, width, frame_limit;
	//open settings and read parameters
	std::ifstream read;
	read.open("Resources\\settings.txt");

	//make a structure settings
	std::string x = "Mini Sudoku";
	if (read.is_open())
	{
		read >> width >> height;
		read >> frame_limit;
		read >> vsync;
		read >> x;
		read.close();
	}
	else
	{
		vsync = 0;
		height = 800,
			width = 600,
			frame_limit = 30;
	}
	sf::RenderWindow* tempwindow = new sf::RenderWindow(sf::VideoMode(width, height), x);
	tempwindow->setFramerateLimit(frame_limit);
	tempwindow->setVerticalSyncEnabled(vsync);
	return tempwindow;
}
void Game::initTable()
{
	sudoku = Table();
}
void Game::initSounds()
{
	music.openFromFile("Resources\\Sounds\\music.ogg");
	music.play();
	music.setLoop(true);
	music.setVolume(10);

	btnPressSoundBuffer.loadFromFile("Resources\\Sounds\\sound.ogg");
	btnPressSound.setBuffer(btnPressSoundBuffer);
	boxPressSound.setVolume(20);

	boxPressSoundBuffer.loadFromFile("Resources\\Sounds\\boxsound.wav");
	boxPressSound.setBuffer(boxPressSoundBuffer);

}