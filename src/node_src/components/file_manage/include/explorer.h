#ifndef COMPONENTS_FILE_MANAGE_INCLUDE_EXPLORER_H_
#define COMPONENTS_FILE_MANAGE_INCLUDE_EXPLORER_H_

#include "sdmmc_cmd.h"

void file_explorer_init(sdmmc_card_t*, const char*);
void file_explorer_run(void);

#endif  // COMPONENTS_FILE_MANAGE_INCLUDE_EXPLORER_H_
