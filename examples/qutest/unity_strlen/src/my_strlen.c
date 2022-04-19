#include "my_strlen.h"

uint16_t my_strlen(char const *str) {
    uint16_t len = 0U;
    for (; *str != '\0'; ++str) {
        ++len;
    }
    return len;
}
