#include "stdafx.h"
#include "SFML\Graphics.hpp"
#include "player.h"
#include "Arena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"
#include <sstream>
#include <fstream>
#include <iostream>

using namespace sf;

int main()
{
	TextureHolder holder;

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
	Vector2i mouseScreenPosition;

	Player player;

	IntRect arena;

	//Background load texture etc
	VertexArray background;
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

	//Prepare horde of zombies
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	//Bullets
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	Time lastPressed;

	//Mouse pointer = Crosshair
	window.setMouseCursorVisible(true);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	//CreatePickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);
	
	//*HUD*
	//scores
	int score = 0;
	int hiscore = 0;

	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	//create view for hud
	View hudView(FloatRect(0, 0, resolution.x, resolution.y));

	//Ammo hud
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);

	//Load the font
	Font font;
	font.loadFromFile("fonts/Zenzai_Itacha.ttf");

	//Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Nacisnij enter \n aby kontynuowac");

	//Game over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Nacisnij enter aby zagrac!");

	//Leveling up
	Text leveUpText;
	leveUpText.setFont(font);
	leveUpText.setCharacterSize(50);
	leveUpText.setFillColor(Color::White);
	leveUpText.setPosition(150, 250);
	stringstream levelUpStream;
	levelUpStream <<
		"\n1- Zwiekszenie szybkosci strzelania" <<
		"\n2- Zwiekszenie pojemnosci magazynka"
		"\n3- Zwieksznie max HP" <<
		"\n4- Zwiekszenie szybkosci poruszania" <<
		"\n5- Zwiekszenie ilosci oraz jakosci apteczek" <<
		"\n6- Zwiekszenie ilosci oraz jakosci paczek amunicji";
	leveUpText.setString(levelUpStream.str());

	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	//Load hiscore from txt file
	ifstream inputFile("GameData/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiscore;
		inputFile.close();	}

	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "HighScore:" << hiscore;
	hiScoreText.setString(s.str());

	// Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);

	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Fala: 0");

	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);

	int framesSinceLastHudUpdate = 0; //when
	int fpsMeasurementFrameInterval = 1000; //how often

	//Main loop
	while (window.isOpen())
	{
		
		Event event;
		
		//EVENT POLLING
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				//Pause & restart game
				if (event.key.code == Keyboard::Return && state == State::Playing)
				{
					state = State::Paused;
				}

				else if (event.key.code == Keyboard::Return && state == State::Paused)
				{
					state = State::Playing;
					clock.restart();//reset to avoid frame jump
				}
				
				//Restart game at game over
				else if (event.key.code == Keyboard::Return && state == State::Game_Over)
				{
					
					state = State::Leveling_UP;
					//prepare next game
					wave = 0;
					score = 0;
					currentBullet = 0;
					bulletSpare = 96;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;
					player.resetPlayerStats();
				}
				
				if (state == State::Playing)
				{
					//Reloading
					if (event.key.code == Keyboard::R)
					{
						if (bulletSpare >= clipSize)
						{
							bulletsInClip = clipSize;
							bulletSpare -= clipSize;
						}
						else if (bulletSpare > 0)
						{
							bulletsInClip = bulletSpare;
							bulletSpare = 0;
						}
						else
						{

						}
					}
				}
			}
		}
		// STOP EVENT POLLING

		//MOVEMENT INPUT
		
		//Player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		//Handle movement
		if (state == State::Playing)
		{
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}
			if (Mouse::isButtonPressed(::Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
				{
					//pass center of player and crosshair
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
					currentBullet++;
					if (currentBullet > 99)
					{
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					bulletsInClip--;
				}
			}

		}

		//END MOVEMENT INPUT

		//HANDLE LEVELING AND LEVEL
		if (state == State::Leveling_UP)
		{
			if (event.key.code == Keyboard::Num1)
			{
				fireRate++;
				state = State::Playing;
			}
			if (event.key.code == Keyboard::Num2)
			{
				clipSize += clipSize;
				state = State::Playing;
			}
			if (event.key.code == Keyboard::Num3)
			{
				player.upgradeHealth();
				state = State::Playing;
			}
			if (event.key.code == Keyboard::Num4)
			{
				player.upgradeSpeed();
				state = State::Playing;
			}
			if (event.key.code == Keyboard::Num5)
			{
				healthPickup.upgrade();
				state = State::Playing;
			}
			if (event.key.code == Keyboard::Num6)
			{
				ammoPickup.upgrade();
				state = State::Playing;
			}
			if (state == State::Playing)
			{
				wave++;
				//prepare level
				arena.width = 500*wave;
				arena.height = 500*wave;
				arena.left = 0;
				arena.top = 0;

				int tileSize = createBackground(background, arena);

				//spawn player
				player.spawn(arena, resolution, tileSize);

				//configure pickups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				//create horde
				numZombies = 5*wave;
				delete[]zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				clock.restart();
			}
		}
		//END LEVELING AND LEVEL HANDLE

		//UPDATE SCENE
		if (state == State::Playing)
		{
			//time 
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();
			
			//mouse
			mouseScreenPosition = Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			//crosshair
			spriteCrosshair.setPosition(mouseWorldPosition);

			//player
			player.Update(dtAsSeconds, Mouse::getPosition());
			Vector2f playerPosition(player.getCenter());//mark new location and set mainview
			mainView.setCenter(player.getCenter());

			//update zombies
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].Update(dt.asSeconds(), playerPosition);
				}
			}

			//update bullets
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}

			//update pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);


			//*Collision Detection*
			
			//is zombie shot?
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].isInFlight() && zombies[j].isAlive())
					{
						if (bullets[i].getPosition().intersects
							(zombies[j].getPosition()))
						{
							//stop the bullet
							bullets[i].stop();

							//register hit, check for kill
							if (zombies[j].hit())
							{
								score += 10;
								if (score >= hiscore)
								{
									hiscore = score;
								}
								numZombiesAlive--;

								if (numZombiesAlive == 0)
								{
									state = State::Leveling_UP;
								}
							}
						}
					}
				}
			}

			//is zombie touching player
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive())
				{
					if (player.hit(gameTimeTotal))
					{
						//todo
					}
					if (player.getHealth() <= 0)
					{
						state = State::Game_Over;
						ofstream outputFile("GameData/scores.txt");
						outputFile << hiscore;
						outputFile.close();
					}
				}
			}

			//is pickupmtouching player
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletSpare += ammoPickup.gotIt();
			}

			//*END of collision detection

			//HUD update
			healthBar.setSize(Vector2f(player.getHealth() * 3,50));
			framesSinceLastHudUpdate++;

			if (framesSinceLastHudUpdate > fpsMeasurementFrameInterval)
			{
				stringstream ssAmmo;
				stringstream ssScore;
				stringstream ssHiScore;
				stringstream ssWave;
				stringstream ssZombiesAlive;

				//update ammo text
				ssAmmo << bulletsInClip << "|" << bulletSpare;
				ammoText.setString(ssAmmo.str());

				// Update score text
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());

				// Update  high score text
				ssHiScore << "Hi Score:" << hiscore;
				hiScoreText.setString(ssHiScore.str());

				// Update  wave text
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());

				// Update high score text
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				framesSinceLastHudUpdate = 0;
			}

			//*END of HUD update
		}
		//END SCENE UPDATE

		//DRAW SCENE
		if (state == State::Playing)
		{
			//cout << "PLAYING!";//debug
			window.clear();

			//camera
			window.setView(mainView);

			//background
			window.draw(background, &textureBackground);

			//zombies
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}

			//bullets
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}

			//pickups
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}
			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}

			//player
			window.draw(player.getSprite());

			//crosshair
			window.draw(spriteCrosshair);

			// Switch to the HUD view
			window.setView(hudView);

			// Draw all the HUD elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);

		}
		if (state == State::Leveling_UP)
		{
			window.draw(spriteGameOver);
			window.draw(leveUpText);
		}
		if (state == State::Paused)
		{
			window.draw(pausedText);
		}
		if (state == State::Game_Over)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}
		window.display();
		//END SCENE DRAW
	}

	delete[] zombies;

    return 0;
}

