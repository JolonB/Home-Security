#include "colortext.h"

#include <stdio.h>

bool colortext_color_str(const char *str, const char *color, char *out) {
    if (str == NULL || color == NULL || out == NULL) {
        return false;
    }

    int n = sprintf(out, "%s%s%s", color, str, COLORTEXT_RESET);
    if (n < 0) {
        return false;
    }

    return true;
}
