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
//	PRMLIB_ParamTblData_t e1 = {1, {"PWN_DISARMED", 900.0, 6}};
//	PRMLIB_ParamTblData_t e2 = {1, {"PWN_MIN", 1000.0, 6}};
//	PRMLIB_ParamTblData_t e3 = {1, {"PWN_MAX", 2000.0, 6}};
//	PRMLIB_ParamTblData_t e4 = {1, {"test_uint8", 1, 1}};
//	PRMLIB_ParamTblData_t e5 = {1, {"test_int8", 1, 2}};
//	PRMLIB_ParamTblData_t e6 = {1, {"test_uint16", 1, 3}};
//	PRMLIB_ParamTblData_t e7 = {1, {"test_int16", 1, 4}};
//	PRMLIB_ParamTblData_t e8 = {1, {"test_uint32", 1, 5}};
//	PRMLIB_ParamTblData_t e9 = {1, {"test_int32", 1, 6}};
//	PRMLIB_ParamTblData_t e12 = {1, {"test_float32", 1.0, 9}};
//
//	PRMLIB_AppData.ParamTbl[0] = e1;
//	PRMLIB_AppData.ParamTbl[1] = e2;
//	PRMLIB_AppData.ParamTbl[2] = e3;
//	PRMLIB_AppData.ParamTbl[3] = e4;
//	PRMLIB_AppData.ParamTbl[4] = e5;
//	PRMLIB_AppData.ParamTbl[5] = e6;
//	PRMLIB_AppData.ParamTbl[6] = e7;
//	PRMLIB_AppData.ParamTbl[7] = e8;
//	PRMLIB_AppData.ParamTbl[8] = e9;
//	PRMLIB_AppData.ParamTbl[9] = e12;


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
			//PRMLIB_PrintParam(param);
			/* Update parameter message with current table index values */
			PRMLIB_AppData.ParamTbl[i].enabled = 1;
			memcpy(PRMLIB_AppData.ParamTbl[i].param_data.name, param.name,
					sizeof(param.name)); //need to clear string?
			PRMLIB_AppData.ParamTbl[i].param_data.type = param.type;
			PRMLIB_SetParamValue(&PRMLIB_AppData.ParamTbl[i].param_data, &param.value);
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
				memcpy(&InOutParam->value, &PRMLIB_AppData.ParamTbl[i].param_data.value, PRMLIB_PARAM_VALUE_MAX_LEN);
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
				memcpy(&InOutParam->value, &PRMLIB_AppData.ParamTbl[i].param_data.value, PRMLIB_PARAM_VALUE_MAX_LEN);
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
				memcpy(&PRMLIB_AppData.ParamTbl[i].param_data.value, &param_data.value, PRMLIB_PARAM_VALUE_MAX_LEN);
				PRMLIB_AppData.ParamTbl[i].param_data.type = param_data.type;
				OS_printf("\nUpdate param:\n");
				PRMLIB_PrintParam(PRMLIB_AppData.ParamTbl[i].param_data);
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
	memcpy(&dest.value, src.value, PRMLIB_PARAM_VALUE_MAX_LEN);
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

	OS_printf("count: %u\n", PRMLIB_AppData.ParamCount);
	/* Iterate over table and get all params */
	for(int i = 0; i < PRMLIB_AppData.ParamCount; ++i)
	{
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			OS_printf("found enabled\n");
			strcpy(params[idx].name, PRMLIB_AppData.ParamTbl[i].param_data.name);
			params[idx].type = PRMLIB_AppData.ParamTbl[i].param_data.type;
			PRMLIB_SetParamValue(&params[idx], &PRMLIB_AppData.ParamTbl[i].param_data.value);
			idx++;
		}
	}

	*ParamCount = PRMLIB_AppData.ParamCount;

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);
}

int32 PRMLIB_GetParamById(char name[], PRMLIB_ParamData_t* InOutParam)
{
	int32 Status = -1;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table and get all params */
	for(int i = 0; i < PRMLIB_AppData.ParamCount; ++i)
	{
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (strcmp(name, PRMLIB_AppData.ParamTbl[i].param_data.name) == 0)
			{
				//memcpy(InOutParam, &PRMLIB_AppData.ParamTbl[i].param_data, sizeof(PRMLIB_ParamData_t));
				Status = CFE_SUCCESS;
			}
		}
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
}

int32 PRMLIB_GetParamValueById(char name[], void* InOutValue)
{
	int32 Status = -1;

	/* Lock the mutex */
	OS_MutSemTake(PRMLIB_AppData.ParamTblMutex);

	/* Iterate over table and get all params */
	for(int i = 0; i < PRMLIB_AppData.ParamCount; ++i)
	{
		if (PRMLIB_AppData.ParamTbl[i].enabled == 1)
		{
			if (strcmp(name, PRMLIB_AppData.ParamTbl[i].param_data.name) == 0)
			{
				memcpy(InOutValue, &PRMLIB_AppData.ParamTbl[i].param_data.value, PRMLIB_PARAM_VALUE_MAX_LEN);
				Status = CFE_SUCCESS;
			}
		}
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
}

int32 PRMLIB_ParamRegister(char name[], void* inOutValue, PRMLIB_ParamType_t type)
{
	int32 Status = -1;
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
				PRMLIB_GetParamValue(PRMLIB_AppData.ParamTbl[i].param_data, inOutValue);
				ParamExists = TRUE;
				Status = CFE_SUCCESS;
			}
		}
	}

	if(ParamExists == FALSE)
	{
		strcpy(param.name, name);
		param.type = type;
		PRMLIB_SetParamValue(&param, inOutValue);
		Status = PRMLIB_AddParam(param);
	}

	/* Unlock the mutex */
	OS_MutSemGive(PRMLIB_AppData.ParamTblMutex);

	return Status;
}

void PRMLIB_PrintParam(PRMLIB_ParamData_t param)
{
	uint8 val_uint8 	= 0;
	int8 val_int8 		= 0;
	uint16 val_uint16 	= 0;
	int16 val_int16		= 0;
	uint32 val_uint32	= 0;
	int32 val_int32 	= 0;

	OS_printf("name: %s \n", param.name);
	OS_printf("type: %u \n", param.type);
	switch(param.type)
	{
		case TYPE_UINT8:
			memcpy(&val_uint8, param.value, PRMLIB_PARAM_VALUE_MAX_LEN);
			OS_printf("value: %u \n", val_uint8);
			break;
		case TYPE_INT8:
			memcpy(&val_int8, param.value, PRMLIB_PARAM_VALUE_MAX_LEN);
			OS_printf("value: %i \n", val_int8);
			break;
		case TYPE_UINT16:
			memcpy(&val_uint16, param.value, PRMLIB_PARAM_VALUE_MAX_LEN);
			OS_printf("value: %u \n", val_uint16);
			break;
		case TYPE_INT16:
			memcpy(&val_int16, param.value, PRMLIB_PARAM_VALUE_MAX_LEN);
			OS_printf("value: %i \n", val_int16);
			break;
		case TYPE_UINT32:
			memcpy(&val_uint32, &param.value, sizeof(uint32));
			OS_printf("value: %u \n", val_uint32);
			break;
		case TYPE_INT32:
			memcpy(&val_int32, param.value, PRMLIB_PARAM_VALUE_MAX_LEN);
			OS_printf("value: %i \n", val_int32);
			break;
		case TYPE_REAL32:
			OS_printf("value: %f \n", param.value);
			break;
		default:
			//unsupported type
			break;
	}
}

void PRMLIB_GetParamValue(PRMLIB_ParamData_t param, void* val)
{
	uint8 val_uint8 	= 0;
	int8 val_int8		= 0;
	uint16 val_uint16	= 0;
	int16 val_int16	= 0;
	uint32 val_uint32	= 0;
	int32 val_int32 	= 0;

	switch(param.type)
	{
		case TYPE_UINT8:
			memcpy(&val_uint8, param.value, PRMLIB_PARAM_VALUE_MAX_LEN);
			memcpy(val, &val_uint8, sizeof(param.value));
			break;
		case TYPE_INT8:
			memcpy(&val_int8, param.value, PRMLIB_PARAM_VALUE_MAX_LEN);
			memcpy(val, &val_int8, sizeof(param.value));
			break;
		case TYPE_UINT16:
			memcpy(&val_uint16, param.value, PRMLIB_PARAM_VALUE_MAX_LEN);
			memcpy(val, &val_uint16, sizeof(param.value));
			break;
		case TYPE_INT16:
			memcpy(&val_int16, param.value, PRMLIB_PARAM_VALUE_MAX_LEN);
			memcpy(val, &val_int16, sizeof(param.value));
			break;
		case TYPE_UINT32:
			memcpy(&val_uint32, param.value, PRMLIB_PARAM_VALUE_MAX_LEN);
			memcpy(val, &val_uint32, sizeof(param.value));
			break;
		case TYPE_INT32:
			memcpy(&val_int32, param.value, PRMLIB_PARAM_VALUE_MAX_LEN);
			memcpy(val, &val_int32, sizeof(param.value));
			break;
		case TYPE_REAL32:
			memcpy(val, param.value, sizeof(param.value));
			break;
		default:
			//unsupported type
			break;
	}
}

void PRMLIB_SetParamValue(PRMLIB_ParamData_t* param, void* val)
{
	uint8 val_uint8 	= 0;
	int8 val_int8 		= 0;
	uint16 val_uint16 	= 0;
	int16 val_int16	= 0;
	uint32 val_uint32	= 0;
	int32 val_int32 	= 0;

	uint32 test	= 0;
	OS_printf("In set param\n");
	switch(param->type)
	{
		case TYPE_UINT8:
			memcpy(&val_uint8, val, sizeof(param->value));
			memcpy(param->value, &val_uint8, PRMLIB_PARAM_VALUE_MAX_LEN);
			break;
		case TYPE_INT8:
			memcpy(&val_int8, val, sizeof(param->value));
			memcpy(param->value, &val_int8, PRMLIB_PARAM_VALUE_MAX_LEN);
			break;
		case TYPE_UINT16:
			memcpy(&val_uint16, val, sizeof(param->value));
			memcpy(param->value, &val_uint16, PRMLIB_PARAM_VALUE_MAX_LEN);
			break;
		case TYPE_INT16:
			memcpy(&val_int16, val, sizeof(param->value));
			memcpy(param->value, &val_int16, PRMLIB_PARAM_VALUE_MAX_LEN);
			break;
		case TYPE_UINT32:
			memcpy(&val_uint32, val, sizeof(uint32));
			OS_printf("set val: %u \n", val_uint32);
			memcpy(&param->value, &val_uint32, sizeof(val_uint32));
			memcpy(&test, &param->value, sizeof(uint32));
			OS_printf("actual: %u \n", test);
			break;
		case TYPE_INT32:
			memcpy(&val_int32, val, sizeof(param->value));
			memcpy(param->value, &val_int32, PRMLIB_PARAM_VALUE_MAX_LEN);
			break;
		case TYPE_REAL32:
			memcpy(param->value, val, sizeof(param->value));
			break;
		default:
			OS_printf("Unsupported type encountered: %u\n", param->type);
			//unsupported type
			break;
	}
}

void PRMLIB_SetValue(void* destVal, void* srcVal, PRMLIB_ParamType_t type)
{
	uint8 val_uint8 	= 0;
	int8 val_int8 		= 0;
	uint16 val_uint16 	= 0;
	int16 val_int16	= 0;
	uint32 val_uint32	= 0;
	int32 val_int32 	= 0;

	uint32 test	= 0;
	OS_printf("In set value\n");
	switch(type)
	{
		case TYPE_UINT8:
			memcpy(&val_uint8, srcVal, sizeof(uint8));
			memcpy(destVal, &val_uint8, PRMLIB_PARAM_VALUE_MAX_LEN);
			break;
		case TYPE_INT8:
			memcpy(&val_int8, srcVal, sizeof(int8));
			memcpy(destVal, &val_int8, PRMLIB_PARAM_VALUE_MAX_LEN);
			break;
		case TYPE_UINT16:
			memcpy(&val_uint16, srcVal, sizeof(uint16));
			memcpy(destVal, &val_uint16, PRMLIB_PARAM_VALUE_MAX_LEN);
			break;
		case TYPE_INT16:
			memcpy(&val_int16, srcVal, sizeof(int16));
			memcpy(destVal, &val_int16, PRMLIB_PARAM_VALUE_MAX_LEN);
			break;
		case TYPE_UINT32:
			memcpy(&val_uint32, srcVal, sizeof(uint32));
			OS_printf("set val: %u \n", val_uint32);
			memcpy(&destVal, &val_uint32, sizeof(val_uint32));
			memcpy(&test, &destVal, sizeof(uint32));
			OS_printf("actual: %u \n", test);
			break;
		case TYPE_INT32:
			memcpy(&val_int32, srcVal, sizeof(int32));
			memcpy(destVal, &val_int32, PRMLIB_PARAM_VALUE_MAX_LEN);
			break;
		case TYPE_REAL32:
			memcpy(destVal, srcVal, sizeof(float));
			break;
		default:
			OS_printf("Unsupported type encountered: %u\n", type);
			//unsupported type
			break;
	}
}


/************************/
/*  End of File Comment */
/************************/
