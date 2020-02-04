

/********************************************************
 *                                                      *
 *              3D - Airport With Conveyors             *
 *                      John Gable                      *
 *                                                      *
 *    This program is used to create a 3-D model        *
 * of an airport entrance. All models are composed of   *
 *  multiple materials, and properties. Camera controls *
 * for movement are WASD, and zooming is shift+w/s. All *
 * lighting controls are able to be used from a lighting*
 * submenu, and the belt animation is controls via main *
 * menu. 						*
 *******************************************************/



#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "soil.h"

GLuint tiles;        // Bitmap data for tiles
GLuint walls;        // Bitmap data for walls
GLuint glass;        // Bitmap data for glass
GLuint woodDesk;     // Bitmap data for woodDesk
GLuint glassDoor;    // Bitmap data for glassDoor
GLuint tex; 

GLUquadricObj* quad;        //global quadric used for certain primitives

long Time, ResetTime;     //longs used for the timer
static int animate = 0;  //boolean for animate or not
double frameNum = 0;    //animation variable 

double vertical = 0.25;      //vertical distance for camera 
double horizontal = 1.5;    //Horizontal distance for camera (location on sphere ring)
double radius = 20.0;      //Radial distance for camera 

double focusX = 2.0;      //X coordinate for cameras focus point
double focusY = 1.0;     //Y coordinate for cameras focus point
double focusZ = 0.0;    //Z coordinate for cameras focus point

double upX = 0.0;       //up vector X value for camera
double upY = 1.0;      //up vector Y value for camera
double upZ = 0.0;     //up vector Z value for camera

int lightToggle = 0;  //toggle variable for door lights and emitters
int xrayToggle = 0;  //toggle variable for xray spotlights and emitters
int tvToggle = 0;   //toggle for tv lights and emitters
int leftPressed = 0; //variable for finding out if the interactive spotlight should be on


float black[] = {0.1992, 0.2265, 0.2344};  //slightly black colors
float silver[] = { 0.19225f, 0.19225f, 0.19225f};	  //silver colors


static double posX;     //x coordinate for interactive spotlights
static double posY;    //y coordinate for interactive spotlights
static double posZ;   //z coordinate for interactive spotlights






/*************************************
 *             globalLight           *
 *************************************/
//Creates the global ambient lighting for the scene
void globalLight(void){

  	//Put arrays for the light's properties here
	GLfloat modelAmbient[] = {0, 0, 0};
	
	GLfloat ambientLight[] = {0.0, 0.0, 0.0};
	GLfloat diffuseLight[] = {1.0, 1.0, 1.0};
	GLfloat specularLight[] = {1.0, 1.0, 1.0};
	GLfloat positionLight[] = {20.0, 50.0, 50.0, 0.0};
	
	//Set light properties here
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
 	glLightfv(GL_LIGHT0, GL_POSITION, positionLight);

	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, modelAmbient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0.0);


}



/*************************************
 *           drawBeltTooth           *
 *************************************/
 //Draw singular conveyor belt tooth
void drawBeltTooth(void){
	//Put arrays for the sphere's properties here
	GLfloat tableAmbientDiffuse[] = {black[0], black[1], black[2]};
	GLfloat tableSpecular[] = {0.5, 0.5, 0.5};
	GLfloat shininess[] = {32.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, tableAmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, tableSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);


	glColor3f(black[0], black[1], black[2]);
	glPushMatrix();
		glScalef(1.0 , 1.0, 11.0);
		glutSolidCube(.5);
	glPopMatrix();
  
}


/*************************************
 *           drawEndTeeth            *
 *************************************/
 //Draws a set of conveyor belt teeth for
 //the end of the conveyor belt (in a circle)
void drawEndTeeth(void){

	glPushMatrix();
	glRotatef(-frameNum*5, 0.0, 0.0, 1.0);
		for(int i=0; i<360; i+=30){
			glPushMatrix();
				glRotatef(i, 0.0, 0.0, 1.0);
				glTranslatef(0.0, 1.75, 0.0);
				drawBeltTooth();
			glPopMatrix();
		}
	glPopMatrix();

}


/*************************************
 *               drawGear            *
 *************************************/
//Draws a single gear modeled at the origin
void drawGear(void){

  glPushMatrix();
  
	GLfloat gearAmbientDiffuse[] = {silver[0], silver[1], silver[2]};
	GLfloat gearSpecular[] = {0.773911f, 0.773911f, 0.773911f};
	GLfloat shininess[] = {100.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gearAmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, gearSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glRotatef(frameNum*-15.0, 0.0, 0.0, 1.0);
    
    for(float i=0.0; i<=360.0; i+= 72.0){
    
      glPushMatrix();

    	glRotatef(i, 0.0, 0.0, 1.0);
    	glScalef(1, 7, 1.0);
    	glutSolidCube(0.5);
      glPopMatrix();
    }
    
    glPushMatrix();
	//glColor3f(0.0, 0.0, 0.0);
	gluDisk(quad, 0, 1.5, 20, 1.0);
    glPopMatrix();

    glPushMatrix();
	    //glColor3f(0.0, 0.0, 0.0);
	    gluCylinder(quad, 1.5, 1.5, 0.5, 20, 1);
    glPopMatrix();
  
  glPopMatrix();

}


/*************************************
 *            drawGearSet            *
 *************************************/
//Draws a set of 5 gears 
void drawGearSet(void){

	drawGear();	
	
	glPushMatrix();
		glTranslatef(-10.0, 0.0, 0.0);
		drawGear();
	glPopMatrix();	
	
	glPushMatrix();
		glTranslatef(-5.0, 0.0, 0.0);
		drawGear();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(5.0, 0.0, 0.0);
		drawGear();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(10.0, 0.0, 0.0);
		drawGear();
	glPopMatrix();


}


/*************************************
 *            drawAllGears           *
 *************************************/
//Draws two sets of 5 gears, one for each side of the conveyor belt
void drawAllGears(void){

	

	drawGearSet();

	glPushMatrix();
		glTranslatef(0.0, 0.0, -5.0);
		drawGearSet();
	glPopMatrix();


}


/*************************************
 *         drawSingleGearRoller      *
 *************************************/
//Draws a large roller between a set of gears
void drawSingleGearRoller(void){

  GLfloat rollerAmbientDiffuse[] = {silver[0], silver[1], silver[2]};
  GLfloat rollerSpecular[] = {0.5083, 0.5083, 0.5083};
  GLfloat shininess[] = {51.2};

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, rollerAmbientDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, rollerSpecular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
  glPushMatrix();
    gluDisk(quad, 0, 1.0, 20, 1.0);
  glPopMatrix();
  
  
  glPushMatrix();
    glRotatef(frameNum*-15.0, 0.0, 0.0, 1.0);
    gluCylinder(quad, 1.5, 1.5, 5.0, 10, 1);
  glPopMatrix();

  
  glPushMatrix();
    glTranslatef(0.0, 0.0, 5.5);
    gluDisk(quad, 0, 1.5, 20, 1.0);
  glPopMatrix();
}


/*************************************
 *       drawAllGearRollers          *
 *************************************/
//Draws a set of 5 gear rollers
void drawAllGearRollers(void){

	
	for(double i=-10; i<=10; i+=5){
		glPushMatrix();
			glTranslatef(i,0.0,-5.0);
			drawSingleGearRoller();
		glPopMatrix();
	}

}


/*************************************
 *      drawConveyorTableSide        *
 *************************************/
//Draws "table" side for the conveyor belt
void drawConveyorTableSide(void){
	GLfloat tableSideAmbientDiffuse[] = {0.8945, 0.7305, 0.2891};
	GLfloat tableSideSpecular[] = {1.0, 1.0, 1.0};
	GLfloat shininess[] = {70.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, tableSideAmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, tableSideSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glPushMatrix();
	glScalef(25, 0.5, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}


/*************************************
 *         drawConveyorTable         *
 *************************************/
//Draws both sides of the conveyor table
void drawConveyorTable(void){

  glPushMatrix();
    glTranslatef(0.0, 0.0, 1.0);
    drawConveyorTableSide();
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.0, 0.0, -6.0);
    drawConveyorTableSide();
  glPopMatrix();
  
}


/*************************************
 *           drawTableLeg            *
 *************************************/
//Draws singular table leg
void drawTableLeg(void){

	GLfloat tableLegAmbientDiffuse[] = {0.2852, 0.4141, 0.7461};
	GLfloat tableLegSpecular[] = {0.2852, 0.4141, 0.7461};
	GLfloat shininess[] = {70.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, tableLegAmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, tableLegSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glPushMatrix();
		glScalef(1.0, 13.0, 0.5);
		glutSolidCube(1);
	glPopMatrix();
  
}


/*************************************
 *      drawTableLegConnectors       *
 *************************************/
//Draws a bar to connect a set of table legs
void drawTableLegConnectors(void){

	GLfloat tableSideAmbientDiffuse[] = {0.2852, 0.4141, 0.7461};
	GLfloat tableSideSpecular[] = {0.2852, 0.4141, 0.7461};
	GLfloat shininess[] = {70.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, tableSideAmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, tableSideSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glPushMatrix();
		glScalef(1.0, 1.0, 7.0);
		glutSolidCube(1);
	glPopMatrix();

}


/*************************************
 *      drawAllTableLegConnectors    *
 *************************************/
//Draws connectors for 5 sets of table legs
void drawAllTableLegConnectors(void){

  for(double i=-10.0; i<=10.0; i+=5.0){
    glPushMatrix();
      glTranslatef(i, -6.5, -2.5);
      drawTableLegConnectors();
    glPopMatrix();
  }
  
}


/*************************************
 *          drawTableLegSet          *
 *************************************/
//draws 5 table legs
void drawTableLegSet(void){

  for(double i=-10.0; i<=10.0; i+=5.0){
    glPushMatrix();
      glTranslatef(i, -6.5, 1.0);
      drawTableLeg();
    glPopMatrix();
  }
  
}
  
  
/*************************************
 *        drawAllTableLegs           *
 *************************************/
//Draws table legs for the front and back of the table
void drawAllTableLegs(void){

  drawTableLegSet();
 
  glPushMatrix();
	glTranslatef(0.0, 0.0, -7.0);
	drawTableLegSet();
  glPopMatrix();
 
 drawAllTableLegConnectors();
  
}


/*************************************
 *             drawSmallGear         *
 *************************************/
//Draws a single small gear at the origin
void drawSmallGear(void){
glPushMatrix();

glRotatef((frameNum * 25.0), 0.0, 0.0, 1.0);

  glPushMatrix();
    glScalef(0.4, 0.4, 0.0);
    drawGear();
  glPopMatrix();
  
glPopMatrix();

}


/*************************************
 *          drawSmallGearSet         *
 *************************************/
//Draws a set of small gears along the front of the belt
void drawSmallGearSet(void){
  
    glPushMatrix();
        glTranslatef(-7.5, 0.0, 0.0);
        drawSmallGear();
    glPopMatrix();
  
    glPushMatrix();
        glTranslatef(-2.5, 0.0, 0.0);
        drawSmallGear();
    glPopMatrix();
  
    glPushMatrix();
        glTranslatef(2.5, 0.0, 0.0);
        drawSmallGear();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(7.5, 0.0, 0.0);
        drawSmallGear();
    glPopMatrix();  
 

}


/*************************************
 *          drawAllSmallGears        *
 *************************************/
//Draws two sets of small gears, one for each side of the belt
void drawAllSmallGears(void){

  drawSmallGearSet();
  
  glPushMatrix();
    glTranslatef(0.0, 0.0, -5.5);
    drawSmallGearSet();
  glPopMatrix();
    
}


/*************************************
 *           drawBeltTeeth           *
 *************************************/
 //Draws a set of belt teeth for the top/bottom 
 // of the conveyor belt
void drawBeltTeeth(void){

	GLfloat beltAmbientDiffuse[] = {black[0], black[1], black[2]};
	GLfloat beltSpecular[] = {0.5, 0.5, 0.5};
	GLfloat shininess[] = {50.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, beltAmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, beltSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	for(double i=-11; i<=23; i+=.75){
		glPushMatrix();
			glTranslatef(i, 1.75, -2.5);
			drawBeltTooth();
		glPopMatrix();
	}

}


/*************************************
 *        drawConveyorSection        *
 *************************************/
//Draws a complete section of a conveyor belt and table: 5 gears long
void drawConveyorSection(void){

  
	glPushMatrix();
		glTranslatef(0.0, 0.0, -6.0);
		drawConveyorTableSide();
	glPopMatrix();

	drawAllTableLegs();
	drawAllGears();
	drawAllGearRollers();
	drawAllSmallGears();
	drawAllTableLegConnectors();
	//drawConveyorTable();

	glPushMatrix();
		glTranslatef(0.0, 0.0, 1.0);
		drawConveyorTableSide();
	glPopMatrix();



}


/*************************************
 *           drawTopBottomBelt       *
 *************************************/
//Draw a completed belt to be wrapped around the rollers
void drawBeltComplete(void){
 
   //Top
   glPushMatrix();
     glTranslatef(0.25*frameNum, 0.0, 0.0);
     drawBeltTeeth();
   glPopMatrix();

   //Bottom
   glPushMatrix();
     glTranslatef(0.25*-frameNum, 0, 0.0);
     glPushMatrix();
       glTranslatef(3.0, -3.5, 0.0);
       drawBeltTeeth();
     glPopMatrix();
   glPopMatrix();
 
   //Left end
   glPushMatrix();
   
     glPushMatrix();
       glTranslatef(-10.0, 0.0, -2.5);
       drawEndTeeth();
     glPopMatrix();
   glPopMatrix();
   
   //Right end
   glPushMatrix();
     glTranslatef(25.0, 0.0, -2.5);
     drawEndTeeth();
   glPopMatrix();
  
}


/*************************************
 *           drawLandscape           *
 *************************************/
 //Draws the ground of the airport
void drawLandscape(void){

	GLfloat LandscapeAmbientDiffuse[] = {0.9654, 0.9432, 0.7654};
	GLfloat LandscapeSpecular[] = {1.0, 1.0, 1.0};
	GLfloat shininess[] = {100.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, LandscapeAmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, LandscapeSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glPushMatrix();
		glTranslatef(1.0, -15.0, 1.0);
		glScalef(1000.0, 1.0, 1000.0);
		glutSolidCube(1);
	glPopMatrix();

	
	
	glPushMatrix();	
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tiles);

		

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glRotatef(180.0, 0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1000.0, 13.5, 1000.0);
		glTexCoord2f(50.0, 0.0);
		glVertex3f(1000.0, 13.5, 1000.0);
		glTexCoord2f(50.0, 50.0);		
		glVertex3f(1000.0, 13.5, -1000.0);
		glTexCoord2f(0.0, 50.0);
		glVertex3f(-1000.0, 13.5, -1000.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	

}


/*************************************
 *             drawWalls             *
 *************************************/
 //Draws the walls of the airport and the window
void drawWalls(void){


	GLfloat LandscapeAmbientDiffuse[] = {0.9023, 0.8555, 0.8008};
	GLfloat LandscapeSpecular[] = {0.9023, 0.8555, 0.8008};
	GLfloat shininess[] = {100.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, LandscapeAmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, LandscapeSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	
	glPushMatrix();
		//glColor3f(0.9023, 0.8555, 0.8008);
		glTranslatef(1.0, -15.0, -100.0);
		glScalef(1000.0, 500.0, 5.0);
		glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();	
		glEnable(GL_TEXTURE_2D);
		
		glBindTexture(GL_TEXTURE_2D, walls);	

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		glRotatef(180.0, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f(50.0, 50.0);
			glVertex3f(-1000.0, -500.0, 97);

			glTexCoord2f(50.0, 0.0);		
			glVertex3f(-1000.0, 500.0, 97);


			glTexCoord2f(0.0, 0.0);
			glVertex3f(1000.0, 500.0, 97);
			

			glTexCoord2f(0.0, 50.0);
			glVertex3f(1000.0, -500.0, 97);

		glEnd();
		

		glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();  


	glPushMatrix();
		glTranslatef(1.0, -15.0, 300.0);
		glScalef(1000.0, 500.0, 5.0);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();	
		glEnable(GL_TEXTURE_2D);
		
			

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		//glRotatef(180.0, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
			
			glTexCoord2f(0.0, 50.0);
			glVertex3f(1000.0, -500.0, 297);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(1000.0, 500.0, 297);

			glTexCoord2f(50.0, 0.0);		
			glVertex3f(-1000.0, 500.0, 297);

			glTexCoord2f(50.0, 50.0);
			glVertex3f(-1000.0, -500.0, 297);

		glEnd();
		

		glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();  


	glPushMatrix();
		glTranslatef(-150.0, -15.0, 1.0);
		glScalef(4.0, 500.0, 1000.0);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();	
		glEnable(GL_TEXTURE_2D);
		
			

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		glRotatef(270.0, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f(50.0, 50.0);
			glVertex3f(-1000.0, -500.0, 147.5);

			glTexCoord2f(50.0, 0.0);		
			glVertex3f(-1000.0, 500.0, 147.5);


			glTexCoord2f(0.0, 0.0);
			glVertex3f(1000.0, 500.0, 147.5);
			

			glTexCoord2f(0.0, 50.0);
			glVertex3f(1000.0, -500.0, 147.5);

		glEnd();
		

		glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();  




	GLfloat WindowAmbientDiffuse[] = {0.5625, 0.9961, 0.9375};
	GLfloat WindowSpecular[] = {0.5625, 0.9961, 0.9375};
	GLfloat shininessW[] = {100.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,WindowAmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, WindowSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininessW);

	glPushMatrix();
		glTranslatef(50.0, 50.0, -99);
		glScalef(250.0, 100.0, 5.0);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();	
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, glass);	

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		glTranslatef(-125.0, -50.0, 0);
		//glRotatef(180.0, 0.0, 1.0, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f(50.0, 50.0);
			glVertex3f(300.0, 150.0, -95);

			glTexCoord2f(0.0, 50.0);		
			glVertex3f(300.0, 50.0, -95);


			glTexCoord2f(0.0, 0.0);
			glVertex3f(50.0, 50.0, -95);
			

			glTexCoord2f(50.0, 0.0);
			glVertex3f(50.0, 150.0, -95);

		glEnd();
		

		glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();  

}


/*************************************
 *           monitorLight            *
 *************************************/
 //Draws monitor lights when tvs turned on
void monitorLight(void){

    glPushMatrix();
	GLfloat ambientLight[] = {0, 0, 1.0, 1.0};
	GLfloat diffuseLight[] = {0, 0, 1.0, 1.0};
	GLfloat specularLight[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat positionLight[] = {-5.0, 0.0, 0.0, 1.0};

	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT3, GL_POSITION, positionLight);
        glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.03);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

  	//glEnable(GL_LIGHT3);
    glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.0, 0.25, 1.5);
		glScalef(2.0, 8.0, 13.0);
		glutSolidCube(1);
	glPopMatrix();

}


/*************************************
 *           drawMonitor             *
 *************************************/
//Draws monitors turned off, and calls the monitor lights when on
void drawMonitor(void){

  
  GLfloat monitorAmbientDiffuse[] = {0.9023, 0.8555, 0.8008};
  GLfloat monitorSpecular[] = {0.55, 0.55, 0.55};
  GLfloat shininess[] = {10.0};
  
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, monitorAmbientDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, monitorSpecular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
  glPushMatrix();
  	glScalef(2.0, 10.0, 15.0);
  	glutSolidCube(1);
  glPopMatrix();

  if(tvToggle == 1){
  glPushMatrix();
	glTranslatef(-1.5, 0.0, -1.5);
  	monitorLight();
  glPopMatrix();
  }
	
 
}


/*************************************
 *           drawCheckInDesk         *
 *************************************/
//Draws the entire check in desk, and attaches monitors
void drawCheckInDesk(void){

  //Brown wood
  GLfloat DeskAmbientDiffuse[] = {0.6953, 0.4727, 0.1602};
  GLfloat DeskSpecular[] = {0.6953, 0.4727, 0.1602};
  GLfloat shininess[] = {30.0};
  
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, DeskAmbientDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, DeskSpecular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
  glPushMatrix();	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, woodDesk);	

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTranslatef(-1.8, -19, -25.0);
	//glRotatef(180.0, 0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(5.0, 5.0);
		glVertex3f(0.0, 0.0, 0.0);

		glTexCoord2f(0.0, 5.0);		
		glVertex3f(0.0, 20.0, 0.0);


		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.0, 20.0, 50.0);
		

		glTexCoord2f(5.0, 0.0);
		glVertex3f(0.0, 0.0, 50.0);

	glEnd();
	

	glDisable(GL_TEXTURE_2D);

  glPopMatrix();  


  //Lower Desk
  glPushMatrix();
  glTranslatef(0.0, -9.0, 0.0);
    //Middle
    glPushMatrix();
      glScalef(3.0, 20.0, 50.0);
      glutSolidCube(1);
    glPopMatrix();
    
    //Right
    glPushMatrix();
      glTranslatef(13.0, 0.0, 25.0); 
      glScalef(25.0, 20.0, 3);
      glutSolidCube(1);
    glPopMatrix();
    
    //Left(back)
    glPushMatrix();
      glTranslatef(13.0, 0.0, -25.0); 
      glScalef(25.0, 20.0, 3);
      glutSolidCube(1);
    glPopMatrix();
  
    
    glTranslatef(0.0, 20.0, 0.0);
    //top middle
    glPushMatrix();
      glScalef(3.0, 3.0, 50.0);
      glutSolidCube(1);
    glPopMatrix();
    
    //top right
    glPushMatrix();
      glTranslatef(13.0, 0.0, 25.0); 
      glScalef(25.0, 3.0, 3);
      glutSolidCube(1);
    glPopMatrix();
    
    //Left(back)
    glPushMatrix();
      glTranslatef(13.0, 0.0, -25.0); 
      glScalef(25.0, 3.0, 3);
      glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
      glTranslatef(-1.0, 0.0, 10.0);
      drawMonitor();
    glPopMatrix();
     
    glPushMatrix();
      glTranslatef(-1.0, 0.0, -12.0);
      drawMonitor();
    glPopMatrix();
    
  glPopMatrix();

  
}


/*************************************
 *           drawConveyorLights      *
 *************************************/
//Draws the emitters for the conveyors xray machines
void drawConveyorLights(void){

	//Red emission light
	GLfloat XRayLight1AmbientDiffuse[] = {1.0, 0.0, 0.0};
	GLfloat emission1[] = {1.0, 0.0, 0.0, 0.0};
	GLfloat emissionReset[]  = {0.0, 0.0, 0.0, 0.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, XRayLight1AmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission1);

	glPushMatrix();
		glTranslatef(1.5, 10.0, -2.0);
		glutSolidCube(.5);
	glPopMatrix();


	//Green emission light
	GLfloat XRayLight2AmbientDiffuse[] = {0.0, 1.0, 0.0};
	GLfloat emission2[] = {0.0, 1.0, 0.0, 0.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, XRayLight2AmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission2);

	glPushMatrix();
		glTranslatef(1.5, 10.0, -3.0);
		glutSolidCube(.5);
	glPopMatrix();
 
	//Green emission light for inside xray
        GLfloat XRayLight3AmbientDiffuse[] = {0.0, 1.0, 0.0};
	GLfloat emission3[] = {0.0, 1.0, 0.0, 0.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, XRayLight3AmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission3);

	glPushMatrix();
		glTranslatef(4, 9.5, -2.5);
		glutSolidCube(0.5);
	glPopMatrix();


        glMaterialfv(GL_FRONT, GL_EMISSION, emissionReset);

  
}


/*************************************
 *           conveyorSpotLight1      *
 *************************************/
//Creates a spotlight for the smaller xray machines
void conveyorSpotLight1(void){

  glPushMatrix();
  	GLfloat diffuseLight[] = {0.0, 1.0, 0.0, 1.0};
  	GLfloat specularLight[] = {1.0, 1.0, 1.0, 1.0};
  	GLfloat positionLight[] = {4, 20.5, -2.5, 1.0};
  	GLfloat spotDirection[] = {0.0, -1.0, 0.0};
  	
  	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
  	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
  	glLightfv(GL_LIGHT1, GL_POSITION, positionLight);
  	
  	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);
  	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 7.0);
    
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glPopMatrix();

}


/*************************************
 *           conveyorSpotLight2      *
 *************************************/
//Creates a spotlight for the larger xray machines
void conveyorSpotLight2(void){

  glPushMatrix();
    GLfloat diffuseLight[] = {0.0, 1.0, 0.0, 1.0};
  	GLfloat specularLight[] = {1.0, 1.0, 1.0, 1.0};
  	GLfloat spotDirection[] = {0.0, -1.0, 0.0};
  	GLfloat positionLight2[] = {4, 13, 13.0, 1.0};
  	
  	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight);
  	glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight);
  	glLightfv(GL_LIGHT2, GL_POSITION, positionLight2);
  	
  	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 40.0);
   	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirection);
  	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0.0);
    
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glPopMatrix();
  
  
}


/*************************************
 *           drawConveyorXRay        *
 *************************************/
//Draws a small conveyor xray machine
void drawConveyorXRay(void){

	GLfloat XRayAmbient[] = {0.19225f, 0.19225f, 0.19225f};
	GLfloat XRayDiffuse[] = {0.50754f, 0.50754f, 0.50754f};
	GLfloat XRaySpecular[] = {0.508273f, 0.508273f, 0.508273f};
	GLfloat shininess[] = {76.8};

	glMaterialfv(GL_FRONT, GL_AMBIENT, XRayAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, XRayDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, XRaySpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	//front
	glPushMatrix();
		glTranslatef(5.0, 5.0, 1.0);
		glScalef(7.0, 10.0, 1.0);
		glutSolidCube(1);
	glPopMatrix();

	//back
	glPushMatrix();
		glTranslatef(5.0, 5.0, -6.0);
		glScalef(7.0, 10.0, 1.0);
		glutSolidCube(1);
	glPopMatrix();

	//top
	glPushMatrix();
		glTranslatef(5.0, 10.0, -2.5);
		glScalef(7.0, 1.0, 6.0);
		glutSolidCube(1);
	glPopMatrix();

	
	if(xrayToggle == 1){
	  drawConveyorLights();
	}
  
}


/*************************************
 *           drawBagPickup           *
 *************************************/
//Draws the end of the conveyor where bags would be picked up
void drawBagPickup(void){
  
  GLfloat XRayAmbientDiffuse[] = {0.6523, 0.6133, 0.6055};
	GLfloat XRaySpecular[] = {1.0, 1.0, 1.0};
	GLfloat shininess[] = {100.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, XRayAmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, XRaySpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);


  glPushMatrix();
  	glTranslatef(32.0, 0.0, 0.0);
  	
  	//bottom
  	glPushMatrix();
      glTranslatef(0.0, 0.0, -2.5);
  		glScalef(10.0, 1.0, 7.0);
  		glutSolidCube(1);
  	glPopMatrix();
  	
  	
  	
  	//back side
  	glPushMatrix();
  		glTranslatef(0.0, 0.0, -6.5);
  		glScalef(10.0, 3.0, 1.0);
  		glutSolidCube(1);
  	glPopMatrix();
  
  
  	//back
  	glPushMatrix();
      glTranslatef(5.0, 0.0, -2.5);
  		glScalef(1.0, 3.0, 8.0);
  		glutSolidCube(1);
  	glPopMatrix();
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(34.0, -7.0, 0.0);
    
    drawTableLeg();
    
    glPushMatrix();
      glTranslatef(0.0, 0.0, -6.5);
      drawTableLeg();
    glPopMatrix();
    
  glPopMatrix(); 
     
}


/*************************************
 *          drawLargeXRay            *
 *************************************/
//Scales a smaller xray machine to become larger
void drawLargeXRay(void){
  
    glPushMatrix();
      glTranslatef(0.0, -13.0, 17.0);
      glScalef(1, 2.25, 1.5);
      drawConveyorXRay();
    glPopMatrix();
  
 
}


/*************************************
 *          drawGuideRopes           *
 *************************************/
//Draws the black guide ropes
void drawGuideRopes(void){

  GLfloat XRayAmbientDiffuse[] = {0.1, 0.1, 0.1};
	GLfloat XRaySpecular[] = {1.0, 1.0, 1.0};
	GLfloat shininess[] = {50.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, XRayAmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, XRaySpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);


  //Rope front
  glPushMatrix();
      glTranslatef(-30.0, 0.0, 20.0);
  		glScalef(35.0, 2.0, 1.0);
  		glutSolidCube(1);
  glPopMatrix();
  
  //Rope back
  glPushMatrix();
      glTranslatef(-30.0, 0.0, 7.0);
  		glScalef(35.0, 2.0, 1.0);
  		glutSolidCube(1);
  glPopMatrix();

}


/*************************************
 *          drawGuideRopePole        *
 *************************************/
//Draws the black guide rope poles 
void drawGuideRopePole(void){

  //Code for guide rope poles
  glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);   
    gluDisk(quad, 0, 1.5, 20, 1.0);
    gluCylinder(quad, 1.5, 1.5, 16.0, 10, 1);
  glPopMatrix();
  
}


/*************************************
 *          drawCompleteGuideRopes   *
 *************************************/
//Attaches two sets of ropes to two sets of poles
void drawCompleteGuideRopes(void){

  drawGuideRopes();
  
  glPushMatrix();
    glTranslatef(-13.5, 1.5, 20.0);
    drawGuideRopePole();
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(-13.5, 1.5, 7.0);
    drawGuideRopePole();
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(-48, 1.5, 20.0);
    drawGuideRopePole();
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(-48, 1.5, 7.0);
    drawGuideRopePole();
  glPopMatrix();


}


/*************************************
 *        interactiveSpotLight       *
 *************************************/
//Creates the interactive spotlight for users when clicking
void interactiveSpotLight(int x, int y, int z){

	glPushMatrix();
		GLfloat diffuseLight[] = {1.0, 1.0, 1.0, 1.0};
		GLfloat specularLight[] = {1.0, 1.0, 1.0, 1.0};
		GLfloat positionLight[] = {x, y, z, 1.0};
		GLfloat spotDirection[] = {0.0, 0.0, -1.0};

		glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuseLight);
		glLightfv(GL_LIGHT4, GL_SPECULAR, specularLight);
		glLightfv(GL_LIGHT4, GL_POSITION, positionLight);

		glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 20.0);
		glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spotDirection);
		glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 0.0);

		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glPopMatrix();

}


/*************************************
 *        doorLocalLight	     *
 *************************************/
//Creates the local light for above the doors
void doorLocalLight(void){
  
  
	GLfloat diffuseLight[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat specularLight[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat positionLight[] = {0, 0.0, 0, 1.0};

	glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT5, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT5, GL_POSITION, positionLight);
        glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.025);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  
}


/*************************************
 *        drawDoorLight		     *
 *************************************/
//Creates emitted light for above the doors
void  drawDoorLight(void){

	GLfloat doorLightAmbientDiffuse[] = {1.0, 1.0, 1.0};
	GLfloat emission[] = {1.0, 1.0, 1.0, 0.0};
	GLfloat emissionReset[]  = {0.0, 0.0, 0.0, 0.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, doorLightAmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);

	glPushMatrix();
		glTranslatef(-147, 22, 0.0);
		glPushMatrix();
			glTranslatef(2, 0.0, 0.0);

			doorLocalLight(); 
  		glPopMatrix();
		
		glPushMatrix();
			glScalef(1.0, 1.0, 15.0);
			glutSolidCube(2);
		glPopMatrix();
	glPopMatrix();
 
	glMaterialfv(GL_FRONT, GL_EMISSION, emissionReset);
 
 
}


/*************************************
 *        	drawDoors            *
 *************************************/
//Draws the set of doors 
void drawDoors(void){

	GLfloat doorsOutlineAmbientDiffuse[] = {0, 0, 0};
	GLfloat doorsOutlineSpecular[] = {1.0, 1.0, 1.0};
	GLfloat shininessOutline[] = {50.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, doorsOutlineSpecular);
	glMaterialfv(GL_FRONT, GL_SPECULAR, doorsOutlineSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininessOutline);
  


	glPushMatrix();
	glTranslatef(-148, 0, 0);

	//grey door outline
	glPushMatrix();
	glScalef(2, 50, 50);
	glutSolidCube(1);
	glPopMatrix();

	//MiddleBar
	glPushMatrix();
	glScalef(4, 40, 2);
	glutSolidCube(1);
	glPopMatrix();

	//Glass door sections
	GLfloat doorsAmbientDiffuse[] = {0.5625, 0.9961, 0.9375};
	  GLfloat doorsSpecular[] = {1.0, 1.0, 1.0};
	  GLfloat shininess[] = {50.0};

	  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, doorsAmbientDiffuse);
	  glMaterialfv(GL_FRONT, GL_SPECULAR, doorsSpecular);
	  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glPushMatrix();
	glTranslatef(0, 0, 5);
	glScalef(3, 40, 20);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -5);
	glScalef(3, 40, 20);
	glutSolidCube(1);
	glPopMatrix();



	glPopMatrix();

	if(lightToggle == 1){
	drawDoorLight();
	}


}


/*************************************
 *        drawCompleteBagCheck       *
 *************************************/
//Draws the full scene
void drawCompleteBagCheck(void){

  drawBeltComplete();
  
  drawConveyorSection();    //Draws one section of conveyor belt
  
  glPushMatrix();
    glTranslatef(15.0, 0.0, 0.0);
    drawConveyorSection();    //Draws second section of belt to extend it
  glPopMatrix();
  
  
  drawConveyorXRay();
  drawBagPickup();
  
  
  drawLargeXRay();
  
  drawCompleteGuideRopes();

  drawDoors();

}



/*************************************
 *               display             *
 *************************************/
void display (void)
{
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);


	double eyeX, eyeY, eyeZ;	

	//Calculations for the cameras location coordinates
	//"draws" a sphere for the camera to be placed on
	eyeX = radius * cos(vertical) *cos(horizontal);
	eyeY = radius * sin(vertical);
	eyeZ = radius * cos(vertical) * sin(horizontal);

  
  
  glPushMatrix();
    //interactiveSpotLight(eyeX+1, eyeY-2, eyeZ);
    gluLookAt(eyeX, eyeY, eyeZ, focusX, focusY, focusZ, upX, upY, upZ);
    globalLight();
    //**** Call to glut 3D functions go here
    drawLandscape(); //Draws floor
    drawWalls();    //Draws walls to building
    globalLight();
    
    conveyorSpotLight1();
    conveyorSpotLight2();
    drawDoors();

    
    glPushMatrix();
      glTranslatef(100.0, 0.0, 5.0);
      drawCheckInDesk();  //Draws check in desk
    glPopMatrix();
    
    drawCompleteBagCheck(); //Draws complete bag check (metal detectors, conveyor, etc)
    
    glPushMatrix();
      glTranslatef(0.0, 0.0, 100.0);
      drawCompleteBagCheck();
      
      glPushMatrix();
	glTranslatef(100.0, 0.0, 5.0);
	drawCheckInDesk();  //Draws check in desk
      glPopMatrix();
    glPopMatrix();
    
    glPushMatrix();
      glTranslatef(0.0, 0.0, 200.0);
      drawCompleteBagCheck();
      
      
      glPushMatrix();
	glTranslatef(100.0, 0.0, 5.0);
	drawCheckInDesk();  //Draws check in desk
      glPopMatrix();
    glPopMatrix();
    
    glEnable(GL_TEXTURE_2D);

    tex = SOIL_load_OGL_texture("envMap.bmp", 4, 0, 0);

    gluQuadricTexture(quad, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glPushMatrix();
 	  glTranslatef(55.0, 0, 0);
	  glRotatef(90, 0.0, 1.0, 0.0);
	  glTranslatef(15.0, 15.0, -30.0);
	  glRotatef(90, 1.0, 0.0, 0.0);
	  gluSphere(quad, 6.0, 40.0, 70.0);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    gluQuadricTexture(quad, GL_FALSE);


  glPopMatrix();

  
  glutSwapBuffers();  //Double buffering for animation

  glFlush ();

}


/*************************************
 *                init               *
 *************************************/
void init (void)
{

	glClearColor (1.0, 1.0, 1.0, 0.0);
	quad = gluNewQuadric();    //Quadric declaration for necessary primitives

	//******* Set 2nd argument
	gluQuadricDrawStyle(quad, GLU_FILL);

	gluQuadricOrientation(quad, GLU_OUTSIDE);
 
 	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
 
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	tiles = SOIL_load_OGL_texture("tiles.bmp", 4, 0, SOIL_FLAG_MIPMAPS);
	walls = SOIL_load_OGL_texture("walls.bmp", 4, 0, 0);
	glass = SOIL_load_OGL_texture("glass.bmp", 4, 0, 0);
	woodDesk = SOIL_load_OGL_texture("woodDesk.bmp", 4, 0, 0);

}


/*************************************
 *              reshape              *
 *************************************/
void reshape (int w, int h)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(75.0, 1.0, 10.0, 400.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
 	gluLookAt(5.0, 10.0, 20.0, 5.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


/*************************************
 *              keyPress              *
 *************************************/
//used to control camera placements
void keyPress(unsigned char key, int x, int y){

   //up
   if(key == 'w'){
  		  vertical += 0.35;
   }
   //left
   if(key == 'a' || key == 'A'){
  		  horizontal += 0.5;
   }
   //down
   if(key == 's'){
  		  vertical -= 0.35;
   }
   //right
   if(key == 'd' || key == 'D'){
  		  	horizontal -= 0.5;
   }
   //Zoom in
   if(key == 'W'){
		radius -= 2;
   }
   //Zoom out
   if(key == 'S'){
		radius += 2;
   }
   
  glutPostRedisplay();
}


/********************************************************
 *                                                      *
 *                    choiceMain		        *
 *                                                      *
 ********************************************************/
//Creates the choices for the main menu
void choiceLights(int selection){

	if(selection == 1){
		//Global Light On
		glEnable(GL_LIGHT0);
	}
	if(selection == 2){
		//GlobalLightOff
		glDisable(GL_LIGHT0);
	}
	if(selection == 3){
		//XRay Light On		
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		xrayToggle = 1;
	}
	if(selection == 4){
		//XRay Light Off	
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		xrayToggle = 0;
	}
	if(selection == 5){
		//TV On
		glEnable(GL_LIGHT3);
		tvToggle = 1;
	}
	if(selection == 6){
		glDisable(GL_LIGHT3);
		tvToggle = 0;
	}
	if(selection == 7){
		glEnable(GL_LIGHT5);
		lightToggle = 1;
	}
	if(selection == 8){
		glDisable(GL_LIGHT5);
		lightToggle = 0;
	}
	
	glutPostRedisplay();

}


/********************************************************
 *                                                      *
 *                    choiceMain		        *
 *                                                      *
 ********************************************************/
//Creates the choices for the main menu
void choiceMain(int selection){

	if(selection == 1){
		//Start		
     		animate = 1;
	}
	if(selection == 2){
		//Stop
    		animate = 0;
	}
	if(selection == 3){
		//Reset
		vertical = 0.5;
		horizontal = 1.5;
		radius = 15.0;
	}
	if(selection == 4){
		//Exit
		exit(0);
	}

	glutPostRedisplay();

}

 
/********************************************************
 *                                                      *
 *                    	  idle			                  	*
 *                                                      *
 ********************************************************/
//Connected to main idle function, and is in charge of animating
//all components of the picture. 
void idle (void)
{
    //Will only animate when user selects start
    if(animate == 1){
   
        Time = clock();
        if (Time > ResetTime) {
            ResetTime = ResetTime + ((1.0/(25.0))) * CLOCKS_PER_SEC;
            Time = clock();
            
            frameNum++;           //changes the multiplier for the gear rotations
          
            if(frameNum == 6.0){
            	frameNum = 1.0;
            }
        
           glutPostRedisplay();
        }
        
    }else{Time=0;}
    
}


/********************************************************
 *                                                      *
 *                       mouse				*
 *                                                      *
 ********************************************************/
//mouse function to toggle the leftPressed variable
void mouse(int button, int state, GLint x, GLint y){

  GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble modelview[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	gluUnProject(x,y,0,modelview, projection, viewport, &posX, &posY, &posZ);

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		leftPressed = 1;
		glEnable(GL_LIGHT4);
	}else{
		leftPressed = 0;
		glDisable(GL_LIGHT4);
	}
  glutPostRedisplay();

}


/********************************************************
 *                                                      *
 *                     dragFunc			        *
 *                                                      *
 ********************************************************/
//Drags the interactive spotlight for the user
void dragFunc (int x, int y) {

    if(leftPressed == 1){
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        
        GLdouble modelview[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        
        GLdouble projection[16];
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        
        gluUnProject(x,y,0,modelview, projection, viewport, &posX, &posY, &posZ);
       
        glPushMatrix();
          glTranslatef(posX*2, -posY*2, posZ*2);
          interactiveSpotLight(0,0,0);
        glPopMatrix();
         glutPostRedisplay();
  }

}



/*************************************
 *                main               *
 *************************************/
int main (int argc, char** argv)
{
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
  glutInitWindowSize (600, 600);
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  glutReshapeFunc(reshape);
  init ();
  glutDisplayFunc (display);
  
  
  glutKeyboardFunc(keyPress);
  
  int lightMenu = glutCreateMenu(choiceLights);  
  glutAddMenuEntry("Global Light On", 1);
  glutAddMenuEntry("Global Light Off", 2);
  glutAddMenuEntry("XRay Enable", 3);
  glutAddMenuEntry("XRay Disable", 4);
  glutAddMenuEntry("TV On", 5);
  glutAddMenuEntry("TV Off", 6);
  glutAddMenuEntry("Door On", 7);
  glutAddMenuEntry("Door Off", 8);

  //Creates main menu for starting and stopping the animation
  glutCreateMenu(choiceMain);
  glutAddMenuEntry("Start", 1);
  glutAddMenuEntry("Stop", 2);
  glutAddSubMenu("Lighting", lightMenu);
  glutAddMenuEntry("Reset", 3);
  glutAddMenuEntry("Exit", 4);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
   
  glutIdleFunc(idle);
  
  glutMouseFunc(mouse);
  glutMotionFunc(dragFunc);
  
  		  
	
  glutMainLoop ();
  return 0;
}
