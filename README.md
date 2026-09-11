# Multiplicación de dos Numeros  de 128 bits 

## Descripción
Este proyecto implementa la multiplicación exacta de dos enteros de 128 bits utilizando estructuras `union` en C y las instrucciones optimizadas a nivel de hardware de la biblioteca **Intel Intrinsics** (`<immintrin.h>`). El resultado se almacena en una estructura de 256 bits para prevenir cualquier desbordamiento.



## Detalles de Implementación

### 1. Representación en Memoria (`union`)
Debido a que la arquitectura estándar maneja registros de 64 bits, se crearon uniones para segmentar los datos:
* `Int128`: Agrupa 2 enteros no firmados de 64 bits (`parts[0]` para la parte baja y `parts[1]` para la alta).
* `Int256`: Agrupa 4 enteros de 64 bits (`parts[0..3]`) formando el registro completo de salida de 256 bits.

### 2. Intel Intrinsics Utilizados
* **`_mulx_u64`**: Realiza la multiplicación sin signo de $64 \times 64$ bits devolviendo la mitad baja y guardando la mitad alta directamente en un puntero, sin afectar las banderas de estado del procesador.
* **`_addcarry_u64`**: Garantiza la propagación eficiente de acarreos a nivel hardware durante la suma de los productos cruzados.

## Requisitos y Compilación

* Compilador GCC con soporte para la extensión de instrucciones **BMI2** (Intel Haswell o superior / AMD Excavator o superior).
