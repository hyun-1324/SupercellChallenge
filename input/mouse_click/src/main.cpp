#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Create a window with the title "Hello SFML" and size 800x600
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Hello SFML");

    sf::RectangleShape rectangle(sf::Vector2f(100, 50));
    rectangle.setFillColor(sf::Color::Red);
    rectangle.setPosition({150, 150});

    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Blue);
    circle.setPosition({375, 275});

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePress->button == sf::Mouse::Button::Left) {
                    std::cout << "Left mouse button pressed at ("
                              << mousePress->position.x << ", "
                              << mousePress->position.y << ")\n";
                    circle.setPosition({static_cast<float>(mousePress->position.x - 50), static_cast<float>(mousePress->position.y - 50)});
                }
                if (mousePress->button == sf::Mouse::Button::Right) {
                    std::cout << "Right mouse button pressed at ("
                              << mousePress->position.x << ", "
                              << mousePress->position.y << ")\n";
                }
            }
            if (const auto* mouseRelease = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseRelease->button == sf::Mouse::Button::Left) {
                    std::cout << "Left mouse button released at ("
                              << mouseRelease->position.x << ", "
                              << mouseRelease->position.y << ")\n";
                }
                if (mouseRelease->button == sf::Mouse::Button::Right) {
                    std::cout << "Right mouse button released at ("
                              << mouseRelease->position.x << ", "
                              << mouseRelease->position.y << ")\n";
                }
            }
        }
        window.clear();
        window.draw(rectangle);
        window.draw(circle);
        window.display();
    }
    return 0;
}
