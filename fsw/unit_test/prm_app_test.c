
#include "prm_app_test.h"
#include "prm_lib.h"
#include "prm_test_utils.h"

#include "uttest.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"

char 				*TEST_MSG_NAME = "TestMsg";
char 				*BAD_TEST_MSG_NAME = "BadTestMsg";
CFE_SB_MsgId_t 		TEST_MSG_ID = 1;
uint16 				TEST_CC = 1;

/**
 * Test PRMLIB_LibInit(), Fail Create Mutex
 */
void Test_PRMLIB_LibInit_Fail_Mutex(void)
{
	int32 retCode = 0;

	/* Set to cause to fail */
	Ut_OSAPI_SetReturnCode(UT_OSAPI_MUTSEMCREATE_INDEX, -1, 1);

	/* Execute the function being tested */
	retCode = PRMLIB_LibInit();

	/* Verify results */
	UtAssert_True(retCode==-1,"Return = -1");
}

/**
 * Test PRMLIB_LibInit(), Nominal
 */
void Test_PRMLIB_LibInit_Nominal(void)
{
	int32 retCode = -1;

	/* Execute the function being tested */
	retCode = PRMLIB_LibInit();

	/* Verify results */
	UtAssert_True(retCode==CFE_SUCCESS,"Return = CFE_SUCCESS");
}


/**************************************************************************
 * Rollup Test Cases
 **************************************************************************/
void PRMLIB_App_Test_AddTestCases(void)
{
    UtTest_Add(Test_PRMLIB_LibInit_Fail_Mutex, PRMLIB_Test_Setup, PRMLIB_Test_TearDown,
               "Test_PRMLIB_LibInit_Fail_Mutex");
    UtTest_Add(Test_PRMLIB_LibInit_Nominal, PRMLIB_Test_Setup, PRMLIB_Test_TearDown,
               "Test_PRMLIB_LibInit_Nominal");
}


