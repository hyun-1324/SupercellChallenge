#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // 방법 1: 기존 방식
    sf::Texture texture1;
    if (!texture1.loadFromFile("../sprite/sprite_basic/assets/player.png")) {
        std::cout << "방법1 실패" << std::endl;
    } else {
        std::cout << "방법1 성공: " << texture1.getSize().x << "x" << texture1.getSize().y << std::endl;
    }

    // 방법 2: 생성자 방식 (SFML 3.0)
    try {
        sf::Texture texture2("../sprite/sprite_basic/assets/player.png");
        std::cout << "방법2 성공: " << texture2.getSize().x << "x" << texture2.getSize().y << std::endl;
    } catch (const std::exception& e) {
        std::cout << "방법2 예외: " << e.what() << std::endl;
    }

    // 방법 3: 잘못된 파일로 테스트
    try {
        sf::Texture texture3("nonexistent.png");
        std::cout << "방법3 - 파일 없는데 성공?" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "방법3 예외 발생: " << e.what() << std::endl;
    }

    return 0;
}
