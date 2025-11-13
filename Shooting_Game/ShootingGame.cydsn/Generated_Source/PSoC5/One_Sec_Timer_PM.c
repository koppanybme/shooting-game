/*******************************************************************************
* File Name: One_Sec_Timer_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "One_Sec_Timer.h"

static One_Sec_Timer_backupStruct One_Sec_Timer_backup;


/*******************************************************************************
* Function Name: One_Sec_Timer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  One_Sec_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void One_Sec_Timer_SaveConfig(void) 
{
    #if (!One_Sec_Timer_UsingFixedFunction)
        One_Sec_Timer_backup.TimerUdb = One_Sec_Timer_ReadCounter();
        One_Sec_Timer_backup.InterruptMaskValue = One_Sec_Timer_STATUS_MASK;
        #if (One_Sec_Timer_UsingHWCaptureCounter)
            One_Sec_Timer_backup.TimerCaptureCounter = One_Sec_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!One_Sec_Timer_UDB_CONTROL_REG_REMOVED)
            One_Sec_Timer_backup.TimerControlRegister = One_Sec_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: One_Sec_Timer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  One_Sec_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void One_Sec_Timer_RestoreConfig(void) 
{   
    #if (!One_Sec_Timer_UsingFixedFunction)

        One_Sec_Timer_WriteCounter(One_Sec_Timer_backup.TimerUdb);
        One_Sec_Timer_STATUS_MASK =One_Sec_Timer_backup.InterruptMaskValue;
        #if (One_Sec_Timer_UsingHWCaptureCounter)
            One_Sec_Timer_SetCaptureCount(One_Sec_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!One_Sec_Timer_UDB_CONTROL_REG_REMOVED)
            One_Sec_Timer_WriteControlRegister(One_Sec_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: One_Sec_Timer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  One_Sec_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void One_Sec_Timer_Sleep(void) 
{
    #if(!One_Sec_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(One_Sec_Timer_CTRL_ENABLE == (One_Sec_Timer_CONTROL & One_Sec_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            One_Sec_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            One_Sec_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    One_Sec_Timer_Stop();
    One_Sec_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: One_Sec_Timer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  One_Sec_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void One_Sec_Timer_Wakeup(void) 
{
    One_Sec_Timer_RestoreConfig();
    #if(!One_Sec_Timer_UDB_CONTROL_REG_REMOVED)
        if(One_Sec_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                One_Sec_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
