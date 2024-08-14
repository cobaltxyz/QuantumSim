#include "quantum_sim.h"
#include <stdio.h>

void circuit() {
    int numQubits = 2;
    QubitState *state = initializeState(numQubits);
 
    printState(state);
    int q[2]; // Array dei qubits
    int c[1]; // Array dei bit classici

    // Istruzione non riconosciuta: OPENQASM 2.0;
    // Istruzione non riconosciuta: include "qelib1.inc";
    // Istruzione non riconosciuta: 
    // Istruzione non riconosciuta: 
    // Istruzione non riconosciuta: 
    // Istruzione non riconosciuta: id q[0];
    applyX(state, 1);
    printf("inverto q1\n");
    printState(state);
    applyHadamard(state, 0);
    printf("H 0\n");
    applyHadamard(state, 1);
    printf("H 1\n");
    printState(state);
    // Istruzione non riconosciuta: //oracolo
    applyCNOT(state, 0, 1);
    printf("Cnot 0->1\n");
    printState(state);
    // Istruzione non riconosciuta: 
    applyHadamard(state, 0);
    applyHadamard(state, 1);
    printf("H 0 and 1\n");
    printState(state);
    //int result = measure(state, 0);
    //c[0] = result;
    //printf("c[0]=%d\n",result);
    //printState(state);
    freeState(state);
}
