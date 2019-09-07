#include <stdio.h>

int main() {
    int x = 0;
    int y = 1;
    int z = 2;

    printf("a: %d\n\r", x + (y * z)                         );
    printf("b: %d\n\r", (x == 0) && (y != 4)                );
    printf("c: %d\n\r", (y < x) < z                             );
    printf("d: %d\n\r", y + (-z)                            );
    printf("e: %d\n\r", (!z) || y                           );
    printf("f: %d\n\r", y ? x : z                    );
    printf("g: %d\n\r", (x - y) < z                             );
    printf("h: %d\n\r", 0 || (z <= y)                           );   // (Give value of expression and final value of x)
    printf("i: %d\n\r", z & (3 == 2)                            );
}