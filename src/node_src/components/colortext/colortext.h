#ifndef COMPONENTS_COLORTEXT_COLORTEXT_H_
#define COMPONENTS_COLORTEXT_COLORTEXT_H_

#include <stdbool.h>

#define COLORTEXT_MAX_LENGTH 16

#define COLORTEXT_RED "\033[31m"
#define COLORTEXT_GREEN "\033[32m"
#define COLORTEXT_YELLOW "\033[33m"
#define COLORTEXT_BLUE "\033[34m"
#define COLORTEXT_MAGENTA "\033[35m"
#define COLORTEXT_CYAN "\033[36m"
#define COLORTEXT_WHITE "\033[37m"
#define COLORTEXT_RESET "\033[0m"

bool colortext_color_str(const char *, const char *, char *);

#endif  // COMPONENTS_COLORTEXT_COLORTEXT_H_
