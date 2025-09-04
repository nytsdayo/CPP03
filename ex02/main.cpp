#include "FragTrap.hpp"

int main() {
    std::cout << "=== Creating FragTrap ===" << std::endl;
    FragTrap frag("FR4G-TP");
    
    std::cout << "\n=== Testing FragTrap abilities ===" << std::endl;
    frag.attack("enemy");
    frag.takeDamage(50);
    frag.beRepaired(20);
    frag.highFivesGuys();
    
    std::cout << "\n=== Creating another FragTrap ===" << std::endl;
    FragTrap frag2("FRAGMENT");
    frag2.highFivesGuys();
    
    std::cout << "\n=== Testing copy and assignment ===" << std::endl;
    FragTrap frag3(frag2);
    frag3.highFivesGuys();
    FragTrap frag4 = frag;
    frag4.attack("another enemy");
    frag4 = frag2;
    frag4.highFivesGuys();
    
    std::cout << "\n=== Testing energy exhaustion ===" << std::endl;
    FragTrap energyTest("ENERGY-TEST");
    for (int i = 0; i < 101; i++) {
        energyTest.attack("dummy");
    }
    energyTest.highFivesGuys();
    
    std::cout << "\n=== Destructors will be called ===" << std::endl;
    return 0;
}
