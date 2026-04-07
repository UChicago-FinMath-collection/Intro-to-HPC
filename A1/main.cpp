#include <iostream>
#include "patterns.h"

int main() {
    std::cout << "=== Fork-Join Demo ===" << std::endl;
    fork_demo();
    std::cout << "=== Done ===" << std::endl;
    
    std::cout << "=== Reduction Demo ===" << std::endl;
    reduction_demo();
    std::cout << "=== Done ===" << std::endl;





    return 0;
}