/*
 * ======================================
 * Name: app.c
 * Author: Maya Hampton
 * Version: 5.0
 * Description: Functionality of all
 * methods required for application
 * ======================================
 */

#include "app.h"
#include "hardware.h"

// platform coordinates
int platformX2 = 40;
int platformY2 = 55;
int platformX1 = 90;
int platformY1 = 60;

// ball size and speed
int radius = 4;
float x = 64.0;
float y = 10.0;
float vx = 100.0;
float vy = 50.0;

float x2 = 64.0;
float y2 = 10.0;
float vx2 = 100.0;
float vy2 = 50.0;
int ball1Active = 1;
int ball2Active = 0;

int count = 0;

/*
 * checks to see if the user has a power-up
 * uses switchflip() to get value
 */
int checkPowerup(int value)
{
	// if the user flips the switch and the score is a multiple of 5
	// activate the power-up
	if (value == 1 && count % 3 == 0)
	{
		// if either ball is inactive activate the other
		if (ball1Active == 0 || ball2Active == 0)
		{
			// activate ball 1
			if (ball1Active == 0){x = 64.0; y = 10.0; ball1Active = 1;}
			// activate ball 2
			if (ball2Active == 0){x2 = 64.0; y2 = 10.0; ball2Active = 1;}
		}
	}
	return 0;
}


/*
 * checks to see what wall or platform the ball has collided with
 */
int checkCollision()
{
	// Ensure the ball stays within bounds
	// checks position for ball 1 if it's active
	if (ball1Active){

		// correct position if ball hits left wall
		if (x - radius < 0)
		{
			x = radius;
			vx = -vx;
		}
		// Correct position if ball hits right wall
		if (x + radius > 128)
		{
			x = 128 - radius;
			vx = -vx;
		}
		// Correct position if ball hits top wall
		if (y - radius < 0)
		{
			y = radius;
			vy = -vy;
		}
		// Correct position if ball hits bottom wall
		if (y + radius > 64)
		{
			// if the other ball is active, keep game going and deactivate current ball
			if (ball2Active)
			{
				eraseBall(x, y);
				ball1Active = 0;
			}
			// if there is only one ball left end the game
			else
			{
				// end game by printing game over message and exiting loop
				drawString();
				return 1;
			}

		}

		// Correct the position if ball hits top of platform
		if ((y + radius > platformY2) && (x + radius < platformX1) && (x - radius > platformX2))
		{
			y = platformY2 - radius;
			vy = -vy;

			// hits top of platform so increase score and display on 7-segment
			count = count + 1;
			increase7Segment();
		}
	}

	// checks position for ball 2 if it's active
	if (ball2Active){

		// correct position if ball hits left wall
		if (x2 - radius < 0)
		{
			x2 = radius;
			vx2 = -vx2;
		}
		// Correct position if ball hits right wall
		if (x2 + radius > 128)
		{
			x2 = 128 - radius;
			vx2 = -vx2;
		}
		// Correct position if ball hits top wall
		if (y2 - radius < 0)
		{
			y2 = radius;
			vy2 = -vy2;
		}
		// Correct position if ball hits bottom wall
		if (y2 + radius > 64)
		{
			// if the other ball is active only deactivate the current ball to keep the game running
			if (ball1Active){
				eraseBall(x2, y2);
				ball2Active = 0;
			}

			else
			{
				// end game by printing game over message and exiting loop
				drawString();
				return 1;
			}
		}

		// Correct position if ball hits top of platform
		if ((y2 + radius > platformY2) && (x2 + radius < platformX1) && (x2 - radius > platformX2))
		{
			y2 = platformY2 - radius;
			vy2 = -vy2;

			// hits top of platform so increase score and display on 7-segment
			count = count + 1;
			increase7Segment();
		}
	}

	return 0;
}

/*
 * move the platform left or right based on button input
 */
int movePlatform(int value)
{
	// if button 1 is pressed, remove the platform at the current position and draw a new platform 10 pixels to the right
	if (value == 1)
	{
		erasePlatform(platformX1, platformY1, platformX2, platformY2);
		platformX1 = platformX1 + 10;
		platformX2 = platformX2 + 10;
		drawPlatform(platformX1, platformY1, platformX2, platformY2);
		usleep(500000);
	}
	// if button 2 is pressed, remove the platform at the current position and draw a new platform 10 pixels to the left
	if (value == 2)
	{
		erasePlatform(platformX1, platformY1, platformX2, platformY2);
		platformX1 = platformX1 - 10;
		platformX2 = platformX2 - 10;
		drawPlatform(platformX1, platformY1, platformX2, platformY2);
		usleep(500000);
	}

	return(0);
}

/*
 * game starts and user plays
 */
int gameStart()
{

	clock_t lastTime = clock();
	int running = 1;

	// LCD screen is initialized
	initLCD();

	// while the ball hasn't touched the bottom edge
	while (running)
	{
		// initializes game time to move ball
		clock_t currentTime = clock();
		double elapsedTime = (double)(currentTime - lastTime) / CLOCKS_PER_SEC; // Convert to seconds
		lastTime = currentTime;

		// if ball 1 is active, move this ball on the screen
		if (ball1Active)
		{
			eraseBall(x, y);

			// update ball's position
			x += vx * elapsedTime * 10;
			y += vy * elapsedTime * 10;

		}

		// if ball 2 is active, move this ball on the screen
		if (ball2Active)
		{
			eraseBall(x2, y2);

			// update ball's position
			x2 += vx2 * elapsedTime * 10;
			y2 += vy2 * elapsedTime * 10;

		}

		// check to see if the ball has collided with any edge on the screen
		if (checkCollision() == 1)
		{
			// if ball collides with bottom edge end the game
			return 0;
		}

		if (ball1Active){drawBall(x, y);}		// draw ball 1 in updated position if active
		if (ball2Active){drawBall(x2, y2);}		// draw ball 2 in updated position if active

		// moves the platform left and right based on button input
		movePlatform(buttonPress());

		// checks to see if the user has a powerup and flipped the switch to use it
		checkPowerup(switchFlip());
	}

	return 0;
}

