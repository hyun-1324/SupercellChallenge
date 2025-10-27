# Merge Game - SFML

클래스 기반 Merge Game (2025 게임잼 대비)

## 🎮 게임 플레이

- **목표**: 같은 숫자를 합쳐서 높은 레벨 만들기
- **조작**: 마우스 드래그로 아이템 이동
- **합치기**: 같은 숫자를 다른 같은 숫자 위에 드래그
- **자동 생성**: 3초마다 랜덤 위치에 레벨 1-2 아이템 생성
- **점수**: 합칠 때마다 `레벨 × 10` 점수 획득

## 📁 프로젝트 구조

```
merge_game/
├── include/
│   ├── Game.h          # 게임 메인 루프
│   ├── Grid.h          # 8x8 그리드
│   └── Item.h          # 아이템 (레벨별 색상)
├── src/
│   ├── main.cpp
│   ├── Game.cpp
│   ├── Grid.cpp
│   └── Item.cpp
└── CMakeLists.txt
```

## 🏗️ 핵심 시스템

### 1. Grid System (8x8 그리드)
```cpp
class Grid {
    bool isEmpty(int x, int y);
    void setItem(int x, int y, Item* item);
    Item* getItem(int x, int y);
    sf::Vector2f gridToWorld(int x, int y);
};
```
- 640x640 픽셀 (80px 셀)
- 포인터 배열로 아이템 관리
- 월드 좌표 ↔ 그리드 좌표 변환

### 2. Item (아이템)
```cpp
class Item {
    int m_level;  // 1-7
    sf::RectangleShape m_shape;
    bool m_isDragging;

    void updateColor();  // 레벨별 색상
};
```
- **레벨 1**: 연한 빨강
- **레벨 2**: 주황
- **레벨 3**: 노랑
- **레벨 4**: 초록
- **레벨 5**: 파랑
- **레벨 6**: 보라
- **레벨 7**: 분홍 (최고 레벨)

### 3. Drag & Drop System
```cpp
void handleMousePress(sf::Vector2i mousePos);
void handleMouseRelease(sf::Vector2i mousePos);
void handleMouseMove(sf::Vector2i mousePos);
```
- 마우스 클릭으로 아이템 선택
- 드래그 중 그리드에서 임시 제거
- 드롭 위치 검증:
  - 같은 레벨 → 합치기
  - 빈 칸 → 이동
  - 그 외 → 원위치

### 4. Merge Logic
```cpp
// Game.cpp:150-170
if (targetItem && targetItem->getLevel() == m_draggedItem->getLevel()) {
    int newLevel = targetItem->getLevel() + 1;
    m_score += newLevel * 10;

    // Remove both items
    // Create new merged item at newLevel
}
```
- 같은 레벨 2개 → 레벨 +1
- 양쪽 아이템 삭제
- 새 아이템 생성
- 점수 추가

### 5. Auto Spawn System
```cpp
void spawnRandomItem() {
    // Find all empty cells
    // Pick random empty cell
    // Spawn level 1 or 2 item
}
```
- 3초마다 자동 생성
- 레벨 1 또는 2만 생성
- 빈 칸이 있을 때만 생성

## 🚀 빌드 방법

```bash
cd merge_game
mkdir build && cd build
cmake ..
make -j4
./merge_game
```

## 🎯 12시간 게임잼 확장 아이디어

### 쉬운 확장 (2-3시간)

#### 1. 특수 아이템
```cpp
enum class ItemType {
    Normal,
    Bomb,      // 주변 아이템 제거
    Rainbow,   // 모든 레벨과 합쳐짐
    Double     // 점수 2배
};
```

#### 2. 목표 시스템
```cpp
class Goal {
    int m_targetLevel;  // "레벨 7을 3개 만드세요"
    int m_targetCount;
    int m_currentCount;
};
```

#### 3. 콤보 시스템
```cpp
void handleMerge() {
    m_comboTimer = 3.0f;  // 3초 안에 연속 합치기
    m_combo++;
    m_score += baseScore * m_combo;  // 콤보배수
}
```

### 중간 확장 (4-5시간)

#### 4. 장애물 타일
```cpp
class Grid {
    enum class CellType {
        Empty,
        Blocked,   // 배치 불가
        Ice,       // 1회만 사용 가능
        Lava       // 매 턴 데미지
    };
};
```

#### 5. 아이템 진화 트리
```cpp
// 레벨 3 + 레벨 3 → 레벨 4 또는 특수 아이템
struct MergeResult {
    int newLevel;
    ItemType specialType;
    float probability;  // 10% 확률로 특수 아이템
};
```

### 고급 확장 (6-8시간)

#### 6. 레벨 시스템
```cpp
class Level {
    Grid m_grid;
    std::vector<Goal> m_goals;
    float m_timeLimit;
    CellType m_specialCells[8][8];
};
```

#### 7. 애니메이션
```cpp
class Animation {
    void mergeAnimation(sf::Vector2f from, sf::Vector2f to);
    void spawnAnimation(Item* item);
    void scorePopup(int score, sf::Vector2f pos);
};
```

## 📊 타임라인 (12시간)

```
Hour 0-2:   베이스 이해 & 빌드
Hour 2-4:   특수 아이템 추가
Hour 4-6:   목표 시스템 추가
Hour 6-8:   콤보 & 애니메이션
Hour 8-10:  레벨 3-5개 디자인
Hour 10-12: 밸런싱 & UI 폴리싱
```

## 🎓 핵심 알고리즘

### Drag & Drop
```
1. mousePress: 아이템 선택, 그리드에서 제거
2. mouseMove: 마우스 따라 이동
3. mouseRelease:
   - 같은 레벨 위? → 합치기
   - 빈 칸? → 이동
   - 그 외 → 원위치
```

### Merge Detection
```
1. 드롭 위치의 아이템 확인
2. 레벨 비교
3. 같으면:
   - 양쪽 삭제
   - 새 아이템 생성 (레벨+1)
   - 점수 추가
```

## 💡 학습 포인트

1. **Grid 시스템**: 포인터 배열로 객체 관리
2. **드래그 앤 드롭**: 마우스 이벤트 처리
3. **벡터 사용**: std::vector로 동적 아이템 관리
4. **스마트 포인터**: std::unique_ptr로 메모리 안전
5. **게임 루프**: deltaTime 기반 타이머

## ⚙️ 밸런싱 팁

```cpp
// 아이템 생성
생성 간격: 3초
생성 레벨: 1-2 (랜덤)

// 점수
레벨 2 합치기: 30점
레벨 3 합치기: 40점
레벨 7 합치기: 80점

// 그리드
크기: 8x8 (64칸)
시작 아이템: 5개
```

## 🔧 디버깅 팁

```cpp
// 그리드 상태 출력
for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
        Item* item = m_grid->getItem(x, y);
        if (item) {
            std::cout << item->getLevel();
        } else {
            std::cout << ".";
        }
    }
    std::cout << "\n";
}

// 드래그 상태 시각화
if (m_draggedItem) {
    std::cout << "Dragging level " << m_draggedItem->getLevel()
              << " from " << m_dragStartGridPos.x << "," << m_dragStartGridPos.y << "\n";
}
```

## 🎮 Tower Defense vs Merge Game 비교

| 특징 | Tower Defense | Merge Game |
|------|--------------|------------|
| 난이도 | 중간 | 쉬움 |
| 핵심 알고리즘 | A* pathfinding | Drag & Drop |
| 게임 템포 | 빠름 (실시간) | 느림 (퍼즐) |
| 구현 시간 | 6-8시간 | 4-6시간 |
| 확장성 | 높음 | 매우 높음 |

---

**Merge Game 완성! 이제 Tower Defense와 Merge Game 모두 준비됐습니다!** 🎮🏆

**다음 단계**: Idle Clicker 또는 Match-3 중 선택?
