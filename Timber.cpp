#include "BackgroundHandler.h"
#include "Tree.h"
#include "Bee.h"
#include "Cloud.h"
#include "TextHandler.h"
#include "BarHandler.h"
#include "Branch.h"
#include "Player.h"
#include "Gravestone.h"
#include "Axe.h"
#include "FlyingLog.h"
#include "SoundHandler.h"

#include <vector>
#include <random>
#include <sstream>
#include <utility>

const int TOTAL_CLOUDS_NUMBER = 10;
const int TOTAL_BRANCH_NUMBER = 6;

void updateBranches(std::vector<Branch>& branchVector);

int main()
{
    // Create a video mode object
    sf::VideoMode vm(1920, 1080);
    // Create and open a window for the game
    sf::RenderWindow window(vm, "Timber!!!", sf::Style::Fullscreen);

    // In game sprites
    BackgroundHandler mainBackGround{};
    Tree fellingTree{810, 0};
    Bee flyingBee{0, 800};
    std::vector<Cloud> clouds{};
    clouds.reserve(TOTAL_CLOUDS_NUMBER);
    Cloud tempCloud{};
    for (size_t i = 0; i < TOTAL_CLOUDS_NUMBER; i++)
    {       
        clouds.push_back(tempCloud);
    }
    Branch tempBranch{};
    std::vector<Branch> branches{};
    branches.reserve(TOTAL_BRANCH_NUMBER);
    for (size_t i = 0; i < TOTAL_BRANCH_NUMBER; i++)
    {
        branches.push_back(tempBranch);
    }

    Player mainPlayer{580, 720};
    Gravestone ripStone{600, 860};
    Axe mainPlayerAxe{ 700, 830 };
    FlyingLog pieceOfWood{ 810, 720 };

    // In game text
    TextHandler inGameTexts{};
    unsigned int score{};

    // In game bar
    BarHandler inGameTimeBar{};

    // Time management
    sf::Clock clock;

    // To start the game
    bool isGamePaused = true;

    // To determine when to listen for chops and when to ignore them
    bool acceptInput = false;

    // Handle Sounds
    SoundHandler mySounds{};

    while (window.isOpen())
    {
        /****************************************
        Handle the players input
        ****************************************/
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyReleased && !isGamePaused)
            {
                // Listen to key pressing again.
                acceptInput = true;

                // Hide the axe
                mainPlayerAxe.getSprite().setPosition(2000.0f, mainPlayerAxe.getSprite().getPosition().y);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }

        // Starts the game...
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        {
            isGamePaused = false;
            score = 0;
            inGameTimeBar.setRemainingGameTime(6);

            // Setting up branches one again...
            for (size_t i = 1; i < branches.size(); i++)
            {
                branches.at(i).getBranchPosition() = branchSide::NONE;
            }

            // Gravestone is hidden now.
            ripStone.getSprite().setPosition(static_cast<float>(675), static_cast<float>(2000));

            // The player is in the position
            mainPlayer.getSprite().setPosition(static_cast<float>(580), static_cast<float>(720));

            acceptInput = true;
        }

        // Wraps the player controls to be able to accept new inputs
        if (acceptInput)
        {

            // Handling to press right cursor...
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                // Make sure the player is on the right
                mainPlayer.getBranchPosition() = playerSide::RIGHT;
                ++score;

                // Increase the time
                inGameTimeBar.setRemainingGameTime(static_cast<float>(inGameTimeBar.getRemainingGameTime() + (2 / score) + 0.15f));

                // Change the axe position
                const auto& [firstAxeValue, secondAxeValue] = mainPlayerAxe.getAxePositions();
                mainPlayerAxe.getSprite().setPosition(secondAxeValue, mainPlayerAxe.getSprite().getPosition().y);

                // Change the player position
                mainPlayer.getSprite().setPosition(1200.0f, 720.0f);

                updateBranches(branches);

                // Make the log flying to the right
                pieceOfWood.getSprite().setPosition(810.0f, 720.0f);
                pieceOfWood.setSpeedX(-5000.0f);
                pieceOfWood.setCutStatus(true);
                acceptInput = false;

                mySounds.getChop().play();
            }

            // Handling to press left cursor...
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                // Make sure the player is on the right
                mainPlayer.getBranchPosition() = playerSide::LEFT;
                ++score;

                // Increase the time
                inGameTimeBar.setRemainingGameTime(static_cast<float>(inGameTimeBar.getRemainingGameTime() + (2 / score) + 0.15f));

                // Change the axe position
                const auto& [firstAxeValue, secondAxeValue] = mainPlayerAxe.getAxePositions();
                mainPlayerAxe.getSprite().setPosition(firstAxeValue, mainPlayerAxe.getSprite().getPosition().y);

                // Change the player position
                mainPlayer.getSprite().setPosition(580, 720.0f);

                updateBranches(branches);

                // Make the log flying to the left
                pieceOfWood.getSprite().setPosition(810.0f, 720.0f);
                pieceOfWood.setSpeedX(5000.0f);
                pieceOfWood.setCutStatus(true);
                acceptInput = false;

                mySounds.getChop().play();
            }
        }
        
        /****************************************
        Update the scene
        ****************************************/

        if (!isGamePaused)
        {
            // Measure Time
            sf::Time elapsedTime = clock.restart();

            // Handle Time Bar According to the Time
            inGameTimeBar.setRemainingGameTime((inGameTimeBar.getRemainingGameTime() - elapsedTime.asSeconds()));
            auto [shrinkRatio, remainingTime, timeBarHeight] = inGameTimeBar.getTimeBarVariables();
            inGameTimeBar.getTimeBar().setSize(sf::Vector2f{ shrinkRatio * remainingTime, timeBarHeight});
            if (remainingTime <= 0.0f)
            {
                isGamePaused = true;

                // Change the message shown to the player.
                inGameTexts.setMessageText("Out of time!!!");

                // Reposition the text...
                sf::FloatRect textRect = inGameTexts.getMessageText().getLocalBounds();
                inGameTexts.getMessageText().setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                inGameTexts.getMessageText().setPosition(1920 / 2.0f, 1080 / 2.0f);

                mySounds.getNoTime().play();
            }

            // Setup the bee
            if (!flyingBee.getFlyStatus())
            {
                // The speed of bee
                std::mt19937 myRandom{ std::random_device{}() };
                std::uniform_int_distribution myList{ 200, 399 };
                flyingBee.setSpeedStatus(static_cast<float>(myList(myRandom)));

                // Position on the Y-Axis
                std::uniform_int_distribution myListTwo{ 500, 999 };
                flyingBee.getSprite().setPosition(2000.0f, static_cast<float>(myListTwo(myRandom)));

                // The bee is now ready to fly
                flyingBee.setFlyStatus(true);
            }
            // Fly the bee
            else
            {
                flyingBee.getSprite().setPosition(flyingBee.getSprite().getPosition().x - (flyingBee.getSpeedStatus() * elapsedTime.asSeconds()), flyingBee.getSprite().getPosition().y);

                // In case the bee reaches the left-hand side of the screen...
                if (flyingBee.getSprite().getPosition().x < -100)
                {
                    // the bee is inactive now.
                    flyingBee.setFlyStatus(false);
                }
            }

            // Setup the clouds
            for (size_t i = 0; i < clouds.size(); i++)
            {
                if (!clouds.at(i).getFlyStatus())
                {
                    // The speed of clouds
                    std::mt19937 myRandom{ std::random_device{}() };
                    std::uniform_int_distribution myList{ 0, 500 };
                    clouds.at(i).setSpeedStatus(static_cast<float>(myList(myRandom)));

                    // Position on the Y-Axis
                    std::uniform_int_distribution myListTwo{ 0, 150 };
                    clouds.at(i).getSprite().setPosition(-200.0f, static_cast<float>(myListTwo(myRandom)));

                    // The bee is now ready to fly
                    clouds.at(i).setFlyStatus(true);
                }
                else
                {
                    clouds.at(i).getSprite().setPosition(clouds.at(i).getSprite().getPosition().x + (clouds.at(i).getSpeedStatus() * elapsedTime.asSeconds()), clouds.at(i).getSprite().getPosition().y);

                    // In case the cloud reaches the left-hand side of the screen...
                    if (clouds.at(i).getSprite().getPosition().x > 1920)
                    {
                        // The cloud is inactive
                        clouds.at(i).setFlyStatus(false);
                    }
                }
            }

            // Setup the branches
            for (size_t i = 0; i < branches.size(); i++)
            {
                float height{ static_cast<float>(i * 150) };
                if (branches.at(i).getBranchPosition() == branchSide::LEFT)
                {
                    // Move the branch sprite to the left side...
                    branches.at(i).getSprite().setPosition(610, height);

                    // Flip the branch sprite to the other way
                    branches.at(i).getSprite().setRotation(180);
                }
                else if (branches.at(i).getBranchPosition() == branchSide::RIGHT)
                {
                    // Move the branch sprite to the right side...
                    branches.at(i).getSprite().setPosition(1330, height);

                    // Flip the branch sprite to the normal
                    branches.at(i).getSprite().setRotation(0);
                }
                else
                {
                    // Hide the branch
                    branches.at(i).getSprite().setPosition(3000, height);
                }
            }

            // Handle flying logs
            if (pieceOfWood.getCutStatus())
            {
                pieceOfWood.getSprite().setPosition(pieceOfWood.getSprite().getPosition().x + (pieceOfWood.getSpeedX() * elapsedTime.asSeconds()), pieceOfWood.getSprite().getPosition().y + (pieceOfWood.getSpeedY() * elapsedTime.asSeconds()));
                
                // If the log reached the right edge?
                if (pieceOfWood.getSprite().getPosition().x < -100 || pieceOfWood.getSprite().getPosition().x > 2000)
                {
                    // Makes it ready to be a whole new log next frame
                    pieceOfWood.setCutStatus(false);
                    pieceOfWood.getSprite().setPosition(static_cast<float>(810), static_cast<float>(720));
                }
            }

            // Has the player been squished?
            if (static_cast<int>(branches.at(5).getBranchPosition()) == static_cast<int>(mainPlayer.getBranchPosition()))
            {
                // Death 
                isGamePaused = true;
                acceptInput = false;

                // Draw the Gravestone
                ripStone.getSprite().setPosition(525.0f, 760.0f);
                
                // Hide the player
                mainPlayer.getSprite().setPosition(2000.0f, 660.0f);

                // Change the text message and center it to the screen
                inGameTexts.setMessageText("SQUISHED!!!");
                sf::FloatRect textRect = inGameTexts.getMessageText().getLocalBounds();
                inGameTexts.getMessageText().setOrigin( textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                inGameTexts.getMessageText().setPosition(1920 / 2.0f, 1080 / 2.0f);

                mySounds.getDeath().play();
            }

            // Handle in game texts
            std::stringstream ss;
            ss << "Score: " << score;
            inGameTexts.setScoreText(ss.str());
        }
        /****************************************
        Draw the scene
        ****************************************/
        // Clear everything from the last frame
        window.clear();

        // Draw our game scene here
        window.draw(mainBackGround.getSprite());
        for (auto& cloud : clouds)
        {
            window.draw(cloud.getSprite());
        }
        for (auto& branch : branches)
        {
            window.draw(branch.getSprite());
        }
        window.draw(fellingTree.getSprite());
        window.draw(mainPlayer.getSprite());
        window.draw(mainPlayerAxe.getSprite());
        window.draw(pieceOfWood.getSprite());
        window.draw(ripStone.getSprite());
        window.draw(flyingBee.getSprite());
        window.draw(inGameTexts.getScoreText());
        window.draw(inGameTimeBar.getTimeBar());
        if (isGamePaused)
        {
            window.draw(inGameTexts.getMessageText());
        }

        // Show everything we just drew
        window.display();
    }
    return 0;
}

void updateBranches(std::vector<Branch>& branchVector)
{
    std::mt19937 myRandom{ std::random_device{}() };
    std::uniform_int_distribution myList{ 0, 5 };

    for (size_t i = branchVector.size() - 1 ; i > 0 ; --i)
    {
        branchVector.at(i).getBranchPosition() = branchVector.at(i - 1).getBranchPosition();
    }

    int r = myList(myRandom);
    switch (r)
    {
    case 0:
        branchVector.at(0).setBranchPosition(branchSide::LEFT);
        break;
    case 1:
        branchVector.at(0).setBranchPosition(branchSide::RIGHT);
        break;
    default:
        branchVector.at(0).setBranchPosition(branchSide::NONE);
        break;
    }
}