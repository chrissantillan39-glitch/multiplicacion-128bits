#include <stdio.h>
#include <immintrin.h>

// Unión para mapear un registro SIMD de 128 bits con un arreglo de 4 enteros
union Vector4int {
    int Num4[4];
    __m128i Num;
};

int main() {
    union Vector4int V4_1;
    union Vector4int V4_2;
    union Vector4int V4_R;

    // Inicializar el primer vector
    V4_1.Num4[0] = 100;
    V4_1.Num4[1] = 200;
    V4_1.Num4[2] = 300;
    V4_1.Num4[3] = 400;

    // Inicializar el segundo vector
    V4_2.Num4[0] = 1000;
    V4_2.Num4[1] = 2000;
    V4_2.Num4[2] = 3000;
    V4_2.Num4[3] = 4000;

    // Suma SIMD en paralelo (128 bits / 4 enteros de 32 bits)
    V4_R.Num = _mm_add_epi32(V4_1.Num, V4_2.Num);

    // Imprimir los resultados
    printf("--- RESULTADO DE LA SUMA VECTORIAL ---\n");
    for (int i = 0; i < 4; i++) {
        printf("V4_R.Num4[%d] = %d\n", i, V4_R.Num4[i]);
    }

    return 0;
}
