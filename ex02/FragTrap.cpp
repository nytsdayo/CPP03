#include "FragTrap.hpp"

FragTrap::FragTrap() : ClapTrap() {
    this->hitPoints = 100;
    this->energyPoints = 100;
    this->attackDamage = 30;
    std::cout << "FragTrap default constructor called" << std::endl;
}

FragTrap::FragTrap(const std::string& name) : ClapTrap(name) {
    this->hitPoints = 100;
    this->energyPoints = 100;
    this->attackDamage = 30;
    std::cout << "FragTrap " << this->name << " constructor called" << std::endl;
}

FragTrap::FragTrap(const FragTrap& other) : ClapTrap(other) {
    std::cout << "FragTrap copy constructor called" << std::endl;
}

FragTrap& FragTrap::operator=(const FragTrap& other) {
    std::cout << "FragTrap assignation operator called" << std::endl;
    if (this != &other) {
        ClapTrap::operator=(other);
    }
    return *this;
}

FragTrap::~FragTrap() {
    std::cout << "FragTrap " << this->name << " destructor called" << std::endl;
}

void FragTrap::highFivesGuys(void) {
    std::cout << "FragTrap " << this->name << " requests a positive high five!" << std::endl;
}
