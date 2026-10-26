//
//  main.cpp
//  DinoGame
//
//  Created by Berke AVCI on 26.10.2026.
//

#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <iostream>

constexpr unsigned int windowWidth = 800;
constexpr unsigned int windowHeight = 400;

constexpr float groundY = 340.f;
constexpr float dinoWidth = 40.f;
constexpr float dinoHeight = 50.f;

constexpr float gravity = 1800.f;
constexpr float jumpHeight = 225.f;

const float initialVelocity = std::sqrt(2.f * gravity * jumpHeight);



int main()
{
    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "DinoGame");
    
    window.setFramerateLimit(60);
    
    // Dino
    sf::RectangleShape dino(sf::Vector2f(dinoWidth, dinoHeight));
    dino.setFillColor(sf::Color(50, 50, 50));
    dino.setPosition({100.f, groundY - dinoHeight});
    
    // Ground
    sf::RectangleShape ground(sf::Vector2f(static_cast<float>(windowWidth), 2.f));
    ground.setPosition({0.f, groundY});
    ground.setFillColor(sf::Color(80, 80, 80));
    
    // Cactus
    sf::RectangleShape cactus(sf::Vector2f(25.f, 45.f));
    cactus.setFillColor(sf::Color(30, 130, 60));
    cactus.setPosition({static_cast<float>(windowWidth), groundY - 45.f});
    
    const float cactusSpeed = 300.f;
    
    // Physics
    bool onGround = true;
    bool jumpRequested = false;
    bool gameOver = false;
    bool cactusPassed = false;
    
    int score = 0;
    
    float jumpStartY = groundY - dinoHeight;
    float flightTime = 0.f;
    
    sf::Clock clock;
    

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    if (!gameOver) {
                        jumpRequested = true;
                    }
                }
                    
                else if (keyPressed->code == sf::Keyboard::Key::R && gameOver) {
                    dino.setPosition({
                           100.f,
                           groundY - dinoHeight
                       });

                       
                    cactus.setPosition({
                        static_cast<float>(windowWidth),
                        groundY - cactus.getSize().y
                    });

                       
                    onGround = true;
                    jumpRequested = false;
                    flightTime = 0.f;
                    jumpStartY = groundY - dinoHeight;

                       
                    score = 0;
                    cactusPassed = false;
                    gameOver = false;

                    clock.restart();
                }
            }
        }
        
        const float dt = clock.restart().asSeconds();
        
        if (!gameOver) {
            if (jumpRequested && onGround) {
                onGround = false;
                flightTime = 0.f;
                
                jumpStartY = dino.getPosition().y;
            }
            
            jumpRequested = false;
            
            if (!onGround) {
                flightTime += dt;
                
                const float y = jumpStartY - initialVelocity * flightTime + 0.5f * gravity * flightTime * flightTime;
                
                dino.setPosition({dino.getPosition().x, y});
                
                if (y + dino.getSize().y >= groundY) {
                    dino.setPosition({dino.getPosition().x, groundY - dino.getSize().y});
                    onGround = true;
                    flightTime = 0.f;
                }
            }
            
            cactus.move({-cactusSpeed * dt, 0.f});
            
            if (cactus.getPosition().x + cactus.getSize().x < 0.f) {
                cactus.setPosition({static_cast<float>(windowWidth), groundY - cactus.getSize().y});
                cactusPassed = false;
            }
            
            if (!cactusPassed && cactus.getPosition().x + cactus.getSize().x < dino.getPosition().x) {
                score++;
                cactusPassed = true;
                
                std::cout << "Score: " << score << std::endl;
            }
            
            if (dino.getGlobalBounds().findIntersection(cactus.getGlobalBounds())) {
                gameOver = true;
                std::cout << "Game Over" << std::endl;
            }
        }
        


        window.clear(sf::Color(245, 245, 245));
        
        // Render
        window.draw(dino);
        window.draw(ground);
        window.draw(cactus);
        
        if (gameOver) {
            sf::RectangleShape marker(sf::Vector2f(180.f, 8.f));
            marker.setFillColor(sf::Color::Red);
            marker.setPosition({310.f, 150.f});
            
            window.draw(marker);
        }
        
        window.setTitle("DinoGame | Score: " + std::to_string(score));
        window.display();
    }
}
