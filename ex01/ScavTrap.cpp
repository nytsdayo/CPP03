#include "ScavTrap.hpp"

// ClapTrapのコンストラクタを呼び出し、ScavTrap固有の初期化を行う
ScavTrap::ScavTrap() : ClapTrap() {
    this->hitPoints = 100;
    this->energyPoints = 50;
    this->attackDamage = 20;
    std::cout << "ScavTrap default constructor called" << std::endl;
}

ScavTrap::ScavTrap(const std::string& name) : ClapTrap(name) {
    this->hitPoints = 100;
    this->energyPoints = 50;
    this->attackDamage = 20;
    std::cout << "ScavTrap " << this->name << " constructor called" << std::endl;
}

ScavTrap::ScavTrap(const ScavTrap& other) : ClapTrap(other) {
    std::cout << "ScavTrap copy constructor called" << std::endl;
}

ScavTrap& ScavTrap::operator=(const ScavTrap& other) {
    std::cout << "ScavTrap assignation operator called" << std::endl;
    if (this != &other) {
        ClapTrap::operator=(other);
    }
    return *this;
}

ScavTrap::~ScavTrap() {
    std::cout << "ScavTrap " << this->name << " destructor called" << std::endl;
}

void ScavTrap::attack(const std::string& target) {
    if (this->energyPoints > 0 && this->hitPoints > 0) {
        std::cout << "ScavTrap " << this->name << " attacks " << target << ", causing " << this->attackDamage << " points of damage!" << std::endl;
        this->energyPoints--;
    } else {
        std::cout << "ScavTrap " << this->name << " cannot attack!" << std::endl;
    }
}

void ScavTrap::guardGate() {
    std::cout << "ScavTrap " << this->name << " is now in Gate keeper mode." << std::endl;
}
