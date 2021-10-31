#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

// Function Declarations
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// Where is the player/branch?
enum class side { LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main()
{
	// VideoMode Object'll be created.
	VideoMode vm{1920, 1080};

	// Create & open a window.
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	// A texture to hold a graphic on the GPU
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");

	// A sprite, that a texture attached. 
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	// A tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810,0);

	// Prepare the bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	//Is the bee currently moving?
	bool beeActive = false;
	// How fast can the bee fly?
	float beeSpeed = 0.0f;

	// Make 3 cloud sprites from 1 texture...
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	spriteCloud1.setPosition(0,0);
	spriteCloud2.setPosition(0,250);
	spriteCloud3.setPosition(0,500);
	//Are the clouds currently on the screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	// How fast is each cloud?
	float cloud1speed = 0.0f;
	float cloud2speed = 0.0f;
	float cloud3speed = 0.0f;

	// Variables to control time itself
	Clock clock;
	// Time Bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2.0f) - (timeBarStartWidth / 2.0f), 980.0f);
	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Track whether the game is running
	bool paused = true;

	// Draw some text
	int score = 0;
	Text messageText;
	Text scoreText;
	// We need to choose a Font
	Font font;
	font.loadFromFile("fonts/font.ttf");
	// Set the font to our message
	messageText.setFont(font);
	scoreText.setFont(font);
	// Assign the actual message
	messageText.setString("Please, press enter to start!");
	scoreText.setString("Score = 0");
	// Make it really big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);
	// Choose a color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	// Position the text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1000 / 2.0f);
	scoreText.setPosition(20, 20);

	// Prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	// Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		// Set the sprite's origin to dead centre...
		// So, we can then spin it round w/o changing its position
		branches[i].setOrigin(220, 20);
	}

	// Prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);
	// The player starts on the LEFT
	side playerSide = side::LEFT;

	// Prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	// Prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);
	// Line the axe with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// Prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);
	// Some useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// Control the player input
	bool acceptInput = false;

	// Prepare the sound
	// For chopping
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);
	
	// For dying
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// For being out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				// Listen for key presses again
				acceptInput = true;
				
				// hide the axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		// Handles the player input...
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Start the game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			score = 0;
			timeRemaining = 6;

			// Make all the branches disappear
			// starting in the second position
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}
			
			// make sure the gravestone is hidden
			spriteRIP.setPosition(675, 2000);

			// Move the player into position
			spritePlayer.setPosition(580, 720);
			acceptInput = true;
		}

		// Wrap the player controls to make sure accepting input
		if (acceptInput)
		{
			// Handle pressing the right cursor key
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerSide = side::RIGHT;
				score++;

				// Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);

				// Update the branches
				updateBranches(score);

				// Set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;

				// Play a chop sound
				chop.play();
			}

			// Handle pressing the left cursor key
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerSide = side::LEFT;
				score++;

				// Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);

				// Update the branches
				updateBranches(score);

				// Set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;
				acceptInput = false;

				// Play a chop sound
				chop.play();
			}
		}

		// ## Update the screen ##
		if (!paused)
		{
			// Measure time
			Time dt = clock.restart();
			// Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			// Size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
			if (timeRemaining <= 0.0f)
			{
				// Pause the game...
				paused = true;
				
				// Chance the message shown on the screen...
				messageText.setString("Out of time!!!");
				// Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// Play a chop sound
				outOfTime.play();
			}

			// Setup the bee
			if (!beeActive)
			{
				// How fast is the bee...
				srand((unsigned int)(time(0)));
				beeSpeed = static_cast<float>((rand() % 200) + 200);
				// How high is the bee...
				srand((unsigned int)(time(0)) * 10);
				float height = static_cast<float>((rand() % 500) + 500);
				spriteBee.setPosition(2000.f, height);
				beeActive = true;
			}
			// Move the bee
			else
			{
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
				// Has the bee reached the left-hand edge of the screen?
				if (spriteBee.getPosition().x < -100)
				{
					// Set it up ready to be a whole new bee next frame
					beeActive = false;

					// Next frame, the score will increase.
					++score;
				}
			}

			// Manage the clouds, cloud1
			if (!cloud1Active)
			{
				// How fast is the cloud
				srand((unsigned int)(time(0)) * 10);
				cloud1speed = static_cast<float>((rand() % 200));
				// How high is the bee
				srand((unsigned int)(time(0)) * 10);
				float height = static_cast<float>((rand() % 150));
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1speed * dt.asSeconds()), spriteCloud1.getPosition().y);
				// Has the cloud reached the right hand edge of the screen?
				if (spriteCloud1.getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud next frame
					cloud1Active = false;
				}
			}

			// Manage the clouds, cloud2
			if (!cloud2Active)
			{
				// How fast is the cloud
				srand((unsigned int)time(0) * 20);
				cloud2speed = static_cast<float>((rand() % 200));
				// How high is the cloud
				srand((unsigned int)time(0) * 20);
				float height = static_cast<float>((rand() % 300) - 150);
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x +
					(cloud2speed * dt.asSeconds()),
					spriteCloud2.getPosition().y);
				// Has the cloud reached the right hand edge of the screen?
				if (spriteCloud2.getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud next frame
					cloud2Active = false;
				}
			}

			// Manage the clouds, cloud3
			if (!cloud3Active)
			{
				// How fast is the cloud
				srand((unsigned int)time(0) * 30);
				cloud3speed = static_cast<float>((rand() % 200));
				// How high is the cloud
				srand((unsigned int)time(0) * 30);
				float height = static_cast<float>((rand() % 450) - 150);
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x +
					(cloud3speed * dt.asSeconds()),
					spriteCloud3.getPosition().y);
				// Has the cloud reached the right hand edge of the screen?
				if (spriteCloud3.getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud next frame
					cloud3Active = false;
				}
			}

			// Update the score text
			std::stringstream ss;
			ss << "Score : " << score;
			scoreText.setString(ss.str());

			// Update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = static_cast<float>(i * 150);
				if (branchPositions[i] == side::LEFT)
				{
					// Move the sprite to the left side.
					branches[i].setPosition(610, height);
					// Flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// Move the sprite to the right side.
					branches[i].setPosition(1330, height);
					// Flip the sprite round the other way
					branches[i].setRotation(0);
				}
				else
				{
					// Hide the branches
					branches[i].setPosition(3000, height);
				}
			}

			// Handle a flying log
			if (logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				// Has the log reached the right hand edge?
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
				{
					// Set it up ready to be whole a new log next frame
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			// Has the player been squished by a branch?
			if (branchPositions[5] == playerSide)
			{
				// Death
				paused = true;
				acceptInput = false;

				// Draw the gravestone
				spriteRIP.setPosition(525,760);

				// Hide the player
				spritePlayer.setPosition(2000, 660);

				// Change the text message 
				messageText.setString("SQUISHED!!!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// Play the death sound
				death.play();
			}
		}

		// ## Draw the scene ##
		// I. Clears everything from the last frame
		window.clear();

		// II. Draw our game scene here...
		window.draw(spriteBackground);
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}
		window.draw(spriteTree);
		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteLog);
		window.draw(spriteRIP);
		window.draw(spriteBee);
		window.draw(scoreText);
		window.draw(timeBar);
		if (paused)
		{
			window.draw(messageText);
		}

		// III. Show everything that we drew...
		window.display();
	}
	return 0;
}

void updateBranches(int seed)
{
	// Move all the branches down one place
	for (int i = NUM_BRANCHES - 1; i > 0; --i)
	{
		branchPositions[i] = branchPositions[i - 1];
	}

	// Spawn new branches at position 0
	// LEFT, RIGHT, NONE
	srand((unsigned int)(time(0)) + seed);
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