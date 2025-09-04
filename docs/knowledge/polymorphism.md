# 多態性（Polymorphism）とバインディング

## 多態性とは？

多態性（Polymorphism）は、**同じインターフェースで異なる動作を実現する仕組み**です。「多くの形を持つ」という意味で、一つの基底クラス型のポインタや参照を使って、実際には異なる派生クラスのオブジェクトを操作できます。

## 多態性の基本例

```cpp
// 基底クラスのポインタで派生クラスのオブジェクトを指す
ClapTrap* robot1 = new ClapTrap("Basic");
ClapTrap* robot2 = new ScavTrap("Guard");  // ScavTrapだが、ClapTrap*で参照
ClapTrap* robot3 = new FragTrap("Frag");   // FragTrapだが、ClapTrap*で参照

// 同じメソッド呼び出しでも、実際のオブジェクトに応じて異なる動作
robot1->attack("enemy");  // ClapTrapのattack()が呼ばれる
robot2->attack("enemy");  // ScavTrapのattack()が呼ばれる（virtualの場合）
robot3->attack("enemy");  // FragTrapのattack()が呼ばれる（virtualの場合）
```

## 仮想関数（Virtual Functions）

### 基本的な使い方
多態性を実現するためには、基底クラスで`virtual`キーワードを使用します：

```cpp
class ClapTrap {
public:
    virtual void attack(const std::string& target);  // virtual指定
    virtual ~ClapTrap();  // 仮想デストラクタ
};

class ScavTrap : public ClapTrap {
public:
    void attack(const std::string& target) override;  // オーバーライド
};
```

### `override`キーワード（C++11以降）
```cpp
class ScavTrap : public ClapTrap {
public:
    // override指定で意図を明確化し、ミスを防ぐ
    void attack(const std::string& target) override;
    
    // タイポがあるとコンパイルエラーになる
    // void atack(const std::string& target) override;  // ❌ エラー
};
```

## 静的バインディング vs 動的バインディング

### 静的バインディング（Early Binding）
- コンパイル時に呼び出す関数が決定される
- `virtual`なしの関数呼び出し

```cpp
class ClapTrap {
public:
    void attack(const std::string& target) {  // virtualなし
        std::cout << "ClapTrap attacks!" << std::endl;
    }
};

class ScavTrap : public ClapTrap {
public:
    void attack(const std::string& target) {
        std::cout << "ScavTrap attacks!" << std::endl;
    }
};

// 静的バインディングの例
ClapTrap* robot = new ScavTrap("Guard");
robot->attack("enemy");  // "ClapTrap attacks!" が出力される
```

### 動的バインディング（Late Binding）
- 実行時に実際のオブジェクト型に基づいて関数が決定される
- `virtual`指定の関数呼び出し

```cpp
class ClapTrap {
public:
    virtual void attack(const std::string& target) {  // virtual指定
        std::cout << "ClapTrap attacks!" << std::endl;
    }
};

class ScavTrap : public ClapTrap {
public:
    void attack(const std::string& target) override {
        std::cout << "ScavTrap attacks!" << std::endl;
    }
};

// 動的バインディングの例
ClapTrap* robot = new ScavTrap("Guard");
robot->attack("enemy");  // "ScavTrap attacks!" が出力される
```

## 仮想関数テーブル（vtable）

### 仕組み
```cpp
class ClapTrap {
public:
    virtual void attack();  // vtable[0]
    virtual void defend();  // vtable[1]
    virtual ~ClapTrap();    // vtable[2]
};

class ScavTrap : public ClapTrap {
public:
    void attack() override;      // vtable[0] を上書き
    void guardGate();            // vtable[3] に新規追加
};
```

### メモリレイアウト
```
ClapTrap object:
[vtable pointer] -> [ClapTrap::attack]
[member data]       [ClapTrap::defend]
                    [ClapTrap::~ClapTrap]

ScavTrap object:
[vtable pointer] -> [ScavTrap::attack]  // オーバーライド
[member data]       [ClapTrap::defend]  // 継承
                    [ScavTrap::~ScavTrap] // オーバーライド
                    [ScavTrap::guardGate] // 新規
```

## 仮想デストラクタの重要性

### 問題のあるコード（仮想デストラクタなし）
```cpp
class ClapTrap {
public:
    ~ClapTrap() { /* 基底クラスの破棄処理 */ }  // virtualなし
};

class ScavTrap : public ClapTrap {
private:
    std::string* specialData;  // 動的メモリ
public:
    ScavTrap() { specialData = new std::string("Gate Keeper"); }
    ~ScavTrap() { delete specialData; }  // 重要な破棄処理
};

// 危険なコード
ClapTrap* robot = new ScavTrap("Guard");
delete robot;  // ❌ ClapTrapのデストラクタのみ呼ばれる！
               // ScavTrapのデストラクタが呼ばれず、メモリリーク発生
```

### 正しいコード（仮想デストラクタあり）
```cpp
class ClapTrap {
public:
    virtual ~ClapTrap() { /* 基底クラスの破棄処理 */ }  // virtual指定
};

class ScavTrap : public ClapTrap {
private:
    std::string* specialData;
public:
    ScavTrap() { specialData = new std::string("Gate Keeper"); }
    ~ScavTrap() { delete specialData; }  // 適切に呼ばれる
};

// 安全なコード
ClapTrap* robot = new ScavTrap("Guard");
delete robot;  // ✅ ScavTrapのデストラクタ → ClapTrapのデストラクタの順で呼ばれる
```

## 多態性の実践例

### 異なるオブジェクトの統一管理
```cpp
#include <vector>
#include <memory>

int main() {
    // 異なる種類のロボットを同じ配列で管理
    std::vector<std::unique_ptr<ClapTrap>> robots;
    
    robots.push_back(std::make_unique<ClapTrap>("Basic"));
    robots.push_back(std::make_unique<ScavTrap>("Guardian"));
    robots.push_back(std::make_unique<FragTrap>("Destroyer"));
    
    // 同じループで異なる動作を実行（多態性）
    for (auto& robot : robots) {
        robot->attack("enemy");  // 各クラスの実装に応じて異なる動作
    }
    
    // unique_ptrが自動的に適切なデストラクタを呼び出す
    return 0;
}
```

### ファクトリーパターンでの応用
```cpp
enum class RobotType {
    CLAP_TRAP,
    SCAV_TRAP,
    FRAG_TRAP
};

std::unique_ptr<ClapTrap> createRobot(RobotType type, const std::string& name) {
    switch (type) {
        case RobotType::CLAP_TRAP:
            return std::make_unique<ClapTrap>(name);
        case RobotType::SCAV_TRAP:
            return std::make_unique<ScavTrap>(name);
        case RobotType::FRAG_TRAP:
            return std::make_unique<FragTrap>(name);
        default:
            return nullptr;
    }
}

// 使用例
auto robot = createRobot(RobotType::SCAV_TRAP, "Guardian");
robot->attack("enemy");  // ScavTrapのattackが呼ばれる
```

## 多態性のメリットと注意点

### メリット
1. **柔軟性**: 新しい派生クラスを追加しても既存のコードを変更不要
2. **保守性**: 共通のインターフェースで異なるオブジェクトを統一的に扱える
3. **拡張性**: プラグイン的な設計が可能

### 注意点
1. **パフォーマンス**: 仮想関数は若干のオーバーヘッド（vtable lookup）がある
2. **メモリ**: 仮想関数テーブル（vtable）が必要
3. **設計**: 適切な抽象化レベルの設定が重要

### パフォーマンス比較
```cpp
// 直接呼び出し（最速）
ScavTrap scav("Guard");
scav.attack("enemy");

// 仮想関数呼び出し（わずかなオーバーヘッド）
ClapTrap* robot = new ScavTrap("Guard");
robot->attack("enemy");  // vtable lookup
```

通常のアプリケーションでは、このオーバーヘッドは問題になりませんが、高性能が要求される場面では考慮が必要です。
