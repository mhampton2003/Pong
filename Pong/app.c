/*
 * ======================================
 * Name: app.c
 * Author: Maya Hampton
 * Version: 2.0
 * Description: Functionality of all
 * methods required for application
 * ======================================
 */

#include "app.h"
#include "hardware.h"

int count = 0;
int platformX2 = 40;
int platformY2 = 55;
int platformX1 = 90;
int platformY1 = 60;
int radius = 4;
float x = 64.0;
float y = 16.0;
float vx = 100.0;
float vy = 50.0;

/*
 * checks to see what the ball has collided with based on the input position
 */
int checkCollision()
{

	// Ensure the ball stays within bounds
	// correct position if it hits left wall
	if (x - radius < 0)
	{
		x = radius;
		vx = -vx;
	}
	// Correct position if it hits right wall
	if (x + radius > 128)
	{
		x = 128 - radius;
		vx = -vx;
	}
	// Correct position if it hits top wall
	if (y - radius < 0)
	{
		y = radius;
		vy = -vy;
	}
	// Correct position if it hits bottom wall
	if (y + radius > 64)
	{
		y = 64 - radius;
		vy = -vy;

		// end game by printing game over message and exiting loop
		drawString();

		return 1;
	}
	// Correct position if it hits top of platform
	if ((y + radius > platformY2) && (x + radius < platformX1) && (x - radius > platformX2))
	{
		y = platformY2 - radius;
		vy = -vy;

		// hits top of platform so increase score and display on 7-segment
		count = count + 1;
		increase7Segment(count);
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
	writeLCD();

	// while the ball hasn't touched the bottom edge
	while (running)
	{
		clock_t currentTime = clock();
		double elapsedTime = (double)(currentTime - lastTime) / CLOCKS_PER_SEC; // Convert to seconds
		lastTime = currentTime;

		eraseBall(x, y);

		// update ball's position
		x += vx * elapsedTime * 10;
		y += vy * elapsedTime * 10;

		// check to see if the ball has collided with any edge on the screen
		if (checkCollision() == 1)
		{
			// if ball collides with bottom edge end the game
			return 0;
		}

		drawBall(x, y);

		// moves the platform left and right based on button input
		movePlatform(buttonPress());

	}

	return 0;
}




