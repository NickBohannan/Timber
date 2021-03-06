// HelloSFML.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <sstream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Audio.hpp>

// make code easier to type with "using namespace"
using namespace sf;

//function declaration
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

//where is the player/branch?
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
	//create a video mode object (videomode is a class, vm is an object of the class)
	VideoMode vm(1920, 1080);

	//create and open a window for the game (RenderWindow is a class, window is the object)
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
	
	//create a texture to hold a graphic on the GPU
	Texture textureBackground;
	
	//load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");

	//easter egg sound file
	SoundBuffer notTheBees;
	notTheBees.loadFromFile("sound/NOT THE BEES short.wav");
	Sound sound;
	sound.setBuffer(notTheBees);

	//create a sprite
	Sprite spriteBackground;

	//attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);
	
	//set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	// Make a tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	// Prepeare the bees!!!!!!!!!!!!!!!!!!!!
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	Sprite spriteBee2;
	Sprite spriteBee3;
	Sprite spriteBee4;
	Sprite spriteBee5;
	Sprite spriteBee6;
	spriteBee.setTexture(textureBee);
	spriteBee2.setTexture(textureBee);
	spriteBee3.setTexture(textureBee);
	spriteBee4.setTexture(textureBee);
	spriteBee5.setTexture(textureBee);
	spriteBee6.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	spriteBee2.setPosition(1920, 700);
	spriteBee3.setPosition(1920, 600);
	spriteBee4.setPosition(1920, 500);
	spriteBee5.setPosition(1920, 400);
	spriteBee6.setPosition(1920, 300);

	//easter egg bees moving?
	bool bee2Active = false;
	bool bee3Active = false;
	bool bee4Active = false;
	bool bee5Active = false;
	bool bee6Active = false;

	//how fast is each easter egg bee?
	float bee2Speed = 0.0f;
	float bee3Speed = 0.0f;
	float bee4Speed = 0.0f;
	float bee5Speed = 0.0f;
	float bee6Speed = 0.0f;

	// Is the bee currently moving?
	bool beeActive = false;

	// How fast can the bee fly?
	float beeSpeed = 0.0f;

	// Make three cloud sprites from one texture
	Texture textureCloud;

	// Load one new texture
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);

	//position the clouds offscreen
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);

	// Are the clouds currently on screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	// how fast is each cloud?
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	// variables to control time itself
	Clock clock;

	// time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	//Track whether the game is running
	bool paused = true;

	//is it easter egg time?
	bool egg1 = false;

	//draw some text
	int score = 0;
	Text messageText;
	Text scoreText;
	Text boheezyText;

	//we need to choose a font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	//set the font to our message
	messageText.setFont(font);
	scoreText.setFont(font);
	boheezyText.setFont(font);

	//assign the actual message
	messageText.setString("Press Enter to Start!");
	scoreText.setString("Score = 0");
	boheezyText.setString("Chop! with Boheezy");

	//make it really big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(75);
	boheezyText.setCharacterSize(50);

	//choose a color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	boheezyText.setFillColor(Color::White);

	//position the text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);
	FloatRect textRectBo = boheezyText.getLocalBounds();
	boheezyText.setOrigin(textRectBo.left +
		textRectBo.width / 2.0f,
		textRectBo.top +
		textRectBo.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);
	boheezyText.setPosition(1920 / 2.0f, 30);

	//prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	//set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		//set the sprite's origin to dead center
		// we can then spin it round without changing its position
		branches[i].setOrigin(220, 20);
	}

	// prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	// the player starts on the left
	side playerSide = side::LEFT;

	// prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	// prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	// line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	// some other useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// control the player input
	bool acceptInput = false;

	while (window.isOpen())
	{
		//handle the players input
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				// listen for key presses again
				acceptInput = true;

				// hide the axe
				spriteAxe.setPosition(2000,
					spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::B))
		{
			egg1 = true;
			sound.play();
		}
		if (egg1)
		{
			Time dt = clock.restart();

			if (!bee2Active)
			{
				// how fast is the bee
				srand((int)time(0) * 10);
				bee2Speed = (rand() % 200 + 500);

				// how high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 200;
				spriteBee2.setPosition(1300, height);
				bee2Active = true;
			}
			else
			{
				spriteBee2.setPosition(
					spriteBee2.getPosition().x -
					(bee2Speed * dt.asSeconds()),
					spriteBee2.getPosition().y);

				// has the bee reached the left hand edge of the screen?
				if (spriteBee2.getPosition().x < 700)
				{
					// set it up ready to be a whole new bee next frame
					bee2Active = false;
				}
			}
			if (!bee3Active)
			{
				// how fast is the bee
				srand((int)time(0) * 10);
				bee3Speed = (rand() % 200 + 500);

				// how high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 300;
				spriteBee3.setPosition(1200, height);
				bee3Active = true;
			}
			else
			{
				spriteBee3.setPosition(
					spriteBee3.getPosition().x -
					(bee3Speed * dt.asSeconds()),
					spriteBee3.getPosition().y);

				// has the bee reached the left hand edge of the screen?
				if (spriteBee3.getPosition().x < 700)
				{
					// set it up ready to be a whole new bee next frame
					bee3Active = false;
				}
			}
			if (!bee4Active)
			{
				// how fast is the bee
				srand((int)time(0) * 10);
				bee4Speed = (rand() % 200 + 500);

				// how high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 400;
				spriteBee4.setPosition(1300, height);
				bee4Active = true;
			}
			else
			{
				spriteBee4.setPosition(
					spriteBee4.getPosition().x -
					(bee4Speed * dt.asSeconds()),
					spriteBee4.getPosition().y);

				// has the bee reached the left hand edge of the screen?
				if (spriteBee4.getPosition().x < 700)
				{
					// set it up ready to be a whole new bee next frame
					bee4Active = false;
				}
			}
			if (!bee5Active)
			{
				// how fast is the bee
				srand((int)time(0) * 10);
				bee5Speed = (rand() % 200 + 500);

				// how high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee5.setPosition(1200, height);
				bee5Active = true;
			}
			else
			{
				spriteBee5.setPosition(
					spriteBee5.getPosition().x -
					(bee5Speed * dt.asSeconds()),
					spriteBee5.getPosition().y);

				// has the bee reached the left hand edge of the screen?
				if (spriteBee5.getPosition().x < 700)
				{
					// set it up ready to be a whole new bee next frame
					bee5Active = false;
				}
			}
			if (!bee6Active)
			{
				// how fast is the bee
				srand((int)time(0) * 10);
				bee6Speed = (rand() % 200 + 500);

				// how high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 600;
				spriteBee6.setPosition(1300, height);
				bee6Active = true;
			}
			else
			{
				spriteBee6.setPosition(
					spriteBee6.getPosition().x -
					(bee6Speed * dt.asSeconds()),
					spriteBee6.getPosition().y);

				// has the bee reached the left hand edge of the screen?
				if (spriteBee6.getPosition().x < 700)
				{
					// set it up ready to be a whole new bee next frame
					bee6Active = false;
				}
			}
		}

		//start the game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			//reset the time and score
			score = 0;
			timeRemaining = 5;

			// make all of the branches disappear
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			// make sure the gravestone is hidden
			spriteRIP.setPosition(675, 2000);

			// move the player into position
			spritePlayer.setPosition(580, 720);
			acceptInput = true;
		}

		// wrap the player controls to make sure we are accepting input
		if (acceptInput)
		{
			// first handle pressing the right cursor key
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				// make sure the player is on the right (in regards to the enumerator)
				playerSide = side::RIGHT;
				score++;

				// add to the amount of time remaining
				timeRemaining += (2 / score) + .15;

				// axe position right
				spriteAxe.setPosition(AXE_POSITION_RIGHT,
					spriteAxe.getPosition().y);

				// actually make the player graphic be on the right
				spritePlayer.setPosition(1200, 720);

				// update the branches
				updateBranches(score);

				// set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;
			}
			// handle the left cursor key
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				// make sure the player is on the lizzeft (in regards to the enumerator)
				playerSide = side::LEFT;
				score++;

				// add to the amount of time remaining
				timeRemaining += (2 / score) + .15;

				// axe position right
				spriteAxe.setPosition(AXE_POSITION_LEFT,
					spriteAxe.getPosition().y);

				// actually make the player graphic be on the right
				spritePlayer.setPosition(580, 720);

				// update the branches
				updateBranches(score);

				// set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
			}
		}

		//Update the scene
		if (!paused)
		{

			// measure time (object dt is delta time, 
			Time dt = clock.restart();

			//subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			
			//size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond *
				timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				//pause the game
				paused = true;

				//change the message shown to the player
				messageText.setString("You ran out of time, ya jabroni!");

				//reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}

			// set up the bee
			if (!beeActive)
			{
				// how fast is the bee
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200);

				// how high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else // move the bee
			{
				spriteBee.setPosition(
					spriteBee.getPosition().x -
					(beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y);

				// has the bee reached the left hand edge of the screen?
				if (spriteBee.getPosition().x < -100)
				{
					// set it up ready to be a whole new bee next frame
					beeActive = false;
				}
			}

			//manage the clouds - cloud 1
			if (!cloud1Active)
			{
				// how fast is the cloud
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 100 + 20);

				// how high is the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x +
					(cloud1Speed * dt.asSeconds()),
					spriteCloud1.getPosition().y);

				// has the cloud reached the right hand side of the screen
				if (spriteCloud1.getPosition().x > 1920)
				{
					// set it up ready to be a whole new cloud next frame
					cloud1Active = false;
				}
			}

			//manage the clouds - cloud 2
			if (!cloud2Active)
			{
				// how fast is the cloud
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 100 + 20);

				// how high is the cloud
				srand((int)time(0) * 20);
				float height = (rand() % 150);
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x +
					(cloud2Speed * dt.asSeconds()),
					spriteCloud2.getPosition().y);

				// has the cloud reached the right hand side of the screen
				if (spriteCloud2.getPosition().x > 1920)
				{
					// set it up ready to be a whole new cloud next frame
					cloud2Active = false;
				}
			}

			//manage the clouds - cloud 3
			if (!cloud3Active)
			{
				// how fast is the cloud
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 100 + 20);

				// how high is the cloud
				srand((int)time(0) * 30);
				float height = (rand() % 150);
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x +
					(cloud3Speed * dt.asSeconds()),
					spriteCloud3.getPosition().y);

				// has the cloud reached the right hand side of the screen
				if (spriteCloud3.getPosition().x > 1920)
				{
					// set it up ready to be a whole new cloud next frame
					cloud3Active = false;
				}
			}

			//update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			// update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;

				if (branchPositions[i] == side::LEFT)
				{
					// move the sprite to the left side
					branches[i].setPosition(610, height);
					// flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// move the sprite to the right side
					branches[i].setPosition(1330, height);
					// set the sprite rotation to normal
					branches[i].setRotation(0);
				}
				else
				{
					// hide the branch
					branches[i].setPosition(3000, height);
				}
			}

			// handle a flying log
			if (logActive)
			{
				spriteLog.setPosition(
					spriteLog.getPosition().x +
					(logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y +
					(logSpeedY * dt.asSeconds()));

				// has the log reached the right hand edge?
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 2000)
				{
					// set it up ready to be a whole new log next frame
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			// has the player been squished by a branch?
			if (branchPositions[5] == playerSide)
			{
				// death
				paused = true;
				acceptInput = false;

				// draw the gravestone
				spriteRIP.setPosition(525, 760);

				// hide the player
				spritePlayer.setPosition(2000, 660);

				// change the text of the message
				messageText.setString("Ya dun got squished, ya one legged punk.");

				// center it on the screen
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f,
					1080 / 2.0f);
			}
		} // end if(!paused)

		//draw the scene

		//clear everything from the last frame
		window.clear();

		//draw the game scene
		window.draw(spriteBackground);

		// draw the goddamned clouds ya one legged jabroni
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		// draw the branches
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}

		// draw the tree ya cracka ass honkey
		window.draw(spriteTree);

		// draw the player
		window.draw(spritePlayer);

		// draw the axe
		window.draw(spriteAxe);

		// draw the flying log
		window.draw(spriteLog);

		// draw the gravestone
		window.draw(spriteRIP);

		// draw the bee ya sexy sexy boi
		window.draw(spriteBee);
		window.draw(spriteBee2);
		window.draw(spriteBee3);
		window.draw(spriteBee4);
		window.draw(spriteBee5);
		window.draw(spriteBee6);

		//draw the score
		window.draw(scoreText);
		window.draw(boheezyText);

		//draw the time bar
		window.draw(timeBar);

		if (paused)
		{
			//draw the message
			window.draw(messageText);
		}

		//show everything we just drew
		window.display();
	}
    return 0;
}

// function definition
void updateBranches(int seed)
{
	// move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}
	// spawn a new branch at position 0
	// LEFT, RIGHT, or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
		
	case 1:
		branchPositions[0] = side::RIGHT;
		break;

	default:
		branchPositions[0] = side::NONE;
		break;
	}
}

