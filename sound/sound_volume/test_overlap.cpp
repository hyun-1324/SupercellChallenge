#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

// 다중 사운드 테스트 - 소리가 겹치는지 확인

int main() {
    sf::RenderWindow window(sf::VideoMode({600, 400}), "Sound Overlap Test");
    window.setFramerateLimit(60);

    // 긴 소리 로드 (배경음악을 효과음처럼 사용)
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("assets/bgmusic.wav")) {
        std::cerr << "Failed to load sound\n";
        return 1;
    }

    std::cout << "Sound duration: " << buffer.getDuration().asSeconds() << " seconds\n";

    // 5개의 Sound 객체 생성
    std::vector<sf::Sound> sounds;
    for (int i = 0; i < 5; i++) {
        sounds.emplace_back(buffer);
        sounds.back().setVolume(50);  // 볼륨 낮춰서 5개 겹쳐도 괜찮게
    }

    int currentSound = 0;
    int playCount = 0;

    sf::Font font;
    font.openFromFile("assets/Arial.ttf");

    sf::Text infoText(font, "Press SPACE to play sound\nPress rapidly to hear overlap!", 20);
    infoText.setPosition({50, 50});
    infoText.setFillColor(sf::Color::White);

    sf::Text statusText(font, "", 16);
    statusText.setPosition({50, 150});
    statusText.setFillColor(sf::Color::Green);

    sf::Text countText(font, "Sounds played: 0", 16);
    countText.setPosition({50, 200});
    countText.setFillColor(sf::Color::Yellow);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    // 현재 인덱스의 Sound 재생
                    sounds[currentSound].play();

                    std::cout << "Playing Sound[" << currentSound << "] ";
                    std::cout << "(Total playing: ";

                    // 몇 개가 재생 중인지 체크
                    int playing = 0;
                    for (const auto& s : sounds) {
                        if (s.getStatus() == sf::Sound::Status::Playing) {
                            playing++;
                        }
                    }
                    std::cout << playing << ")\n";

                    playCount++;
                    countText.setString("Sounds played: " + std::to_string(playCount));

                    // 다음 인덱스로
                    currentSound = (currentSound + 1) % sounds.size();
                }
            }
        }

        // 현재 재생 중인 Sound 개수 표시
        int playing = 0;
        std::string statusStr = "Currently playing: ";
        for (size_t i = 0; i < sounds.size(); i++) {
            if (sounds[i].getStatus() == sf::Sound::Status::Playing) {
                statusStr += std::to_string(i) + " ";
                playing++;
            }
        }
        statusStr += "(" + std::to_string(playing) + " total)";
        statusText.setString(statusStr);

        window.clear(sf::Color::Black);
        window.draw(infoText);
        window.draw(statusText);
        window.draw(countText);
        window.display();
    }

    return 0;
}
