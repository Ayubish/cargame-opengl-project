#include <bits/stdc++.h>
#include<GL/glut.h>
#define MAX_CARS 3
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void drawCar();
void drawFootPath();
void drawDivider();
void drawSurroundings();
void stopGame();
void drawOtherCars(int);
void drawOther();
void setCars();
void moveOtherCars();
int detectCollision();
void drawString(std::string);
void drawMainMenu();
void drawExitMenu();
void drawTime();
void stopGame();
void resumeGame();
void drawScore(int);
void drawDistanceBar();
void drawEnd();
void moveEnd();
void drawFuelBar();
void fuelMessage();
void drawFuel();
void drawNavMenu();
void about();
void drawLevel();
void draw_status(std::string, bool);


int foot_y = 200;
int div_y = 220;
int end_y = 160;

int a = 0;
int speed = 1;
int steerSpeed = 1;
int temp = 0;
int seconds = 0;
int distance = 178;
int fuel = 178;

int game_state = 0;

void moveDivider();
void moveRoad();
void moveCar();
void periodicFunction(int);
void moveFuel();

int lane[MAX_CARS];
int pos[MAX_CARS];
int speeds[MAX_CARS];
int speedsTemp[MAX_CARS];

GLdouble width=1200, height=800;
//some boolean to handle the game
bool carMoveLeft = false, carMoveRight = false, carMoveFast = false, carStopped = true, gameStopped = true, horn = false;
bool highlightStart = false, highlightExit = false, highlightLvl1 = false, highlightLvl2 = false, highlightLvl3 = false, highlightAdventure = false, highlightBack = false, breakk =false;
bool reachedEnd = false, gameCompleted = false, fuelOver = false;
//Car variables
int car_x = 0;
int car_y = -100;

int fuel_x = 0;
int fuel_y = -80;

//
void drawText(const char* text, float x, float y){
    glRasterPos2f(x, y);
    for(size_t i=0; i<strlen(text); i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}
void draw_string(std::string str){
    for(unsigned int i=0;i<str.length();i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *(str.begin()+i)); //32 -> 127 //maximum top 119.05 units //the bottom descends 33.33 units
    }
}

void maindisp(){

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);


   switch(game_state)
	{
        case 1 : glClearColor(0.5, 0.5, 0.5, 0.0);
                 drawFootPath();
                 drawSurroundings();
                 drawDivider();
                 drawCar();
                 drawOther();
                 drawTime();
                 if(!highlightAdventure) drawDistanceBar();
                 drawFuelBar();
                 drawFuel();
                 carStopped = false;
                 gameStopped = false;
                 break;

        case 0 : drawMainMenu();drawNavMenu();

                 break;

        case 2 : glClearColor(0.5, 0.5, 0.5, 0.0);
                 drawFootPath();
                 drawSurroundings();
                 drawCar();
                 drawDistanceBar();
                 drawFuelBar();
                 drawExitMenu();
                 drawTime();
                 drawNavMenu();
                 if(gameCompleted){
                    drawEnd();
                    draw_status("You Have Won", 1);
                 } else if(fuelOver){
                    draw_status("Fuel Exhausted!", 0);
                 }else draw_status("You Have Lost!", 0);
                 about();
                 break;
        case 3:
            drawLevel();
            break;

	}
	glFlush();
	glutSwapBuffers();
}


void keyBoardFunc(int key, int x, int y){
	switch(key)
	{
		case GLUT_KEY_LEFT : carMoveLeft = true; carMoveRight = false; break;
		case GLUT_KEY_RIGHT : carMoveRight = true; carMoveLeft = false; break;
		case GLUT_KEY_UP 	: carMoveFast = true; break;
	}
}


void keyboard_up_func(int k, int x, int y){
    switch(k)
    {
        case GLUT_KEY_LEFT: carMoveLeft=false;
                            break;
        case GLUT_KEY_RIGHT:carMoveRight=false;
                            break;

		case GLUT_KEY_UP : carMoveFast = false; break;
		case GLUT_KEY_DOWN : carStopped = false; break;
    }
}
//
void normalKeyBoardFunc(unsigned char key, int x, int y){
    if(key == 'q' or key == 'Q'){
        exit(0);
    }
    if(key == 'e' or key == 'E'){
        game_state = 3;
        gameStopped = true;

    }
    if(game_state == 1)
    {
        switch(key)
        {
            //Horn
            case 'H':
            case 'h': horn = true; break;

        }
    }

    else if(game_state == 0)
    {
        if(key == 13)
        {
            setCars();
            game_state = 3;
        }else if(key == 'a' || key == 'A'){
            about();
        }
    }else if(game_state == 2){ // Check if the game is in the restartable state
    if(key == 'R' || key == 'r' || key==13)
    {
        gameStopped = false;
        gameCompleted = false;
        reachedEnd = false;
        seconds = 0;
        distance = 178;
        fuel = 178;
        fuelOver = false;
        end_y = 160;
        setCars();
        game_state = 1;
        car_x = 0;
    }
}
}

void draw_status(std::string text, bool c){
    glPushMatrix();
    glTranslated(-100, 70, 0);
    glLineWidth(6);
    glScalef(0.2, 0.2, 0.2);

    if(c) glColor3f(0.0, 1.0, 0.0);
    else glColor3f(1.0, 0.0, 0.0);

    draw_string(text);
    glPopMatrix();
    glLineWidth(1);
}

void drawLevel(){
    //Draw start button
    glClearColor(0.5, 0.5, 0.5, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    drawFootPath();
    drawSurroundings();
    drawDivider();
    drawCar();

    glColor3f(0, 0, 0);
    glLineWidth(3);
    //Level 1 Button
    glPushMatrix();
    glTranslated(-80, 30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-100, 25, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("Lvl 1");

    glPopMatrix();

    //Level 2 Button
    glPushMatrix();
    glTranslated(0, 30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-20, 25, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("Lvl 2");
    glPopMatrix();

    //level 3 Button
    glPushMatrix();
    glTranslated(80, 30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(60, 25, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("Lvl 3");
    glPopMatrix();

    //Adventure Mode

    glPushMatrix();
    glTranslated(0, -20, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(60, 15); //top-right
        glVertex2f(60, -15); //bottom-right
        glVertex2f(-60, -15); //bottom-left
        glVertex2d(-60, 15); //top-left
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-50, -20, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("Adventure Mode");
    glPopMatrix();

    //Back Button
    glPushMatrix();
    glTranslated(0, -75, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-20, -80, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("Back");
    glPopMatrix();


    //level 1 hover highlight
    if(highlightLvl1)
    {
        glColor3f(0.0f, 1.0f, 0.0f);

        glPushMatrix();
    glTranslated(-80, 30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(35, 20);
        glVertex2f(35, -20);
        glVertex2f(-35, -20);
        glVertex2d(-35, 20);
    glEnd();
    glPopMatrix();

    }
    //Level 2 hover highlight
    if(highlightLvl2)
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        glPushMatrix();
        glTranslated(0, 30, 0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(35, 20);
            glVertex2f(35, -20);
            glVertex2f(-35, -20);
            glVertex2d(-35, 20);
        glEnd();
        glPopMatrix();
    }

    //Level 3 hover highlight
    if(highlightLvl3)
    {
        glColor3f(0.0f, 1.0f, 0.0f);

         glPushMatrix();
    glTranslated(80, 30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(35, 20);
        glVertex2f(35, -20);
        glVertex2f(-35, -20);
        glVertex2d(-35, 20);
    glEnd();
    glPopMatrix();

    }
    //Adventure hover highlight
    if(highlightAdventure)
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        glPushMatrix();
        glTranslated(0, -20, 0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(65, 20); //top-right
            glVertex2f(65, -20); //bottom-right
            glVertex2f(-65, -20); //bottom-left
            glVertex2d(-65, 20); //top-left
        glEnd();
        glPopMatrix();
    }
    //Back button hover highlight
    if(highlightBack)
    {
        glColor3f(1.0f, 0.0f, 0.0f);
         glPushMatrix();
        glTranslated(0, -75, 0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(35, 20);
            glVertex2f(35, -20);
            glVertex2f(-35, -20);
            glVertex2d(-35, 20);
        glEnd();
        glPopMatrix();
    }
}

//
void mouse_func(int button, int state, int x, int y){
    switch(game_state)
    {
        case 0 : if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
                 {
                    if(x >=  630 && x <= 810 && y >= 260 && y <= 360)
                    {
                        //gameStopped = false;
                        //setCars();
                        game_state = 3;
                    }

                    else if(x >=  630 && x <= 810 && y >= 400 && y <= 540) exit(0);
                 }
                 break;

        case 2 : if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
                 {
                    if(x >=  630 && x <= 810 && y >= 260 && y <= 360)
                    {
                        game_state = 3;
                        //gameStopped = false;
                        //gameCompleted = false;
                        //reachedEnd = false;
                        seconds = 0;
                        distance = 178;
                        fuel = 178;
                        fuelOver = false;
                        end_y = 160;
                        setCars();
                        //game_state = 1;
                        car_x = 0;
                    }

                    else if (x >=  630 && x <= 810 && y >= 400 && y <= 540)
                        exit(0);
                 }
                 break;
        case 3: if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
            if((x >=  360 && x <= 580 && y >= 260 && y <= 360) || (x >= 590 && x <= 810 && y >= 260 && y <= 360) || (x >=  820 && x <= 1030 && y >= 260 && y <= 360) || (x >=  550 && x <= 850 && y >= 380 && y <= 480)){
                gameStopped = false;
                gameCompleted = false;
                reachedEnd = false;
                seconds = 0;
                distance = 178;
                fuel = 178;
                fuelOver = false;
                end_y = 160;
                setCars();
                game_state = 1;
                car_x = 0;
            } else if(x >= 590 && x <= 810 && y >= 260 && y <= 360){
                gameStopped = false;
        } else if(x >=  820 && x <= 1030 && y >= 260 && y <= 360){
                gameStopped = false;

    } else if(x >=  590 && x <= 810 && y >= 500 && y <= 600){
                gameStopped = false;
                game_state = 0;
                drawNavMenu();
                gameStopped = true;

    }

    }
}
}
//
void mouse_hover(int x, int y){
        if(x >=  630 && x <= 810 && y >= 260 && y <= 360){
            highlightStart = true;
            highlightExit = false;
        } else if(x >=  630 && x <= 810 && y >= 400 && y <= 540){
            highlightStart = false;
            highlightExit = true;
        } else{
            highlightExit = false;
            highlightStart = false;
    }
        //if(game_state == 3) {

            if(x >=  360 && x <= 580 && y >= 260 && y <= 360){
                highlightLvl1 = true;
                highlightLvl2 = false;
                highlightLvl3 = false;
                highlightAdventure = false;
                highlightBack = false;
            }else if(x >= 590 && x <= 810 && y >= 260 && y <= 360){
                highlightLvl1 = false;
                highlightLvl2 = true;
                highlightLvl3 = false;
                highlightAdventure = false;
                highlightBack = false;
            } else if(x >=  820 && x <= 1030 && y >= 260 && y <= 360){
                highlightLvl1 = false;
                highlightLvl2 = false;
                highlightLvl3 = true;
                highlightAdventure = false;
                highlightBack = false;
            } else if(x >=  550 && x <= 850 && y >= 380 && y <= 480){
                highlightLvl1 = false;
                highlightLvl2 = false;
                highlightLvl3 = false;
                highlightAdventure = true;
                highlightBack = false;
            } else if(x >=  590 && x <= 810 && y >= 500 && y <= 600){
                highlightLvl1 = false;
                highlightLvl2 = false;
                highlightLvl3 = false;
                highlightAdventure = false;
                highlightBack = true;
            } else{
                highlightLvl1 = false;
                highlightLvl2 = false;
                highlightLvl3 = false;
                highlightAdventure = false;
                highlightBack = false;
            }
        //}
}
//


void drawCar(void){

    glPushMatrix();
    glTranslated(car_x, car_y, 0.0);
    glColor3f(1.0, 1.0, 0.0);


     glColor3f(0.3, 0.3, 0.3);
     glBegin(GL_QUADS);
        glVertex2f(-6.0,15.0); //top-left
        glVertex2f(-10.0,6.0); //bottom-left
        glVertex2f(10.0,6.0); //bottom-right
        glVertex2f(6.0,15.0); //top-right
    glEnd();
    //body
    glRectd(-10, 6, 10, -26);
    //back coffin
    glBegin(GL_QUADS);
        glVertex2f(-10.0,-26.0); //top-left
        glVertex2f(-8.0,-32.0); //bottom-left
        glVertex2f(8.0,-32.0); //bottom-right
        glVertex2f(10.0,-26.0); //top-right
    glEnd();
    //tires
    glColor3f(0,0,0); //color
    glRectd(-12, 5, -10, -2);
    glRectd(10, 5, 12, -2);
    glRectd(-12, -22, -10, -29);
    glRectd(10, -22, 12, -29);

    //pausa lights
    glColor3f(1,1,0);
    glRectd(-6,13, -4, 10);
    glRectd(4,13, 6, 10);

    //sunroof
    glColor3f(0,0,1);
    glRectd(-8, 6, 8, -4);
    glRectd(-8, -11, 8, -21);

    glColor3f(0,0,0);
    glRectd(-6, -32, 6, -34);

	glPopMatrix();
}
//
void drawOtherCars(int i){
    glPushMatrix();

    glTranslated((lane[i] - 1)*37, pos[i], 0.0);

    switch(speeds[i])
    {
        case 2 : glColor3f(0.0, 1.0, 0.0); break;
        case 3 : glColor3f(0.74,0.25,0.74); break;
        case 4 :
        case 5 :
        case 6:
        case 7 : glColor3f(1.0, 0.0, 0.0); break;
        case 0 : glColor3f(0.0, 0.0, 0.0); break;
    }

     glBegin(GL_QUADS);
        glVertex2f(-6.0,15.0); //top-left
        glVertex2f(-10.0,6.0); //bottom-left
        glVertex2f(10.0,6.0); //bottom-right
        glVertex2f(6.0,15.0); //top-right
    glEnd();
    //body
    glRectd(-10, 6, 10, -26);
    //back
    glBegin(GL_QUADS);
        glVertex2f(-10.0,-26.0); //top-left
        glVertex2f(-8.0,-32.0); //bottom-left
        glVertex2f(8.0,-32.0); //bottom-right
        glVertex2f(10.0,-26.0); //top-right
    glEnd();
    //tires
    glColor3f(0.2,0.2,0.2); //color
    glRectd(-12, 5, -10, -2);
    glRectd(10, 5, 12, -2);
    glRectd(-12, -22, -10, -29);
    glRectd(10, -22, 12, -29);

    //pausa lights
    glColor3f(1,1,0);
    glRectd(-6,13, -4, 10);
    glRectd(4,13, 6, 10);

    //sunroof
    glColor3f(0,0,1);
    glRectd(-8, 6, 8, -4);
    glRectd(-8, -11, 8, -21);

    glColor3f(0,0,0);
    glRectd(-6, -32, 6, -34);


	glPopMatrix();
}
//
void drawFootPath(){
	int i, y;
	glPushMatrix();

	//Draw right side foot path
	glTranslated(60, 0, 0);
	y = foot_y + 20;

	for(i = 0; i < 20; i++)
	{
		if(a == 0)
		{
			if(i % 2 == 0)
				glColor3f(1.0, 1.0, 1.0);
			else
				glColor3f(0.0, 0.0, 0.9);
		}else{
			if(i % 2 == 1)
				glColor3f(1.0, 1.0, 1.0);
			else
				glColor3f(0.0, 0.0, 0.9);
		}


		y -= 20;
		glRectd(5, y, -5, y - 20);
	}
	glPopMatrix();

	//Draw left side foot path
	glPushMatrix();
	glTranslated(-60, 0, 0);

	y = foot_y + 20;
	for(i = 0; i < 20; i++)
	{
		if(a == 0)
		{
			if(i % 2 == 0)
				glColor3f(1.0, 1.0, 1.0);
			else
				glColor3f(0.0, 0.0, 0.9);
		}

		else
		{
			if(i % 2 == 1)
				glColor3f(1.0, 1.0, 1.0);
			else
				glColor3f(0.0, 0.0, 0.9);
		}

		y -= 20;
		glRectd(5, y, -5, y - 20);
	}
	glPopMatrix();

}
//
void drawDivider(){
	int i;
	int y = div_y + 80;
	glColor3f(1.0, 1.0, 1.0);

	for(i = 0 ; i < 8; i++)
	{
		y-=80;
		glRectd(22, y, 18, y-40);
		glRectd(-18, y, -22, y-40);
	}

}
//
void drawSurroundings(){
	glColor3f(0.9f, 0.9f, 0.9f);
	glRectd(240, 160, 65, -160);
	glRectd(-240, 160, -65, -160);
}
//
void stopGame(){
	speed = 0;
	steerSpeed = 0;
}
//
void resumeGame(){
    speed = 2;
    steerSpeed = 1;
}
//
void drawOther(){
	int i;
	for(i = 0; i < MAX_CARS; i++)
	{
		drawOtherCars(i);
	}
}
//
void setCars(){
	int i;
	for(i = 0; i < MAX_CARS; i++)
	{
		lane[i] = i;
		pos[i] = 110 + rand()%100;
        speeds[i] = 1 + i + rand()%4;
	}
}
//
void moveDivider(){
	div_y -= speed;
	if(div_y < 120 && distance > 0)
    {
        div_y = 200;

        if(carMoveFast)
        {
            if(!highlightAdventure) distance -= 1.5;
            fuel -= 3;
        }
        else
        {

            if(!highlightAdventure) distance -= 0.5;
            fuel -= 2;
        }

        if(fuel < 0)
            fuelOver = true;

        if(distance < 5)
            reachedEnd = true;

    }
}
//
void moveCar(){
	if(carMoveLeft)
		car_x -= steerSpeed;
	else if(carMoveRight)
		car_x += steerSpeed;

	if(car_x > 45 || car_x < -45)
    {
        game_state = 2;
        gameStopped = true;
    }
}
//
void moveRoad(){
	foot_y -= speed;
	if(foot_y < 160)
	{
		foot_y = 180;
		if(a == 0)
			a = 1;
		else
			a = 0;
	}
}
//
void periodicFunction(int v){
    if(gameStopped)
        stopGame();

    else
        resumeGame();

	if(speed != 0)
	{
    if(highlightLvl1){
		if(carMoveFast)
            speed = 5;
		else
            speed = 2;
    }else if(highlightLvl2){
		if(carMoveFast)
            speed = 5;
		else
            speed = 2;
    } else if(highlightLvl3){
		if(carMoveFast)
            speed = 5;
		else
            speed = 2;
    } else if(highlightAdventure){
        if(carMoveFast){
            speed = 4 + seconds/5;
            //if(seconds>5)speed = 15;
        }
		else{
            speed = 2 + seconds/5;
            //if(seconds>5)speed = 6;
		}
    }
	}
	if(fuelOver)
	{
        gameStopped = true;
        game_state = 2;

	}

	moveRoad();
	moveDivider();
	moveCar();
    moveOtherCars();
    moveEnd();
    moveFuel();

    if(!detectCollision() && !gameStopped && game_state == 1)
        temp += 15;
	if(temp > 1000)
    {
        temp  = 0;
        seconds++;
    }

	glutPostRedisplay();
	glutTimerFunc(10, periodicFunction, v);
}
//
void moveOtherCars(){
    int i;
    for(i = 0; i < MAX_CARS; i++)
    {
        pos[i] += -speed + speeds[i];

        if(pos[i] < -200 || pos[i] > 200)
        {
            pos[i] = 200 + rand()%100;
            speeds[i] = 2 + rand()%4;
        }
    }

    if(horn)
    {
        speeds[(car_x + 60)/40]++;
        if(speeds[(car_x + 60)/40] > 7)
            speeds[(car_x + 60)/40] = 7;
        horn = false;
    }

}

int detectCollision(){
    if(game_state != 1)
        return 0;
    int i, limit;
    for(i = 0; i < MAX_CARS; i++)
    {
        if(pos[i] < -50 && pos[i] > -150)
        {
            limit = (i - 1)*40;
            if(car_x < limit + 25 && car_x > limit - 25)
            {
                speeds[i] = 0;
                gameStopped = true;
                game_state = 2;
                return 1;
            }
        }
    }

    if((fuel_x > car_x && fuel_x - car_x < 20) || (fuel_x < car_x && car_x - fuel_x < 20))
    {
        if(fuel_y < -80 && fuel_y > -120)
        {
            fuel+=40;
            if(fuel > 178)
                fuel = 178;
            fuel_y = 600 + rand()%150;
            fuel_x = (rand()%3 - 1)*37;
        }
    }

    return 0;

}
//


void drawMainMenu(){
    //Draw start button
    glClearColor(0.5, 0.5, 0.5, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    drawFootPath();
    drawSurroundings();
    drawDivider();
    drawCar();

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(0, 30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-20, 30, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    draw_string("START");

    glPopMatrix();

    //Draw exit button
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslated(0, -30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-15, -30, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("EXIT");

    glPopMatrix();

    if(highlightStart)
    {
        glColor3f(0.0f, 1.0f, 0.0f);

        glPushMatrix();
        glTranslated(0, 30, 0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(35, 20);
            glVertex2f(35, -20);
            glVertex2f(-35, -20);
            glVertex2d(-35, 20);
        glEnd();
        glPopMatrix();

    }

    if(highlightExit)
    {
        glColor3f(1.0, 0.0, 0.0);

        glPushMatrix();
        glTranslated(0, -30, 0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(35, 20);
            glVertex2f(35, -20);
            glVertex2f(-35, -20);
            glVertex2d(-35, 20);
        glEnd();
        glPopMatrix();

    }
}
//
void drawExitMenu(){
    //Draw start button
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(0, 30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-25, 30, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    if (gameCompleted){
        draw_string("Continue");
    }else {
        draw_string("RESTART");
    }

    glPopMatrix();

    //Draw exit button
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslated(0, -30, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(30, 15);
        glVertex2f(30, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-15, -30, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("EXIT");

    glPopMatrix();

    if(highlightStart)
    {
        glColor3f(0.0f, 1.0f, 0.0f);

        glPushMatrix();
        glTranslated(0, 30, 0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(35, 20);
            glVertex2f(35, -20);
            glVertex2f(-35, -20);
            glVertex2d(-35, 20);
        glEnd();
        glPopMatrix();

    }

    if(highlightExit)
    {
        glColor3f(1.0, 0.0, 0.0);

        glPushMatrix();
        glTranslated(0, -30, 0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(35, 20);
            glVertex2f(35, -20);
            glVertex2f(-35, -20);
            glVertex2d(-35, 20);
        glEnd();
        glPopMatrix();

    }
}
//
void drawTime(){
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslated(-200, 90, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(50, 15);
        glVertex2f(50, -15);
        glVertex2f(-30, -15);
        glVertex2d(-30, 15);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-220, 85, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 1.0, 1.0);
    draw_string("Score: ");
    glPopMatrix();

    glPushMatrix();
    glTranslated(-180, 85, 0);
    glScalef(0.1, 0.1, 0.1);
    glColor3f(1.0, 0.0, 0.0);
    //glutStrokeCharacter(GLUT_STROKE_ROMAN, '4');
    drawScore(seconds);
    glPopMatrix();
}
void drawNavMenu(){
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslated(80, 85, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("Navigation");
    glPopMatrix();
    glColor3f(0.1, 0.1, 0.1);
    drawText("Press 'Enter' to Start", 85, 70);
    drawText("Press 'UP key' to speed up", 85, 55);
    drawText("Press 'LEFT key' to move left", 85, 40);
    drawText("Press 'RIGHT key' to move right", 85, 25);
    drawText("Press 'H' to make horn and move the front cars", 85, 10);
    drawText("Press 'Q' to quit", 85, -5);
    //drawText("Press 'E' for ab", 85, -20);





}
void about(){
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslated(-180, 35, 0);
    glScalef(0.1, 0.1, 0.1);
    draw_string("Developed by:");
    glPopMatrix();
    glColor3f(0.1, 0.1, 0.1);
    drawText("1. Ayub Nasir", -180, 20);
    drawText("2. Ammar Amin", -180, 5);
    drawText("3. Dagem Zerihun", -180, -10);
    drawText("4. Abenezer Alemayehu", -180, -25);
    drawText("5. Adonay Alemu", -180, -40);
    //drawText("Press 'Q' to quit", -85, -5);
}


//
void drawScore(int score){
    int temp = score;
    int str[20],i=0;

    while(temp>0)
    {
        str[i++] = (temp%10);
        temp /= 10;
    }
    i--;
    while(i>=0)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i--]+'0');
    }
    if(score == 0) glutStrokeCharacter(GLUT_STROKE_ROMAN, '0');
}
//
void drawDistanceBar(){
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glRotatef(-90, 0, 0,1);
    glTranslated(150, 18, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(5, 90);
        glVertex2f(5, -90);
        glVertex2f(-5, -90);
        glVertex2d(-5, 90);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glRotatef(-90, 0, 0,1);
    glTranslated(150, -70, 0);
    glColor3f(0.9, 0.2, 0.2);
    glRectd(4, 0, -4, distance);
    glPopMatrix();

}
//
void drawFuelBar(){
    glPushMatrix();
    glRotatef(-90, 0, 0,1);
    glColor3f(0.0, 0.0, 0.0);
    glTranslated(140, 18, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(5, 90);
        glVertex2f(5, -90);
        glVertex2f(-5, -90);
        glVertex2d(-5, 90);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glRotatef(-90, 0, 0,1);
    glTranslated(140, -70, 0);
    glColor3f(0 , 1 , 0);
    glRectd(4, 0, -4, fuel);
    glPopMatrix();

}
//n
void drawEnd(){
    int i, j;
    for(i = 0; i < 5; i++)
    {
        if(i % 2 == 0)
            glColor3f(0.0, 0.0, 0.0);
        else
            glColor3f(1.0, 1.0, 1.0);
        glPushMatrix();

        glTranslated(-55 , end_y + 10*i, 0);

        for(j = 0; j < 11; j++)
        {
            if(i%2)
            {
                if(j % 2 == 0)
                    glColor3f(0.0, 0.0, 0.0);
                else
                    glColor3f(1.0, 1.0, 1.0);
            }
            else
            {
                if(j % 2)
                    glColor3f(0.0, 0.0, 0.0);
                else
                    glColor3f(1.0, 1.0, 1.0);
            }

            glRectd(10 * j,0,10*(j+1),10);
        }

        glPopMatrix();
    }
}
//

/*void fuelMessage(){
    glPushMatrix();
    glTranslated(75, -70, 0);
    glScalef(0.1,0.1,0.1);
    glColor3f(0, 1.0,0);
    draw_string("Fuel Exhausted\n");
    glPopMatrix();
}*/
//

void moveEnd(){
    if(reachedEnd)
        end_y-= speed;

    if(end_y < -125)
    {
        gameStopped = true;
        gameCompleted = true;
        game_state = 2;
    }
}
//

void drawFuel(){
    glPushMatrix();
    glTranslated(fuel_x, fuel_y, 0);
    glColor3f(0.0, 0.9, 0.0);
    glRectf(5, 5, -5, -5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(fuel_x, fuel_y+5, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(5, 0);
        glVertex2f(5, 5);
        glVertex2f(-3, 5);
        glVertex2d(-5, 0);
    glEnd();
    glPopMatrix();

}


void moveFuel(){
    fuel_y-=speed;

    if(fuel_y < -200)
    {
        fuel_y = 600 + rand()%150;
        fuel_x = (rand()%3 - 1)*37;
    }
}



int main(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH );
	glutInitWindowSize((int) width, (int) height);
	glutCreateWindow("Highway Driving");
	glutFullScreen();


	gluOrtho2D(-240.0, 240.0, -160.0, 160.0);

	glutDisplayFunc(maindisp);
	glutTimerFunc(100, periodicFunction, 0);

    glutSpecialFunc(keyBoardFunc);
    glutSpecialUpFunc(keyboard_up_func);
    glutKeyboardFunc(normalKeyBoardFunc);
    glutMouseFunc(mouse_func);
    glutPassiveMotionFunc(mouse_hover);

	glutMainLoop();
	return 0;
}
