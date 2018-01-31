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
	PRMLIB_ParamTblData_t e1 = {1, {"PWN_DISARMED", 900.0, 6}};
	PRMLIB_ParamTblData_t e2 = {1, {"PWN_MIN", 1000.0, 6}};
	PRMLIB_ParamTblData_t e3 = {1, {"PWN_MAX", 2000.0, 6}};

	PRMLIB_AppData.ParamTbl[0] = e1;
	PRMLIB_AppData.ParamTbl[1] = e2;
	PRMLIB_AppData.ParamTbl[2] = e3;

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
int32 PRMLIB_AddParam(PRMLIB_ParamData_t param)
{
	int32 Status = -1;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table to find first empty index */
	for(int i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		if (PRMLIB_AppData.ParamTbl[i].enabled == 0)
		{
			PRMLIB_PrintParam(param);
			/* Update parameter message with current table index values */
			PRMLIB_AppData.ParamTbl[i].enabled = 1;
			PRMLIB_AppData.ParamTbl[i].param_data.value = param.value;
			memcpy(PRMLIB_AppData.ParamTbl[i].param_data.name, param.name,
					sizeof(param.name)); //need to clear string?
			PRMLIB_AppData.ParamTbl[i].param_data.type = param.type;
			Status = CFE_SUCCESS;
			break;
		}
	}

	/* Increment total parameter count */
	PRMLIB_AppData.ParamCount++;

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get Parameter Data			                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PRMLIB_GetParamData(PRMLIB_ParamData_t* InOutParam, uint16* ParamIndex, uint16* ParamCount)
{
	int32 Status = -1;
	uint16 idx = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table to find parameter */
	for(int i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		/* Only check enabled parameters */
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (PRMLIB_ParamsEqual(*InOutParam, PRMLIB_AppData.ParamTbl[i].param_data))
			{
				/* Update parameter message with current table index values */
				InOutParam->value = PRMLIB_AppData.ParamTbl[i].param_data.value;
				InOutParam->type = PRMLIB_AppData.ParamTbl[i].param_data.type;
				*ParamIndex = idx;
				Status = CFE_SUCCESS;
				break;
			}

			idx++;
		}
	}

	*ParamCount = PRMLIB_AppData.ParamCount;

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get Parameter Value			                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PRMLIB_GetParamDataAtIndex(PRMLIB_ParamData_t* InOutParam, uint16 ParamIndex)
{
	int32 Status = -1;
	uint16 idx = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table to find parameter */
	for(int i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		/* Only check enabled parameters */
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (idx == ParamIndex)
			{
				/* Update parameter message with current table index values */
				strcpy(InOutParam->name, PRMLIB_AppData.ParamTbl[i].param_data.name);
				InOutParam->value = PRMLIB_AppData.ParamTbl[i].param_data.value;
				InOutParam->type = PRMLIB_AppData.ParamTbl[i].param_data.type;
				Status = CFE_SUCCESS;
				break;
			}

			idx++;
		}
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Test Parameter Equality			                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean PRMLIB_ParamsEqual(PRMLIB_ParamData_t param1, PRMLIB_ParamData_t param2)
{
	boolean equal = TRUE;

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
	OS_printf("val: %f \n", param_data.value);
	OS_printf("type: %u \n", param_data.type);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Check if Parameter Exists		                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
boolean PRMLIB_ParamExists(char param_name[])
{
	boolean paramExists = FALSE;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table to find parameter */
	for(int i = 0; i < PRMLIB_PARAM_TBL_MAX_ENTRY; ++i)
	{
		/* Only check enabled parameters */
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (strcmp(param_name, PRMLIB_AppData.ParamTbl[i].param_data.name) == 0)
			{
				paramExists = TRUE;
				break;
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
int32 PRMLIB_UpdateParam(PRMLIB_ParamData_t param_data)
{
	int32 Status = -1;

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
				Status = CFE_SUCCESS;
				break;
			}
		}
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Remove Parameter 				                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 PRMLIB_RemoveParam(PRMLIB_ParamData_t param_data)
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
				PRMLIB_AppData.ParamTbl[i].enabled = 0; // Note: this could cause collisions
				Status = CFE_SUCCESS;
			}
		}
	}

	PRMLIB_AppData.ParamCount--;

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
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


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Copy Parameter Data				                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void PRMLIB_CopyParamData(PRMLIB_ParamData_t dest, PRMLIB_ParamData_t src)
{
	dest.type = src.type;
	dest.value = src.value;
	strcpy(dest.name, src.name);
}

uint16 PRMLIB_GetParamCount()
{
	return PRMLIB_AppData.ParamCount;
}

void PRMLIB_GetParams(PRMLIB_ParamData_t* params, uint16* ParamCount)
{
	uint16 idx = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table and get all params */
	for(int i = 0; i < PRMLIB_AppData.ParamCount; ++i)
	{
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			params[idx].value = PRMLIB_AppData.ParamTbl[i].param_data.value;
			strcpy(params[idx].name , PRMLIB_AppData.ParamTbl[i].param_data.name);
			params[idx].type = PRMLIB_AppData.ParamTbl[i].param_data.type;
			idx++;
		}
	}

	*ParamCount = PRMLIB_AppData.ParamCount;

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);
}

// TODO: Should this have error checking?
uint32 PRMLIB_GetParamValueById_uint32(char name[])
{
	uint32 ReturnValue = 0;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table and get all params */
	for(int i = 0; i < PRMLIB_AppData.ParamCount; ++i)
	{
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (strcmp(name, PRMLIB_AppData.ParamTbl[i].param_data.name) == 0)
			{
				ReturnValue = (uint32) PRMLIB_AppData.ParamTbl[i].param_data.value;
			}
		}
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return ReturnValue;
}

uint32 PRMLIB_ParamRegister_uint32(char name[], uint32 default_value, PRMLIB_ParamType_t type)
{
	uint32 ReturnValue = 0;
	boolean ParamExists = FALSE;
	PRMLIB_ParamData_t param = {0};

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table and get all params */
	for(int i = 0; i < PRMLIB_AppData.ParamCount; ++i)
	{
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (strcmp(name, PRMLIB_AppData.ParamTbl[i].param_data.name) == 0)
			{
				ReturnValue = (uint32) PRMLIB_AppData.ParamTbl[i].param_data.value;
				ParamExists = TRUE;
			}
		}
	}

	if(ParamExists == FALSE)
	{
		strcpy(param.name, name);
		param.value = default_value;
		param.type = type;
		PRMLIB_AddParam(param);
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return ReturnValue;
}



/************************/
/*  End of File Comment */
/************************/