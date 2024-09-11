
#include "quantum_sim.h"
#include <stdio.h>


void grover_algorithm(QubitState *state) {
    // Applicazione della Hadamard ai qubit 4, 5 e 6 (qubit computazionali)
    applyHadamard(state, 4);
    applyHadamard(state, 5);
    applyHadamard(state, 6);

    // Configurazione del qubit ancillare |1> (applica X al qubit 7)
    applyX(state, 7);

    // Oracle per lo stato target |1000>
    applyX(state, 3); // Inverti il qubit 3
    applyX(state, 2); // Inverti il qubit 2
    applyX(state, 1); // Inverti il qubit 1
    applyCNOT(state, 2, 4); // CNOT tra q2 e q4

    // Rotazioni e applicazione del T-dagger
    applyHadamard(state, 7); 
    applyTdag(state, 4);     
    applyCNOT(state, 1, 4);  
    applyT(state, 4);        
    applyCNOT(state, 2, 4);  
    applyTdag(state, 4);     
    applyT(state, 2);        
    applyCNOT(state, 1, 4);  
    applyT(state, 4);        
    applyCNOT(state, 1, 2);  
    applyHadamard(state, 4); 
    applyTdag(state, 2);     
    applyT(state, 1);        
    applyCNOT(state, 1, 2);  

    // Continua con la diffusione
    applyCNOT(state, 4, 5);
    applyTdag(state, 5);
    applyCNOT(state, 3, 5);
    applyT(state, 5);
    applyCNOT(state, 4, 5);
    applyTdag(state, 5);
    applyT(state, 4);
    applyCNOT(state, 3, 5);
    applyT(state, 5);
    applyCNOT(state, 3, 4);
    applyT(state, 3);
    applyTdag(state, 4);
    applyHadamard(state, 5);
    applyCNOT(state, 3, 4);
    applyCNOT(state, 5, 6);
}

void circuit() {

  int  ignored[] = {4,5,6};
  QubitState* state =  initializeState(8);
  
  //initializeSingleQubitToOne(state, 0);
  //applyX(state,0);
  //printStateIgnoringQubits(state, ignored, 3);
  printState(state);
  printf("\n-----------\n");
  grover_algorithm(state);
  //printStateIgnoringQubits(state, ignored, 3);
  printState(state);
  freeState(state);
    
}
