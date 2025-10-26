#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>

// Volume Control & Sound Mixing - SFML 3.0
//
// 볼륨 관리:
// - setVolume(0-100): 각 사운드/음악의 볼륨 설정
// - 0 = 무음, 100 = 최대
// - 마스터 볼륨 개념: 모든 사운드에 적용되는 전역 볼륨
//
// 게임에서 볼륨 관리:
// - 효과음 볼륨 (SFX)
// - 배경음악 볼륨 (BGM)
// - 마스터 볼륨 (전체)
//
// 여러 사운드 동시 재생:
// - sf::Sound는 여러 개를 동시에 재생 가능
// - 각각 독립적으로 제어

int main() {
    sf::RenderWindow window(sf::VideoMode({900, 650}), "Volume Control - SFML 3.0");
    window.setFramerateLimit(60);

    // 배경음악
    sf::Music bgMusic;
    if (!bgMusic.openFromFile("assets/music.ogg")) {
        if (!bgMusic.openFromFile("assets/music.wav")) {
            std::cout << "Warning: No music file found\n";
        }
    }
    bgMusic.setLooping(true);  // SFML 3.0
    bgMusic.setVolume(50);

    // 효과음 버퍼들
    sf::SoundBuffer explodeBuffer, coinBuffer, jumpBuffer;
    if (!explodeBuffer.loadFromFile("assets/click.wav")) {
        std::cout << "Warning: click.wav not found\n";
    }
    if (!coinBuffer.loadFromFile("assets/coin.wav")) {
        std::cout << "Warning: coin.wav not found\n";
    }
    if (!jumpBuffer.loadFromFile("assets/jump.wav")) {
        std::cout << "Warning: jump.wav not found\n";
    }

    // 효과음들 (동시 재생을 위해 여러 개)
    std::vector<sf::Sound> explosions;
    for (int i = 0; i < 5; i++) {  // 최대 5개 동시 재생
        explosions.emplace_back(explodeBuffer);
        explosions.back().setVolume(70);
    }

    sf::Sound coinSound(coinBuffer);
    coinSound.setVolume(80);

    sf::Sound jumpSound(jumpBuffer);
    jumpSound.setVolume(60);

    // 볼륨 설정
    float masterVolume = 100.0f;  // 마스터 볼륨
    float bgmVolume = 50.0f;      // 배경음악 볼륨
    float sfxVolume = 70.0f;      // 효과음 볼륨

    // UI 설정
    sf::Font font;
    if (!font.openFromFile("assets/Arial.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    // 제목
    sf::Text titleText(font, "Volume Control & Sound Mixing", 28);
    titleText.setPosition({250, 30});

    // 슬라이더 배경
    sf::RectangleShape masterSliderBg({400, 20});
    masterSliderBg.setPosition({400, 150});
    masterSliderBg.setFillColor(sf::Color(200, 200, 200));

    sf::RectangleShape bgmSliderBg({400, 20});
    bgmSliderBg.setPosition({400, 220});
    bgmSliderBg.setFillColor(sf::Color(200, 200, 200));

    sf::RectangleShape sfxSliderBg({400, 20});
    sfxSliderBg.setPosition({400, 290});
    sfxSliderBg.setFillColor(sf::Color(200, 200, 200));

    // 슬라이더 바
    sf::RectangleShape masterSlider({400, 20});
    masterSlider.setPosition({400, 150});
    masterSlider.setFillColor(sf::Color::Blue);

    sf::RectangleShape bgmSlider({200, 20});
    bgmSlider.setPosition({400, 220});
    bgmSlider.setFillColor(sf::Color::Green);

    sf::RectangleShape sfxSlider({280, 20});
    sfxSlider.setPosition({400, 290});
    sfxSlider.setFillColor(sf::Color::Red);

    // 텍스트
    sf::Text masterText(font, "Master Volume: 100%", 18);
    masterText.setPosition({100, 150});

    sf::Text bgmText(font, "BGM Volume: 50%", 18);
    bgmText.setPosition({100, 220});

    sf::Text sfxText(font, "SFX Volume: 70%", 18);
    sfxText.setPosition({100, 290});

    // 버튼들
    sf::RectangleShape playMusicBtn({180, 50});
    playMusicBtn.setPosition({100, 400});
    playMusicBtn.setFillColor(sf::Color::Green);

    sf::RectangleShape explodeBtn({180, 50});
    explodeBtn.setPosition({300, 400});
    explodeBtn.setFillColor(sf::Color::Red);

    sf::RectangleShape coinBtn({180, 50});
    coinBtn.setPosition({500, 400});
    coinBtn.setFillColor(sf::Color::Yellow);

    sf::RectangleShape jumpBtn({180, 50});
    jumpBtn.setPosition({700, 400});
    jumpBtn.setFillColor(sf::Color::Cyan);

    sf::Text playMusicText(font, "Play Music", 16);
    playMusicText.setPosition({125, 415});

    sf::Text explodeText(font, "Explode", 16);
    explodeText.setPosition({345, 415});

    sf::Text coinText(font, "Coin", 16);
    coinText.setPosition({565, 415});
    coinText.setFillColor(sf::Color::Black);

    sf::Text jumpText(font, "Jump", 16);
    jumpText.setPosition({760, 415});
    jumpText.setFillColor(sf::Color::Black);

    sf::Text instructionText(font,
        "Controls:\n"
        "1/2: Master Volume Up/Down\n"
        "3/4: BGM Volume Up/Down\n"
        "5/6: SFX Volume Up/Down\n"
        "M: Play Music    E: Explode    C: Coin    J: Jump", 14);
    instructionText.setPosition({100, 500});

    int currentExplosion = 0;  // 다음에 재생할 폭발음 인덱스

    while (window.isOpen()) {
        // 이벤트 처리
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // 마우스 클릭
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mousePos(static_cast<float>(mousePressed->position.x),
                                     static_cast<float>(mousePressed->position.y));

                if (playMusicBtn.getGlobalBounds().contains(mousePos)) {
                    if (bgMusic.getStatus() != sf::Music::Status::Playing) {
                        bgMusic.play();
                    }
                }
                if (explodeBtn.getGlobalBounds().contains(mousePos)) {
                    explosions[currentExplosion].play();
                    currentExplosion = (currentExplosion + 1) % explosions.size();
                }
                if (coinBtn.getGlobalBounds().contains(mousePos)) {
                    coinSound.play();
                }
                if (jumpBtn.getGlobalBounds().contains(mousePos)) {
                    jumpSound.play();
                }
            }

            // 키보드 입력
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                // 마스터 볼륨
                if (keyPressed->code == sf::Keyboard::Key::Num1) {
                    masterVolume = std::min(100.f, masterVolume + 10);
                }
                if (keyPressed->code == sf::Keyboard::Key::Num2) {
                    masterVolume = std::max(0.f, masterVolume - 10);
                }
                // BGM 볼륨
                if (keyPressed->code == sf::Keyboard::Key::Num3) {
                    bgmVolume = std::min(100.f, bgmVolume + 10);
                }
                if (keyPressed->code == sf::Keyboard::Key::Num4) {
                    bgmVolume = std::max(0.f, bgmVolume - 10);
                }
                // SFX 볼륨
                if (keyPressed->code == sf::Keyboard::Key::Num5) {
                    sfxVolume = std::min(100.f, sfxVolume + 10);
                }
                if (keyPressed->code == sf::Keyboard::Key::Num6) {
                    sfxVolume = std::max(0.f, sfxVolume - 10);
                }

                // 사운드 재생
                if (keyPressed->code == sf::Keyboard::Key::M) {
                    if (bgMusic.getStatus() != sf::Music::Status::Playing) {
                        bgMusic.play();
                    }
                }
                if (keyPressed->code == sf::Keyboard::Key::E) {
                    explosions[currentExplosion].play();
                    currentExplosion = (currentExplosion + 1) % explosions.size();
                }
                if (keyPressed->code == sf::Keyboard::Key::C) {
                    coinSound.play();
                }
                if (keyPressed->code == sf::Keyboard::Key::J) {
                    jumpSound.play();
                }
            }
        }

        // 볼륨 적용 (마스터 볼륨 * 개별 볼륨 / 100)
        bgMusic.setVolume(masterVolume * bgmVolume / 100.0f);
        for (auto& explosion : explosions) {
            explosion.setVolume(masterVolume * sfxVolume / 100.0f);
        }
        coinSound.setVolume(masterVolume * sfxVolume / 100.0f);
        jumpSound.setVolume(masterVolume * sfxVolume / 100.0f);

        // UI 업데이트
        masterText.setString("Master Volume: " + std::to_string(static_cast<int>(masterVolume)) + "%");
        bgmText.setString("BGM Volume: " + std::to_string(static_cast<int>(bgmVolume)) + "%");
        sfxText.setString("SFX Volume: " + std::to_string(static_cast<int>(sfxVolume)) + "%");

        // 슬라이더 크기 조정
        masterSlider.setSize({400.f * masterVolume / 100.f, 20});
        bgmSlider.setSize({400.f * bgmVolume / 100.f, 20});
        sfxSlider.setSize({400.f * sfxVolume / 100.f, 20});

        // 렌더링
        window.clear(sf::Color::White);

        // 슬라이더
        window.draw(masterSliderBg);
        window.draw(bgmSliderBg);
        window.draw(sfxSliderBg);
        window.draw(masterSlider);
        window.draw(bgmSlider);
        window.draw(sfxSlider);

        // 버튼
        window.draw(playMusicBtn);
        window.draw(explodeBtn);
        window.draw(coinBtn);
        window.draw(jumpBtn);

        // 텍스트
        window.draw(titleText);
        window.draw(masterText);
        window.draw(bgmText);
        window.draw(sfxText);
        window.draw(playMusicText);
        window.draw(explodeText);
        window.draw(coinText);
        window.draw(jumpText);
        window.draw(instructionText);

        window.display();
    }

    return 0;
}
