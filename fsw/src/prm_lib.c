/****************************************************************************
 *
 *   Copyright (c) 2016-2017 Windhover Labs, L.L.C. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name Windhover Labs nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/

#include "prm_lib.h"
#include "prm_version.h"

PRMLIB_AppData_t PRMLIB_AppData;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* CFS Library Initialization Routine                              */
/* cFE requires that a library have an initialization routine      */ 
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PRMLIB_LibInit(void)
{
	int32 Status = CFE_SUCCESS;

	/* Create mutex for param table */
	Status = OS_MutSemCreate(&PRMLIB_AppData.ParamTblMutex, PRMLIB_PARAM_TBL_MUTEX_NAME, 0);
	if (Status != CFE_SUCCESS)
	{
		goto PRMLIB_LibInit_Exit_Tag;
	}

	PRMLIB_InitDefaultParameters(); //TODO:Remove
PRMLIB_LibInit_Exit_Tag:
	if (Status == CFE_SUCCESS)
	{
		OS_printf ("PRMLIB Initialized.  Version %d.%d.%d.%d\n",
    		PRMLIB_MAJOR_VERSION,
			PRMLIB_MINOR_VERSION,
			PRMLIB_REVISION,
			PRMLIB_MISSION_REV);
	}

    return Status;
 
}/* End PRMLIB_LibInit */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PRMLIB_InitDefaultParameters(void)
{
	PRMLIB_ParamTblData_t e1 = {1, {"TEST_PARAM", 1.0, 9, 1, 1}};
	PRMLIB_ParamTblData_t e2 = {1, {"TEST_PARAM2", 2.0, 9, 1, 1}};

	PRMLIB_AppData.ParamTbl[0] = e1;
	PRMLIB_AppData.ParamTbl[1] = e2;

	PRMLIB_UpdateParamCount();
//	PRMLIB_AppData.ParamTbl[3] = {1, {"TEST_PARAM4", 4.0, 9, 1, 1}};
//	PRMLIB_AppData.ParamTbl[4] = {1, {"TEST_PARAM5", 5.0, 9, 1, 1}};
//	PRMLIB_AppData.ParamTbl[5] = {1, {"TEST_PARAM6", 6.0, 9, 1, 1}};
//	PRMLIB_AppData.ParamTbl[6] = {1, {"TEST_PARAM7", 7.0, 9, 1, 1}};
//	PRMLIB_AppData.ParamTbl[7] = {1, {"TEST_PARAM8", 8.0, 9, 1, 1}};
//	PRMLIB_AppData.ParamTbl[8] = {1, {"TEST_PARAM9", 9.0, 9, 1, 1}};
//	PRMLIB_AppData.ParamTbl[9] = {1, {"TEST_PARAM10", 10.0, 9, 1, 1}};
}/* End PRMLIB_LibInit */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Add Parameter to Table			                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PRMLIB_AddParam(PRMLIB_ParamData_t param)
{
	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table to find first empty index */
	for(int i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		if (PRMLIB_AppData.ParamTbl[i].enabled == 0)
		{
			/* Update parameter message with current table index values */
			PRMLIB_AppData.ParamTbl[i].enabled = 1;
			PRMLIB_AppData.ParamTbl[i].param_data.value = param.value;
			memcpy(PRMLIB_AppData.ParamTbl[i].param_data.name, param.name,
					sizeof(param.name)); //need to clear string?
			PRMLIB_AppData.ParamTbl[i].param_data.type = param.type;
			PRMLIB_AppData.ParamTbl[i].param_data.vehicle_id = param.vehicle_id;
			PRMLIB_AppData.ParamTbl[i].param_data.component_id = param.component_id;
		}
	}

	/* Increment total parameter count */
	PRMLIB_AppData.ParamCount++;

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Test Parameter Equality			                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean PRMLIB_ParamsEqual(PRMLIB_ParamData_t param1, PRMLIB_ParamData_t param2)
{
	boolean equal = TRUE;

	if (param1.vehicle_id != param2.vehicle_id)
	{
		equal = FALSE;
		goto PRMLIB_ParamsEqual_Exit_Tag;
	}

	if (param1.component_id != param2.component_id)
	{
		equal = FALSE;
		goto PRMLIB_ParamsEqual_Exit_Tag;
	}

	if (strcmp(param1.name, param2.name) != 0)
	{
		equal = FALSE;
		goto PRMLIB_ParamsEqual_Exit_Tag;
	}

PRMLIB_ParamsEqual_Exit_Tag:
	return equal;
}

void PRMLIB_PrintParam(PRMLIB_ParamData_t param_data)
{
	OS_printf("name: %s \n", param_data.name);
	OS_printf("val: %u \n", param_data.value);
	OS_printf("type: %u \n", param_data.type);
	OS_printf("vehicle_id: %u \n", param_data.vehicle_id);
	OS_printf("component_id: %u \n", param_data.component_id);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Check if Parameter Exists		                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean PRMLIB_ParamExists(PRMLIB_ParamData_t param_data)
{
	int32 Status;
	boolean paramExists = FALSE;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table to find parameter */
	for(int i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		/* Only check enabled parameters */
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (PRMLIB_ParamsEqual(param_data, PRMLIB_AppData.ParamTbl[i].param_data))
			{
				paramExists = TRUE;
			}
		}
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return paramExists;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update Parameter Value			                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PRMLIB_UpdateParam(PRMLIB_ParamData_t param_data)
{
	int32 Status;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table to find parameter */
	for(int i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		/* Only check enabled parameters */
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (PRMLIB_ParamsEqual(param_data, PRMLIB_AppData.ParamTbl[i].param_data))
			{
				/* Update parameter message with current table index values */
				PRMLIB_AppData.ParamTbl[i].param_data.value = param_data.value;
				PRMLIB_AppData.ParamTbl[i].param_data.type = param_data.type;
			}
		}
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Remove Parameter 				                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PRMLIB_RemoveParam(PRMLIB_ParamData_t param_data)
{
	int32 Status;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table to find parameter */
	for(int i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		/* Only check enabled parameters */
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (PRMLIB_ParamsEqual(param_data, PRMLIB_AppData.ParamTbl[i].param_data))
			{
				/* Clear parameter data */
				//PRMLIB_AppData.ParamTbl[i].param_data = {"",0};
				PRMLIB_AppData.ParamTbl[i].enabled = 0;
			}
		}
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Update Total Parameter Count	                                   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PRMLIB_UpdateParamCount()
{
	uint16 count = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table and count enabled entries */
	for(int i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			count++;
		}
	}

	PRMLIB_AppData.ParamCount = count;

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);
}

uint16 PRMLIB_GetParamCount()
{
	return PRMLIB_AppData.ParamCount;
}

PRMLIB_ParamTblData_t* PRMLIB_GetParamTable()
{
	return PRMLIB_AppData.ParamTbl;
}


/************************/
/*  End of File Comment */
/************************/
