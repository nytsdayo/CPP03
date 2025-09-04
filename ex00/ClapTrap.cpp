#include "ClapTrap.hpp"

ClapTrap::ClapTrap() : name("Default"), hitPoints(10), energyPoints(10), attackDamage(0) {
    std::cout << "ClapTrap default constructor called" << std::endl;
}

ClapTrap::ClapTrap(const std::string& name) : name(name), hitPoints(10), energyPoints(10), attackDamage(0) {
    std::cout << "ClapTrap " << this->name << " constructor called" << std::endl;
}

ClapTrap::ClapTrap(const ClapTrap& other) {
    std::cout << "ClapTrap copy constructor called" << std::endl;
    *this = other;
}

ClapTrap& ClapTrap::operator=(const ClapTrap& other) {
    std::cout << "ClapTrap assignation operator called" << std::endl;
    if (this != &other) {
        this->name = other.name;
        this->hitPoints = other.hitPoints;
        this->energyPoints = other.energyPoints;
        this->attackDamage = other.attackDamage;
    }
    return *this;
}

ClapTrap::~ClapTrap() {
    std::cout << "ClapTrap " << this->name << " destructor called" << std::endl;
}

void ClapTrap::attack(const std::string& target) {
    if (this->energyPoints > 0 && this->hitPoints > 0) {
        std::cout << "ClapTrap " << this->name << " attacks " << target << ", causing " << this->attackDamage << " points of damage!" << std::endl;
        this->energyPoints--;
    } else {
        std::cout << "ClapTrap " << this->name << " cannot attack!" << std::endl;
    }
}

void ClapTrap::takeDamage(unsigned int amount) {
    std::cout << "ClapTrap " << this->name << " takes " << amount << " points of damage!" << std::endl;
    if (amount >= this->hitPoints) {
        this->hitPoints = 0;
    } else {
        this->hitPoints -= amount;
    }
}

void ClapTrap::beRepaired(unsigned int amount) {
    if (this->energyPoints > 0 && this->hitPoints > 0) {
        std::cout << "ClapTrap " << this->name << " is repaired for " << amount << " points!" << std::endl;
        this->hitPoints += amount;
        this->energyPoints--;
    } else {
        std::cout << "ClapTrap " << this->name << " cannot be repaired!" << std::endl;
    }
}
