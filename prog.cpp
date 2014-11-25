//CS180 Programming Assignment 3 by John Thomason, Fall 14

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
const float TAU = 6.28318530718;

void makeBox(float x,float y,float z,float red,float green,float blue,bool shadowmode);

void makeEave(float x,float y,float z);

void makePillar(float x,float y,float z);

void deepTriangle(float v1[3],float v2[3],float v3[3],float xchange,float ychange,float zchange,bool drawOtherSide,bool s1,bool s2,bool s3);

void deepRectangle(float x1, float y1, float x2, float y2, float depth, bool drawOtherSide);

void makeShape(int numSides,float radius,bool drawOtherSide);

void makeRing(float r1,float r2,float depth, float theta0,float dTheta,int numTimes,bool drawOtherSide);

void moveNumber(int startNumber,int stopRotateNumber,bool fromLeft);

float* sVect(float v[],float x,float y,float z){
	v[0]=x;
	v[1]=y;
	v[2]=z;
	return v;
}

float vCol[3];
float amntOne = 20;
float animFrame = 0;
float brightness = .7;
GLuint woodtex;


//The display function:
void display(bool shadows){
	float v1[3];
	float v2[3];
	float v3[3];
	GLfloat lDif[] = {1,1,1,1};
	GLfloat lAmb[] = {.15,.15,.15,1};

	sVect(vCol,1,1,1);
	glPushMatrix();
	glTranslatef(0,-10,0);
	if(shadows){
		lDif[0]=0;
		lDif[1]=0;
		lDif[2]=0;
		lAmb[0]=1;
		lAmb[1]=1;
		lAmb[2]=1;
		sVect(vCol,brightness,brightness,brightness);
		glColor3f(0,0,0);
		float shadowmatrix[16] = {40,0,0,0, 0,.05,-20,-1, 0,0,40,0, 0,0,0,40};
		glMultMatrixf(shadowmatrix);
	}
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lDif);
	glLightfv(GL_LIGHT0,GL_AMBIENT,lAmb);
	glPushMatrix();
		//Box1
		//glMultMatrixf(shadowmatrix);
		glTranslatef(-17,10,0);
		glRotatef(-85+amntOne,0,1,0);
		if(!shadows){
			makeBox(-4,4,4,1,0,0,false);
		} else {
			makeBox(-4,4,4,brightness,brightness,brightness,true);
			sVect(vCol,brightness,brightness,brightness);
		}
		glColor3f(1,1,1);
		//triangle shape face front
		deepTriangle(sVect(v1,-2,2,4),sVect(v2,2,2,4),sVect(v3,0,-2,4),0,0,-.5,false,false,false,false);
		//'T' Face right
		glPushMatrix();
			glTranslatef(4,0,0);
			glRotatef(90,0,1,0);
			deepRectangle(-2,2,2,1,.5,false);
			deepRectangle(-.5,1,.5,-2.5,.5,false);
		glPopMatrix();
		//diamond face back
		glPushMatrix();
			glTranslatef(0,0,-4);
			glRotatef(180,0,1,0);
			deepTriangle(sVect(v1,0,-2.5,0),sVect(v2,-2,0,0),sVect(v3,2,0,0),0,0,-.5,false,false,true,false);
			deepTriangle(sVect(v1,0,2.5,0),sVect(v2,-2,0,0),sVect(v3,2,0,0),0,0,-.5,false,false,true,false);
		glPopMatrix();
		//+ face left
		glPushMatrix();
			glTranslatef(-4,0,0);
			glRotatef(-90,0,1,0);
			deepRectangle(-2.5,.5,2.5,-.5,.5,false);
			deepRectangle(-.5,2.5,.5,-2.5,.5,false);
		glPopMatrix();
		//dice 1 face top
		glPushMatrix();
			glTranslatef(0,4,0);
			glRotatef(-90,1,0,0);
			makeShape(20,1,false);
		glPopMatrix();
		// / face bottom
		deepTriangle(sVect(v1,1.5,-4,2.5),sVect(v2,2.5,-4,2.5),sVect(v3,-2.5,-4,-2.5),0,.5,0,false,false,true,false);
		deepTriangle(sVect(v1,-1.5,-4,-2.5),v2,v3,0,.5,0,false,false,true,false);
	glPopMatrix();

	glPushMatrix();
		//Box2
		glTranslatef(-10,10,-10);
		glRotatef(-45+(1.3*amntOne),0,1,0);
		//glRotatef(-45,1,0,0);
		if(!shadows){
			makeBox(-4,4,4,0,1,0,false);
		} else {
			makeBox(-4,4,4,brightness,brightness,brightness,true);
			sVect(vCol,brightness,brightness,brightness);
		}
		glColor3f(1,1,1);
		//square shape face front
		glPushMatrix();
			glTranslatef(0,0,4);
			glRotatef(45,0,0,1);
			makeShape(4,2.5,false);
		glPopMatrix();
		//'E' face right
		glPushMatrix();
			glTranslatef(4,0,0);
			glRotatef(90,0,1,0);
			deepRectangle(-2.5,2.5,-1.5,-2.5,.5,false);
			deepRectangle(-1.5,2.5,2.5,1.5,.5,false);
			deepRectangle(-1.5,.5,2.5,-.5,.5,false);
			deepRectangle(-1.5,-1.5,2.5,-2.5,.5,false);
		glPopMatrix();
		//trapezoid face back
		glPushMatrix();
			glTranslatef(0,0,-4);
			glRotatef(180,0,1,0);
			deepRectangle(-1,2,1,-2,.5,false);
			deepTriangle(sVect(v1,-1,2,0),sVect(v2,-2.5,-2,0),sVect(v3,1,-2,0),0,0,-.5,false,false,false,true);
			deepTriangle(sVect(v1,1,2,0),sVect(v2,2.5,-2,0),sVect(v3,-1,-2,0),0,0,-.5,false,false,false,true);
		glPopMatrix();
		//- face left
		//deepTriangle(sVect(v1,-4,-.5,2.5),sVect(v2,-4,.5,2.5),sVect(v3,-4,-.5,-2.5),.5,0,0,false,false,true,false);
		//deepTriangle(v2,v3,sVect(v1,-4,.5,-2.5),.5,0,0,false
		glPushMatrix();
			glTranslatef(-4,0,0);
			glRotatef(-90,0,1,0);
			deepRectangle(-2.5,.5,2.5,-.5,.5,false);
		glPopMatrix();
		//dice 2 face top
		glPushMatrix();
			glTranslatef(0,4,0);
			glRotatef(-90,1,0,0);
			glPushMatrix();
				glTranslatef(1.8,1.8,0);
				makeShape(20,1,false);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-1.8,-1.8,0);
				makeShape(20,1,false);
			glPopMatrix();
		glPopMatrix();
		// box face bottom
		glPushMatrix();
			glTranslatef(0,-4,0);
			glRotatef(90,1,0,0);
			deepRectangle(2.5,2.5,1.5,-2.5,.5,false);
			deepRectangle(-2.5,2.5,-1.5,-2.5,.5,false);
			deepRectangle(2.5,2.5,-2.5,1.5,.5,false);
			deepRectangle(2.5,-2.5,-2.5,-1.5,.5,false);		
		glPopMatrix();
	glPopMatrix();
	
	glPushMatrix();
		//Box3
		glTranslatef(0,10,-16);
		//glRotatef(-30,1,1,0);
		glRotatef(-30-(2*amntOne),0,1,0);
		if(!shadows){
			makeBox(-4,4,4,0,0,1,false);
		} else {
			makeBox(-4,4,4,brightness,brightness,brightness,true);
			sVect(vCol,brightness,brightness,brightness);
		}
		glColor3f(1,1,1);
		//pentagon shape face front
		glPushMatrix();
			glTranslatef(0,0,4);
			glRotatef(45,0,0,1);
			makeShape(5,2.5,false);
		glPopMatrix();
		//'F' face right
		glPushMatrix();
			glTranslatef(4,0,0);
			glRotatef(90,0,1,0);
			deepRectangle(-2.5,2.5,-1.5,-2.5,.5,false);
			deepRectangle(-1.5,2.5,2.5,1.5,.5,false);
			deepRectangle(-1.5,.5,2.5,-.5,.5,false);
		glPopMatrix();
		//rhombus face back
		glPushMatrix();
			glTranslatef(0,0,-4);
			glRotatef(180,0,1,0);
			deepRectangle(-1,2,1,-2,.5,false);
			deepTriangle(sVect(v1,-1,2,0),sVect(v2,-2.5,-2,0),sVect(v3,1,-2,0),0,0,-.5,false,false,false,true);
			deepTriangle(sVect(v1,1,-2,0),sVect(v2,2.5,2,0),sVect(v3,-1,2,0),0,0,-.5,false,false,false,true);
		glPopMatrix();
		//X face left
		glPushMatrix();
			glTranslatef(-4,0,0);
			glRotatef(-90,0,1,0);
			glPushMatrix();
				glRotatef(45,0,0,1);
				deepRectangle(-.8,2.5,.8,-2.5,.5,false);
			glPopMatrix();
			glPushMatrix();
				glRotatef(-45,0,0,1);
				deepRectangle(-.8,2.5,.8,-2.5,.5,false);
			glPopMatrix();
		glPopMatrix();
		//dice 3 face top
		glPushMatrix();
			glTranslatef(0,4,0);
			glRotatef(-90,1,0,0);
			glPushMatrix();
				glTranslatef(1.8,1.8,0);
				makeShape(20,1,false);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-1.8,-1.8,0);
				makeShape(20,1,false);
			glPopMatrix();
				makeShape(20,1,false);
		glPopMatrix();
		// triangle face w/hole bottom
		glPushMatrix();
			glTranslatef(0,-4,0);
			glRotatef(90,1,0,0);
			deepTriangle(sVect(v1,-1,-1,0),sVect(v2,1,-1,0),sVect(v3,-2.5,-2,0),0,0,-.5,false,false,true,false);
			deepTriangle(sVect(v1,2.5,-2,0),v2,v3,0,0,-.5,false,false,true,false);	
			deepTriangle(v1,v2,sVect(v3,0,2.5,0),0,0,-.5,false,true,false,false);
			deepTriangle(sVect(v1,0,1,0),v2,v3,0,0,-.5,false,false,true,false);
			deepTriangle(v1,sVect(v2,-2.5,-2,0),v3,0,0,-.5,false,true,false,true);
			deepTriangle(v1,v2,sVect(v3,-1,-1,0),0,0,0-.5,false,false,false,true);
		glPopMatrix();
	glPopMatrix();	
	
	glPushMatrix();
		//Box 4
		glTranslatef(9,10,-10);
		glRotatef(-20+(.2*amntOne),0,1,0);
		//glRotatef(40,1,1,0);
		//glRotatef(180,1,0,0);
		if(!shadows){
			makeBox(-4,4,4,1,0,1,false);
		} else {
			makeBox(-4,4,4,brightness,brightness,brightness,true);
			sVect(vCol,brightness,brightness,brightness);
		}
		glColor3f(1,1,1);
		//hexagon shape face front
		glPushMatrix();
			glTranslatef(0,0,4);
			makeShape(6,2.5,false);
		glPopMatrix();
		//'H' face right
		glPushMatrix();
			glTranslatef(4,0,0);
			glRotatef(90,0,1,0);
			deepRectangle(-2.5,2.5,-1.5,-2.5,.5,false);
			deepRectangle(-1.5,.5,2.4,-.5,.5,false);
			deepRectangle(1.5,2.5,2.5,-2.5,.5,false);
		glPopMatrix();
		//V face back
		glPushMatrix();
			glTranslatef(0,0,-4);
			glRotatef(180,0,1,0);
			deepTriangle(sVect(v1,-2.5,2.5,0),sVect(v2,-1.5,2.5,0),sVect(v3,0,-2.5,0),0,0,-.5,false,false,true,false);
			deepTriangle(sVect(v1,0,-1,0),v2,v3,0,0,-.5,false,false,true,true);
			deepTriangle(v1,sVect(v2,2.5,2.5,0),v3,0,0,-.5,false,true,false,true);
			deepTriangle(v1,v2,sVect(v3,1.5,2.5,0),0,0,-.5,false,true,false,false);
		glPopMatrix();
		//% face left
		glPushMatrix();
			glTranslatef(-4,0,0);
			glRotatef(-90,0,1,0);
			deepRectangle(-2.5,.8,2.5,-.8,.5,false);
			glPushMatrix();
				glTranslatef(0,2,0);
				makeShape(20,1,false);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0,-2,0);
				makeShape(20,1,false);
			glPopMatrix();
		glPopMatrix();
		//dice 4 face top
		glPushMatrix();
			glTranslatef(0,4,0);
			glRotatef(-90,1,0,0);
			glPushMatrix();
				glTranslatef(1.8,1.8,0);
				makeShape(20,1,false);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-1.8,-1.8,0);
				makeShape(20,1,false);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(1.8,-1.8,0);
				makeShape(20,1,false);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-1.8,1.8,0);
				makeShape(20,1,false);
			glPopMatrix();
		glPopMatrix();
		//ring face bottom
		glPushMatrix();
			glTranslatef(0,-4,0);
			glRotatef(90,1,0,0);
			makeRing(1.5,2.5,.5,0,.2094395,39,false);
		glPopMatrix();
	glPopMatrix();	
	
	glPushMatrix();
		//Box 5
		glTranslatef(15,10,0);
		glRotatef(-20-(1.7*amntOne),0,1,0);
		//glRotatef(125,1,1,0);
		if(!shadows){
			makeBox(-4,4,4,0,1,1,false);
		} else {
			makeBox(-4,4,4,brightness,brightness,brightness,true);
			sVect(vCol,brightness,brightness,brightness);
		}
		glColor3f(1,1,1);
		//septagon shape face front
		glPushMatrix();
			glTranslatef(0,0,4);
			makeShape(7,2.5,false);
		glPopMatrix();
		//'i' face right
		glPushMatrix();
			glTranslatef(4,0,0);
			glRotatef(90,0,1,0);
			deepRectangle(-.7,.5,.7,-2.5,.5,false);
			glPushMatrix();
				glTranslatef(0,1.6,0);
				makeShape(20,.7,false);
			glPopMatrix();
		glPopMatrix();
		//star face back
		glPushMatrix();
			glTranslatef(0,0,-4);
			glRotatef(180,0,1,0);
			deepTriangle(sVect(v1,0,2.8,0),sVect(v2,-1,.8,0),sVect(v3,1,.9,0),0,0,-.5,false,false,true,false);
			deepTriangle(sVect(v1,1.6,-.7,0),v2,v3,0,0,-.5,false,true,true,true);
			deepTriangle(v1,sVect(v2,2.8,.9,0),v3,0,0,-.5,false,false,false,true);
			deepTriangle(v1,sVect(v2,0,-1.8,0),sVect(v3,-1.6,-.7,0),0,0,-.5,false,true,true,true);
			deepTriangle(v1,v2,sVect(v3,2.2,-2.7,0),0,0,-.5,false,true,false,false);
			deepTriangle(sVect(v1,-2.2,-2.7,0),v2,sVect(v3,-1.6,-.7,0),0,0,-.5,false,false,true,false);
			deepTriangle(sVect(v1,1.6,-.7,0),sVect(v2,-1,.9,0),v3,0,0,-.5,false,true,true,true);
			deepTriangle(sVect(v1,-2.8,.9,0),v2,v3,0,0,-.5,false,false,true,false);
		glPopMatrix();
		//= face left
		glPushMatrix();
			glTranslatef(-4,0,0);
			glRotatef(-90,0,1,0);
			deepRectangle(-2.5,-2.2,2.5,-.7,.5,false);
			deepRectangle(-2.5,2.2,2.5,.7,.5,false);
		glPopMatrix();
		//dice 5 face top
		glPushMatrix();
			glTranslatef(0,4,0);
			glRotatef(-90,1,0,0);
			glPushMatrix();
				glTranslatef(1.8,1.8,0);
				makeShape(20,1,false);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-1.8,-1.8,0);
				makeShape(20,1,false);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(1.8,-1.8,0);
				makeShape(20,1,false);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-1.8,1.8,0);
				makeShape(20,1,false);
			glPopMatrix();
				makeShape(20,1,false);
		glPopMatrix();
		// heart bottom
		glPushMatrix();
			glTranslatef(0,-4,0);
			glRotatef(90,1,0,0);
			glPushMatrix();
				glTranslatef(1.1,1,0);
				makeShape(20,1.2,false);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-1.1,1,0);
				makeShape(20,1.2,false);
			glPopMatrix();
			deepTriangle(sVect(v1,-2.32,.63,0),sVect(v2,2.32,.63,0),sVect(v3,0,-2,0),0,0,-.5,false,true,false,false);
		glPopMatrix();
	glPopMatrix();	

	sVect(vCol,brightness,brightness,brightness);
	//Numbers!*******************************************************************************
	//ONE:
	if(!shadows){
		sVect(vCol,1,0,0);
	}
	glPushMatrix();
		moveNumber(0,120,true);
		deepRectangle(0,0,1,6,1,true);
	glPopMatrix();
	//TWO:
	if(!shadows){
		sVect(vCol,0,1,0);
	}
	glPushMatrix();
		moveNumber(80,160,true);
		makeRing(1,2,1,-TAU/8,.3,12,true);
		//Hard code these at some point
		deepTriangle(sVect(v1,cos(-TAU/8),sin(-TAU/8),0),sVect(v2,2*cos(-TAU/8),2*sin(-TAU/8),0),sVect(v3,-2,-4,0),0,0,-1,true,false,true,false);
		deepTriangle(sVect(v1,-1,-4,0),v2,v3,0,0,-1,true,false,true,false);
		deepRectangle(-1,-4,2,-3,1,true);
	glPopMatrix();
	//THREE:
	if(!shadows){
		sVect(vCol,1,1,0);
	}
	glPushMatrix();
		moveNumber(140,220,false);
		glScalef(1.4,.9,1);
		glPushMatrix();
			glTranslatef(0,1.5,0);
			makeRing(1,2,1,-TAU/4,.1256637,25,true);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0,-1.5,0);
			makeRing(1,2,1,-TAU/4,.1256637,25,true);
		glPopMatrix();
	glPopMatrix();
	//FOUR
	if(!shadows){
		sVect(vCol,0,0,1);
	}
	glPushMatrix();
		moveNumber(200,320,false);
		deepRectangle(0,0,1,5.9,1,true);
		deepRectangle(-2.5,3,2,4,1,true);
		deepRectangle(-2.5,3,-1.5,6,1,true);
	glPopMatrix();
	//FIVE
	if(!shadows){
		sVect(vCol,1,0,1);
	}
	glPushMatrix();
		moveNumber(300,380,true);
		glPushMatrix();
			glScalef(1.5,1,1);
			makeRing(1,2,1,TAU/4,-.1256637,25,true);
		glPopMatrix();
		deepRectangle(0,1,1,4,1,true);
		deepRectangle(1,4,3,3,1,true);
	glPopMatrix();
	//SIX
	if(!shadows){
		sVect(vCol,0,1,1);
	}
	glPushMatrix();
		moveNumber(360,480,false);
		makeRing(1,2,1,0,.209439510,30,true);
		glPushMatrix();
			glTranslatef(0,2,0);
			makeRing(1,2,1,TAU/4,.19634954,8,true);
		glPopMatrix();
		deepRectangle(-2,0,-1,2,1,true);	
	glPopMatrix();
	//SEVEN
	if(!shadows){
		sVect(vCol,.2,.2,.2);
	}
	glPushMatrix();
		moveNumber(460,560,true);
		deepRectangle(0,0,4,1,1,true);
		deepTriangle(sVect(v1,3,0,0),sVect(v2,4,0,0),sVect(v3,1,-5,0),0,0,-.5,true,true,true,false);
		deepTriangle(sVect(v1,2,-5,0),v2,v3,0,0,-.5,true,false,true,false);
	glPopMatrix();
	//EIGHT
	if(!shadows){
		sVect(vCol,.5,.2,1);
	}
	glPushMatrix();
		moveNumber(540,640,false);
		glPushMatrix();
			glTranslatef(0,1.3,0);
			makeRing(.8,1.8,1,0,.209439510,30,true);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0,-1.3,0);
			makeRing(.8,1.8,1,0,.209439510,30,true);
		glPopMatrix();
	glPopMatrix();
	//NINE
	if(!shadows){
		sVect(vCol,1,.8,.2);
	}
	glPushMatrix();
		moveNumber(620,715,true);
		makeRing(.8,1.8,1,0,.209439510,30,true);
		deepRectangle(.8,0,1.8,-2,1,true);
		glPushMatrix();
			glTranslatef(0,-2,0);
			makeRing(.8,1.8,1,0,-.20943951,12,true);	
		glPopMatrix();
	glPopMatrix();
	//TEN
	if(!shadows){
		sVect(vCol,.2,.8,1);
	}
	glPushMatrix();
		moveNumber(685,775,false);
		deepRectangle(-4,3,-3,-3,1,true);
		glPushMatrix();
			glScalef(1,1.5,1);
			makeRing(1,2,1,0,.209439510,30,true);
		glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void drawThings(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	display(true);
	display(false);

	//BACKGROUND
	sVect(vCol,1,1,1);
	glPushMatrix();
		glTranslatef(0,0,-70);	
		deepRectangle(-50,-10,50,50,.1,false);
		glTranslatef(0,-10,50);
		glRotatef(-90,1,0,0);
		sVect(vCol,2,2,2);
		deepRectangle(-50,-50,50,50,.1,false);
	glPopMatrix();

	glFlush();
	glutSwapBuffers();

}

void idle(){
	amntOne=amntOne + .5;
	animFrame+=.5;
	if(animFrame>900){
		animFrame = 0;
	}
	glutPostRedisplay();

}

void moveNumber(int startNumber,int stopRotateNumber,bool fromLeft){
	int rotateAmnt = 0;
	int theta = 0;
	if(animFrame >= startNumber){
		if(animFrame < stopRotateNumber){
			rotateAmnt = animFrame - startNumber;
		}else{
			rotateAmnt = stopRotateNumber - startNumber;
		}
	}
	if(fromLeft==true){
		theta = 90-rotateAmnt;
		if(animFrame>=stopRotateNumber){
			glTranslatef((sin(theta*0.017453)*(animFrame-stopRotateNumber))/2,0,(cos(theta*0.017453)*(animFrame-stopRotateNumber))/2);
		}
		glTranslatef(-20,10,0);
		glRotatef(theta,0,1,0);
		glTranslatef(20,0,0);
	}else{
		theta = 90+rotateAmnt;
		if(animFrame>=stopRotateNumber){
			glTranslatef(-(sin(theta*0.017453)*(animFrame-stopRotateNumber))/2,0,-(cos(theta*0.017453)*(animFrame-stopRotateNumber))/2);
		}
		glTranslatef(20,10,0);
		glRotatef(theta,0,1,0);
		glTranslatef(20,0,0);
		glRotatef(180,0,1,0);
	}
}

void makeRing(float r1,float r2,float depth,float theta0,float dTheta,int numTimes,bool drawOtherSide){
	float v1[3];
	float v2[3];
	float v3[3];
	float theta;
	float theta2;
	for(int i = 0; i < numTimes;i++){
		theta = theta0+(i*dTheta);
		theta2 = theta+dTheta;
		deepTriangle(sVect(v1,r1*cos(theta),r1*sin(theta),0),sVect(v2,r2*cos(theta),r2*sin(theta),0),sVect(v3,r1*cos(theta2),r1*sin(theta2),0),0,0,-depth,drawOtherSide,false,true,false);
		deepTriangle(sVect(v1,r2*cos(theta2),r2*sin(theta2),0),v2,v3,0,0,-depth,drawOtherSide,false,true,false);
	}
}

void deepRectangle(float x1, float y1, float x2, float y2, float depth, bool drawOtherSide){
	float v1[3];
	float v2[3];
	float v3[3];
	float xoffset = (x2-x1)/2;
	float yoffset = (y2-y1)/2;
	float zoffset = -depth/2;
	glColor3f(vCol[0],vCol[1],vCol[2]);
	glPushMatrix();
		glNormal3f(0,0,1);
		glRotatef(180,0,0,1);
		glTranslatef(-x1,-y1,0);
		glTranslatef(-xoffset,-yoffset,zoffset);
		glNormal3f(0,0,1);
		glPushMatrix();
			glBegin(GL_QUADS);
				glVertex3f(-xoffset,-yoffset,-zoffset);
				glVertex3f(xoffset,-yoffset,-zoffset);
				glVertex3f(xoffset,yoffset,-zoffset);
				glVertex3f(-xoffset,yoffset,-zoffset);		
			glEnd();
		glPopMatrix();
		glPushMatrix();
			glRotatef(180,1,0,0);
			glBegin(GL_QUADS);
				glVertex3f(-xoffset,-yoffset,-zoffset);
				glVertex3f(xoffset,-yoffset,-zoffset);
				glVertex3f(xoffset,yoffset,-zoffset);
				glVertex3f(-xoffset,yoffset,-zoffset);		
			glEnd();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(xoffset,0,0);
			glRotatef(90,0,1,0);
			glBegin(GL_QUADS);
				glVertex3f(-zoffset,-yoffset,0);
				glVertex3f(zoffset,-yoffset,0);
				glVertex3f(zoffset,yoffset,0);
				glVertex3f(-zoffset,yoffset,0);
			glEnd();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-xoffset,0,0);
			glRotatef(-90,0,1,0);
			glBegin(GL_QUADS);
				glVertex3f(-zoffset,-yoffset,0);
				glVertex3f(zoffset,-yoffset,0);
				glVertex3f(zoffset,yoffset,0);
				glVertex3f(-zoffset,yoffset,0);
			glEnd();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0,yoffset,0);
			glRotatef(90,1,0,0);
			glBegin(GL_QUADS);
				glVertex3f(-xoffset,-zoffset,0);
				glVertex3f(xoffset,-zoffset,0);
				glVertex3f(xoffset,zoffset,0);
				glVertex3f(-xoffset,zoffset,0);
			glEnd();
		glPopMatrix();
		glNormal3f(0,0,-1);
		glPushMatrix();
			glTranslatef(0,-yoffset,0);
			glRotatef(-90,1,0,0);
			glBegin(GL_QUADS);
				glVertex3f(-xoffset,-zoffset,0);
				glVertex3f(xoffset,-zoffset,0);
				glVertex3f(xoffset,zoffset,0);
				glVertex3f(-xoffset,zoffset,0);
			glEnd();
		glPopMatrix();
		glNormal3f(0,0,1);

	glPopMatrix();
}

void makeShape(int numSides,float radius,bool drawOtherSide){
	float v1[3];
	float v2[3];
	float v3[3];
	float x1 = radius;
	float y1 = 0;
	float xn = 0;
	float yn = 0;
	float theta = 0;
	for(int i = 1;i < numSides;i++){
		theta = i*(TAU/numSides);
		xn = radius*cos(theta);
		yn = radius*sin(theta);
		deepTriangle(sVect(v1,0,0,0),sVect(v2,x1,y1,0),sVect(v3,xn,yn,0),0,0,-.5,drawOtherSide,true,false,true);
		x1=xn;
		y1=yn;
	}
	deepTriangle(sVect(v1,0,0,0),sVect(v2,xn,yn,0),sVect(v3,radius,0,0),0,0,-.5,drawOtherSide,true,false,true);
}

void deepTriangle(float v1[3],float v2[3],float v3[3],float xc,float yc,float zc,bool drawOtherSide,bool skip1,bool skip2,bool skip3){
	glColor3f(vCol[0],vCol[1],vCol[2]);
	glNormal3f(0,0,1);
	//The basic triangle
	glBegin(GL_TRIANGLES);
		//front
		glVertex3f(v1[0],v1[1],v1[2]);
		glVertex3f(v2[0],v2[1],v2[2]);
		glVertex3f(v3[0],v3[1],v3[2]);

		glNormal3f(0,0,-1);
		glVertex3f(v1[0]+xc,v1[1]+yc,v1[2]+zc);
		glVertex3f(v2[0]+xc,v2[1]+yc,v2[2]+zc);
		glVertex3f(v3[0]+xc,v3[1]+yc,v3[2]+zc);
	glEnd();
	//fill in the sides
	glBegin(GL_QUADS);
		if(!skip1){
			glNormal3f(v2[0],v1[1],v1[2]);
			glVertex3f(v2[0]+xc,v2[1]+yc,v2[2]+zc);
			glVertex3f(v1[0]+xc,v1[1]+yc,v1[2]+zc);
			glVertex3f(v1[0],v1[1],v1[2]);
			glVertex3f(v2[0],v2[1],v2[2]);
		}

		if(!skip2){
			glNormal3f(v3[0],v2[1],v2[2]);
			glVertex3f(v2[0],v2[1],v2[2]);
			glVertex3f(v2[0]+xc,v2[1]+yc,v2[2]+zc);
			glVertex3f(v3[0]+xc,v3[1]+yc,v3[2]+zc);
			glVertex3f(v3[0],v3[1],v3[2]);
		}

		if(!skip3){
			glNormal3f(v1[0],v3[1],v3[2]);
			glVertex3f(v3[0],v3[1],v3[2]);
			glVertex3f(v3[0]+xc,v3[1]+yc,v3[2]+zc);
			glVertex3f(v1[0]+xc,v1[1]+yc,v1[2]+zc);
			glVertex3f(v1[0],v1[1],v1[2]);
		}
	glEnd();
	glNormal3f(0,0,1);
}

void makeBox(float x,float y, float z,float red,float green,float blue,bool shadowmode){
	//protruding edges
	glColor3f(red,green,blue);
	sVect(vCol,red,green,blue);
	for(int i = 0; i < 4;i++){
		glPushMatrix();
			glRotatef(90*i,0,1,0);
			glTranslatef(0,0,4);
			deepRectangle(x+1,y-1,x+8,y,1,true);
			deepRectangle(x+1,y-7,x+8,y-8,1,true);
			deepRectangle(x+7,y-1,x+8,y-7,1,true);
		glPopMatrix();
	}
	//inner cudes
	if(!shadowmode){
		glBindTexture(GL_TEXTURE_2D,woodtex);
		glEnable(GL_TEXTURE_2D);
		glColor3f(1,1,1);
	} else {
		glColor3f(brightness,brightness,brightness);
	}
	for(int i = 0;i < 4;i++){
	glPushMatrix();
		glRotatef(90*i,0,1,0);
		glTranslatef(0,0,3);
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2f(0,0);
			glVertex3f(x+.5,y-.5,.5);
			glTexCoord2f(0,1);
			glVertex3f(x+7.5,y-.5,.5);
			glTexCoord2f(1,1);
			glVertex3f(x+7.5,y-7.5,.5);
			glTexCoord2f(1,0);
			glVertex3f(x+.5,y-7.5,.5);
		glEnd();
	glPopMatrix();
	}
	//top/bottom
	for(int i = 1; i < 4;i+=2){
	glPushMatrix();
	 	glRotatef(90*i,1,0,0);
		glTranslatef(0,0,3);
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2f(0,1);
			glVertex3f(x+.5,y-.5,.5);
			glTexCoord2f(1,1);
			glVertex3f(x+7.5,y-.5,.5);
			glTexCoord2f(1,0);
			glVertex3f(x+7.5,y-7.5,.5);
			glTexCoord2f(0,0);
			glVertex3f(x+.5,y-7.5,.5);
		glEnd();	
	glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);
	sVect(vCol,1,1,1);
}

//Easy exiting
void keys(unsigned char key, int x, int y){
	switch(key){
		case 'q':
			exit(0);
			break;
		case 'r':
			amntOne=amntOne + 3;
			animFrame+=1;
			glutPostRedisplay();
			break;
	}
}

int mouse_x;
int mouse_y;
bool mouse_clicked = false;
float theta_hor = 0;
float theta_ver = TAU/4;

void mouseclick(int button,int state, int x, int y){
	mouse_x = x;
	mouse_y = y;
	if(button == GLUT_LEFT_BUTTON){
		if(state == GLUT_DOWN){
			mouse_clicked = true;
		}
		else{
			mouse_clicked = false;
		}
	}
}

void mousemove(int x2, int y2){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	int dx = x2-mouse_x;
	int dy = y2-mouse_y;
	mouse_x = x2;
	mouse_y = y2;
	theta_hor+=((float)dx/5);
	theta_ver-=((float)dy/100);
	if(theta_ver > TAU/2.1){
		theta_ver = TAU/2.1;
	} else if(theta_ver < .1){
		theta_ver = .1;
	}
	gluLookAt(0,50*cos(theta_ver),50*sin(theta_ver),0,0,0,0,1,0);
	glRotatef(theta_hor,0,1,0);
	GLfloat lPos[] = {0,40,20,1};
	glLightfv(GL_LIGHT0,GL_POSITION,lPos);
}

int main(int argc, char **argv){
	int win_width = 700;
	int win_height = 700;
	

	//Set up the window and things
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(win_width,win_height);
	glutCreateWindow("John Thomason Assignment 3");
	glutDisplayFunc(drawThings);
	glutIdleFunc(idle);
	glutKeyboardFunc(keys);
	glutMouseFunc(mouseclick);
	glutMotionFunc(mousemove);

	//Set the initial settings
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glNormal3f(0,0,1);
	glEnable(GL_NORMALIZE);
	glFrontFace(GL_CW);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST); //<- hidden surface elimination eventually
	
	//Make the texture
	glGenTextures(1, &woodtex);
	glBindTexture(GL_TEXTURE_2D, woodtex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glEnable(GL_TEXTURE_2D);
	
	GLubyte tex_array[256][256][3];
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	//lets make
	for(int i = 0; i < 256; i++){
		for(int ii = 0; ii < 256; ii++){
			tex_array[i][ii][0] = (GLubyte)238-sin(ii/5.0)*10;
			tex_array[i][ii][1] = (GLubyte)197-sin(ii/5.0)*10;
			tex_array[i][ii][2] = (GLubyte)145;
		}
	}

	//mess things up
	int shift = 1;
	int shift_prev = 0;
	for(int i = 0; i < 256; i++){
		if(i%6==0){
			int temp = shift;
			shift=shift+shift_prev % 7;
			shift_prev = temp;
		}
		int noise = -1*(i%4);
		for(int ii = 0; ii < 256; ii++){
			tex_array[i][ii][0] = tex_array[i][ii+noise+shift%256][0];	
			tex_array[i][ii][1] = tex_array[i][ii+noise+shift%256][1];	
		}
	}

	glBindTexture(GL_TEXTURE_2D,woodtex);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,256,256,0,GL_RGB,GL_UNSIGNED_BYTE,tex_array);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(40.0,1,.1,180);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,50.0,0,0,0,0,1.0,0); //i am behind and up

	GLfloat lDif[] = {1,1,1,1};
	GLfloat lAmb[] = {.15,.15,.15,1};
	GLfloat lPos[] = {0,40,20,1};
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lDif);
	glLightfv(GL_LIGHT0,GL_AMBIENT,lAmb);
	glLightfv(GL_LIGHT0,GL_POSITION,lPos);

	glutMainLoop();
	
	return 0;
}
