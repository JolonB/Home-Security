#ifndef COMPONENTS_TESTABLE_TESTABLE_H_
#define COMPONENTS_TESTABLE_TESTABLE_H_

#include <stdbool.h>

/**
 * @brief Calls function for mockable component.
 * @retval true Called function returns zero.
 * @retval false Called functions does not returns zero.
 */
bool testable_calls_mockable(void);

int testable_mean(const int*, int);

#endif  // COMPONENTS_TESTABLE_TESTABLE_H_
