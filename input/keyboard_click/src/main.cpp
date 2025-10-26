#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Mouse Click Example");

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

            if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPress->code == sf::Keyboard::Key::Escape) {
                    std::cout << "Escape key pressed, closing window.\n";
                    window.close();
                }
                if (keyPress->code == sf::Keyboard::Key::Space) {
                    std::cout << "Space key pressed\n";
                    circle.setFillColor(sf::Color::Green);
                }
                if (keyPress->code == sf::Keyboard::Key::R) {
                    std::cout << "R key pressed\n";
                    circle.setFillColor(sf::Color::Red);
                }
            }
            if (const auto* keyRelease = event->getIf<sf::Event::KeyReleased>()) {
                if (keyRelease->code == sf::Keyboard::Key::Space) {
                    std::cout << "Space key released\n";
                    circle.setFillColor(sf::Color::Blue);
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
