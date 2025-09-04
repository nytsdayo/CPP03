# アクセス指定子とカプセル化

## アクセス指定子の種類と特徴

| 指定子 | 同じクラス内 | 派生クラス | 外部 | 用途 |
|--------|-------------|-----------|------|------|
| `private` | ✓ | ✗ | ✗ | 内部実装のみ |
| `protected` | ✓ | ✓ | ✗ | 継承で使用 |
| `public` | ✓ | ✓ | ✓ | 外部インターフェース |

## アクセス指定子の実践的な使い分け

### `private` - 内部実装の隠蔽

```cpp
class ClapTrap {
private:
    std::string name;           // 外部から直接変更されたくない
    unsigned int hitPoints;     // 内部状態の管理
    unsigned int energyPoints;  // バリデーション必要な値
    unsigned int attackDamage;  // 計算ロジックを通してのみ変更

public:
    // 制御されたアクセス方法を提供
    void setAttackDamage(unsigned int damage) {
        if (damage <= MAX_DAMAGE) {  // バリデーション
            attackDamage = damage;
        }
    }
};
```

### `protected` - 継承での共有

```cpp
class ClapTrap {
protected:
    std::string name;           // 派生クラスで直接アクセス可能
    unsigned int hitPoints;     // 派生クラスで初期化必要
    unsigned int energyPoints;  // 派生クラスで管理必要
    unsigned int attackDamage;  // 派生クラスで設定必要

public:
    void attack(const std::string& target);
};

class ScavTrap : public ClapTrap {
public:
    ScavTrap(const std::string& name) : ClapTrap(name) {
        // protectedメンバに直接アクセス可能
        this->hitPoints = 100;
        this->energyPoints = 50;
        this->attackDamage = 20;
    }
};
```

### `public` - 外部インターフェース

```cpp
class ClapTrap {
public:
    // コンストラクタ・デストラクタ
    ClapTrap(const std::string& name);
    ~ClapTrap();
    
    // 公開API
    void attack(const std::string& target);
    void takeDamage(unsigned int amount);
    void beRepaired(unsigned int amount);
};
```

## カプセル化の原則

### 1. データの隠蔽
```cpp
class ClapTrap {
private:
    unsigned int hitPoints;  // 直接操作不可
    
public:
    // 制御されたアクセス
    void takeDamage(unsigned int amount) {
        if (amount >= hitPoints) {
            hitPoints = 0;  // 負の値にならないよう制御
        } else {
            hitPoints -= amount;
        }
    }
    
    // 読み取り専用アクセス
    unsigned int getHitPoints() const {
        return hitPoints;
    }
};
```

### 2. 実装の隠蔽
```cpp
class ClapTrap {
private:
    // 内部ヘルパー関数
    bool canPerformAction() const {
        return hitPoints > 0 && energyPoints > 0;
    }
    
public:
    void attack(const std::string& target) {
        if (canPerformAction()) {  // 内部ロジックを隠蔽
            // 攻撃処理
        }
    }
};
```

## 継承における注意点

### なぜClapTrapの属性を`protected`にするのか？

**問題のあるケース（`private`使用）:**
```cpp
class ClapTrap {
private:
    unsigned int hitPoints;  // 派生クラスからアクセス不可

public:
    ClapTrap() : hitPoints(10) {}
};

class ScavTrap : public ClapTrap {
public:
    ScavTrap() : ClapTrap() {
        // hitPoints = 100;  // ❌ コンパイルエラー！
    }
};
```

**正しいケース（`protected`使用）:**
```cpp
class ClapTrap {
protected:
    unsigned int hitPoints;  // 派生クラスからアクセス可能

public:
    ClapTrap() : hitPoints(10) {}
};

class ScavTrap : public ClapTrap {
public:
    ScavTrap() : ClapTrap() {
        this->hitPoints = 100;  // ✅ 正常にアクセス可能
    }
};
```

## 設計指針

### 1. 最小権限の原則
- 必要最小限のアクセス権限を与える
- `private` > `protected` > `public`の順で検討

### 2. インターフェースの安定性
```cpp
class ClapTrap {
public:
    // 安定したインターフェース
    void attack(const std::string& target);
    
protected:
    // 継承での拡張ポイント
    virtual void performAttack(const std::string& target);
    
private:
    // 変更可能な内部実装
    void logAction(const std::string& action);
};
```

### 3. 責任の分離
```cpp
class ClapTrap {
protected:
    // データ責任
    std::string name;
    unsigned int hitPoints;
    
public:
    // 動作責任
    void attack(const std::string& target);
    void takeDamage(unsigned int amount);
};
```
