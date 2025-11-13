/*******************************************************************************
* File Name: UART_BT_RX.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_UART_BT_RX_H)
#define CY_ISR_UART_BT_RX_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void UART_BT_RX_Start(void);
void UART_BT_RX_StartEx(cyisraddress address);
void UART_BT_RX_Stop(void);

CY_ISR_PROTO(UART_BT_RX_Interrupt);

void UART_BT_RX_SetVector(cyisraddress address);
cyisraddress UART_BT_RX_GetVector(void);

void UART_BT_RX_SetPriority(uint8 priority);
uint8 UART_BT_RX_GetPriority(void);

void UART_BT_RX_Enable(void);
uint8 UART_BT_RX_GetState(void);
void UART_BT_RX_Disable(void);

void UART_BT_RX_SetPending(void);
void UART_BT_RX_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the UART_BT_RX ISR. */
#define UART_BT_RX_INTC_VECTOR            ((reg32 *) UART_BT_RX__INTC_VECT)

/* Address of the UART_BT_RX ISR priority. */
#define UART_BT_RX_INTC_PRIOR             ((reg8 *) UART_BT_RX__INTC_PRIOR_REG)

/* Priority of the UART_BT_RX interrupt. */
#define UART_BT_RX_INTC_PRIOR_NUMBER      UART_BT_RX__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable UART_BT_RX interrupt. */
#define UART_BT_RX_INTC_SET_EN            ((reg32 *) UART_BT_RX__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the UART_BT_RX interrupt. */
#define UART_BT_RX_INTC_CLR_EN            ((reg32 *) UART_BT_RX__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the UART_BT_RX interrupt state to pending. */
#define UART_BT_RX_INTC_SET_PD            ((reg32 *) UART_BT_RX__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the UART_BT_RX interrupt. */
#define UART_BT_RX_INTC_CLR_PD            ((reg32 *) UART_BT_RX__INTC_CLR_PD_REG)


#endif /* CY_ISR_UART_BT_RX_H */


/* [] END OF FILE */
