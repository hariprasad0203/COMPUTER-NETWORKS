#include <stdio.h>

int main() {
    int P;
    scanf("%d", & P);

    while (P--) {
        char L[1001];
        scanf("%s", L);

        int s = 0;
        for (int i = 0; L[i] != '\0'; i++) {
            if (L[i] >= '0' && L[i] <= '9') {
                s += L[i] - '0';
            }
        }
        printf("%d\n", s);
    }

    return 0;
}
