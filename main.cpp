#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <filesystem>
#include <unistd.h>

int main() {
    // Set the working directory to the game root
    if (chdir("/Users/torinwolff/Documents/GitHub/Box-Brawl/BoxBrawl") != 0) {
        std::cerr << "Error: Failed to change working directory!" << std::endl;
        return -1;
    }
    // Enable antialiasing
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8; // Higher values improve quality but may reduce performance

    // Create a window with antialiasing
    sf::RenderWindow window(sf::VideoMode(800, 800), "Box Brawl", sf::Style::Default, settings);

    // Create the background (100x100 map)
    const int mapSize = 1000;
    sf::RectangleShape background(sf::Vector2f(mapSize, mapSize));
    background.setFillColor(sf::Color(245, 245, 245));
    background.setOutlineThickness(-0.1f);
    background.setOutlineColor(sf::Color(100, 100, 100)); // Dark grey border

    // Create grid lines
    sf::VertexArray gridLines(sf::Lines);
    for (int i = 0; i <= mapSize; ++i) {
        // Vertical lines
        gridLines.append(sf::Vertex(sf::Vector2f(i, 0), sf::Color::Black));
        gridLines.append(sf::Vertex(sf::Vector2f(i, mapSize), sf::Color::Black));
        // Horizontal lines
        gridLines.append(sf::Vertex(sf::Vector2f(0, i), sf::Color::Black));
        gridLines.append(sf::Vertex(sf::Vector2f(mapSize, i), sf::Color::Black));
    }

    // Player setup
    sf::RectangleShape player(sf::Vector2f(1, 1));
    player.setFillColor(sf::Color(150, 50, 50)); // Subtle red
    player.setOrigin(0.5f, 0.5f); // Center the origin for rotation
    player.setPosition(mapSize / 2, mapSize / 2); // Centered on the map

    // Direction indicator
    sf::VertexArray frontLine(sf::Lines, 2);

    // View setup (30x30 units visible)
    sf::View gameView(sf::FloatRect(0, 0, 30, 30)); // Game view (visible world)
    window.setView(gameView);

    // Health display
    int health = 100;
    sf::Font font;
    std::string fontPath;

    // Get the current working directory and construct the font path dynamically
    std::filesystem::path currentPath = std::filesystem::current_path();
    fontPath = (currentPath / "Contents" / "Resources" / "fonts" / "Pulang.ttf").string();

    if (!font.loadFromFile(fontPath)) {
        std::cerr << "Error loading font: " << fontPath << std::endl;
        std::cout << "Current Path: " << currentPath << std::endl;
        return -1;
    }

    // Health text setup with scaled size
    sf::Text healthText;
    healthText.setFont(font);
    healthText.setString("Health: " + std::to_string(health));
    healthText.setCharacterSize(18);
    healthText.setFillColor(sf::Color::White); // Changed to white
    healthText.setScale(0.05f, 0.05f);

    // Create background for health text
    sf::RectangleShape healthBackground;
    healthBackground.setFillColor(sf::Color(100, 100, 100, 200)); // Grey with transparency
    // Size will be updated in the game loop based on text bounds
    healthBackground.setScale(0.05f, 0.05f); // Match text scaling

    // Movement state
    bool movingUp = false, movingDown = false, movingLeft = false, movingRight = false;

    // Clock for consistent movement
    sf::Clock clock;

    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Key press events
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::W:
                        movingUp = true;
                        break;
                    case sf::Keyboard::S:
                        movingDown = true;
                        break;
                    case sf::Keyboard::A:
                        movingLeft = true;
                        break;
                    case sf::Keyboard::D:
                        movingRight = true;
                        break;
                    default:
                        break;
                }
            }

            // Key release events
            if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code) {
                    case sf::Keyboard::W:
                        movingUp = false;
                        break;
                    case sf::Keyboard::S:
                        movingDown = false;
                        break;
                    case sf::Keyboard::A:
                        movingLeft = false;
                        break;
                    case sf::Keyboard::D:
                        movingRight = false;
                        break;
                    default:
                        break;
                }
            }
        }

        // Delta time
        float deltaTime = clock.restart().asSeconds();

        // Player movement
        float speed = 5.0f; // Adjust speed to a reasonable value
        sf::Vector2f movement(0.f, 0.f);
        if (movingUp) movement.y -= speed * deltaTime;
        if (movingDown) movement.y += speed * deltaTime;
        if (movingLeft) movement.x -= speed * deltaTime;
        if (movingRight) movement.x += speed * deltaTime;

        // Apply movement and clamp within bounds
        sf::Vector2f newPosition = player.getPosition() + movement;
        newPosition.x = std::max(0.f, std::min(static_cast<float>(mapSize), newPosition.x));
        newPosition.y = std::max(0.f, std::min(static_cast<float>(mapSize), newPosition.y));
        player.setPosition(newPosition);

        // Player rotation towards the mouse
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition);
        float angle = std::atan2(worldMousePosition.y - player.getPosition().y,
                                 worldMousePosition.x - player.getPosition().x) *
                      180.f / 3.14159f;
        player.setRotation(angle);

        // Update the front line
        frontLine[0] = sf::Vertex(player.getPosition(), sf::Color::Red);
        sf::Vector2f frontTip = player.getPosition() +
                                sf::Vector2f(std::cos(player.getRotation() * 3.14159f / 180.f),
                                             std::sin(player.getRotation() * 3.14159f / 180.f)) *
                                    2.f; // Length of 2 units
        frontLine[1] = sf::Vertex(frontTip, sf::Color::Red);

        // Update the game view to follow the player (for movement and rotation)
        gameView.setCenter(player.getPosition());
        window.setView(gameView);

        healthText.setString("Health: " + std::to_string(health));
        sf::Vector2f viewCenter = gameView.getCenter();
        sf::Vector2f viewSize = gameView.getSize();
        sf::Vector2f healthPosition(
            viewCenter.x - viewSize.x/2 + 0.5f,
            viewCenter.y + viewSize.y/2 - 1.5f
        );
        
        healthText.setPosition(healthPosition);

        // Update health background size and position
        sf::FloatRect textBounds = healthText.getLocalBounds();
        healthBackground.setSize(sf::Vector2f(textBounds.width + 20, textBounds.height + 10)); // Add padding
        healthBackground.setPosition(healthPosition.x - 0.5f, healthPosition.y - 0.25f); // Offset slightly to center text

        // Rendering
        window.clear();
        window.draw(background);
        window.draw(gridLines);
        window.draw(player);
        window.draw(frontLine);
        window.draw(healthBackground); // Draw background first
        window.draw(healthText);      // Draw text on top

        window.display();
    }

    return 0;
}
