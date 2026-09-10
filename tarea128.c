#include <stdio.h>
#include <immintrin.h> // Incluye _mulx_u64

// Unión para ver los 128 bits como dos bloques de 64 bits o como vector SIMD
union Vector128 {
    unsigned long long part[2]; // part[0] = Bajas (64b), part[1] = Altas (64b)
    __m128i vec;
};

int main() {
    // Definimos los dos operandos de 64 bits a multiplicar
    unsigned long long a = 0xFFFFFFFFFFFFFFFFULL; // Número máximo de 64 bits
    unsigned long long b = 0x2ULL;

    union Vector128 Resultado;
    unsigned long long high_bits;

    // Uso directo de _mulx_u64:
    // Retorna la parte BAJA (dst) y guarda la parte ALTA en la dirección de 'high_bits'
    Resultado.part[0] = _mulx_u64(a, b, &high_bits);
    Resultado.part[1] = high_bits;

    // Impresión de resultados
    printf("--- MULTIPLICACION 64-bit x 64-bit = 128-bit (_mulx_u64) ---\n");
    printf("Operando A: %016llX\n", a);
    printf("Operando B: %016llX\n", b);
    printf("-----------------------------------------------------------\n");
    printf("Parte Alta (Bits 127:64): %016llX\n", Resultado.part[1]);
    printf("Parte Baja (Bits 63:0)  : %016llX\n", Resultado.part[0]);
    printf("\nResultado completo (128 bits en Hexadecimal):\n");
    printf("0x%016llX%016llX\n", Resultado.part[1], Resultado.part[0]);

    return 0;
}