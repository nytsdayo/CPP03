# 継承設計のベストプラクティス

## 設計原則

### 1. 適切なアクセス指定子の使用

```cpp
class ClapTrap {
private:
    // 内部実装のみのメンバ
    void logAction(const std::string& action);
    static const unsigned int MAX_DAMAGE = 1000;
    
protected:
    // 派生クラスでアクセスが必要なメンバ
    std::string name;
    unsigned int hitPoints;
    unsigned int energyPoints;
    unsigned int attackDamage;
    
public:
    // 外部からアクセスが必要なメンバ
    ClapTrap(const std::string& name);
    virtual ~ClapTrap();
    void attack(const std::string& target);
    void takeDamage(unsigned int amount);
    void beRepaired(unsigned int amount);
};
```

### 2. 明確な「is-a」関係

**良い例（is-a関係）:**
```cpp
class Vehicle {  // 乗り物
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};

class Car : public Vehicle {  // 車 は 乗り物
public:
    void start() override { /* エンジン始動 */ }
    void stop() override { /* エンジン停止 */ }
};

class Bicycle : public Vehicle {  // 自転車 は 乗り物
public:
    void start() override { /* ペダル開始 */ }
    void stop() override { /* ブレーキ */ }
};
```

**悪い例（has-a関係を継承で表現）:**
```cpp
class Engine {
    void start();
    void stop();
};

// ❌ 悪い設計：車がエンジンを継承
class Car : public Engine {  // 車 は エンジン？（意味的におかしい）
};

// ✅ 正しい設計：車がエンジンを持つ
class Car {
private:
    Engine engine;  // 車 は エンジンを持つ
public:
    void start() { engine.start(); }
    void stop() { engine.stop(); }
};
```

### 3. 仮想デストラクタの使用

```cpp
class Base {
public:
    virtual ~Base() = default;  // 仮想デストラクタを明示的にデフォルト化
    
    // 多態性を使用する場合は必須
    virtual void someFunction() = 0;
};

class Derived : public Base {
private:
    std::unique_ptr<int[]> data;
    
public:
    Derived() : data(std::make_unique<int[]>(100)) {}
    
    // 仮想デストラクタにより適切に呼び出される
    ~Derived() {
        // unique_ptrが自動的にメモリを解放
        std::cout << "Derived destructor called" << std::endl;
    }
    
    void someFunction() override {
        // 実装
    }
};
```

## 継承の設計パターン

### 1. テンプレートメソッドパターン

```cpp
class GameCharacter {
protected:
    // テンプレートメソッド（アルゴリズムの骨格）
    virtual void performAttack() {
        prepareAttack();    // 共通処理
        executeAttack();    // 派生クラス固有
        finishAttack();     // 共通処理
    }
    
    virtual void executeAttack() = 0;  // 純粋仮想関数
    
private:
    void prepareAttack() {
        std::cout << "Preparing to attack..." << std::endl;
    }
    
    void finishAttack() {
        std::cout << "Attack finished!" << std::endl;
    }
    
public:
    void attack() {
        performAttack();
    }
};

class Warrior : public GameCharacter {
protected:
    void executeAttack() override {
        std::cout << "Sword slash!" << std::endl;
    }
};

class Mage : public GameCharacter {
protected:
    void executeAttack() override {
        std::cout << "Magic spell!" << std::endl;
    }
};
```

### 2. ストラテジーパターン（継承版）

```cpp
class AttackStrategy {
public:
    virtual ~AttackStrategy() = default;
    virtual void execute() = 0;
};

class MeleeAttack : public AttackStrategy {
public:
    void execute() override {
        std::cout << "Close combat attack!" << std::endl;
    }
};

class RangedAttack : public AttackStrategy {
public:
    void execute() override {
        std::cout << "Ranged attack!" << std::endl;
    }
};

class Character {
private:
    std::unique_ptr<AttackStrategy> strategy;
    
public:
    Character(std::unique_ptr<AttackStrategy> s) : strategy(std::move(s)) {}
    
    void attack() {
        strategy->execute();
    }
    
    void setStrategy(std::unique_ptr<AttackStrategy> s) {
        strategy = std::move(s);
    }
};
```

## エラーハンドリング

### 1. 例外安全な設計

```cpp
class SafeClapTrap {
private:
    std::string name;
    unsigned int hitPoints;
    std::unique_ptr<std::string> specialData;
    
public:
    SafeClapTrap(const std::string& n) : name(n), hitPoints(10) {
        try {
            specialData = std::make_unique<std::string>("Special");
        } catch (...) {
            // 既にname, hitPointsは設定済み
            // unique_ptrは例外時に自動的にnullptrになる
            throw;
        }
    }
    
    void takeDamage(unsigned int amount) {
        if (amount > hitPoints) {
            hitPoints = 0;  // アンダーフローを防ぐ
        } else {
            hitPoints -= amount;
        }
    }
};
```

### 2. 不変条件の維持

```cpp
class RobustClapTrap {
private:
    std::string name;
    unsigned int hitPoints;
    unsigned int energyPoints;
    
    // 不変条件チェック
    void validateState() const {
        assert(!name.empty());
        assert(hitPoints <= 1000);  // 最大値チェック
        assert(energyPoints <= 1000);
    }
    
public:
    RobustClapTrap(const std::string& n) : name(n), hitPoints(10), energyPoints(10) {
        if (name.empty()) {
            throw std::invalid_argument("Name cannot be empty");
        }
        validateState();
    }
    
    void setHitPoints(unsigned int hp) {
        if (hp > 1000) {
            throw std::invalid_argument("Hit points too large");
        }
        hitPoints = hp;
        validateState();
    }
};
```

## パフォーマンス考慮事項

### 1. 仮想関数のオーバーヘッド最小化

```cpp
class PerformanceClapTrap {
public:
    // 頻繁に呼ばれる関数は非仮想に
    void takeDamage(unsigned int amount) {  // non-virtual
        hitPoints = (amount >= hitPoints) ? 0 : hitPoints - amount;
    }
    
    // 多態性が必要な部分のみ仮想
    virtual void attack(const std::string& target) {
        // 実装
    }
    
    // デストラクタは仮想（安全性のため）
    virtual ~PerformanceClapTrap() = default;
    
protected:
    unsigned int hitPoints;
};
```

### 2. インライン化の活用

```cpp
class InlineClapTrap {
private:
    unsigned int hitPoints;
    
public:
    // 単純なゲッターはインライン化
    inline unsigned int getHitPoints() const {
        return hitPoints;
    }
    
    // 頻繁に使用される簡単な関数
    inline bool isAlive() const {
        return hitPoints > 0;
    }
};
```

## テストの容易性

### 1. 依存性注入

```cpp
class TestableRobot {
private:
    std::unique_ptr<Logger> logger;
    std::unique_ptr<Damage Calculator> calculator;
    
public:
    // 依存性を外部から注入
    TestableRobot(std::unique_ptr<Logger> l, std::unique_ptr<DamageCalculator> c)
        : logger(std::move(l)), calculator(std::move(c)) {}
    
    void attack(const std::string& target) {
        auto damage = calculator->calculate(attackDamage);
        logger->log("Attacking " + target + " for " + std::to_string(damage) + " damage");
    }
};

// テスト用のモック実装
class MockLogger : public Logger {
public:
    void log(const std::string& message) override {
        lastMessage = message;
    }
    std::string lastMessage;
};
```

### 2. 保護された仮想関数

```cpp
class TestableCharacter {
protected:
    // テストで上書き可能な仮想関数
    virtual void logAction(const std::string& action) {
        std::cout << action << std::endl;
    }
    
public:
    void attack(const std::string& target) {
        // ...攻撃処理...
        logAction("Attacked " + target);
    }
};

// テスト用の派生クラス
class TestCharacter : public TestableCharacter {
public:
    std::vector<std::string> actions;
    
protected:
    void logAction(const std::string& action) override {
        actions.push_back(action);  // ログをコンソールではなく配列に保存
    }
};
```
