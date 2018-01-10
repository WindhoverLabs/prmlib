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

#ifndef PC_LIB_H
#define PC_LIB_H

/*************************************************************************
** Includes
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cfe.h"

/*************************************************************************
** Macro Definitions
*************************************************************************/
#define PRMLIB_PARAM_TBL_MAX_ENTRY  (1000)
#define PRMLIB_PARAM_TBL_MUTEX_NAME ("PRMLIB_PARAM_TBL_MUTEX")
#define PRMLIB_MSG_PARAM_NAME_LEN  (64)

/************************************************************************
** Local Structure Definitions
*************************************************************************/

typedef struct
{
	char name[PRMLIB_MSG_PARAM_NAME_LEN];
    float value;
    uint8 type;
	uint8 vehicle_id;
	uint8 component_id;
} PRMLIB_ParamData_t;

/** \brief Definition for a single config table entry */
typedef struct
{
	uint8 enabled;
	PRMLIB_ParamData_t param_data;
} PRMLIB_ParamTblData_t;

/**
**  \brief Param Lib Data
*/
typedef struct
{
	/** \brief Array for parameters */
	PRMLIB_ParamTblData_t  ParamTbl[PRMLIB_PARAM_TBL_MAX_ENTRY];

	/** \brief Mutex for registration array */
	uint32          	   ParamTblMutex;

	/** \brief Mutex for registration array */
	uint16          	   ParamCount;

}PRMLIB_AppData_t;


/*************************************************************************
** Local Function Prototypes
*************************************************************************/

/************************************************************************/
/** \brief Initialize the Parameter Library (PRMLIB)
**
**  \par Description
**       Protobuf Library initialization routine. This function performs all
**       the required startup steps to initialize PBL's registration
**       array.
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS    \endcode
**  \retstmt Return codes from #OS_MutSemCreate \endcode
**  \endreturns
**
*************************************************************************/
int32 PRMLIB_LibInit(void);

/************************************************************************/
/** \brief
**
**  \par Description
**
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**
**  \endreturns
**
*************************************************************************/
void PRMLIB_AddParam(PRMLIB_ParamData_t param);


/************************************************************************/
/** \brief
**
**  \par Description
**
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**
**  \endreturns
**
*************************************************************************/
boolean PRMLIB_ParamsEqual(PRMLIB_ParamData_t param1, PRMLIB_ParamData_t param2);


/************************************************************************/
/** \brief
**
**  \par Description
**
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**
**  \endreturns
**
*************************************************************************/
boolean PRMLIB_ParamExists(PRMLIB_ParamData_t param_data);


/************************************************************************/
/** \brief
**
**  \par Description
**
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**
**  \endreturns
**
*************************************************************************/
void PRMLIB_UpdateParam(PRMLIB_ParamData_t param_data);


/************************************************************************/
/** \brief
**
**  \par Description
**
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**
**  \endreturns
**
*************************************************************************/
void PRMLIB_RemoveParam(PRMLIB_ParamData_t param_data);


/************************************************************************/
/** \brief
**
**  \par Description
**
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**
**  \endreturns
**
*************************************************************************/
void PRMLIB_UpdateParamCount();


/************************************************************************/
/** \brief
**
**  \par Description
**
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**
**  \endreturns
**
*************************************************************************/
uint16 PRMLIB_GetParamCount();


/************************************************************************/
/** \brief
**
**  \par Description
**
**
**  \par Assumptions, External Events, and Notes:
**       None
**
**  \returns
**
**  \endreturns
**
*************************************************************************/
PRMLIB_ParamTblData_t* PRMLIB_GetParamTable();

/************************/
/*  End of File Comment */
/************************/
#endif
