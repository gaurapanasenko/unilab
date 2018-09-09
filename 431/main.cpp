#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include "classes.h"

//~ const int screenWidth = 640;
// width of screen window in pixels
//~ const int screenHeight = 480;
// height of screen window in pixels
//~ GLdouble A, B, C, D;
// values used for scaling and shifting

Canvas cvs;
// global canvas object

Object l1;

//~ class Vector {
//~ private:
	//~ Point a, b, d;
	//~ GLfloat length, angle;
	//~ bool length_buf, angle_buf;
//~ public:
	//~ Vector() : a(), b(), d() {}
	//~ Vector(Point one, Point two) : a(), b(), d() {
		//~ set(one, two);
	//~ }
	//~ void set(Point one, Point two) {
		//~ length_buf = 0;
		//~ angle_buf = 0;
		//~ a = one;
		//~ b = two;
		//~ d.set(b.get_x()-a.get_x(), b.get_y()-a.get_y());
	//~ }
	//~ GLfloat get_length() {
		//~ if (length_buf == 0) {
			//~ length = sqrt(pow(d.get_x(),2.0)+pow(d.get_y(),2.0));
			//~ length_buf = 1;
		//~ }
		//~ return length;
	//~ }
	//~ GLfloat get_angle() {
		//~ if (angle_buf == 0) {
			//~ GLfloat x = d.get_x(), y = d.get_y();
			//~ GLfloat angl = acos(x/get_length()) * 180 / M_PI;
			//~ if (y >= 0) angle = angl;
			//~ else if (x < 0 && y < 0) angle = 360 - angl;
			//~ else angle = 360 - angl;
			//~ angle_buf = 1;
		//~ }
		//~ return angle;
	//~ }
	//~ void generate(Point one, GLfloat len, GLfloat angl) {
		//~ a = one;
		//~ length_buf = 1; length = len;
		//~ angle_buf = 1; angle = angl;
		//~ d.set(cos(angle * M_PI / 180) * length, sin(angle * M_PI / 180) * length);
		//~ b.set(a.get_x() + d.get_x(), a.get_y() + d.get_y());
	//~ }
	//~ Point get_a() {return a;}
	//~ Point get_b() {return b;}
	//~ Point get_d() {return d;}
//~ };

//<<<<<<<<<<<<<<< display >>>>>>>>>>>>>>>
void display(void) {
	cvs.clear_screen();
	// clear the screen
	cvs.set_window(-50, 800, -50, 800);
	l1.set("M 70 58 Q 70 21 103 6.5 136 -8 208 -8 274 -8 340.5 -4.5 407 -1 475 -1 499 -1 514 15.5 529 32 529 49 529 66 514 82.5 499 99 475 99 396 99 321.5 96 247 93 170 92 168 144 165.5 195.5 163 247 161 299 q 50 1 100 1.5 50 0.5 101 0.5 23 0 38.5 14.5 Q 416 330 416 351 416 372 400.5 386.5 385 401 362 401 311 401 260 400.5 209 400 158 399 q -2 41 -2 81 v 81 q 0 21 1 42 1 21 3 42 38 0 79 -1 41 -1 81 -2.5 40 -1.5 77 -2.5 37 -1 67 -1 23 0 38.5 14.5 Q 518 667 518 688 518 709 502.5 723.5 487 738 464 738 q -31 0 -71 1 -40 1 -85.5 2 -45.5 1 -94 2 Q 165 744 117 744 78 744 67 697.5 56 651 56 566 56 504 58 437.5 60 371 63 305 66 239 68 176 70 113 70 58 Z");
	l1.render();
	//~ Point a(0,0), b(2,-1), c(0, 0), center(0,0);
	//~ GLfloat angle = 0;
	//~ Vector vec(a,b);
	//~ glBegin(GL_LINE_STRIP);
		//~ center.set(0, 0);
		//~ vec.generate(center, 50.0, 50.0);
		//~ a = vec.get_b();
		//~ a.vertex();
		//~ for(GLfloat x = 50.0; x > 10.0; x -= 0.095) {
			//~ a.calculate_third(center, a, x, 1, 1);
			//~ a.vertex();
		//~ }
		//~ for(GLfloat x = 10.0; x < 60.0; x += 0.105) {
			//~ a.calculate_third(center, a, x, 1, 2);
			//~ a.vertex();
		//~ }
		//~ vec.generate(center, 50.0, 50.0);
		//~ vec.get_b().vertex();
	//~ glEnd();
	glFlush();
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
	glutMainLoop();
	// go into a perpetual loop
	return 0;
}
