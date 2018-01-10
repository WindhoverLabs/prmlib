
#ifndef PRMLIB_TEST_UTILS_H
#define PRMLIB_TEST_UTILS_H

/*
 * Includes
 */

#include "prm_lib.h"

extern PRMLIB_AppData_t  PRMLIB_AppData;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Function Definitions
 */

void PRMLIB_Test_Setup(void);
void PRMLIB_Test_Setup_InitTbls(void);
void PRMLIB_Test_TearDown(void);


#ifdef __cplusplus
}
#endif

#endif /* PRMLIB_TEST_UTILS_H */

