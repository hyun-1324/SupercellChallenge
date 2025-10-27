# Top-Down Shooter - Class-Based Refactored Version

## 📁 프로젝트 구조

```
shooter_basic/
├── include/              # 헤더 파일들
│   ├── Game.h           # 게임 메인 루프 관리
│   ├── Player.h         # 플레이어 클래스
│   ├── Enemy.h          # 적 클래스
│   └── Bullet.h         # 총알 클래스
├── src_refactored/      # 구현 파일들
│   ├── main.cpp         # 프로그램 진입점
│   ├── Game.cpp
│   ├── Player.cpp
│   ├── Enemy.cpp
│   └── Bullet.cpp
├── CMakeLists.txt       # CMake 빌드 설정
└── README_REFACTORED.md # 이 파일
```

## 🎯 변경 사항 (struct → class)

### Before (struct 기반)
```cpp
// 모든 코드가 main.cpp 한 파일에
struct Bullet {
    sf::CircleShape shape;  // public
    sf::Vector2f velocity;  // public
};

int main() {
    std::vector<Bullet> bullets;
    // 300줄의 게임 로직...
}
```

### After (class 기반)
```cpp
// Bullet.h (헤더)
class Bullet {
public:
    Bullet(sf::Vector2f position, sf::Vector2f velocity);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;

private:
    sf::CircleShape m_shape;  // 캡슐화
    sf::Vector2f m_velocity;
};

// Bullet.cpp (구현)
Bullet::Bullet(sf::Vector2f position, sf::Vector2f velocity)
    : m_velocity(velocity)
{
    m_shape.setRadius(5);
    // ...
}
```

## 🏗️ 클래스 구조

### 1. Game (게임 관리자)
- **역할**: 게임 루프, 이벤트 처리, 충돌 감지
- **주요 메서드**:
  - `run()`: 메인 게임 루프
  - `processEvents()`: 입력 이벤트 처리
  - `update(deltaTime)`: 게임 상태 업데이트
  - `render()`: 화면 렌더링
  - `checkCollisions()`: 충돌 감지

### 2. Player (플레이어)
- **역할**: 플레이어 이동, 조준, 사격
- **주요 메서드**:
  - `handleInput(deltaTime)`: 키보드 입력 처리 (WASD)
  - `updateAimLine(mousePos)`: 조준선 업데이트
  - `canShoot()`: 사격 가능 여부 확인
  - `pushBack()`: 적과 충돌 시 밀려남

### 3. Enemy (적)
- **역할**: 플레이어 추적, 피격, 사망
- **주요 메서드**:
  - `update(deltaTime, playerPos)`: 플레이어를 향해 이동
  - `takeDamage(amount)`: 피해 입기
  - `isDead()`: 사망 여부 확인
  - `updateColor()`: HP에 따른 색 변경

### 4. Bullet (총알)
- **역할**: 직선 이동, 화면 밖 감지
- **주요 메서드**:
  - `update(deltaTime)`: 이동
  - `isOffScreen()`: 화면 밖 판정
  - `getBounds()`: 충돌 감지용 경계

## 🔑 핵심 개념

### 캡슐화 (Encapsulation)
```cpp
class Enemy {
private:
    int m_hp;              // 외부에서 직접 수정 불가
    sf::CircleShape m_shape;

public:
    void takeDamage(int damage);  // 제어된 방법으로만 수정
    bool isDead() const;
};
```

### 책임 분리 (Separation of Concerns)
- **Game**: 전체 게임 흐름 관리
- **Player**: 플레이어 행동만 담당
- **Enemy**: 적 행동만 담당
- **Bullet**: 총알 행동만 담당

### 스마트 포인터 사용
```cpp
std::unique_ptr<Player> m_player;
std::vector<std::unique_ptr<Bullet>> m_bullets;
std::vector<std::unique_ptr<Enemy>> m_enemies;
```
→ 자동 메모리 관리 (메모리 누수 방지)

## 🚀 빌드 방법

```bash
# 1. 빌드 디렉토리 생성
mkdir build_refactored && cd build_refactored

# 2. CMake 설정
cmake -DCMAKE_BUILD_TYPE=Release ..

# 3. 빌드
make -j4

# 4. 실행
./shooter_basic_refactored
```

## 🎮 게임 조작법

- **WASD**: 이동
- **마우스**: 조준
- **좌클릭**: 사격

## 📊 성능 특징

- **델타타임 적용**: 모든 컴퓨터에서 동일한 속도
- **벡터 정규화**: 대각선 이동 속도 보정
- **효율적인 삭제**: erase-remove idiom 사용

## 🎓 학습 포인트

1. **struct vs class**: 언제 어떤 걸 쓸까?
2. **헤더/구현 분리**: 왜 .h와 .cpp를 나눌까?
3. **캡슐화**: private로 데이터 보호
4. **스마트 포인터**: 메모리 관리 자동화
5. **const 정확성**: `const` 메서드와 매개변수
6. **책임 분리**: 클래스마다 하나의 역할

## 🔄 원본 vs 리팩토링 비교

| 항목 | 원본 (struct) | 리팩토링 (class) |
|------|--------------|-----------------|
| 파일 수 | 1개 | 9개 (헤더 4 + 구현 5) |
| 코드 줄 수 | ~280줄 | ~400줄 (주석 포함) |
| 유지보수성 | ⭐⭐ | ⭐⭐⭐⭐⭐ |
| 확장성 | ⭐⭐ | ⭐⭐⭐⭐⭐ |
| 학습 난이도 | 쉬움 | 중간 |
| 실무 적합성 | ⭐⭐ | ⭐⭐⭐⭐⭐ |

## 💡 다음 단계

이 리팩토링된 구조를 바탕으로:
1. 파워업 시스템 추가
2. 게임 상태 관리 (메뉴, 일시정지, 게임오버)
3. 사운드 효과 추가
4. 스코어 저장 기능
5. 다양한 무기 타입

---

**실전 프로젝트 구조를 경험해보세요!** 🚀
