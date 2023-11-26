/******************************************************************************/
/*                                                                            */
/* !Layer           : Application Abstraction Layer                           */
/*                                                                            */
/* !Component       : MODE                                                    */
/* !Description     : Hazzard Indicator Function module                       */
/*                                                                            */
/* !Module          : MODE                                                    */
/* !Description     : MODE Module interface                                   */
/*                                                                            */
/* !File            : mode_prg.c                                              */
/*                                                                            */
/* !Scope           : Private                                                 */
/*                                                                            */
/* !Coding language : C                                                       */
/*                                                                            */
/* !Project         : Induction training exercise project                     */
/*                                                                            */
/* !Target          : ATMega32                                                */
/*                                                                            */
/* !Compiler        : AVR GCC Cross Compiler                                  */
/*                                                                            */
/*                                                                            */
/******************************************************************************/
/* MODIFICATION LOG :                                                         */
/******************************************************************************/
/*                                                                            */
/* !Coded by        : Nada Hassan             !Date : NOV 25, 2023    */
/*                                                                            */
/******************************************************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "SW_interface.h"
#include "SSD_interface.h"
#include "CLCD_interface.h"


#include "mode_int.h"

/******************************************************************************/

/* Global Variables Definition */

MODE_enutModeState MODE_Status;
MODE_enutModeState LocPrevious;

/*****************************************************************************************/
extern SSD_Type R_SSD ;
extern SSD_Type L_SSD ;

extern SW_Type R_SW ;
extern SW_Type L_SW ;
extern SW_Type H_SW ;

/*****************************************************************************************/

/******************************************************************************/
/*! \Description Helper Functions                                             */
/*! \return      None                                                         */
/*! \Trace_To                                                                 */
/******************************************************************************/
void APP_vidSetMode(MODE_enutModeState enutSetMode)
{
	switch(enutSetMode)
	{
	case IDLE:
		SSD_voidDisable ( R_SSD);
		SSD_voidDisable ( L_SSD);
		CLCD_voidSendString ((u8*)"IDLE Status");
		CLCD_voidSetPosition(1,1);
		CLCD_voidClearScreen();
		break;
	case RIGHT_BLINK:
		SSD_voidEnable ( R_SSD);
		SSD_voidDisable ( L_SSD);
		CLCD_voidSendString ((u8*)"Right Blink Status");
		CLCD_voidSetPosition(1,1);
		CLCD_voidClearScreen();
		break;
	case LEFT_BLINK:
		SSD_voidEnable ( L_SSD);
		SSD_voidDisable ( R_SSD);
		CLCD_voidSendString ((u8*)"Left Blink Status");
		CLCD_voidSetPosition(1,1);
		CLCD_voidClearScreen();
		break;
	case HAZZARD_BLINK:
		SSD_voidEnable ( L_SSD);
		SSD_voidEnable ( R_SSD);
		CLCD_voidSendString ((u8*)"Hazzard Status");
		CLCD_voidSetPosition(1,1);
		CLCD_voidClearScreen();
		break;

	 /* Switch to the Mode  */
	}

}

/***************************************/
void MODE_vidIdle (void)
{
	APP_vidSetMode(IDLE);
	if (SW_u8GetPressed(R_SW)){

		MODE_Status = RIGHT_BLINK;

	}
	else if (SW_u8GetPressed(L_SW)){

		MODE_Status = LEFT_BLINK;

	}
	else if (SW_u8GetPressed(H_SW)){

		MODE_Status = HAZZARD_BLINK;
		LocPrevious = IDLE;


	}
	/* Set Mode to IDLE */
	/* Check the SWs */
}
void MODE_vidRightBlink (void)
{
	APP_vidSetMode(RIGHT_BLINK);
	if (SW_u8GetPressed(L_SW)){

		MODE_Status = IDLE;

	}
	else if (SW_u8GetPressed(H_SW)){

		MODE_Status = HAZZARD_BLINK;
		LocPrevious = RIGHT_BLINK;

	}
	/* Set Mode to RIGHT_BLINK */
	/* Check the SWs */
}
void MODE_vidLeftBlink (void)
{
	APP_vidSetMode(LEFT_BLINK);
		if (SW_u8GetPressed(R_SW)){

			MODE_Status = IDLE;

		}
		else if (SW_u8GetPressed(H_SW)){

			MODE_Status = HAZZARD_BLINK;
			LocPrevious = LEFT_BLINK;

		}
	/* Set Mode to LEFT_BLINK */
	/* Check the SWs */
}
void MODE_vidHazardBlink (void)
{
	APP_vidSetMode(HAZZARD_BLINK);

		if (SW_u8GetPressed(H_SW)){


			MODE_Status = LocPrevious;

		}

	/* Set Mode to HAZZARD_BLINK */
	/* Check the SWs */
}


/*****************************************************************************************/

/******************************************************************************/
/*! \Description Initialize the MODE driver variables                         */
/*! \return      None                                                         */
/*! \Trace_To                                                                 */
/******************************************************************************/
void MODE_vidInit(void)
{
	APP_vidSetMode(IDLE);
	MODE_Status = IDLE;
	/* Set Mode to Default (IDLE) */
}

/******************************************************************************/
/*! \Description The MODE driver task, should be called periodically.
				 This service update the state of all configured push
                 buttons.                                                     */
/*! \return      None                                                         */
/*! \Trace_To                                                                 */
/******************************************************************************/
void MODE_vidTask(void)
{
	if (MODE_Status == IDLE){

		MODE_vidIdle ();

	}
	else if (MODE_Status == RIGHT_BLINK){

		MODE_vidRightBlink ();
		SSD_voidSendNumber       ( R_SSD, 0 );

	}
	else if (MODE_Status == LEFT_BLINK){

		MODE_vidLeftBlink ();
		SSD_voidSendNumber       ( L_SSD, 0 );

	}
	else if(MODE_Status == HAZZARD_BLINK){
		MODE_vidHazardBlink ();
		SSD_voidSendNumber       ( R_SSD, 0 );
		SSD_voidSendNumber       ( L_SSD, 0 );
	}
	/* Check the global enum then call Mode API According to it */
}

/**********************************************************************************************/
