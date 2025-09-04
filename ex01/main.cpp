#include "ScavTrap.hpp"

int main() {
    std::cout << "=== Testing ScavTrap Construction/Destruction Chain ===" << std::endl;
    {
        ScavTrap scav("SC4V-TP");
        std::cout << "\n=== Testing ScavTrap abilities ===" << std::endl;
        scav.attack("enemy robot");
        scav.takeDamage(50);
        scav.beRepaired(20);
        scav.guardGate();
        
        std::cout << "\n=== Testing multiple attacks ===" << std::endl;
        scav.attack("target1");
        scav.attack("target2");
        scav.guardGate();
    }
    
    std::cout << "\n=== Testing inheritance with different ScavTrap ===" << std::endl;
    {
        ScavTrap guardian("GUARDIAN");
        guardian.guardGate();
        guardian.attack("intruder");
        
        std::cout << "\n=== Testing copy functionality ===" << std::endl;
        ScavTrap copy(guardian);
        copy.attack("copied target");
        
        ScavTrap assigned("ASSIGNED");
        assigned = guardian;
        assigned.guardGate();
    }
    
    std::cout << "\n=== Testing energy exhaustion ===" << std::endl;
    {
        ScavTrap energyTest("ENERGY-TEST");
        // Attack 50 times to exhaust energy points
        for (int i = 0; i < 51; i++) {
            energyTest.attack("dummy");
        }
        energyTest.guardGate(); // Should still work
    }
    
    std::cout << "\n=== All destructors will be called in reverse order ===" << std::endl;
    return 0;
}
