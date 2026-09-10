#include <stdio.h>
#include <stdint.h>
#include <immintrin.h> // Intel Intrinsics (_mulx_u64 y _addcarry_u64)

// Union para numeros de 128 bits (2 partes de 64 bits)
typedef union {
    uint64_t parts[2]; // parts[0] = baja, parts[1] = alta
} Int128;

// Union para el registro del resultado de 256 bits (4 partes de 64 bits)
typedef union {
    uint64_t parts[4];   // Arreglo de 4 enteros de 64 bits (256 bits en total)
    Int128 halves[2];    // halves[0] = 128 bits bajos, halves[1] = 128 bits altos
} Int256;

// Multiplicacion completa de 128 bits x 128 bits -> Resultado en 256 bits
Int256 multiplicar_128x128_a_256(Int128 a, Int128 b) {
    Int256 res;

    uint64_t p00_high, p00_low;
    uint64_t p01_high, p01_low;
    uint64_t p10_high, p10_low;
    uint64_t p11_high, p11_low;

    // 1. Multiplicaciones de 64 bits x 64 bits -> 128 bits cada una
    p00_low = _mulx_u64(a.parts[0], b.parts[0], (unsigned long long *)&p00_high);
    p01_low = _mulx_u64(a.parts[0], b.parts[1], (unsigned long long *)&p01_high);
    p10_low = _mulx_u64(a.parts[1], b.parts[0], (unsigned long long *)&p10_high);
    p11_low = _mulx_u64(a.parts[1], b.parts[1], (unsigned long long *)&p11_high);

    // 2. Posicion 0 (Bits 0 a 63)
    res.parts[0] = p00_low;

    // 3. Posicion 1 (Bits 64 a 127) y gestion de acarreos
    unsigned char c1_1 = _addcarry_u64(0, p00_high, p01_low, (unsigned long long *)&res.parts[1]);
    unsigned char c1_2 = _addcarry_u64(0, res.parts[1], p10_low, (unsigned long long *)&res.parts[1]);
    uint64_t acarreo_a_p2 = (uint64_t)c1_1 + (uint64_t)c1_2;

    // 4. Posicion 2 (Bits 128 a 191) y gestion de acarreos
    unsigned char c2_1 = _addcarry_u64(0, p01_high, p10_high, (unsigned long long *)&res.parts[2]);
    unsigned char c2_2 = _addcarry_u64(0, res.parts[2], p11_low, (unsigned long long *)&res.parts[2]);
    unsigned char c2_3 = _addcarry_u64(0, res.parts[2], acarreo_a_p2, (unsigned long long *)&res.parts[2]);
    uint64_t acarreo_a_p3 = (uint64_t)c2_1 + (uint64_t)c2_2 + (uint64_t)c2_3;

    // 5. Posicion 3 (Bits 192 a 255)
    uint64_t temp_p3;
    _addcarry_u64(0, p11_high, acarreo_a_p3, (unsigned long long *)&temp_p3);
    res.parts[3] = temp_p3;

    return res;
}

int main() {
    Int128 n1, n2;

    // Asignamos el valor maximo de 128 bits a ambos numeros para verificar el resultado de 256 bits
    n1.parts[0] = 0xFFFFFFFFFFFFFFFF;
    n1.parts[1] = 0xFFFFFFFFFFFFFFFF;

    n2.parts[0] = 0xFFFFFFFFFFFFFFFF;
    n2.parts[1] = 0xFFFFFFFFFFFFFFFF;

    Int256 res = multiplicar_128x128_a_256(n1, n2);

    printf("--- Resultado de 256 bits en la Union (Hex) ---\n");
    printf("Parte [3] (Bits 192..255): 0x%016llX\n", (unsigned long long)res.parts[3]);
    printf("Parte [2] (Bits 128..191): 0x%016llX\n", (unsigned long long)res.parts[2]);
    printf("Parte [1] (Bits  64..127): 0x%016llX\n", (unsigned long long)res.parts[1]);
    printf("Parte [0] (Bits   0..63 ): 0x%016llX\n", (unsigned long long)res.parts[0]);

    return 0;
}
