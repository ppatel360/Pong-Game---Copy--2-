// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <sstream> 
#include <math.h> 
#include <gl\gl.h>
#include <gl\glu.h>
#include "GL/freeglut.h"
#include <fstream>
using namespace std;
#pragma comment(lib, "Opengl32.lib")

// keycodes
#define VK_W 0x57
#define VK_S 0x53

// window size and update rate (60 fps)
int width = 500;
int height = 200;
int interval = 1000 / 60;

// score
int score_left = 0;
int score_right = 0;

// rackets in general
int racket_width = 10;
int racket_height = 80;
int racket_speed = 3;

// left racket position
float racket_left_x = 10.0f;
float racket_left_y = 50.0f;

// right racket position
float racket_right_x = width - racket_width - 10;
float racket_right_y = 50;

// ball
float ball_pos_x = width / 2;
float ball_pos_y = height / 2;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
int ball_size = 8;
int ball_speed = 2;

//Number of times ball hits the rackets
int noOfHits = 0;
int ii = 0;
int leftFrequency=12;
int rightFrequency=40;
int repeat = 10;
std::string int2str(int x) {
	// converts int to string
    std::stringstream ss;
    ss << x;
    return ss.str( );
}

void vec2_norm(float& x, float &y) {
        // sets a vectors length to 1 (which means that x + y == 1)
        float length = sqrt((x * x) + (y * y));
        if (length != 0.0f) {
            length = 1.0f / length;
            x *= length;
            y *= length;
        }
    }

/*void keyboard() {
    // left racket
    if (GetAsyncKeyState(VK_W)) racket_left_y += racket_speed;
	if (GetAsyncKeyState(VK_S)) racket_left_y -= racket_speed;

	// right racket
    if (GetAsyncKeyState(VK_UP)) racket_right_y += racket_speed;
    if (GetAsyncKeyState(VK_DOWN)) racket_right_y -= racket_speed;
}*/

void updateLeftRacket() {
	if (GetAsyncKeyState(VK_W)) racket_left_y += racket_speed;
	if (GetAsyncKeyState(VK_S)) racket_left_y -= racket_speed;
}

void updateRightRacket() {
	if (GetAsyncKeyState(VK_UP)) racket_right_y += racket_speed;
    if (GetAsyncKeyState(VK_DOWN)) racket_right_y -= racket_speed;
}

void writeToFile(int x, int y, int rl_x, int rl_y, int rr_x, int rr_y, int hits) {
	//writing position of ball to a file
	ofstream myfile;
	myfile.open ("example.txt",ios::app);
	
	myfile << "(" << x << ", " << y << ")" <<setw(15) <<  "(" << rl_x << ", " << rl_y << ")" << setw(15) <<  "(" << rr_x << ", " << rr_y << ")" << setw(15) << hits << endl;
	myfile.close();
}

void updateBall() {
	
    // fly a bit
    ball_pos_x += ball_dir_x * ball_speed;
    ball_pos_y += ball_dir_y * ball_speed;

	// write the position of ball to the file & No. of hits as well
	writeToFile(ball_pos_x, ball_pos_y, racket_left_x, racket_left_y, racket_right_x, racket_right_y, noOfHits);
	// hit by left racket?
    if (ball_pos_x < racket_left_x + racket_width && 
        ball_pos_x > racket_left_x &&
        ball_pos_y < racket_left_y + racket_height &&
        ball_pos_y > racket_left_y) {
        // set fly direction depending on where it hit the racket
        // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
        float t = ((ball_pos_y - racket_left_y) / racket_height) - 0.5f;
        ball_dir_x = fabs(ball_dir_x); // force it to be positive
        ball_dir_y = t;
		noOfHits = noOfHits+1;
    }

	// hit by right racket?
    if (ball_pos_x > racket_right_x && 
        ball_pos_x < racket_right_x + racket_width &&
        ball_pos_y < racket_right_y + racket_height &&
        ball_pos_y > racket_right_y) {
        // set fly direction depending on where it hit the racket
        // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
        float t = ((ball_pos_y - racket_right_y) / racket_height) - 0.5f;
        ball_dir_x = -fabs(ball_dir_x); // force it to be negative
        ball_dir_y = t;
		noOfHits = noOfHits+1;
    }

    // hit left wall?
    if (ball_pos_x < 0) {
        ++score_right;
        ball_pos_x = width / 2;
        ball_pos_y = height / 2;
        ball_dir_x = fabs(ball_dir_x); // force it to be positive
        ball_dir_y = 0;
    }

    // hit right wall?
    if (ball_pos_x > width) {
        ++score_left;
        ball_pos_x = width / 2;
        ball_pos_y = height / 2;
        ball_dir_x = -fabs(ball_dir_x); // force it to be negative
        ball_dir_y = 0;
    }

    // hit top wall?
    if (ball_pos_y > height) {
        ball_dir_y = -fabs(ball_dir_y); // force it to be negative
    }

    // hit bottom wall?
    if (ball_pos_y < 0) {
        ball_dir_y = fabs(ball_dir_y); // force it to be positive
    }

    // make sure that length of dir stays at 1
    vec2_norm(ball_dir_x, ball_dir_y);
}
int i = 0;
void update(int value) {
   // input handling
   //keyboard();
  
   //Update Left Racket
   //updateLeftRacket();
   
   //Update Right Racket
   //updateRightRacket();

   // update ball
   //updateBall();
   
   // Call update() again in 'interval' milliseconds
   glutTimerFunc(interval, update, 0);

   // Redisplay frame
   glutPostRedisplay();
}

void enable2D(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
}

void drawText(float x, float y, std::string text) {
	glRasterPos2f(x, y);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
}

void drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);	
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void draw() {
    // clear (has to be done at the beginning)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	if((i%rightFrequency)>=0 && (i%rightFrequency)<=repeat) {
        glColor3f(1.0f,1.0f,1.0f);
	// draw rackets
	//drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
	//drawRect(racket_right_x, racket_right_y, racket_width, racket_height);
	
	// draw ball
	drawRect(ball_pos_x - ball_size / 2, ball_pos_y - ball_size / 2, ball_size, ball_size);
    }
    else {
        glColor3f(0.0f,0.0f,0.0f);
	// draw rackets
	//drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
	//drawRect(racket_right_x, racket_right_y, racket_width, racket_height);
	
	// draw ball
	drawRect(ball_pos_x - ball_size / 2, ball_pos_y - ball_size / 2, ball_size, ball_size);
    }
	//glColor3f(1.0f,1.0f,1.0f);
	// draw rackets
	//drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
	//drawRect(racket_right_x, racket_right_y, racket_width, racket_height);
	
	// draw ball
	//drawRect(ball_pos_x - ball_size / 2, ball_pos_y - ball_size / 2, ball_size, ball_size);

	// ToDo: draw our scene

	// draw score
    //drawText(width / 2 - 10, height - 15, int2str(score_left) + ":" + int2str(score_right)); 

	//draw number of hits on racket
	//drawText(width - 20, height - 15, int2str(noOfHits)); 

	//drawText(20, height - 15, "(" + int2str(ball_pos_x) + ", " + int2str(ball_pos_y) + ")"); 
	// swap buffers (has to be done at the end)
	glutSwapBuffers();
	//glutPostRedisplay();
	i++;
}

int _tmain(int argc, char** argv)
{

	// initialize opengl (via glut)
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("noobtuts.com Pong");
	
	// Register callback functions   
    glutDisplayFunc(draw);
    glutTimerFunc(interval, update, 0);

	// setup scene to 2d mode and set draw color to white
    enable2D(width, height);
    glColor3f(1.0f, 1.0f, 1.0f);
	
	// start the whole thing
	glutMainLoop();
	
	return 0;
}

