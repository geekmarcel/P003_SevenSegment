/*--------------------------------------------------------------------------------------------------------------------------------------------------------
 * Project: 		Post 3 Seven Segment Display
 * Hardware:		Arduino UNO
 * Micro:			ATMEGA328P
 * IDE:			Atmel Studio 6.2
 *
 * Name:    		main.c
 * Purpose: 		Seven segment example, using IC CA3161
 * Date:			20-09-2015
 *
 * Hardware setup:	
 *				CA3161 connected to pins 1 till 4 of PORTD
 *				Pin 1 = 2^0, Pin 2 = 2^1, Pin 3 = 2^2 and Pin 4 = 2^3
 *				Pin 5 connected to point of Segment via 270 Ohm.
 *
 * Note(s):
 *--------------------------------------------------------------------------------------------------------------------------------------------------------*/
  
/************************************************************************/
/* Includes				                                                                  */
/************************************************************************/
#include <avr/io.h>
#include "util/delay.h"

/************************************************************************/
/* Defines				                                                                  */
/************************************************************************/
#define	BYTE	unsigned char	
#define	BOOL	unsigned char
#define MINUS	0b1010
#define E_CHAR	0b1011
#define H_CHAR	0b1100
#define L_CHAR	0b1101
#define P_CHAR	0b1110
#define CLEAR	0b1111

/***************************************************************************
*  Function:		DisplayNumber(int number)
*  Description:		Sets pins 1 till 4 of PORTD to the BCD value which represents a number between 0 and 9.
*  Receives:		Number to display (must be between 0 and 9)
*				Boolean which determines if the point needs to be shown (showPoint > 0) or not (showPoint = 0).
*  Returns:		Nothing
***************************************************************************/
void DisplayNumber(BYTE number, BOOL showPoint)
{
	/* Check if the number is smaller than 10, we can only show numbers 1 to 9 */
	if(number < 10)
	{
		/* Show the number by setting pins 1 to 4 of PORTD 
		 * We dont want to change the values of pins 0 and 5 to 7 so we first AND the current values of PORTD with 0xE1 (the mask) 
		 * This will clear the bit 1 to 4, then we OR in the shifted number.
		 * If we just OR the shifted number with PIND then a old number-value can influence the new setting. 
		 */
		PORTD = (PIND & 0xE1) | (number << 1);	
		
		/* Optionally show Point, showPoint is low active */
		if(showPoint == 0)
		{
			/* We only want to change pin 5, so the mask is now set to 0xDF and anded with the shited inverse of showPoint */
			/* This will clear bit 5 */
			PORTD = (PIND & 0xDF) & ~(showPoint << 5);
		}
		else
		{
			/* We only want to change pin 5, so the mask is now set to 0xDF and ored with the showPoint */
			/* This will set bit 5 */
			PORTD = (PIND & 0xDF) | (showPoint << 5);
		}
	}			
}

/***************************************************************************
*  Function:		DisplaySpecialCharacter(int number)
*  Description:		Sets pins 0 till 3 of PORTD to the BCD value.
*  Receives:		Number to display (must be between 0 and 9)
*				Boolean which determines if the point needs to be shown (showPoint > 0)or not (showPoint = 0).
*  Returns:		Nothing
***************************************************************************/
void DisplaySpecialCharacter(BYTE specialCharacter, BOOL showPoint)
{
	/* Check if the specialCharacter is greater or equal to 10, and smaller then 15 */
	if(specialCharacter >= 10 && specialCharacter < 15)
	{
		/* Show the special character by setting pins 1 to 4 of PORTD 
		 * We dont want to change the values of pins 0 and 5 to 7 so we first AND the current values of PORTD with 0xE1 (the mask) 
		 * This will clear the bit 1 to 4, then we OR in the shifted number.
		 * If we just OR the shifted number with PIND then a old number-value can influence the new setting. 
		 */
		PORTD = (PIND & 0xE1) | (specialCharacter << 1);	
		
		/* Optionally show Point, showPoint is low active */
		if(showPoint == 0)
		{
			/* We only want to change pin 5, so the mask is now set to 0xDF and anded with the shited inverse of showPoint */
			/* This will clear bit 5 */
			PORTD = (PIND & 0xDF) & ~(showPoint << 5);
		}
		else
		{
			/* We only want to change pin 5, so the mask is now set to 0xDF and ored with the showPoint */
			/* This will set bit 5 */
			PORTD = (PIND & 0xDF) | (showPoint << 5);			
		}	
	}			
}

/***************************************************************************
*  Function:		ClearDisplay()
*  Description:		Clears the display and point, by switching off all segments
*  Receives:		Nothing
*				Nothing
*  Returns:		Nothing
***************************************************************************/
void ClearDisplay()
{
	PORTD = (PIND & 0xD1) | (0x1F << 1);
}

/***************************************************************************
*  Function:		Setup(int number)							
*  Description:		Setup PORTD and the timer, this function need to be called at
*				the beginning of main.			
*  Receives:		Nothing			
*  Returns:		Nothing										
***************************************************************************/
void Setup()
{
	/* Setup PORTD */
	/* Set pins 1 till 5 as output , and pins 1 and 6 to 7 as input*/
	/* We skip pin 0 because this pin is needed for programming the chip */
	DDRD = 0b00111110;	
}

/***************************************************************************
*  Function:		Main(void)
*  Description:		Main function of the program.
*  Receives:		Nothing
*  Returns:		Optionally the exit code.
***************************************************************************/
int main(void)
{
	/* Initialize variables */
	BYTE bcdCode = 0;
	BOOL showPoint = 1;
	
	/* Setup PORT D */
	Setup();
	
	/* Test the code by showing all the 15 characters + clear and loop indefinitely */
    while(1)
    {
		/* If the value is below 10, then display a number ,otherwise display special character */
		if(bcdCode < 10)
		{
			/* Display the BCD code and afterwards increment the bcdCode variable for the next iteration. */
			DisplayNumber(bcdCode++, showPoint);
		}
		else
		{
			/* Show special character and afterwards increment bcdCode variable for the next iteration */
			DisplaySpecialCharacter(bcdCode++, showPoint);
		}		
		
		/* If we are at the end of BCD codes then reset to 0 */ 
		if(bcdCode == 15)
		{
			bcdCode = 0;
			
			/* Toggle showPoint Boolean */
			showPoint ^= 1;
				
			ClearDisplay();		
		}
		
		/* Delay for 1 second */	
		_delay_ms(1000);
    }
}