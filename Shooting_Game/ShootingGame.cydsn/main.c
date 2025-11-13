/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "stdio.h"

#define READY 0
#define RUNNING 1
#define END 2
#define TARGETCOUNT 3
#define _OR ||
#define EEPROM_ADDR 100

void ActivateTarget(int id);

volatile int state;
volatile int time = 0;
volatile int timeout_flag = 0;
volatile int points = 0;
volatile int finished = 0;
int active_target = -1;

CY_ISR(One_Sec_Handler){
    time--;
    char message[32];
    sprintf(message, "Time: %d, Score: %d\r\n", time, points);  //Format and print data into a string
    UART_BT_PutString(message);                                 //Print to UART
    One_Sec_Timer_ReadStatusRegister();                         //Clear interrupt source
}

//When the debounce timer is finished, it signals end of timeout
CY_ISR(Button_Timeout_Handler){
    timeout_flag = 0;                                           //No timeout
    points++;
    UART_BT_PutString("hit\r\n");                               //Print "hit" to UART
    Debounce_Timer_ReadStatusRegister();                        //Clear interrupt source
    Debounce_Timer_Stop();                                      //Stop the timer
    int target = active_target;
    if(active_target >= 3 ){
        target = 1;
    } else {
        target++;
    }
    ActivateTarget(target);
}

//Activate a given target
void ActivateTarget(int id){
    //Deactivate last target
    switch(active_target){
        case 1:
            Target_LED_1_Write(0);
            break;
        case 2:
            Target_LED_2_Write(0);
            break;
        case 3:
            Target_LED_3_Write(0);
            break;
        default:
            break;
    }
    //Target_LED_1_Write(0);
    //Target_LED_2_Write(0);
    //Target_LED_3_Write(0);
    //Activate current target
    active_target = id;
    switch(id){                                                 //Turn on the active target's LED
        case 1:
            Target_LED_1_Write(1);
            break;
        case 2:
            Target_LED_2_Write(1);
            break;
        case 3:
            Target_LED_3_Write(1);
            break;
        default:
            break;
    }
}

//Return the active target's hit status
//Invert reading because buttons are active low
int getActiveTargetState(){
    switch(active_target){
        case 1:
            return !Target_BTN_1_Read();            
        case 2:
            return !Target_BTN_2_Read();            
        case 3:
            return !Target_BTN_3_Read();            
        default:
            return 0;       
    }
}

void Init(){
    Debounce_Timer_Init();
    One_Sec_Timer_Init();
    UART_BT_Start();
    UART_BT_PutString("hello\r\n");    
    ISR_One_Sec_StartEx(One_Sec_Handler);
    ISR_Button_Timeout_StartEx(Button_Timeout_Handler);
    EEPROM_Start();
    ActivateTarget(1);
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    Init();
    int highScore = EEPROM_ReadByte(EEPROM_ADDR);
    char message[32];
    sprintf(message, "Current High Score: %d\r\n", highScore);  //Format and print data into a string
    UART_BT_PutString(message);
    state = READY;
    for(;;)
    {
        switch(state){            
            case READY: {
                time = 60;
                points = 0;
                if((char)UART_BT_GetChar() == 's'){             //READY -> RUNNING if 's' received through UART
                    LED_Write(1);                           //Signal running state with built-in LED
                    state = RUNNING;
                    One_Sec_Timer_Start();
                }
                break;
            }
            case RUNNING: {
                if(getActiveTargetState() && !timeout_flag){    //active target is hit and currently no timeout
                    timeout_flag = 1;
                    Debounce_Timer_Start();                     //start timeout to avoid reading multiple hits
                }
                if(time <= 0){                                  //RUNNING -> END if time is up 
                    Debounce_Timer_Stop();                      //Stop timers
                    One_Sec_Timer_Stop();                       //
                    LED_Write(0);                               //Turn off built-in LED
                    state = END;
                    ActivateTarget(0);
                }                               
                
                break;
            }
            case END: {
                if(!finished){
                    int highScore = EEPROM_ReadByte(EEPROM_ADDR);   //Read last highscore
                    if(points > highScore || highScore == 255){     //Write if necessary, check if default is 0xff
                        UART_BT_PutString("New High Score\r\n");
                        EEPROM_WriteByte(points, EEPROM_ADDR);
                    }
                    finished = 1;
                } else if((char)UART_BT_GetChar() == 'r'){         //END -> READY if 'r' received through UART                    
                    state = READY;
                    ActivateTarget(1);
                    char message[32];
                    sprintf(message, "Current High Score: %d\r\n", highScore);  //Format and print data into a string                
                    UART_BT_PutString(message);
                }
                break;
            }
        }
    }
}

/* [] END OF FILE */
