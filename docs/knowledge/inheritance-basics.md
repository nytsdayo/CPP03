# 継承の基本概念

## 継承とは？

継承（Inheritance）は、オブジェクト指向プログラミング（OOP）の核となる概念の一つです。既存のクラス（基底クラス・親クラス）から新しいクラス（派生クラス・子クラス）を作成する仕組みです。

## 基本的な書き方

```cpp
// 基底クラス（Base Class）
class ClapTrap {
protected:  // 派生クラスからアクセス可能
    std::string name;
    unsigned int hitPoints;
    // ...
public:
    void attack(const std::string& target);
    // ...
};

// 派生クラス（Derived Class）
class ScavTrap : public ClapTrap {
public:
    void guardGate();  // 新しい機能を追加
    void attack(const std::string& target) override;  // 既存機能を上書き
};
```

## 継承の種類

### 1. public継承
- **最も一般的な継承方法**
- 基底クラスの`public`メンバは派生クラスでも`public`のまま
- 基底クラスの`protected`メンバは派生クラスでも`protected`のまま
- 「is-a」関係を表現（ScavTrap **は** ClapTrapである）

```cpp
class ScavTrap : public ClapTrap {
    // ScavTrap is a ClapTrap
};
```

### 2. protected継承・private継承
- 特殊な用途で使用（一般的なプロジェクトでは稀）
- アクセス権限が制限される

## 継承の利点

### 1. コードの再利用
```cpp
// ClapTrapの機能をScavTrapが自動的に継承
ScavTrap scav("Guardian");
scav.attack("enemy");      // ClapTrapから継承
scav.takeDamage(50);       // ClapTrapから継承  
scav.beRepaired(20);       // ClapTrapから継承
scav.guardGate();          // ScavTrap独自の機能
```

### 2. 拡張性
- 基底クラスを変更することなく、新しい機能を追加できる
- 既存のコードを壊すことなく、機能を拡張できる

### 3. 多態性（Polymorphism）の基盤
```cpp
ClapTrap* robot = new ScavTrap("Guardian");
robot->attack("enemy");  // ScavTrapのattack()が呼ばれる（virtual関数の場合）
```

## 構築子と破壊子の呼び出し順序

### 構築順序：基底クラス → 派生クラス
```cpp
ScavTrap scav("Guardian");
// 出力:
// ClapTrap Guardian constructor called
// ScavTrap Guardian constructor called
```

### 破棄順序：派生クラス → 基底クラス（逆順）
```cpp
{
    ScavTrap scav("Guardian");
} // スコープ終了
// 出力:
// ScavTrap Guardian destructor called
// ClapTrap Guardian destructor called
```

**理由**: 派生クラスは基底クラスに依存しているため、基底クラスを先に構築し、後から破棄する必要があります。

## 関数のオーバーライド（上書き）

派生クラスで基底クラスの関数を再定義することができます：

```cpp
// ClapTrap（基底クラス）
void attack(const std::string& target) {
    std::cout << "ClapTrap " << name << " attacks..." << std::endl;
}

// ScavTrap（派生クラス）
void attack(const std::string& target) {
    std::cout << "ScavTrap " << name << " attacks..." << std::endl;  // 異なるメッセージ
}
```

## 継承の階層構造

このプロジェクトの継承構造：

```
     ClapTrap (基底クラス)
    /        \
ScavTrap   FragTrap (派生クラス)
```
