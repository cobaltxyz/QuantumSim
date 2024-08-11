#include "quantum_sim.h"
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

#include "quantum_sim.h"
#include <stdio.h>

void circuit() {
    // Test su 1 qubit
    printf("Test su 1 qubit:\n");
    int numQubits = 1;
    QubitState *state = initializeState(numQubits);

    // Caso 1: Stato iniziale |0>
    printf("\nCaso 1: Stato iniziale |0>\n");
    printState(state);

    // Caso 2: Applico X a |0>
    applyX(state, 0);
    printf("Caso 2: Stato dopo X:\n");
    printState(state);

    // Caso 3: Applico Hadamard a |0>
    applyHadamard(state, 0);
    printf("Caso 3: Stato dopo Hadamard:\n");
    printState(state);

    // Caso 4: Applico Z a |0>
    applyZ(state, 0);
    printf("Caso 4: Stato dopo Z:\n");
    printState(state);

    // Pulizia della memoria per 1 qubit
    freeState(state);

    // Test su 2 qubit
    printf("\nTest su 2 qubit:\n");
    numQubits = 2;
    state = initializeState(numQubits);

    // Caso 1: Stato iniziale |00>
    printf("\nCaso 1: Stato iniziale |00>\n");
    printState(state);

    // Caso 2: Applico X al primo qubit |10>
    applyX(state, 0);
    printf("Caso 2: Stato dopo X su qubit 0:\n");
    printState(state);

    // Caso 3: Applico X al secondo qubit |11>
    applyX(state, 1);
    printf("Caso 3: Stato dopo X su qubit 1:\n");
    printState(state);

    // Caso 4: Applico Hadamard al primo qubit
    applyHadamard(state, 0);
    printf("Caso 4: Stato dopo Hadamard su qubit 0:\n");
    printState(state);

    // Caso 5: Applico CNOT con il primo qubit come controllo e il secondo come target
    applyCNOT(state, 0, 1);
    printf("Caso 5: Stato dopo CNOT (qubit 0 come controllo e qubit 1 come target):\n");
    printState(state);

    // Pulizia della memoria per 2 qubit
    freeState(state);

    // Test su 3 qubit
    printf("\nTest su 3 qubit:\n");
    numQubits = 3;
    state = initializeState(numQubits);

    // Caso 1: Stato iniziale |000>
    printf("\nCaso 1: Stato iniziale |000>\n");
    printState(state);

    // Caso 2: Applico X al primo qubit |100>
    applyX(state, 0);
    printf("Caso 2: Stato dopo X su qubit 0:\n");
    printState(state);

    // Caso 3: Applico X al terzo qubit |101>
    applyX(state, 2);
    printf("Caso 3: Stato dopo X su qubit 2:\n");
    printState(state);

    // Caso 4: Stato di Bell |000> + |111>
    initializeStateTo(state, 0);
    applyHadamard(state, 0);
    applyCNOT(state, 0, 1);
    applyCNOT(state, 0, 2);
    printf("Caso 4: Stato di Bell |000> + |111>\n");
    printState(state);

    // Misura finale dello stato di Bell
    int* results = measure_all(state);
    printf("Risultato della misura dello stato di Bell:\n");
    for (int i = 0; i < numQubits; i++) {
        printf("Qubit %d: %d\n", i, results[i]);
    }

    // Pulizia della memoria per 3 qubit
    free(results);
    freeState(state);
}


void ____circuit() {
    int numQubits = 3;
    QubitState *state = initializeState(numQubits);

    // Prepara lo stato di Bell
    initializeStateTo(state, 0);  // Resetta lo stato a |000>
    applyHadamard(state, 0);
    applyCNOT(state, 0, 1);
    applyCNOT(state, 1, 2);

    printf("Stato prima della misura:\n");
    printState(state);

    // Misura tutti i qubit
    int* results = measure_all(state);
    printf("Risultato della misura:\n");
    for (int i = 0; i < numQubits; i++) {
        printf("Qubit %d: %d\n", i, results[i]);
    }
    printState(state);

    free(results);
    freeState(state);
}



void ______circuit() {
    int numQubits = 3;  // Lavoriamo con 3 qubit
    QubitState *state = initializeState(numQubits);

    // Caso 1: Stato iniziale |000>
    printf("Caso 1: Stato iniziale |000>\n");
    int* results = measure_all(state);
    printf("Risultato della misura:\n");
    for (int i = 0; i < numQubits; i++) {
        printf("Qubit %d: %d\n", i, results[i]);
    }
    free(results);
    printState(state);

    // Caso 2: Applico X al qubit 0 (stato |100>)
    applyX(state, 0);
    printf("\nCaso 2: Stato dopo X su qubit 0 |100>\n");
    results = measure_all(state);
    printf("Risultato della misura:\n");
    for (int i = 0; i < numQubits; i++) {
        printf("Qubit %d: %d\n", i, results[i]);
    }
    free(results);
    printState(state);

    // Caso 3: Applico X al qubit 2 (stato |101>)
    applyX(state, 2);
    printf("\nCaso 3: Stato dopo X su qubit 2 |101>\n");
    results = measure_all(state);
    printf("Risultato della misura:\n");
    for (int i = 0; i < numQubits; i++) {
        printf("Qubit %d: %d\n", i, results[i]);
    }
    free(results);
    printState(state);

    // Caso 4: Sovrapposizione |000> e |111> (Applico H a q0, poi e CNOT tra tutti)
    initializeStateTo(state, 0);  // Resetta lo stato a |000>
    
    applyHadamard(state, 0);
    
    applyCNOT(state, 0, 1);
    applyCNOT(state, 1, 2);
    printf("\nCaso 4: Stato di Bell |000> + |111>\n");
    printState(state);
    results = measure_all(state);
    printf("Risultato della misura:\n");
    for (int i = 0; i < numQubits; i++) {
        printf("Qubit %d: %d\n", i, results[i]);
    }
    free(results);
    printState(state);

    // Caso 5: Stato complesso con Hadamard su qubit 0 e CNOT (stato entangled)
    initializeStateTo(state, 0);  // Resetta lo stato a |000>
    applyHadamard(state, 0);
    applyCNOT(state, 0, 1);
    printf("\nCaso 5: Stato entangled |000> + |110>\n");
    results = measure_all(state);
    printf("Risultato della misura:\n");
    for (int i = 0; i < numQubits; i++) {
        printf("Qubit %d: %d\n", i, results[i]);
    }
    free(results);
    printState(state);

    // Libera la memoria dello stato quantistico
    freeState(state);
}



void _circuit() {
    int numQubits = 3;
    QubitState *state = initializeState(numQubits);

    // Inizializzazione dello stato con Hadamard su tutti i qubit
    for (int i = 0; i < numQubits; i++) {
        applyHadamard(state, i);
    }
    printf("Stato dopo l'inizializzazione con Hadamard su tutti i qubit:\n");
    printState(state);

    // Numero di iterazioni di Grover
    int iterations = 1;  // Per 3 qubit, di solito una sola iterazione è sufficiente

    // Stato marcato (ad esempio, lo stato |101>)
    int markedState = 5;  // |101> corrisponde all'indice 5

    for (int k = 0; k < iterations; k++) {
        // Oracolo di Grover (inverte l'ampiezza dello stato marcato)
        printf("\nApplico l'oracolo di Grover per marcare lo stato %d:\n", markedState);
        state->amplitudes[markedState] *= -1;
        printState(state);

        // Diffusione di Grover (Inversione rispetto alla media)
        printf("\nApplico la diffusione di Grover:\n");

        // Applica Hadamard a tutti i qubit
        for (int i = 0; i < numQubits; i++) {
            applyHadamard(state, i);
        }

        // Applica X a tutti i qubit
        for (int i = 0; i < numQubits; i++) {
            applyX(state, i);
        }

        // Applica una porta multi-controlled-Z (in questo caso, controllato su tutti i qubit)
        applyCNOT(state, 0, 2);  // CNOT tra il qubit 0 (controllo) e il qubit 2 (target)
        applyCNOT(state, 1, 2);  // CNOT tra il qubit 1 (controllo) e il qubit 2 (target)
        applyZ(state, 2);        // Applica la porta Z sul target finale

        // Applica X a tutti i qubit
        for (int i = 0; i < numQubits; i++) {
            applyX(state, i);
        }

        // Applica Hadamard a tutti i qubit
        for (int i = 0; i < numQubits; i++) {
            applyHadamard(state, i);
        }

        printState(state);
    }

    // Misura lo stato finale
    long long result = 0;
    double maxAmplitude = 0;

    for (long long i = 0; i < (1 << numQubits); i++) {
        double probability = pow(cabs(state->amplitudes[i]), 2);
        if (probability > maxAmplitude) {
            maxAmplitude = probability;
            result = i;
        }
    }

    printf("Risultato della misura: %lld\n", result);

    // Libera la memoria
    freeState(state);
}


void circuit_test_All() {
    for (int numQubits = 1; numQubits <= 3; numQubits++) {
        printf("\n--- Test per %d qubit ---\n", numQubits);

        QubitState *state = initializeState(numQubits);

        // Test del gate X su ciascun qubit
        for (int i = 0; i < numQubits; i++) {
            printf("\nTest X su |%0*d> applicato al qubit %d:\n", numQubits, 0, i);
            applyX(state, i);
            printState(state);
            // Reset dello stato
            state->amplitudes[0] = 1.0 + 0.0 * I;
            for (int j = 1; j < (1 << numQubits); j++) {
                state->amplitudes[j] = 0.0 + 0.0 * I;
            }
        }

        // Test del gate Hadamard su ciascun qubit
        for (int i = 0; i < numQubits; i++) {
            printf("\nTest Hadamard su |%0*d> applicato al qubit %d:\n", numQubits, 0, i);
            applyHadamard(state, i);
            printState(state);
            // Reset dello stato
            state->amplitudes[0] = 1.0 + 0.0 * I;
            for (int j = 1; j < (1 << numQubits); j++) {
                state->amplitudes[j] = 0.0 + 0.0 * I;
            }
        }

        // Test del gate Z su ciascun qubit
        for (int i = 0; i < numQubits; i++) {
            printf("\nTest Z su |%0*d> applicato al qubit %d:\n", numQubits, 0, i);
            applyZ(state, i);
            printState(state);
            // Reset dello stato
            state->amplitudes[0] = 1.0 + 0.0 * I;
            for (int j = 1; j < (1 << numQubits); j++) {
                state->amplitudes[j] = 0.0 + 0.0 * I;
            }
        }

        // Test del gate CNOT (solo se ci sono almeno 2 qubit)
        if (numQubits > 1) {
            for (int control = 0; control < numQubits; control++) {
                for (int target = 0; target < numQubits; target++) {
                    if (control != target) {
                        printf("\nTest CNOT su |%0*d> con control %d e target %d:\n", numQubits, 0, control, target);
                        applyX(state, control); // Imposta il qubit di controllo a 1
			printState(state);
                        applyCNOT(state, control, target);
			printf("Applicazione CNOT:\n");
                        printState(state);
                        // Reset dello stato
                        state->amplitudes[0] = 1.0 + 0.0 * I;
                        for (int j = 1; j < (1 << numQubits); j++) {
                            state->amplitudes[j] = 0.0 + 0.0 * I;
                        }
                    }
                }
            }
        }

        freeState(state);
    }

    int numQubits = 2;
    QubitState *state = initializeState(numQubits);

    // Stampa dello stato iniziale
    printf("Stato iniziale:\n");
    printState(state);

    // Applica il gate Hadamard al primo qubit (qubit 0)
    printf("\nApplico Hadamard al qubit 0:\n");
    applyHadamard(state, 0);
    printState(state);

    // Applica il gate CNOT con il qubit 0 come controllo e il qubit 1 come target
    printf("\nApplico CNOT con qubit 0 come controllo e qubit 1 come target:\n");
    applyCNOT(state, 0, 1);
    printState(state);

    // Libera la memoria
    freeState(state);
    
}
