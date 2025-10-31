#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "Player.h"
#include "Enemy.h"
#include "Hammer.h"
#include "Projectile.h"

int main() {
    // 윈도우 생성
    sf::RenderWindow window(sf::VideoMode(1600, 1200), "Rotating Hammer Game");
    window.setFramerateLimit(60);

    // 랜덤 시드
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // 플레이어 생성 (화면 중앙)
    Player player(800.0f, 600.0f);

    // 망치 생성 (특수 스킬)
    Hammer hammer(player.getPosition(), 150.0f);

    // 미사일 리스트
    std::vector<std::unique_ptr<Projectile>> projectiles;

    // 적 리스트
    std::vector<std::unique_ptr<Enemy>> enemies;

    // 적 스폰 타이머
    float spawnTimer = 0.0f;
    float spawnInterval = 0.2f;  // 0.2초마다 적 생성 (10배 증가!)

    // 폰트 및 텍스트
    sf::Font font;
    if (!font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) {
        // 폰트 로드 실패 시 기본 동작
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.0f, 10.0f);

    sf::Text pointsText;
    pointsText.setFont(font);
    pointsText.setCharacterSize(25);
    pointsText.setFillColor(sf::Color::Yellow);
    pointsText.setPosition(10.0f, 50.0f);

    sf::Text controlsText;
    controlsText.setFont(font);
    controlsText.setCharacterSize(20);
    controlsText.setFillColor(sf::Color::White);
    controlsText.setPosition(10.0f, 85.0f);
    controlsText.setString("Controls: WASD/Arrows to move | Left Click to shoot | E for Hammer | R for Dash");

    sf::Clock clock;

    // 게임 루프
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // 이벤트 처리
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            player.handleInput(event, window);
        }

        // 업데이트
        player.updateMousePosition(window);  // 마우스 방향 업데이트
        player.update(deltaTime);

        // 미사일 발사
        if (player.shouldShoot()) {
            projectiles.push_back(std::make_unique<Projectile>(
                player.getPosition(),
                player.getShootDirection()
            ));
            player.resetShoot();
        }

        // 망치 스킬 활성화
        if (player.shouldActivateHammer()) {
            hammer.activate();
            player.useHammer();  // 포인트 차감
            player.resetHammerActivation();
        }

        // 망치 업데이트
        hammer.update(deltaTime, player.getPosition());

        // 미사일 업데이트
        for (auto it = projectiles.begin(); it != projectiles.end();) {
            (*it)->update(deltaTime);

            // 화면 밖으로 나간 미사일 제거
            if ((*it)->isOffScreen(1600.0f, 1200.0f)) {
                it = projectiles.erase(it);
            } else {
                ++it;
            }
        }

        // 적 스폰
        spawnTimer += deltaTime;
        if (spawnTimer >= spawnInterval) {
            spawnTimer = 0.0f;

            // 랜덤 위치에서 적 생성 (화면 가장자리)
            int edge = std::rand() % 4;
            float x, y;

            switch (edge) {
                case 0: // 위쪽
                    x = static_cast<float>(std::rand() % 1600);
                    y = -50.0f;
                    break;
                case 1: // 아래쪽
                    x = static_cast<float>(std::rand() % 1600);
                    y = 1250.0f;
                    break;
                case 2: // 왼쪽
                    x = -50.0f;
                    y = static_cast<float>(std::rand() % 1200);
                    break;
                case 3: // 오른쪽
                    x = 1650.0f;
                    y = static_cast<float>(std::rand() % 1200);
                    break;
            }

            enemies.push_back(std::make_unique<Enemy>(x, y));
        }

        // 적 업데이트 및 충돌 체크
        for (auto it = enemies.begin(); it != enemies.end();) {
            (*it)->update(deltaTime, player.getPosition());

            bool enemyKilled = false;

            // 대쉬 공격 체크
            if (player.isDashing() && (*it)->isAlive()) {
                if (player.getDashHitbox().intersects((*it)->getBounds())) {
                    (*it)->kill();
                    player.addPoints(1);  // 포인트 추가
                    player.addScore(1);   // 스코어 추가
                    enemyKilled = true;
                }
            }

            // 망치와 적 충돌 체크 (망치가 활성화되었을 때만)
            // 손잡이 또는 머리에 닿으면 적 처치
            if (!enemyKilled && hammer.isActive() && (*it)->isAlive()) {
                if (hammer.getHandleBounds().intersects((*it)->getBounds()) ||
                    hammer.getHeadBounds().intersects((*it)->getBounds())) {
                    (*it)->kill();
                    player.addPoints(1);  // 포인트 추가
                    player.addScore(1);   // 스코어 추가
                    enemyKilled = true;
                }
            }

            // 미사일과 적 충돌 체크
            if (!enemyKilled && (*it)->isAlive()) {
                for (auto projIt = projectiles.begin(); projIt != projectiles.end();) {
                    if ((*projIt)->getBounds().intersects((*it)->getBounds())) {
                        (*it)->kill();
                        player.addPoints(1);  // 포인트 추가
                        player.addScore(1);   // 스코어 추가
                        enemyKilled = true;
                        projIt = projectiles.erase(projIt);  // 미사일도 제거
                        break;
                    } else {
                        ++projIt;
                    }
                }
            }

            // 죽은 적 제거
            if (!(*it)->isAlive()) {
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }

        // 텍스트 업데이트
        scoreText.setString("Score: " + std::to_string(player.getScore()));
        pointsText.setString("Points: " + std::to_string(player.getPoints()) +
                            " | Hammer(E): 30 | Dash(R): 15");

        // 렌더링
        window.clear(sf::Color(30, 30, 30));  // 어두운 회색 배경

        // 게임 오브젝트 그리기
        player.draw(window);

        // 미사일 그리기
        for (auto& proj : projectiles) {
            proj->draw(window);
        }

        // 망치 그리기
        hammer.draw(window);

        // 적 그리기
        for (auto& enemy : enemies) {
            enemy->draw(window);
        }

        // UI 그리기
        window.draw(scoreText);
        window.draw(pointsText);
        window.draw(controlsText);

        window.display();
    }

    return 0;
}
