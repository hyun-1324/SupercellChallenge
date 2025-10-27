# Idle Clicker (Cookie Clicker) - SFML

클래스 기반 Idle Clicker 게임 (2025 게임잼 대비)

## 🎮 게임 플레이

- **목표**: 쿠키를 클릭해서 최대한 많이 모으기
- **조작**: 큰 쿠키 클릭 → 쿠키 +1
- **업그레이드**: 오른쪽 버튼으로 자동 생산 구매
- **자동 생산**: 1초마다 자동으로 쿠키 증가
- **점점 커지는 숫자**: K, M, B, T 단위 표기

## 📁 프로젝트 구조

```
idle_clicker/
├── include/
│   ├── Game.h          # 게임 메인 루프
│   ├── Button.h        # 클릭 가능한 버튼
│   ├── Upgrade.h       # 업그레이드 (자동 생산)
│   └── Utils.h         # 큰 숫자 포맷팅
├── src/
│   ├── main.cpp
│   ├── Game.cpp
│   ├── Button.cpp
│   └── Upgrade.cpp
└── CMakeLists.txt
```

## 🏗️ 핵심 시스템

### 1. Click System (클릭 시스템)
```cpp
void Game::handleClick(sf::Vector2i mousePos) {
    if (m_clickArea.getGlobalBounds().contains(mousePosF)) {
        m_cookies += m_clickPower;  // 기본 1
        m_clickArea.setScale({1.1f, 1.1f});  // 시각 피드백
    }
}
```
- 큰 쿠키 클릭 → 쿠키 증가
- 클릭 시 쿠키가 커졌다 작아짐 (피드백)
- 마우스 호버 시 살짝 커짐

### 2. Upgrade System (업그레이드)
```cpp
class Upgrade {
    int m_level;           // 구매 횟수
    long long m_cost;      // 현재 비용
    long long m_production; // 초당 생산량
};
```

**5가지 업그레이드:**
- **Cursor** (커서): 비용 15, +1/초
- **Grandma** (할머니): 비용 100, +5/초
- **Farm** (농장): 비용 500, +25/초
- **Mine** (광산): 비용 3,000, +150/초
- **Factory** (공장): 비용 10,000, +500/초

### 3. Auto Production (자동 생산)
```cpp
void Game::update(float deltaTime) {
    m_productionTimer += deltaTime;
    if (m_productionTimer >= 1.0f) {
        m_cookies += m_cookiesPerSecond;
        m_productionTimer = 0.0f;
    }
}
```
- 1초마다 자동으로 쿠키 추가
- cookiesPerSecond = 모든 업그레이드 생산량 합계

### 4. Cost Scaling (비용 증가)
```cpp
void Upgrade::updateCost() {
    // 비용은 1.15배씩 증가
    m_cost = m_baseCost * std::pow(1.15, m_level);
}
```
- Cursor 1개: 15
- Cursor 2개: 17
- Cursor 3개: 19
- ... 지수적 증가

### 5. Big Number Formatting (큰 숫자 표기)
```cpp
std::string formatNumber(long long num) {
    if (num < 1000) return std::to_string(num);
    if (num < 1000000) return std::to_string(num / 1000) + "K";
    if (num < 1000000000) return std::to_string(num / 1000000) + "M";
    // B, T...
}
```
- 1,000 → 1K
- 1,000,000 → 1M
- 1,000,000,000 → 1B
- 1,000,000,000,000 → 1T

### 6. Button System (버튼)
```cpp
class Button {
    void update(sf::Vector2i mousePos);  // 호버 감지
    bool isClicked(sf::Vector2i mousePos);
    void setEnabled(bool enabled);  // 구매 가능 여부
};
```
- 호버 시 밝은 초록색
- 구매 불가 시 회색
- 클릭 감지

## 🚀 빌드 방법

```bash
cd idle_clicker
mkdir build && cd build
cmake ..
make -j4
./idle_clicker
```

## 🎯 12시간 게임잼 확장 아이디어

### 쉬운 확장 (1-2시간)

#### 1. Click Power 업그레이드
```cpp
class ClickPowerUpgrade : public Upgrade {
    void purchase() override {
        m_clickPower *= 2;  // 클릭당 2배
    }
};
```

#### 2. 업적 시스템
```cpp
class Achievement {
    std::string m_name;
    long long m_requirement;  // "쿠키 1000개 모으기"
    bool m_unlocked;
    int m_rewardMultiplier;  // 보상: 생산 1.1배
};
```

#### 3. 프레스티지 시스템
```cpp
void prestige() {
    if (m_cookies >= 1000000) {
        m_prestigePoints++;
        resetGame();  // 모든 진행도 리셋
        m_globalMultiplier = 1.0 + m_prestigePoints * 0.1;  // 10%씩 증가
    }
}
```

### 중간 확장 (3-4시간)

#### 4. 황금 쿠키 (랜덤 이벤트)
```cpp
class GoldenCookie {
    float m_spawnTimer;
    void update(float deltaTime) {
        if (rand() % 1000 < 1) {  // 0.1% 확률
            spawn();  // 화면 랜덤 위치
        }
    }
    void onClick() {
        m_cookies *= 2;  // 2배 보너스
    }
};
```

#### 5. 미니게임
```cpp
class MiniGame {
    // 쿠키를 클릭해서 모으는 미니게임
    void spawnFallingCookies();
    void onCatch() {
        m_cookies += 100;
    }
};
```

### 고급 확장 (5-6시간)

#### 6. 세이브/로드
```cpp
void saveGame() {
    std::ofstream file("save.txt");
    file << m_cookies << "\n";
    file << m_clickPower << "\n";
    for (auto& upgrade : m_upgrades) {
        file << upgrade->getLevel() << "\n";
    }
}
```

#### 7. 멀티플라이어 시스템
```cpp
class Multiplier {
    float m_clickMultiplier;   // 클릭 보너스
    float m_productionMultiplier;  // 생산 보너스
    float m_duration;  // 30초간 2배
};
```

## 📊 타임라인 (12시간)

```
Hour 0-1:   베이스 이해 & 빌드
Hour 1-2:   Click Power 업그레이드 추가
Hour 2-4:   업적 시스템
Hour 4-6:   프레스티지 시스템
Hour 6-8:   황금 쿠키 (랜덤 이벤트)
Hour 8-10:  미니게임
Hour 10-12: 세이브/로드 & UI 폴리싱
```

## 🎓 핵심 개념

### Timer System
```
1. deltaTime으로 타이머 증가
2. 1초 지나면 → 자동 생산
3. 타이머 리셋
```

### Exponential Growth (지수 성장)
```
비용: baseCost × 1.15^level
생산: baseProduction × level

레벨 1: 15 비용, +1/초
레벨 10: 60 비용, +10/초
레벨 100: 17,795 비용, +100/초
```

### Button State Management
```
1. 마우스 위치 확인
2. 호버 상태 업데이트
3. 색상 변경
4. 클릭 감지
```

## 💡 학습 포인트

1. **타이머 시스템**: deltaTime으로 시간 관리
2. **지수 성장**: pow() 함수 사용
3. **UI 업데이트**: 매 프레임마다 텍스트 갱신
4. **버튼 상태**: 호버, 클릭, 비활성화
5. **큰 숫자 처리**: long long 타입
6. **숫자 포맷팅**: K, M, B, T 변환

## ⚙️ 밸런싱 팁

```cpp
// 기본 설정
클릭 파워: 1
시작 쿠키: 0

// 업그레이드 비용 (첫 구매)
Cursor:  15     (+1/s)
Grandma: 100    (+5/s)
Farm:    500    (+25/s)
Mine:    3,000  (+150/s)
Factory: 10,000 (+500/s)

// 비용 증가율
1.15배씩 증가 (Cookie Clicker 기준)

// 목표
- 100 쿠키: 1분 (클릭만)
- 1,000 쿠키: 5분 (Cursor 몇 개)
- 10,000 쿠키: 15분 (Farm까지)
```

## 🎮 3가지 장르 비교

| 특징 | Tower Defense | Merge Game | Idle Clicker |
|------|--------------|------------|--------------|
| 난이도 | 중상 | 중하 | 하 |
| 구현 시간 | 6-8시간 | 4-6시간 | 2-3시간 |
| 핵심 알고리즘 | A* pathfinding | Drag & Drop | Timer |
| 게임 템포 | 빠름 | 중간 | 느림 |
| 확장성 | 높음 | 매우 높음 | 최고 |
| 안전성 | 중간 | 높음 | 최고 |

## 🏆 완성!

**이제 3가지 장르 모두 완성했습니다!**

1. ✅ Tower Defense (A* 경로 찾기)
2. ✅ Merge Game (드래그 앤 드롭)
3. ✅ Idle Clicker (타이머 & UI)

**2025 Supercell 게임잼 준비 완료!** 🎉

---

**실행 방법:**
```bash
cd /Users/hm/VM/projects/idle_clicker
./build/idle_clicker
```

**플레이 팁:**
- 처음엔 클릭으로 쿠키 모으기
- 15 쿠키 모으면 Cursor 구매
- 100 쿠키 모으면 Grandma 구매
- 점점 자동화되면서 쿠키가 빠르게 증가!
