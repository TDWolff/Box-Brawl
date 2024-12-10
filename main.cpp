#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

int main() {
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

    // Enable smooth edges for textures
    sf::Texture::getMaximumSize(); // Only required if you're using textured shapes later

    // Direction indicator
    sf::VertexArray frontLine(sf::Lines, 2);

    // View setup (30x30 units visible)
    sf::View view(sf::FloatRect(0, 0, 30, 30));
    window.setView(view);

    // Clock for consistent movement
    sf::Clock clock;

    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Delta time
        float deltaTime = clock.restart().asSeconds();

        // Player movement
        float speed = 5.0f; // Adjust speed to a reasonable value
        sf::Vector2f movement(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += speed * deltaTime;

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

        // Update the view to follow the player
        view.setCenter(player.getPosition());
        window.setView(view);

        // Rendering
        window.clear();
        window.draw(background);
        window.draw(gridLines);
        window.draw(player);
        window.draw(frontLine);
        window.display();
    }

    return 0;
}
