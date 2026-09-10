#include <stdio.h>
#include <emmintrin.h>
#include <immintrin.h>

// Unión para compartir 256 bytes entre distintos tipos SIMD
union PersonaU {
    unsigned char MSG[256];
    unsigned int MSGi[64];
    unsigned long long MSGll[32];
    __m128 MSG128[16];
    __m256 MSG256[8];
};

int main() {
    union PersonaU MG;

    printf("\nTamaño de la union: %lu bytes\n\n", sizeof(MG));

    // Llenar el buffer byte por byte
    for (int i = 0; i < 256; i++) {
        MG.MSG[i] = i;
    }

    // Leer el contenido agrupado en enteros de 64 bits (32 elementos)
    for (int i = 0; i < 32; i++) {
        printf("%llu ", MG.MSGll[i]);
    }

    printf("\n");

    return 0;
}