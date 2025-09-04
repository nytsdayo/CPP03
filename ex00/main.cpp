#include "ClapTrap.hpp"

int main() {
    std::cout << "=== Creating ClapTraps ===" << std::endl;
    ClapTrap clap1("CLAP-01");
    ClapTrap clap2("CLAP-02");
    
    std::cout << "\n=== Basic Actions ===" << std::endl;
    clap1.attack("target dummy");
    clap2.takeDamage(5);
    clap2.beRepaired(3);
    
    std::cout << "\n=== Testing Energy Exhaustion ===" << std::endl;
    ClapTrap clap3("ENERGY-TEST");
    // Attack 10 times to exhaust energy (10 energy points)
    for (int i = 0; i < 10; i++) {
        clap3.attack("target");
    }
    // This should fail due to no energy
    clap3.attack("target");
    clap3.beRepaired(1);
    
    std::cout << "\n=== Testing Hit Points Exhaustion ===" << std::endl;
    ClapTrap clap4("HP-TEST");
    clap4.takeDamage(15); // More damage than HP (10)
    clap4.attack("target"); // Should fail due to no HP
    clap4.beRepaired(1); // Should fail due to no HP
    
    std::cout << "\n=== Testing Copy Constructor and Assignment ===" << std::endl;
    ClapTrap clap5("ORIGINAL");
    ClapTrap clap6(clap5); // Copy constructor
    ClapTrap clap7 = clap5; // Copy constructor
    clap7 = clap6; // Assignment operator
    
    std::cout << "\n=== Destructors will be called ===" << std::endl;
    return 0;
}
