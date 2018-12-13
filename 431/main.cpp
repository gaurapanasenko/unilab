/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.cpp
 * Copyright (C) 2018 Egor Panasenko <gaura.panasenko@gmail.com>
 *
 * foobar-cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * foobar-cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


// Including GL libraries
// ----------------------
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>


// Including standard libraries
// ----------------------------
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <vector>

// Including my classes
// --------------------
#include "classes.h"

using namespace std;

// Create canvas as display controller
// -----------------------------------
Canvas cvs;


// Array of objects
// ----------------
vector<Object> arr;


// Some variables
// --------------
int active = -1;
int ani1 = 0, ani2 = 0; //
GLfloat ang = 0;

// Render function
// ---------------
void display(void) {
	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set area of visibility
	cvs.set_ortho(-425*1.25, 425*1.25, -425*1.25, 425*1.25);
	
	glPushMatrix();
		glRotatef(ang, 0, 0, 1); // Rotate all letters
		if (arr.size() > 0) arr[0].render();
		if (arr.size() > 1) arr[1].render();
	glPopMatrix();

	// Show on screen
	glutSwapBuffers();
}

// Resize callback
// ---------------
void reshape(GLint W, GLint H) {
	cvs.set_window_size (W, H);
}

// Keyboard callback
// -----------------
void keyboard(unsigned char theKey, GLint mouse_x, GLint mouse_y) {
	switch (theKey) {
		case 27:
			exit(0);
			break;
		case 'w':
			if (active == -1)
				for (size_t i = 0; i < arr.size(); i++)
					arr[i].move_rel(Point(0, 50));
			else arr[active].move_rel(Point(0, 50));
			break;
		case 's':
			if (active == -1)
				for (size_t i = 0; i < arr.size(); i++)
					arr[i].move_rel(Point(0, -50));
			else arr[active].move_rel(Point(0, -50));
			break;
		case 'd':
			if (active == -1)
				for (size_t i = 0; i < arr.size(); i++)
					arr[i].move_rel(Point(50, 0));
			else arr[active].move_rel(Point(50, 0));
			break;
		case 'a':
			if (active == -1)
				for (size_t i = 0; i < arr.size(); i++)
					arr[i].move_rel(Point(-50, 0));
			else arr[active].move_rel(Point(-50, 0));
			break;
		case 'q':
			if (active == -1)
				for (size_t i = 0; i < arr.size(); i++)
					arr[i].rotate_rel(-5);
			else arr[active].rotate_rel(-5);
			break;
		case 'e':
			if (active == -1)
				for (size_t i = 0; i < arr.size(); i++)
					arr[i].rotate_rel(5);
			else arr[active].rotate_rel(5);
			break;
		case '\t':
			active++;
			if ((int)arr.size() == active) active = -1;
			break;
		case 'z':
			arr[0].fill(Color(rand()%256*1.0/256,
			                           rand()%256*1.0/256,
			                           rand()%256*1.0/256));
			break;
		case 'x':
			arr[1].fill(Color(rand()%256*1.0/256,
			                           rand()%256*1.0/256,
			                           rand()%256*1.0/256));
			break;
		case 'c':
			arr[0].refill();
			arr[1].refill();
			break;
		case 'r':
			if (arr.size() > 0) {
				arr[0].rotate_to(-10);
				arr[0].move_to(Point(-100, 100));
			}
			if (arr.size() > 1) {
				arr[1].rotate_to(10);
				arr[1].move_to(Point(100, -100));
			}
			break;
	}
	display();
}

// Special keyboard callback
// -------------------------
void special_keyboard(int theKey, GLint mouse_x, GLint mouse_y) {
	switch (theKey) {
		case GLUT_KEY_F6:
			if (ani1 != -1 || ani2 != -1) {
				ani1 = -1;ani2 = -1;
			} else {ani1 = 0; ani2 = 0;}
			break;
		case GLUT_KEY_F7:
			if (ani1 != 1 || ani2 != 1) {
				ani1 = 1; ani2 = 1;
			} else {ani1 = 0;ani2 = 0;}
			break;
		case GLUT_KEY_F8:
			if (ani1 != 1 || ani2 != -1) {
				ani1 = 1; ani2 = -1;
			} else {ani1 = 0; ani2 = 0;}
			break;
		case GLUT_KEY_PAGE_UP:
			ang += 10;
			break;
		case GLUT_KEY_PAGE_DOWN:
			ang -= 10;
			break;
	}
	display();
}

// Timer function for animation
// ----------------------------
void timer_function(int value) {
	if (ani1 && arr.size() > 0) arr[0].rotate_rel(ani1 * 10);
	if (ani2 && arr.size() > 1) arr[1].rotate_rel(ani2 * 10);
	if (ani1 || ani2)
		glutPostRedisplay();
	glutTimerFunc(33, timer_function, 1);
}

int main(int argc, char * argv[]) {
	// Letters data
	std::stringstream ss(
"B 0.8 0.8 0.8 F 0.2 0.8 0.2 M 70 58 Q 70 21 103 6.5 136 -8 208 -8 274 -8 "
"340.5 -4.5 407 -1 475 -1 499 -1 514 15.5 529 32 529 49 529 66 514 82.5 499 99 "
"475 99 396 99 321.5 96 247 93 170 92 168 144 165.5 195.5 163 247 161 299 q 50 "
"1 100 1.5 50 0.5 101 0.5 23 0 38.5 14.5 Q 416 330 416 351 416 372 400.5 386.5 "
"385 401 362 401 311 401 260 400.5 209 400 158 399 q -2 41 -2 81 v 81 q 0 21 1 "
"42 1 21 3 42 38 0 79 -1 41 -1 81 -2.5 40 -1.5 77 -2.5 37 -1 67 -1 23 0 38.5 "
"14.5 Q 518 667 518 688 518 709 502.5 723.5 487 738 464 738 q -31 0 -71 1 -40 "
"1 -85.5 2 -45.5 1 -94 2 Q 165 744 117 744 78 744 67 697.5 56 651 56 566 56 "
"504 58 437.5 60 371 63 305 66 239 68 176 70 113 70 58 Z B 0.2 0.2 0.2 F 0.1 "
"0.6 0.3 M 151 26 Q 147 0 132 -12 117 -24 100 -24 81 -24 66 -9.5 51 5 51 34 "
"q 2 83 9 165 7 82 15 164 l 9 106 q -1 17 -3 47 -2 30 -2 62 0 24 2 47 2 23 6.5 "
"42 4.5 19 12.5 33 8 14 21 20 12 5 26.5 7 14.5 2 28.5 2 54 0 105.5 -5 51.5 -5 "
"108.5 -5 19 0 37 1 18 1 35 5 20 -2 31 -9.5 11 -7.5 21 -24.5 14 -41 18.5 -82.5 "
"4.5 -41.5 4.5 -83.5 0 -63 -3 -126 -3 -63 -7.5 -126 l -9 -126 q 0 0 -6.5 -126 "
"-4 -26 -19 -38 -15 -12 -32 -12 -19 0 -34 14.5 Q 411 0 411 29 q 4 120 14 238.5 "
"10 118.5 10 239.5 0 29 -1 58.5 -1 29.5 -3 58.5 -63 0 -124 1.5 -61 1.5 -124 "
"1.5 0 -6 -0.5 -12 -0.5 -6 -0.5 -12 0 -26 1.5 -52 1.5 -26 1.5 -52 0 -9 -0.5 "
"-17.5 Q 184 473 184 465 177 354 165 245.5 153 137 151 26 Z");
	// Convert data to objects
	ss >> arr;
	// Set initial positions
	if (arr.size() > 0) {
		arr[0].rotate_to(-10);
		arr[0].move_to(Point(-100, 100));
	}
	if (arr.size() > 1) {
		arr[1].rotate_to(10);
		arr[1].move_to(Point(100, -100));
	}
	// Initialize the toolkit
	glutInit(&argc, argv);
	// Set display mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	// Set window size
	glutInitWindowSize(700, 700);
	// Set window position
	glutInitWindowPosition(25, 25);
	// Open the screen window
	glutCreateWindow("Laboratory work 1 by Egor Panasenko");
	// Config view controller
	cvs.set_window_size(700, 700);
	// Fill background
	glClearColor(0.95, 0.95, 0.95, 0);
	// register redraw function
	glutDisplayFunc(display);
	// specifies the function called on a resize event
	glutReshapeFunc(reshape);
	// keyboard event
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special_keyboard);
	// set timer function
	glutTimerFunc(33, timer_function, 1);
	// go into a perpetual loop
	glutMainLoop();
	return 0;
}
