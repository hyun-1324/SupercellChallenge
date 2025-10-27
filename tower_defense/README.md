# Tower Defense - SFML

클래스 기반 Tower Defense 게임 (2025 게임잼 대비)

## 🎮 게임 플레이

- **목표**: 적들이 끝(파란 타일)에 도달하지 못하게 막기
- **조작**: 마우스 클릭으로 타워 배치
- **웨이브**: 웨이브마다 적 증가
- **생명**: 20개 (적이 통과하면 -1)
- **골드**: 타워 구매 (100 골드), 적 처치 보상 (25 골드)

## 📁 프로젝트 구조

```
tower_defense/
├── include/
│   ├── Game.h          # 게임 메인 루프
│   ├── Grid.h          # 타일 기반 맵
│   ├── PathFinding.h   # A* 경로 찾기
│   ├── Tower.h         # 타워 (공격)
│   ├── Enemy.h         # 적 (경로 따라 이동)
│   └── Projectile.h    # 투사체
├── src/
│   ├── main.cpp
│   ├── Game.cpp
│   ├── Grid.cpp
│   ├── PathFinding.cpp
│   ├── Tower.cpp
│   ├── Enemy.cpp
│   └── Projectile.cpp
└── CMakeLists.txt
```

## 🏗️ 핵심 시스템

### 1. Grid System (타일 맵)
```cpp
class Grid {
    TileType getTile(int x, int y);
    void setTile(int x, int y, TileType type);
    bool canPlaceTower(int x, int y);
};
```
- 16x12 그리드 (50px 타일)
- 타일 타입: Empty, Path, Tower, Start, End

### 2. A* Pathfinding (경로 찾기)
```cpp
class PathFinding {
    static std::vector<sf::Vector2i> findPath(
        Grid* grid,
        sf::Vector2i start,
        sf::Vector2i end
    );
};
```
- Manhattan 휴리스틱
- 4방향 이동
- Path 타일만 통과 가능

### 3. Tower (타워)
```cpp
class Tower {
    float m_range;      // 사거리
    float m_damage;     // 데미지
    float m_fireRate;   // 공격 속도

    Enemy* findTarget();
    void shoot(Enemy* target);
};
```
- 범위 내 가장 가까운 적 자동 공격
- 투사체 발사

### 4. Enemy (적)
```cpp
class Enemy {
    std::vector<sf::Vector2f> m_path;
    int m_hp;
    float m_speed;

    void update(float deltaTime);
    void takeDamage(int damage);
};
```
- 경로 따라 이동
- HP 바 표시
- HP에 따라 색 변화

### 5. Wave System (웨이브)
```cpp
void Game::spawnWave() {
    m_currentWave++;
    m_enemiesPerWave = 5 + m_currentWave * 2;
}
```
- Wave 1: 5명
- Wave 2: 7명
- Wave 3: 9명...

## 🚀 빌드 방법

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4
./tower_defense
```

## 🎯 12시간 게임잼 확장 아이디어

### 쉬운 확장 (2-3시간)

#### 1. 새 타워 타입
```cpp
class FrostTower : public Tower {
    void shoot(Enemy* target) override {
        target->slow(0.5f);  // 50% 슬로우
    }
};

class AoETower : public Tower {
    void shoot(Enemy* target) override {
        damageNearby(target, 50.0f);  // 범위 공격
    }
};
```

#### 2. 새 적 타입
```cpp
class FastEnemy : public Enemy {
    FastEnemy() : Enemy(path, tileSize) {
        m_speed = 200.0f;  // 2배 빠름
    }
};

class TankEnemy : public Enemy {
    TankEnemy() : Enemy(path, tileSize) {
        m_hp = 300;  // 3배 체력
    }
};
```

#### 3. 타워 업그레이드
```cpp
class Tower {
    int m_level = 1;

    void upgrade() {
        m_level++;
        m_damage *= 1.5f;
        m_range += 25.0f;
    }
};
```

### 중간 확장 (4-5시간)

#### 4. 복잡한 경로
```cpp
// L자 경로
void createLPath() {
    for (int x = 0; x < 8; x++) {
        grid->setTile(x, 5, TileType::Path);
    }
    for (int y = 5; y < 10; y++) {
        grid->setTile(8, y, TileType::Path);
    }
}
```

#### 5. 특수 능력
```cpp
class PoisonTower : public Tower {
    void applyDotDamage(Enemy* target) {
        target->addDamageOverTime(5, 3.0f);  // 3초간 5 DPS
    }
};
```

### 고급 확장 (6-8시간)

#### 6. 레벨 시스템
```cpp
class Level {
    Grid m_grid;
    std::vector<sf::Vector2i> m_path;
    int m_startGold;
    int m_waves;

    void load(std::string filename);
};
```

## 📊 타임라인 (12시간)

```
Hour 0-2:   베이스 이해 & 빌드
Hour 2-5:   새 타워 타입 2-3개 추가
Hour 5-8:   새 적 타입 2-3개 추가
Hour 8-10:  업그레이드 시스템
Hour 10-12: 밸런싱 & UI 폴리싱
```

## 🎓 핵심 알고리즘

### A* Pathfinding
```
1. Open List에 시작 노드 추가
2. Open List에서 f 값이 가장 낮은 노드 선택
3. 목적지에 도달하면 경로 반환
4. 인접 노드들 탐색
5. g, h, f 값 계산
6. 2번으로 돌아가기
```

### Tower Targeting
```
1. 범위 내 모든 적 검색
2. 가장 가까운 적 선택
3. 투사체 발사
4. 쿨다운 시작
```

## 💡 학습 포인트

1. **Grid 시스템**: 타일 기반 게임 기초
2. **A* 알고리즘**: 경로 찾기 필수 지식
3. **클래스 설계**: Tower, Enemy 확장 가능 구조
4. **벡터 수학**: 방향, 거리 계산
5. **게임 루프**: deltaTime 기반 업데이트

## ⚙️ 밸런싱 팁

```cpp
// 타워
타워 비용: 100 골드
공격력: 25
사거리: 150px
공격 속도: 1.0 (1초)

// 적
HP: 100
속도: 100px/s
보상: 25 골드

// 웨이브
Wave N: 5 + N * 2 명
```

## 🔧 디버깅 팁

```cpp
// 경로 시각화
for (const auto& point : m_path) {
    sf::CircleShape debug(5);
    debug.setPosition(point);
    debug.setFillColor(sf::Color::Green);
    window.draw(debug);
}

// 타워 범위 표시 (이미 구현됨)
m_rangeCircle.setOutlineColor(sf::Color(255, 255, 255, 50));
```

---

**Tower Defense 완성! 이제 12시간 게임잼에서 타워 디펜스가 나와도 문제없습니다!** 🎮🏆
