/*
 * QuantumSim: A Quantum Circuit Simulator for C Programmers
 * Copyright (C) 2024 Francesco Sisini
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "quantum_sim.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <time.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


/**
 * Inizializza lo stato quantistico a uno stato di base specifico.
 *
 * Questa funzione prende in input un puntatore a una struttura `QubitState`, che rappresenta
 * lo stato di un sistema quantistico, e un indice `index` che indica quale stato di base 
 * il sistema dovrebbe assumere. La funzione imposta tutte le ampiezze degli stati quantistici 
 * a zero, eccetto quella corrispondente all'indice specificato, che viene impostata a 1.0, 
 * rappresentando così lo stato di base desiderato.
 *
 * @param state Un puntatore a una struttura `QubitState` che contiene il numero di qubit 
 *              e il vettore delle ampiezze da inizializzare.
 * @param index L'indice dello stato di base desiderato, che va da 0 a 2^numQubits - 1.
 *
 * Dettagli:
 * - `dim` rappresenta la dimensione dello spazio di Hilbert, pari a 2^numQubits.
 * - Tutti gli elementi di `state->amplitudes` vengono inizializzati a 0.0.
 * - L'elemento corrispondente all'indice specificato viene impostato a 1.0 + 0.0 * I, 
 *   indicando che il sistema è nello stato di base associato a quell'indice.
 */
void initializeStateTo(QubitState *state, int index) {
    long long dim = 1LL << state->numQubits;
    for (long long i = 0; i < dim; i++) {
        state->amplitudes[i] = 0.0 + 0.0 * I;
    }
    state->amplitudes[index] = 1.0 + 0.0 * I;  // Imposta l'ampiezza dello stato indicato a 1
}

void printState(QubitState *state) {
    long long dim = 1LL << state->numQubits;
    for (long long i = 0; i < dim; i++) {
        printf("Stato %lld: %f + %fi\n", i, creal(state->amplitudes[i]), cimag(state->amplitudes[i]));
    }
}
void printStateIgnoringQubits(QubitState *state, int *ignoreQubits, int numIgnoreQubits) {
    long long dim = 1LL << state->numQubits;  // Dimensione totale dello stato quantistico
    int ignore = 0;
    long long mappedIndex;

    for (long long i = 0; i < dim; i++) {
        ignore = 0;
        mappedIndex = 0;
        long long shift = 0;

        // Determina se l'indice contiene uno dei qubit da ignorare
        for (int j = 0; j < numIgnoreQubits; j++) {
            int qubitIndex = ignoreQubits[j];
            if (((i >> qubitIndex) & 1) != 0) {
                ignore = 1;  // Se uno dei qubit ignorati è 1, ignora questo stato
                break;
            }
        }

        if (!ignore) {
            // Costruisci il nuovo indice ignorando i qubit specificati
            for (int k = 0; k < state->numQubits; k++) {
                int isIgnored = 0;

                // Verifica se il qubit k è ignorato
                for (int j = 0; j < numIgnoreQubits; j++) {
                    if (ignoreQubits[j] == k) {
                        isIgnored = 1;
                        break;
                    }
                }

                if (!isIgnored) {
                    // Aggiungi il bit k alla posizione corretta di mappedIndex
                    mappedIndex |= (((i >> k) & 1) << shift);
                    shift++;
                }
            }

            // Stampa lo stato con il nuovo indice
            printf("Stato %lld: %f + %fi\n", mappedIndex, creal(state->amplitudes[i]), cimag(state->amplitudes[i]));
        }
    }
}



QubitState* initializeState(int numQubits) {
     QubitState *state = (QubitState *)malloc(sizeof(QubitState));
    state->numQubits = numQubits;
    long long dim = 1LL << numQubits; // 2^numQubits
    state->amplitudes = (double complex *)calloc(dim, sizeof(double complex));

    // Verifica che tutte le ampiezze siano inizializzate a 0
    for (long long i = 0; i < dim; i++) {
        state->amplitudes[i] = 0.0 + 0.0 * I;
    }

    // Imposta lo stato |0>^N
    state->amplitudes[0] = 1.0 + 0.0 * I; 

    return state;
}

void initializeSingleQubitToOne(QubitState* state, int target) {
    long long dim = 1LL << state->numQubits;
    
    // Scorri tutte le ampiezze e modifica solo quelle che hanno il qubit target a 0
    for (long long i = 0; i < dim; i++) {
        if (((i >> (state->numQubits - 1 - target)) & 1) == 0) {
            // Inverti il bit target per impostare lo stato a |1>
            long long j = i ^ (1LL << (state->numQubits - 1 - target));
            state->amplitudes[j] = state->amplitudes[i];
            state->amplitudes[i] = 0.0 + 0.0 * I;
        }
    }
}



void freeState(QubitState *state) {
    free(state->amplitudes);
    free(state);
}
void applySingleQubitGate(QubitState *state, int target, double complex gate[2][2]) {
    long long dim = 1LL << state->numQubits;
    double complex new_amplitudes[dim];

    // Applica il gate al qubit target
    for (long long i = 0; i < dim; i++) {
        int bitValue = (i >> (state->numQubits - 1 - target)) & 1; // Determina se il bit target è 0 o 1
        long long j = i ^ (1LL << (state->numQubits - 1 - target)); // Calcola l'indice con il bit target invertito

        if (bitValue == 0) {
            new_amplitudes[i] = gate[0][0] * state->amplitudes[i] + gate[0][1] * state->amplitudes[j];
        } else {
            new_amplitudes[i] = gate[1][0] * state->amplitudes[j] + gate[1][1] * state->amplitudes[i];
        }
    }

    // Aggiorna le ampiezze nello stato originale
    for (long long i = 0; i < dim; i++) {
        state->amplitudes[i] = new_amplitudes[i];
    }
}

void applyHadamard(QubitState *state, int target) {
    double complex H[2][2] = {
        {1.0 / sqrt(2.0), 1.0 / sqrt(2.0)},
        {1.0 / sqrt(2.0), -1.0 / sqrt(2.0)}
    };
    applySingleQubitGate(state, target, H);
}


void applyX(QubitState *state, int target) {
double complex X[2][2] = {
        {0, 1},
        {1, 0}
    };
    applySingleQubitGate(state, target, X);
}

void applyZ(QubitState *state, int target) {
    double complex Z[2][2] = {
        {1, 0},
        {0, -1}
    };
    applySingleQubitGate(state, target, Z);
}

void applyT(QubitState *state, int target) {
    double complex T[2][2] = {
        {1, 0},
        {0, cexp(I * M_PI / 4.0)}
    };
    applySingleQubitGate(state, target, T);
}

void applyTdag(QubitState *state, int target) {
    double complex Tdag[2][2] = {
        {1, 0},
        {0, cexp(-I * M_PI / 4.0)}
    };
    applySingleQubitGate(state, target, Tdag);
}


void applyS(QubitState *state, int target) {
    double complex S[2][2] = {
        {1, 0},
        {0, I}
    };
    applySingleQubitGate(state, target, S);
}


void applyCNOT(QubitState *state, int control, int target) {
    long long dim = 1LL << state->numQubits;
    double complex new_amplitudes[dim];

    // Copia le ampiezze originali nel nuovo array
    for (long long i = 0; i < dim; i++) {
        new_amplitudes[i] = state->amplitudes[i];
    }

    // Applica il gate CNOT con il controllo e il target numerati da sinistra a destra
    for (long long i = 0; i < dim; i++) {
        // Calcola il bit di controllo e il bit target correttamente orientati
        int control_bit = (i >> (state->numQubits - 1 - control)) & 1;
        int target_bit = (i >> (state->numQubits - 1 - target)) & 1;

        if (control_bit == 1) {
            long long j = i ^ (1LL << (state->numQubits - 1 - target)); // Inverti solo il bit target
            new_amplitudes[i] = state->amplitudes[j];
            new_amplitudes[j] = state->amplitudes[i];
        }
    }

    // Aggiorna le ampiezze nello stato originale
    for (long long i = 0; i < dim; i++) {
        state->amplitudes[i] = new_amplitudes[i];
    }
}
void applyCPhaseShift(QubitState *state, int control, int target, double complex phase) {
    long long dim = 1LL << state->numQubits;
    double complex new_amplitudes[dim];

    // Copia le ampiezze originali nel nuovo array
    for (long long i = 0; i < dim; i++) {
        new_amplitudes[i] = state->amplitudes[i];
    }

    // Applica il gate CPhaseShift con il controllo e il target numerati da sinistra a destra
    for (long long i = 0; i < dim; i++) {
        // Calcola il bit di controllo e il bit target correttamente orientati
        int control_bit = (i >> (state->numQubits - 1 - control)) & 1;
        int target_bit = (i >> (state->numQubits - 1 - target)) & 1;

        // Applica lo shift di fase solo se il qubit di controllo è 1 e il qubit target è 1
        if (control_bit == 1 && target_bit == 1) {
            new_amplitudes[i] *= phase;  // Moltiplica l'ampiezza dello stato per la fase specificata
        }
    }

    // Aggiorna le ampiezze nello stato originale
    for (long long i = 0; i < dim; i++) {
        state->amplitudes[i] = new_amplitudes[i];
    }
}


MeasurementResult measure(QubitState *state, int qubit) {
    long long dim = 1LL << state->numQubits;
    double prob0 = 0.0;

    // Calcola la probabilità che il qubit sia nello stato |0>
    for (long long i = 0; i < dim; i++) {
        // Verifica se il qubit specificato è nello stato |0> (qubit nel bit i)
        if (((i >> (state->numQubits - qubit - 1)) & 1) == 0) {
            prob0 += pow(cabs(state->amplitudes[i]), 2);
        }
    }

    // La probabilità di misurare |1> è complementare
    double prob1 = 1.0 - prob0;

    // Genera un numero casuale per decidere il risultato
    double rand_val = (double)rand() / RAND_MAX;
    int result = (rand_val < prob0) ? 0 : 1;

    // Collassa lo stato in base al risultato della misura
    double scale_factor = 0.0;
    if (result == 0) {
        scale_factor = 1.0 / sqrt(prob0);
        for (long long i = 0; i < dim; i++) {
            if (((i >> (state->numQubits - qubit - 1)) & 1) == 1) {
                state->amplitudes[i] = 0.0 + 0.0 * I;  // Annulla le ampiezze con qubit = 1
            } else {
                state->amplitudes[i] *= scale_factor;  // Normalizza le ampiezze con qubit = 0
            }
        }
    } else {
        scale_factor = 1.0 / sqrt(prob1);
        for (long long i = 0; i < dim; i++) {
            if (((i >> (state->numQubits - qubit - 1)) & 1) == 0) {
                state->amplitudes[i] = 0.0 + 0.0 * I;  // Annulla le ampiezze con qubit = 0
            } else {
                state->amplitudes[i] *= scale_factor;  // Normalizza le ampiezze con qubit = 1
            }
        }
    }

    // Restituisce sia le probabilità che il risultato
    MeasurementResult m_result;
    m_result.prob0 = prob0;
    m_result.prob1 = prob1;
    m_result.result = result;

    return m_result;
}


int* measure_all(QubitState *state) {
    long long dim = 1LL << state->numQubits;  // Dimensione dello spazio di Hilbert: 2^numQubits
    double cumulativeProb = 0.0;  // Probabilità cumulativa inizializzata a 0
    double randNum = (double)rand() / RAND_MAX;  // Numero casuale tra 0 e 1
    long long collapse_index = -1;  // Indice dello stato in cui il sistema collasserà

    // Calcolo della distribuzione cumulativa delle probabilità
    for (long long i = 0; i < dim; i++) {
        cumulativeProb += pow(cabs(state->amplitudes[i]), 2);  // Somma delle probabilità
        if (randNum < cumulativeProb) {  // Trova lo stato in cui il sistema collassa
            collapse_index = i;  // Imposta l'indice del collasso
            break;  // Esce dal ciclo una volta trovato lo stato corrispondente
        }
    }

    // Risultato della misura: array contenente i valori di ciascun qubit
    int* results = (int*)malloc(state->numQubits * sizeof(int));  // Alloca memoria per il risultato
    for (int i = 0; i < state->numQubits; i++) {
        results[i] = (collapse_index >> (state->numQubits - i - 1)) & 1;  // Estrae il valore di ciascun qubit
    }

    // Far collassare lo stato del sistema
    for (long long j = 0; j < dim; j++) {
        if (j == collapse_index) {
            state->amplitudes[j] = 1.0 + 0.0 * I;  // Lo stato collassa nello stato base corrispondente
        } else {
            state->amplitudes[j] = 0.0 + 0.0 * I;  // Tutti gli altri stati vengono azzerati
        }
    }

    return results;  // Restituisce l'array dei risultati della misura
}

QubitAmplitudes getQubitAmplitudes(QubitState* state, int target) {
    long long dim = 1LL << state->numQubits;  // Dimensione dello spazio di Hilbert (2^numQubits)
    
    QubitAmplitudes result;
    result.amplitude0 = 0.0 + 0.0 * I;
    result.amplitude1 = 0.0 + 0.0 * I;

    // Itera attraverso tutti gli stati del sistema
    for (long long i = 0; i < dim; i++) {
        // Verifica se il qubit target è nello stato |0> o |1> (qubit 0 è il più significativo)
        if (((i >> (state->numQubits - 1 - target)) & 1) == 0) {
            result.amplitude0 += state->amplitudes[i];  // Somma l'ampiezza associata allo stato |0> del qubit target
        } else {
            result.amplitude1 += state->amplitudes[i];  // Somma l'ampiezza associata allo stato |1> del qubit target
        }
    }
    
    return result;
}


void printQubitAmplitudes(QubitAmplitudes amplitudes) {
    printf("Ampiezza di |0>: %f + %fi\n", creal(amplitudes.amplitude0), cimag(amplitudes.amplitude0));
    printf("Ampiezza di |1>: %f + %fi\n", creal(amplitudes.amplitude1), cimag(amplitudes.amplitude1));
}


//------------------ 3 qubit gates ---------------------------//
// Implementazione del Toffoli Gate
// Descrizione: Un gate CCNOT (Toffoli) controllato su due qubit che inverte il qubit target.
void applyToffoli(QubitState* state, int control1, int control2, int target) {
    // Applica la decomposizione Toffoli con gate a due qubit
    // Implementazione basata sulla decomposizione visualizzata (con H, T e CNOT)
  applyHadamard(state,target);
  applyCNOT(state,control2,target);
  applyTdag(state,target);
  applyCNOT(state,control1,target);
  applyT(state,target);
  applyCNOT(state,control2,target);
  applyTdag(state,target);
  applyCNOT(state,control1,target);
  applyT(state,target);
  applyHadamard(state,target);
  //---
  applyT(state,control2);
  applyCNOT(state,control1,control2);
  applyT(state,control1);
  applyTdag(state,control2);
  applyCNOT(state,control1,control2);

}
