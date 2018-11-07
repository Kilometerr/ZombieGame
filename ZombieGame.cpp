#include "stdafx.h"
#include "SFML\Graphics.hpp"
#include "player.h"

using namespace sf;

int main()
{
	//States of the game
	enum class State
	{
		Paused, Leveling_UP, Game_Over, Playing
	};

	State state = State::Game_Over;

	//Get screen res and ini vid mode
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	//Render window and set view
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie", Style::Fullscreen);
	View mainView(FloatRect(0, 0, resolution.x, resolution.y));

	Clock clock;

	Time gameTimeTotal; //time of Playing state

	Vector2f mouseWorldPosition;
	Vector2f mouseScreenPosition;

	Player player;

	IntRect arena;

	//Main loop
	while (window.isOpen())
	{

	}

    return 0;
}

