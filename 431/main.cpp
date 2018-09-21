#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include "classes.h"

using namespace std;

Classes::Canvas cvs;
// global canvas object

vector<Classes::Object> arr;

//<<<<<<<<<<<<<<< display >>>>>>>>>>>>>>>
void display(void) {
	cvs.clear_screen();
	// clear the screen
	cvs.set_window(-425, 425, -425, 425);
	arr[0].rotate_to(90);
	//~ arr[0].move_to(Classes::Point(-100, 325));
	arr[0].render();
	//~ exit(0);
	// send all output to display
}

//<<<<<<<<<<<<<<< reshape >>>>>>>>>>>>>>>
void reshape(GLint W, GLint H) {
	cvs.resize(W, H);
}

//<<<<<<<<<<<<<<< keyboard >>>>>>>>>>>>>>>
void keyboard(unsigned char theKey, GLint mouse_x, GLint mouse_y) {
	GLint x = mouse_x, y = cvs.get_height() - mouse_y;
	// flip the y value as always
	switch (theKey) {
		case 27:
			exit(0);
	}
}

//<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>
int main(int argc, char * argv[]) {
	cvs.init(argc, argv, 500, 500, (char *)"Лабораторна робота 1");
	// the window opened in the Canvas constructor
	cvs.set_background_color(0.95, 0.95, 0.95);
	// background is white
	cvs.set_color(0, 0, 0);
	// set drawing color
	glutDisplayFunc(display);
	// register redraw function
	glutReshapeFunc(reshape);
	// specifies the function called on a resize event
	glutKeyboardFunc(keyboard);
	// keyboard event
	parse_string(arr,"M 70 58 Q 70 21 103 6.5 136 -8 208 -8 274 -8 340.5 -4.5 407 -1 475 -1 499 -1 514 15.5 529 32 529 49 529 66 514 82.5 499 99 475 99 396 99 321.5 96 247 93 170 92 168 144 165.5 195.5 163 247 161 299 q 50 1 100 1.5 50 0.5 101 0.5 23 0 38.5 14.5 Q 416 330 416 351 416 372 400.5 386.5 385 401 362 401 311 401 260 400.5 209 400 158 399 q -2 41 -2 81 v 81 q 0 21 1 42 1 21 3 42 38 0 79 -1 41 -1 81 -2.5 40 -1.5 77 -2.5 37 -1 67 -1 23 0 38.5 14.5 Q 518 667 518 688 518 709 502.5 723.5 487 738 464 738 q -31 0 -71 1 -40 1 -85.5 2 -45.5 1 -94 2 Q 165 744 117 744 78 744 67 697.5 56 651 56 566 56 504 58 437.5 60 371 63 305 66 239 68 176 70 113 70 58 Z");
	glutMainLoop();
	// go into a perpetual loop
	return 0;
}
