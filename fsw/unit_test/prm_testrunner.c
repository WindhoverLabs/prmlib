#include "prm_lib.h"
#include "uttest.h"

#include "prm_app_test.h"

int main(void)
{   
    PRMLIB_App_Test_AddTestCases();

    return(UtTest_Run());

}

