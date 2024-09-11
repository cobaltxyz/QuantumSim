#include "quantum_sim.h"
#include <stdio.h>

void circuit() {
    printf("---- Test con 8 qubit, ignorando i qubit 4, 5, 6 ----\n");

    // Inizializza lo stato a 8 qubit
    QubitState *state = initializeState(8);

    // Definisci i qubit da ignorare (4, 5, 6)
    int ignoreQubits[] = {4, 5, 6};
    int numIgnoreQubits = 3;

    // Verifica tutti i possibili stati di base per i qubit 0-3
    for (int i = 0; i < 16; i++) {
        initializeStateTo(state, i);  // Imposta lo stato sui primi 4 qubit
        printf("Stato di base %d (per i qubit 0-3):\n", i);
        printStateIgnoringQubits(state, ignoreQubits, numIgnoreQubits);  // Ignora i qubit 4, 5, 6
    }

    freeState(state);
}
