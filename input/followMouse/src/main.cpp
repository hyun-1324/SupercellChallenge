#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Mouse Following Example");

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

            if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
                std::cout << "Mouse moved to ("
                          << mouseMove->position.x << ", "
                          << mouseMove->position.y << ")\n";
                circle.setPosition({mouseMove->position.x - 50.f, mouseMove->position.y - 50.f});
            }
        }
        window.clear();
        window.draw(rectangle);
        window.draw(circle);
        window.display();
    }
    return 0;
}
