# C++ 継承プロジェクト (CPP03)

## 目次
- [継承とは？](#継承とは)
  - [継承の基本概念](#継承の基本概念)
  - [継承の種類](#継承の種類)
  - [アクセス指定子](#アクセス指定子)
  - [継承の利点](#継承の利点)
  - [多態性（Polymorphism）とは？](#多態性polymorphismとは)
  - [構築子と破壊子の呼び出し順序](#構築子と破壊子の呼び出し順序)
  - [関数のオーバーライド（上書き）](#関数のオーバーライド上書き)
  - [継承の階層構造](#継承の階層構造)
  - [実際の使用例](#実際の使用例)
  - [継承設計のベストプラクティス](#継承設計のベストプラクティス)
- [このプロジェクトの学習目標](#このプロジェクトの学習目標)
- [クラスの性能比較](#クラスの性能比較)
- [プロジェクト実行方法](#プロジェクト実行方法)
- [継承に関するよくある質問](#継承に関するよくある質問)
- [さらなる学習リソース](#さらなる学習リソース)

## 継承とは？

継承（Inheritance）は、オブジェクト指向プログラミング（OOP）の核となる概念の一つです。
既存のクラス（基底クラス・親クラス）から新しいクラス（派生クラス・子クラス）を作成する仕組みです。

### 継承の基本概念

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

### 継承の種類

#### 1. public継承
- **最も一般的な継承方法**
- 基底クラスの`public`メンバは派生クラスでも`public`のまま
- 基底クラスの`protected`メンバは派生クラスでも`protected`のまま
- 「is-a」関係を表現（ScavTrap **は** ClapTrapである）

```cpp
class ScavTrap : public ClapTrap {
    // ScavTrap is a ClapTrap
};
```

#### 2. protected継承・private継承
- 特殊な用途で使用（このプロジェクトでは使用しない）
- アクセス権限が制限される

### アクセス指定子

| 指定子 | 同じクラス内 | 派生クラス | 外部 |
|--------|-------------|-----------|------|
| `private` | ✓ | ✗ | ✗ |
| `protected` | ✓ | ✓ | ✗ |
| `public` | ✓ | ✓ | ✓ |

**重要**: このプロジェクトでClapTrapのメンバを`protected`にしているのは、派生クラス（ScavTrap、FragTrap）からアクセスできるようにするためです。

### 継承の利点

#### 1. コードの再利用
```cpp
// ClapTrapの機能をScavTrapが自動的に継承
ScavTrap scav("Guardian");
scav.attack("enemy");      // ClapTrapから継承
scav.takeDamage(50);       // ClapTrapから継承  
scav.beRepaired(20);       // ClapTrapから継承
scav.guardGate();          // ScavTrap独自の機能
```

#### 2. 拡張性
- 基底クラスを変更することなく、新しい機能を追加できる
- 既存のコードを壊すことなく、機能を拡張できる

#### 3. 多態性（Polymorphism）の基盤
```cpp
ClapTrap* robot = new ScavTrap("Guardian");
robot->attack("enemy");  // ScavTrapのattack()が呼ばれる（virtual関数の場合）
```

### 多態性（Polymorphism）とは？

多態性（Polymorphism）は、**同じインターフェースで異なる動作を実現する仕組み**です。「多くの形を持つ」という意味で、一つの基底クラス型のポインタや参照を使って、実際には異なる派生クラスのオブジェクトを操作できます。

#### 多態性の基本例
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

#### 仮想関数（Virtual Functions）
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

#### なぜ仮想デストラクタが必要？

**問題のあるコード（仮想デストラクタなし）:**
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

**正しいコード（仮想デストラクタあり）:**
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

#### 多態性の実践例

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

#### 静的バインディング vs 動的バインディング

**静的バインディング（virtual なし）:**
```cpp
ClapTrap* robot = new ScavTrap("Guard");
robot->attack("enemy");  // コンパイル時に決定 → ClapTrap::attack()が呼ばれる
```

**動的バインディング（virtual あり）:**
```cpp
ClapTrap* robot = new ScavTrap("Guard");
robot->attack("enemy");  // 実行時に決定 → ScavTrap::attack()が呼ばれる
```

#### 多態性のメリット

1. **柔軟性**: 新しい派生クラスを追加しても既存のコードを変更不要
2. **保守性**: 共通のインターフェースで異なるオブジェクトを統一的に扱える
3. **拡張性**: プラグイン的な設計が可能

#### 注意点

1. **パフォーマンス**: 仮想関数は若干のオーバーヘッドがある
2. **メモリ**: 仮想関数テーブル（vtable）が必要
3. **設計**: 適切な抽象化レベルの設定が重要

このプロジェクトでは基本的な継承を学習しているため、必須ではありませんが、本格的なC++開発では多態性は非常に重要な概念です。

### 構築子と破壊子の呼び出し順序

継承において重要な概念の一つが、オブジェクトの生成と破棄の順序です：

#### 構築順序：基底クラス → 派生クラス
```cpp
ScavTrap scav("Guardian");
// 出力:
// ClapTrap Guardian constructor called
// ScavTrap Guardian constructor called
```

#### 破棄順序：派生クラス → 基底クラス（逆順）
```cpp
{
    ScavTrap scav("Guardian");
} // スコープ終了
// 出力:
// ScavTrap Guardian destructor called
// ClapTrap Guardian destructor called
```

**理由**: 派生クラスは基底クラスに依存しているため、基底クラスを先に構築し、後から破棄する必要があります。

### 関数のオーバーライド（上書き）

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

### 継承の階層構造

このプロジェクトの継承構造：

```
     ClapTrap (基底クラス)
    /        \
ScavTrap   FragTrap (派生クラス)
```

### 実際の使用例

```cpp
int main() {
    // 基底クラス
    ClapTrap basic("BASIC");
    basic.attack("target");  // 基本の攻撃（ダメージ0）
    
    // 派生クラス1
    ScavTrap guard("GUARD");
    guard.attack("target");   // 強化された攻撃（ダメージ20）
    guard.guardGate();        // 独自機能
    
    // 派生クラス2  
    FragTrap frag("FRAG");
    frag.attack("target");    // さらに強化された攻撃（ダメージ30）
    frag.highFivesGuys();     // 独自機能
    
    return 0;
}
```

### 継承設計のベストプラクティス

1. **適切なアクセス指定子を使用**
   - 派生クラスでアクセスが必要なメンバは`protected`
   - 外部からアクセスが必要なメンバは`public`
   - 内部実装のみのメンバは`private`

2. **仮想デストラクタの使用**
   ```cpp
   virtual ~ClapTrap();  // 多態性を使用する場合は必須
   ```

3. **明確な「is-a」関係**
   - ScavTrap **は** ClapTrap（Guard robot **は** Robot）
   - 継承は「has-a」関係ではなく「is-a」関係で使用

4. **機能の段階的拡張**
   - 基底クラス：基本機能
   - 派生クラス：特化された機能を追加

## このプロジェクトの学習目標

### Exercise 00: ClapTrap（基底クラス）
- **目標**: 基本的なクラス設計とカプセル化
- **学習内容**: 
  - プライベート属性とパブリックメソッド
  - コンストラクタ/デストラクタ
  - 基本的なロジック実装（エネルギー管理など）

### Exercise 01: ScavTrap（継承の基本）
- **目標**: 基本的な継承の理解
- **学習内容**:
  - `public`継承の使用方法
  - 基底クラスのメンバ変数を`protected`にする理由
  - 派生クラスでの属性の再初期化
  - 関数のオーバーライド
  - 構築/破棄チェーンの理解

### Exercise 02: FragTrap（継承の応用）
- **目標**: 複数の派生クラスと継承の柔軟性
- **学習内容**:
  - 同じ基底クラスから異なる派生クラスを作成
  - 各派生クラスが独自の特性を持つ設計
  - 継承の再利用性の実感

### 段階的学習アプローチ
```
Step 1: 基底クラス設計
       ↓
Step 2: 単一継承（ScavTrap）
       ↓  
Step 3: 複数派生クラス（FragTrap）
       ↓
（Step 4: より複雑な継承（DiamondTrap - ex03予定））
```

## クラスの性能比較

| 特徴                      | ClapTrap      | ScavTrap                     | FragTrap                                               |
| :---------------------- | :------------ | :--------------------------- | :----------------------------------------------------- |
| **継承元**                 | なし (基底クラス)    | ClapTrap                     | ClapTrap                                               |
| **ヒットポイント**             | 10            | 100                          | 100                                                    |
| **エネルギーポイント**           | 10            | 50                           | 100                                                    |
| **攻撃ダメージ**              | 0             | 20                           | 30                                                     |
| **特殊能力**                | なし            | `guardGate()` (ゲートキーパーモード表示) | `highFivesGuys()` (ハイタッチ要求表示)                          |
| **コンストラクタ/デストラクタメッセージ** | 独自のメッセージを出力する | ClapTrapとは異なる独自のメッセージを出力する   | ClapTrapとは異なる独自のメッセージを出力する                             |
| **attack() メッセージ**      | 独自のメッセージを出力する | ClapTrapとは異なる独自のメッセージを出力する   | ClapTrapと同じメッセージを出力する（明示されていませんが、ScavTrapのみ異なる記述があるため） |


## 継承に関するよくある質問

### Q1: なぜClapTrapの属性を`protected`にするのですか？
**A**: `private`だと派生クラス（ScavTrap、FragTrap）からアクセスできません。`public`だと外部からもアクセスできてしまいます。`protected`は派生クラスからのみアクセス可能で、カプセル化を保ちながら継承を実現できます。

### Q2: 仮想デストラクタ（`virtual ~ClapTrap()`）は必要ですか？
**A**: このプロジェクトでは必須ではありませんが、良い習慣です。将来的に多態性（ポインタを使った基底クラス参照）を使用する場合、適切なデストラクタが呼ばれることを保証します。

### Q3: 派生クラスで基底クラスのコンストラクタを呼ぶ理由は？
**A**: 
```cpp
ScavTrap::ScavTrap(const std::string& name) : ClapTrap(name) {
    // ClapTrap(name)を先に呼んで基底クラス部分を初期化
    this->hitPoints = 100;  // その後、派生クラス固有の初期化
}
```
基底クラスの初期化を確実に行うためです。

### Q4: `attack()`関数をオーバーライドする理由は？
**A**: 各クラスで異なるメッセージを表示するためです。機能は同じですが、出力メッセージでクラスの違いを明確にします。

### Q5: コピーコンストラクタと代入演算子も継承されますか？
**A**: 自動的には継承されません。適切な動作のために、派生クラスで明示的に定義する必要があります：
```cpp
ScavTrap::ScavTrap(const ScavTrap& other) : ClapTrap(other) {
    // 基底クラス部分をコピー後、派生クラス固有の処理
}
```

