#include <stdio.h>
#include "../src/quantum_sim.h"

void circuit(void){
  
  QubitState* state = initializeState(3);
  applyX(state,2);
  printState(state);
  
  
}
