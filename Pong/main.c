/*
 * ======================================
 * Name: main.c
 * Author: Maya Hampton
 * Version: 1.0
 * Description: Functionality of all
 * methods required for application
 * ======================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hardware.h"
#include "main.h"

// whenever the ball hits the platform the score increases by one and updates the 7-segment display
void incScore()
{
	printf("ball hits platform - score increases by 1\n");
	write7Segment();
}

// when the ball hits the bottom edge of the screen the game is over and this will display
void gameOver()
{
	printf("ball hits bottom edge - display game over screen\n");
	writeLCD("Game Over\n");
	// user presses button to end the game
	buttonInput();
}

// game starts and allows user to play
void gameStart()
{
	printf("Game is starting\n");

	// display game components (ball and platform) no text
	writeLCD(" ");
	// drop ball
	/*
	 * if buttonInput == left
	 * 		move platform left
	 * else if buttonInput == right
	 * 		move platform right
	 * else
	 * 		do nothing
	 *
	 * if ball touches bottom edge
	 * 		gameOver();
	 *
	 * if ball touches platform
	 * 		incScore();
	 *
	 * if ball touches left edge
	 * 		ball go opposite direction
	 *
	 * if ball touches right edge
	 * 		ball go opposite direction
	 */
}

int main(void)
{

	gameStart();
	gameOver();
	incScore();
	write7Segment();
	writeLCD("Test print to LCD");
	buttonInput();

	return EXIT_SUCCESS;
}


