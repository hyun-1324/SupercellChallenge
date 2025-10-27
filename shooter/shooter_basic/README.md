# Shooter Basic - struct vs class 비교 프로젝트

## 📂 디렉토리 구조

```
shooter_basic/
├── src/                    # ⚠️ 원본 (struct 기반) - 학습 참고용
│   └── main.cpp           # 모든 코드가 한 파일 (280줄)
│
├── include/               # ✅ 리팩토링 (class 기반) - 실전 구조
│   ├── Game.h
│   ├── Player.h
│   ├── Enemy.h
│   └── Bullet.h
│
├── src_refactored/        # ✅ 리팩토링 구현 파일들
│   ├── main.cpp
│   ├── Game.cpp
│   ├── Player.cpp
│   ├── Enemy.cpp
│   └── Bullet.cpp
│
├── CMakeLists.txt         # 리팩토링 버전 빌드
└── README.md              # 이 파일
```

## 🎯 두 버전 비교

### 원본 (struct) - `src/main.cpp`
- **목적**: 학습용, 빠른 프로토타입
- **구조**: 모든 코드가 한 파일에
- **데이터**: 모두 public (직접 접근)
- **파일 수**: 1개

### 리팩토링 (class) - `include/` + `src_refactored/`
- **목적**: 실전 프로젝트 구조
- **구조**: 헤더/구현 분리
- **데이터**: private 캡슐화
- **파일 수**: 9개

## 🚀 빌드 방법

### 리팩토링 버전 빌드 (추천)
```bash
mkdir build_refactored
cd build_refactored
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4
./shooter_basic_refactored
```

### 원본 버전 빌드 (학습용)
```bash
# 직접 컴파일
c++ -std=c++20 src/main.cpp -o shooter_basic_original \
    $(pkg-config --cflags --libs sfml-graphics sfml-window sfml-system)
./shooter_basic_original
```

## 📚 코드 비교 예시

### struct (원본)
```cpp
// src/main.cpp - 모든 코드가 여기에
struct Enemy {
    sf::CircleShape shape;  // public - 누구나 수정 가능
    int hp;
};

int main() {
    std::vector<Enemy> enemies;
    Enemy enemy;
    enemy.hp = -999;  // ❌ 잘못된 값도 대입 가능!

    // 300줄의 게임 로직...
}
```

### class (리팩토링)
```cpp
// Enemy.h
class Enemy {
public:
    void takeDamage(int damage);
    bool isDead() const;

private:
    sf::CircleShape m_shape;  // private - 보호됨
    int m_hp;
};

// Enemy.cpp
void Enemy::takeDamage(int damage) {
    m_hp -= damage;
    if (m_hp < 0) m_hp = 0;  // ✅ 검증된 방식으로만 수정
}
```

## 💡 언제 어떤 걸 쓸까?

| 상황 | 추천 |
|------|------|
| 빠른 프로토타입 | struct (원본) |
| 학습/실험 | struct (원본) |
| 실전 프로젝트 | class (리팩토링) |
| 팀 작업 | class (리팩토링) |
| 게임잼 (48시간) | struct (원본) |
| 유지보수 필요 | class (리팩토링) |

## 🎓 학습 순서

1. **원본 이해**: `src/main.cpp` 읽어보기
2. **리팩토링 비교**: `include/`와 `src_refactored/` 구조 확인
3. **실행 비교**: 두 버전 모두 실행해보기 (기능은 동일)
4. **코드 분석**: 같은 기능을 어떻게 다르게 구현했는지 비교

## 📝 주요 차이점

| 항목 | struct (원본) | class (리팩토링) |
|------|--------------|-----------------|
| 파일 구조 | 1개 | 9개 |
| 접근 제어 | public | private + public |
| 캡슐화 | ❌ | ✅ |
| 유지보수 | ⭐⭐ | ⭐⭐⭐⭐⭐ |
| 작성 속도 | 빠름 | 느림 |
| 확장성 | 낮음 | 높음 |

## ⚠️ 중요 안내

- **원본 `src/main.cpp`는 삭제하지 마세요!**
- 두 버전을 비교하며 학습하기 위한 자료입니다.
- 실제 사용은 리팩토링 버전(`src_refactored/`)을 추천합니다.

---

더 자세한 내용은 `README_REFACTORED.md`를 참고하세요!
