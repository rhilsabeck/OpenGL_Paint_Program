// a simple paint program, soon to become A-Dopy-Illustrator V1
//Ryan Hilsabeck

//Everything implemented 

// include needed libraries
//#include <Windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include "CShape.h"
#include "CImage.h"
#include <iostream>
#include <sstream>

#define NULL 0

// defins shapes
#define POINTS 1
#define LINE 2
#define LINESTR 3
#define TRIANGLE 4
#define RECTANGLE 5
#define POLYGON 6
#define CIRCLE 7
#define ARC 8
#define TEXT 9
#define LOAD 10
#define SAVE 11
#define RED 12
#define GREEN 13
#define BLUE 14


// function prototypes
void mouse(int, int, int, int);
void key(unsigned char, int, int);
void displayText( int, int, float, float, float, const char );
void displayTextString( int, int, float, float, float, std::string );
//void fillString(int);
void drawModeText(int);
void display(void);
void myReshape(GLsizei, GLsizei);
void myinit(void);
void drag(int, int);
void passive(int, int);
void screen_box(int, int, int);
void right_menu(int);
void middle_menu(int);
void color_menu(int);
void pixel_menu(int);
void fill_menu(int);
int pick(int, int);

//global vars program state vars
GLsizei wh = 600, ww = 1000;  // window size
int draw_mode = NULL;        // current drawing mode 
int rx, ry;                  //raster position (rx, ry)
//global vars for rendering engine state
GLfloat rCurrent = ww/10 + ww/100; //current red slider x position
GLfloat gCurrent = ww/10 + ww/100; //current green slider x position
GLfloat bCurrent = ww/10 + ww/100; //current blue slider x position
GLfloat min = ww/100;  //min on the slider bar
GLfloat max = ww/10 + ww/100; //max on the slider bar
GLfloat dist = ww/10;  //dist of the slider bar
GLfloat r = (rCurrent - min) / dist, g = (gCurrent - min) / dist, b = (bCurrent - min) /dist; // current drawing color 
GLfloat cr = 0.0, cg = 0.0, cb = 0.0; // current clear color
bool fill = false;           // draw solid or outlines
GLfloat ptSize = 3.0f;   //initial point size
GLfloat lWidth = 1.0f;  //initial line width
int count = 0;  //count for vertices for some shapes such as triangle and polygon
int where = NULL;  //where mouse was clicked, helped to pick buttons
float PI = 3.14159;
bool dragFlag = false;  //drag flag for mouse
int buttonCount = 0; // used for polygon and line string to know when mode button is pushed twice to end current shape
char * sLoad = "Load", *sSave = "Save", *sPoint = "Point", *sLine = "Line", *sLineStr = "LineStr", *sRect = "Rectangle",
	*sTri = "Triangle", *sCircle = "Circle", *sPolygon = "Polygon", *sArc = "Arc", *sText = "Text", *message = "Pick a draw mode",
	*red = "R", *green = "G", *blue = "B", *color = "Color";

//strings for status line
std::string drawMode = "";
std::string sfill = "Off";
std::string sPtSize = "3px";
std::string sLnSize = "1px";
std::string mouseX = "";
std::string mouseY = "";
std::string sKeyBuff;


int keyboardindex = 0;
int intMX;  //passive mouse movement x
int intMY;	//passive mouse movement y

CShape * sPtr = NULL, *shapeHead = NULL, *currentShape = NULL;
CCoord * cPtr;

CImage * picture = new CImage();

CButton * bLoad;
CButton * bSave;
CButton * bPoint;
CButton * bLine;
CButton * bLineStr;
CButton * bTriangle;
CButton * bRectangle;
CButton * bPolygon;
CButton * bCircle;
CButton * bArc;
CButton * bText;

CSlider * sRed;
CSlider * sGreen;
CSlider * sBlue;


// rehaping routine called whenever window is resized or moved 

void myReshape(GLsizei w, GLsizei h)
{
	// adjust clipping box
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// adjust viewport and  clear 
	glViewport(0, 0, w, h);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glutPostRedisplay();
	//display();
	glFlush();

	// set global size for use by drawing routine
	ww = w; wh = h;
}

void myinit(void)
{
	glViewport(0, 0, ww, wh);
	 //Pick 2D clipping window to match size of X window 
	// This choice avoids having to scale object coordinates
	// each time window is resized 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)ww, 0.0, (GLdouble)wh, -1.0, 1.0);
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);  // set clear color to black and clear window 
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

//Rubberbanding and sliding color sliders
void drag(int x, int y)
{
	int ny = wh - y;

	if(dragFlag && (x >= ww/5) && (x <= ww) && (ny <= (wh - wh/15)) && (ny >= (wh/12)))
	{
	currentShape->mouseDrag(x,ny);
	}
	else if(where == RED && dragFlag && (x >= ww/100) && (x<= ww/100 + ww/10) && (ny <= (wh/6 - wh/120)) && (ny >= wh/8))
	{

		printf("we are in red slider drag mode\n");
		rCurrent = x;
		r = (rCurrent - min) / dist;
	}
	else if(where == GREEN && dragFlag && (x >= ww/100) && (x<= ww/100 + ww/10) && (ny <= (wh/10 + wh/120)) && (ny >= (wh/10- wh/120)))
	{
		printf("we are in green slider drag mode\n");
		gCurrent = x;
		g = (gCurrent - min) / dist;
	}
	else if(where == BLUE && dragFlag && (x >= ww/100) && (x<= ww/100 + ww/10) && (ny <= (wh/20 + wh/120)) && (ny >= (wh/30 + wh/120)))
	{
		printf("we are in blue slider drag mode\n");
		bCurrent = x;
		b = (bCurrent - min) / dist;
	}



	glutPostRedisplay();
}

//get x, y coords when mouse is moving but no button pushed
void passive(int x, int y)
{
	int ny = wh -y;
	intMX = x;
	intMY = ny;

	std::ostringstream xtrStream;
	std::ostringstream ytrStream;

	xtrStream << x;
	ytrStream << ny;

	mouseX = xtrStream.str();
	mouseY = ytrStream.str();
	glutPostRedisplay();
}



void mouse(int btn, int state, int x, int y)  //defines the drawing shapes
{

	int gy = wh - y;

	if (btn == GLUT_LEFT_BUTTON)
	{

	  if(state == GLUT_DOWN)
	  {
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		where = pick(x, gy);
		//glColor3f(r, g, b);
		if (where != 0)
		{
			count = 0;
			if(where == SAVE)
			{
				picture->save("currentImage.pic");
				picture->clear();
				message = "Save Successful!";

			}
			else if(where == LOAD)
			{
				picture->load("currentImage.pic", picture);
				message = "Image Loaded";
			}
			else if(where == RED || where == GREEN || where == BLUE)
			{
				dragFlag = true;
			}
			else
			{
				draw_mode = where;
				drawModeText(draw_mode);
				if(draw_mode == LINESTR || draw_mode == POLYGON)
					buttonCount++;
				else
					dragFlag = false;
					currentShape = NULL;
					buttonCount = 0;
			}
		}
		//Only draw in drawing area 
		else if ((x <= ww/5) || (gy >= wh- wh/15) || (gy<= wh/12))
		{
			count = 0;
		}
		else switch (draw_mode)
		{
		case(LINE) :
			dragFlag = true;
			cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
			
			if(!currentShape)
			{
				currentShape = new CLine();
				currentShape->lWidth = lWidth;
				picture->insertFigure(currentShape);
				currentShape->insertVertex(cPtr);
				cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
				currentShape->insertVertex(cPtr);
			}
			else
			{
				printf("ERROR\n");
			}
		  break;
		case(RECTANGLE) :
			dragFlag = true;
			cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
			
			if(!currentShape)
			{
				currentShape = new CRect();
				currentShape->shapeFill = fill;
				currentShape->lWidth = lWidth;
				picture->insertFigure(currentShape);
				currentShape->insertVertex(cPtr);
				cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
				currentShape->insertVertex(cPtr);
			}
			else
			{
				printf("ERROR\n");
			}
		break;
		case (TRIANGLE) :
			dragFlag = true;
			cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
			
			if(!currentShape)
			{
				currentShape = new CTri();
				currentShape->shapeFill = fill;
				currentShape->lWidth = lWidth;
				picture->insertFigure(currentShape);
				currentShape->insertVertex(cPtr);
				count++;
				cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
				currentShape->insertVertex(cPtr);
		        count++;
			}
			else
			{
				currentShape->insertVertex(cPtr);
				count++;
			}
			break;
		case(POINTS) :
		{
		   cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
			
			if(!currentShape)
			{
				currentShape = new CPoint();
				currentShape->ptSize = ptSize;
				currentShape->insertVertex(cPtr);
				picture->insertFigure(currentShape);
			}
			else
			{
				printf("ERROR\n");
			}
		}
		break;
		case(TEXT) :			
		{
			cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
			
			if(!currentShape)
			{
				currentShape = new Ctext();
				currentShape->text = sKeyBuff;
				currentShape->insertVertex(cPtr);
				picture->insertFigure(currentShape);
			}
			else
			{
				printf("ERROR\n");
			}
		}
		break;
		case(CIRCLE) :
			dragFlag = true;
			cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
			
			if(!currentShape)
			{
				currentShape = new CCircle();
				currentShape->shapeFill = fill;
				currentShape->lWidth = lWidth;
				picture->insertFigure(currentShape);
				currentShape->insertVertex(cPtr);
				cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
				currentShape->insertVertex(cPtr);
			}
			else
			{
				printf("ERROR\n");
			}
		break;
		case(LINESTR) :
			dragFlag = true;
			cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
			
			if(!currentShape)
			{
				currentShape = new CLineStr();
				currentShape->lWidth = lWidth;
				picture->insertFigure(currentShape);
				currentShape->insertVertex(cPtr);
				count++;
				cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
				currentShape->insertVertex(cPtr);
				count++;
			}
			else
			{
				currentShape->insertVertex(cPtr);
				count++;
			}
		break;
		case(POLYGON) :
			dragFlag = true;
			cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
			
			if(!currentShape)
			{
				currentShape = new CPoly();
				currentShape->shapeFill = fill;
				currentShape->lWidth = lWidth;
				picture->insertFigure(currentShape);
				currentShape->insertVertex(cPtr);
				count++;
				cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
				currentShape->insertVertex(cPtr);
		        count++;
			}
			else
			{
				currentShape->insertVertex(cPtr);
				count++;
			}
		break;
		case(ARC) :
			dragFlag = true;
			cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
			
			if(!currentShape)
			{
				currentShape = new CArc();
				currentShape->shapeFill = fill;
				currentShape->lWidth = lWidth;
				//currentShape->angle = arcAngle;
				picture->insertFigure(currentShape);
				currentShape->insertVertex(cPtr);
				cPtr = new CCoord(x, gy, 0, r, g, b, 1.0f);
				currentShape->insertVertex(cPtr);
			}
			else
			{
				printf("ERROR\n");
			}
		break;
		}

		glPopAttrib();
		glFlush();
	}
	//when left mouse button is up after pushed down
	else
	{
		if(draw_mode == LINESTR)
		{
			if(buttonCount == 2)
			{
			dragFlag = false;
			currentShape = NULL;
			count = 0;
			buttonCount = 1;
			}
			
		}
		else if(draw_mode == TRIANGLE)
		{
			if(count == 3)
			{
				dragFlag = false;
				currentShape = NULL;
				count = 0;
			}

		}
		else if(draw_mode == POLYGON)
		{
			if(buttonCount == 2 || count == 10)
			{
			dragFlag = false;
			currentShape = NULL;
			buttonCount = 1;
			count = 0;
			}
		}
		else if(draw_mode == TEXT)
		{
			sKeyBuff.clear();
			keyboardindex = 0;
			currentShape = NULL;
		}
		else if(draw_mode == ARC)
		{
			//sKeyBuff.clear();
			dragFlag = false;
			currentShape = NULL;
		}
		else if(where == LOAD)
		{
				
			
		}
		else if(where == SAVE)
		{
			
		}
		else if(where == RED || where == GREEN || where == BLUE)
		{
			dragFlag = false;
		
		}
		else
		{
			dragFlag = false;
			currentShape = NULL;
		}
	}
}

 glutPostRedisplay();
}

int pick(int x, int y)  //which button is selected?
{
	//y = wh - y;
	if (x > wh/3)      return NULL;
	else if (bPoint->isPicked(x,y,0))   return POINTS;
	else if (bLine->isPicked(x,y,0))      return LINE;
	else if (bLineStr->isPicked(x,y,0))      return LINESTR;
	else if (bTriangle->isPicked(x,y,0))       return TRIANGLE;
	else if (bRectangle->isPicked(x,y,0))       return RECTANGLE;
	else if (bPolygon->isPicked(x,y,0))  return POLYGON;
	else if (bCircle->isPicked(x,y,0))       return CIRCLE;
	else if (bArc->isPicked(x,y,0))       return ARC;
	else if (bText->isPicked(x,y,0))       return TEXT;
	else if (bSave->isPicked(x,y,0))	return SAVE;
	else if (bLoad->isPicked(x,y,0))	return LOAD;
	else if (sRed->isPicked(x,y,0))		return RED;
	else if (sGreen->isPicked(x,y,0))  return GREEN;
	else if (sBlue->isPicked(x,y,0))  return BLUE;
	else return 0;
}

// mouse menu code
void right_menu(int id)
{
	if (id == 1) exit(0);
	if (id == 2)
	{
		if(draw_mode == LINESTR || draw_mode == POLYGON)
		{
		dragFlag = false;
		currentShape = NULL;
		buttonCount = 1;
		count = 0;
		}
		picture->clear();
		
	}
	else glutPostRedisplay();
}

void middle_menu(int id)
{
}



// set the current drawing color - couldn't get slider to work on time so just using this for color selection
void color_menu(int id)
{
	if (id == 1) { r = 1.0; g = 0.0; b = 0.0; rCurrent = max; gCurrent = min; bCurrent = min;  }
	else if (id == 2) { r = 0.0; g = 1.0; b = 0.0; rCurrent = min; gCurrent = max; bCurrent = min; }
	else if (id == 3) { r = 0.0; g = 0.0; b = 1.0; rCurrent = min; gCurrent = min; bCurrent = max; }
	else if (id == 4) { r = 0.0; g = 1.0; b = 1.0; rCurrent = min; gCurrent = max; bCurrent = max; }
	else if (id == 5) { r = 1.0; g = 0.0; b = 1.0; rCurrent = max; gCurrent = min; bCurrent = max; }
	else if (id == 6) { r = 1.0; g = 1.0; b = 0.0; rCurrent = max; gCurrent = max; bCurrent = min; }
	else if (id == 7) { r = 1.0; g = 1.0; b = 1.0; rCurrent = max; gCurrent = max; bCurrent = max; }
	else if (id == 8) { r = 0.0; g = 0.0; b = 0.0; rCurrent = min; gCurrent = min; bCurrent = min; }
}

// set the current drawing color
void clear_color_menu(int id)
{
	if (id == 1) { cr = 1.0; cg = 0.0; cb = 0.0; }
	else if (id == 2) { cr = 0.0; cg = 1.0; cb = 0.0; }
	else if (id == 3) { cr = 0.0; cg = 0.0; cb = 1.0; }
	else if (id == 4) { cr = 0.0; cg = 1.0; cb = 1.0; }
	else if (id == 5) { cr = 1.0; cg = 0.0; cb = 1.0; }
	else if (id == 6) { cr = 1.0; cg = 1.0; cb = 0.0; }
	else if (id == 7) { cr = 1.0; cg = 1.0; cb = 1.0; }
	else if (id == 8) { cr = 0.0; cg = 0.0; cb = 0.0; }
}

// set point size
void point_size_menu(int id)
{
	if (id == 1) { ptSize = 1.0f; sPtSize = "1px";}
	else if (id == 2) { ptSize = 2.0f; sPtSize = "2px";}
	else if (id == 3) { ptSize = 3.0f; sPtSize = "3px"; }
	else if (id == 4) { ptSize = 4.0f; sPtSize = "4px"; }
	else if (id == 5) { ptSize = 5.0f; sPtSize = "5px"; }
}

// set line width
void line_width_menu(int id)
{
	if (id == 1) { lWidth = 1.0f; sLnSize = "1px"; }
	else if (id == 2) { lWidth = 2.0f; sLnSize = "2px";}
	else if (id == 3) { lWidth = 3.0f; sLnSize = "3px";}
	else if (id == 4) { lWidth = 4.0f; sLnSize = "4px";}
	else if (id == 5) { lWidth = 5.0f; sLnSize = "5px";}
}

// set fill mode
void fill_menu(int id)
{

	if(id == 1)
	{
		fill = 1;
		sfill = "On";
	}
	else
	{
		fill = 0;
		sfill = "Off";
	}
}

// KB input
void key(unsigned char k, int xx, int yy)
{
	if (draw_mode == TEXT)
	{
	if(k == 8)
	{
		if(keyboardindex != 0)
		{
			sKeyBuff.pop_back();
			keyboardindex--;
			glutPostRedisplay();
		}
	}
	else
	{
	sKeyBuff.push_back(k);

	keyboardindex++;
	glutPostRedisplay();
	}
	}
	else
		return;

}

//this helps display text on buttons and other places other then status line
void displayText(int x, int y, float r, float g, float b, const char *string ) {
	int j = strlen( string );
 
	glColor3f( r, g, b );
	glRasterPos2i( x, y );
	for( int i = 0; i < j; i++ ) {
		glutBitmapCharacter( GLUT_BITMAP_8_BY_13, string[i] );
	}
}

//This will help display text on status line
void displayTextString(int x, int y, float r, float g, float b, std::string type) {
	std::string status;
	if(type == "draw_mode")
	{
		status = "Draw Mode:" + drawMode;
	}
	else if(type == "fill")
	{
		status =	"Fill:" + sfill;
	}
	else if(type == "point_size")
	{
		status =	"Pt Size:" + sPtSize;
	}
	else if(type == "line_size")
	{
		status =	"Ln Size:" + sLnSize;
	}
	else if(type == "mouse_coords")
	{
		status =	"Mouse Coords:(" + mouseX + "," + mouseY + ")";
	}

	const char * cStr = status.c_str();
	int j = strlen(cStr);
 
	glColor3f( r, g, b );
	glRasterPos2i( x, y );
	for( int i = 0; i < j; i++ ) {
		glutBitmapCharacter( GLUT_BITMAP_8_BY_13, cStr[i] );
	}
}

//This will put what draw mode to put in status line and what message to display on bottom of screen
void drawModeText(int dMode)
{
	switch(dMode)
	{
		case POINTS:
			drawMode = "Points";
			message = "Click on draw area to draw points. Click Center button for point size menu";
		break;
		case LINE:
			drawMode = "Line";
			message = "Click and drag to draw lines. Click Center button for line size menu";
		break;
		case LINESTR:
			drawMode = "LineStr";
			message = "Click and drag to draw line strings. Click LineStr Mode button again, to start new string ";
		break;
		case TRIANGLE:
			drawMode = "Triangle";
			message = "Click and drag to create a line segment, repeat to close the triangle and drag to rubberband.";
		break;
		case RECTANGLE:
			drawMode = "Rectangle";
			message = "Click and drag to draw rectangle. Click Center button for line size menu and to change fill mode";
		break;
		case POLYGON:
			drawMode = "Polygon";
			message = "Click and drag to create points for a polygon";
		break;
		case CIRCLE:
			drawMode = "Circle";
			message = "Click and drag to draw circle from the center point";
		break;
		case ARC:
			drawMode = "Arc";
			message = "Click and drag for arc. Clockwise for negative arc and counter clockwise for positive arc  ";
		break;
		case TEXT:
			drawMode = "Text";
			message = "Enter text: ";
		break;
		case NULL:
			drawMode = " ";
			message = " ";
		break;
	}
}



// drawing callback
void display(void)
{

	// save rendering state
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	// draw the painting screen
	glClearColor(cr, cg, cb, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Pick 2D clipping window to match size of X window 
	// This choice avoids having to scale object coordinates
	// each time window is resized
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, ww, wh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)ww, 0.0, (GLdouble)wh, -1.0, 1.0);



	//draw menu sections
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2i(0,wh);
	glVertex2i(0, 0);
	glVertex2i(ww/5, 0);
	glVertex2i(ww/5, wh);

	glColor3f(0.5f, 0.5f,0.5f);
	glVertex2i(0,wh/6);
	glVertex2i(0, 0);
	glVertex2i(ww/5, 0);
	glVertex2i(ww/5, wh/6);

	glColor3f(0.0f, 0.6f, 0.6f);
	glVertex2i(ww/5,wh);
	glVertex2i(ww, wh);
	glVertex2i(ww,wh - wh/15);
	glVertex2i(ww/5, wh - wh/15);

	
	glVertex2i(ww/5,0);
	glVertex2i(ww/5, wh/12);
	glVertex2i(ww,wh/12);
	glVertex2i(ww, 0);


	//red color bar
	glColor3f(0.0f, g, b);
	glVertex2i(ww/100,wh/6 - wh/60);
	glVertex2i(ww/100, wh/10 + wh/30);
	glColor3f(1.0f, g, b);
	glVertex2i(ww/10 + ww/100, wh/10 + wh/30);
	glVertex2i(ww/10 + ww/100, wh/6 - wh/60);

	//green color bar
	glColor3f(r, 0.0f, b);
	glVertex2i(ww/100,wh/10);
	glVertex2i(ww/100, wh/10 - wh/60);
	glColor3f(r, 1.0f, b);
	glVertex2i(ww/10 + ww/100, wh/10 - wh/60);
	glVertex2i(ww/10 + ww/100, wh/10);

	//blue color bar
	glColor3f(r, g, 0.0f);
	glVertex2i(ww/100,wh/20);
	glVertex2i(ww/100, wh/30);
	glColor3f(r, g, 1.0f);
	glVertex2i(ww/10 + ww/100, wh/30);
	glVertex2i(ww/10 + ww/100, wh/20);

	//Current Color Selection box
	glColor3f(r,g,b);
	glVertex2i(ww/10 + (ww/25 - ww/100), wh/10 + wh/30);
	glVertex2i(ww/5 - ww/100, wh/10 + wh/30);
	glVertex2i(ww/5 - ww/100, wh/20);
	glVertex2i(ww/10 +(ww/25 - ww/100), wh/20);

	glEnd();

	//black outline around current color box
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(ww/10 + (ww/25 - ww/100), wh/10 + wh/30);
	glVertex2i(ww/5 - ww/100, wh/10 + wh/30);
	glVertex2i(ww/5 - ww/100, wh/20);
	glVertex2i(ww/10 +(ww/25 - ww/100), wh/20);
	glEnd();

	//create buttons
	bLoad = new CButton(0,wh,LOAD,ww/10,wh/15);
	bSave = new CButton(ww/10,wh,SAVE,ww/10,wh/15);
	bPoint = new CButton(ww/20,wh - wh/12,POINTS,ww/10,wh/15);
	bLine = new CButton(ww/20,wh/2 + wh/3, LINE,ww/10,wh/15);
	bLineStr = new CButton(ww/20,wh/2 + wh/4,LINESTR,ww/10,wh/15);
	bTriangle = new CButton(ww/20,wh/2 + wh/6,TRIANGLE,ww/10,wh/15);
	bRectangle = new CButton(ww/20,wh/2 + wh/12,RECTANGLE,ww/10,wh/15);
	bPolygon = new CButton(ww/20,wh/2,POLYGON,ww/10,wh/15);
	bCircle = new CButton(ww/20,wh/3 +wh/12,CIRCLE,ww/10,wh/15);
	bArc = new CButton(ww/20,wh/3,ARC,ww/10,wh/15);
	bText = new CButton(ww/20,wh/6 + wh/12,TEXT,ww/10,wh/15);

	//create sliders
	sRed = new CSlider(rCurrent,wh/6 - wh/120,RED,ww/250, wh/30,ww/100,ww/10 + ww/100,rCurrent);
	sGreen = new CSlider(gCurrent,wh/10 + wh/120,GREEN,ww/250, wh/30,ww/100,ww/10 + ww/100,gCurrent);
	sBlue = new CSlider(bCurrent,wh/20 + wh/120,BLUE,ww/250, wh/30,ww/100,ww/10 + ww/100,bCurrent);

	//draw all buttons needed
	bLoad->draw();
	bSave->draw();
	bPoint->draw();
	bLine->draw();
	bLineStr->draw();
	bTriangle->draw();
	bRectangle->draw();
	bPolygon->draw();
	bCircle->draw();
	bArc->draw();
	bText->draw();

	//draw slider
	sRed->draw();
	sGreen->draw();
	sBlue->draw();

	//display text on buttons
	displayText(ww/34, wh - wh/24, 0.0f, 0.0f, 0.0f,sLoad);
	displayText(ww/8, wh - wh/24, 0.0f, 0.0f, 0.0f,sSave);
	displayText(ww/15, wh - wh/8, 0.0f, 0.0f, 0.0f,sPoint);
	displayText(ww/15, wh * 2 / 3 + wh/8, 0.0f, 0.0f, 0.0f,sLine);
	displayText(ww/15, wh * 2 / 3 + wh/24, 0.0f, 0.0f, 0.0f,sLineStr);
	displayText(ww/15, wh/2 + wh/8, 0.0f, 0.0f, 0.0f,sTri);
	displayText(ww/15, wh/2 + wh/24, 0.0f, 0.0f, 0.0f,sRect);
	displayText(ww/15, wh/3 + wh/8, 0.0f, 0.0f, 0.0f,sPolygon);
	displayText(ww/15, wh/3 + wh/24, 0.0f, 0.0f, 0.0f,sCircle);
	displayText(ww/15, wh/3 - wh/24, 0.0f, 0.0f, 0.0f,sArc);
	displayText(ww/15, wh/3 - wh/8, 0.0f, 0.0f, 0.0f,sText);
	displayText(ww/10 + (ww/200 + ww/100), wh/10 + wh/30, 0.0f, 0.0f, 0.0f, red);
	displayText(ww/10 + (ww/200 + ww/100), wh/10 - wh/60, 0.0f, 0.0f, 0.0f, green);
	displayText(ww/10 + (ww/200 + ww/100), wh/30, 0.0f, 0.0f, 0.0f, blue);
	displayText(ww/25 + ww/10, wh/6 - wh/60, 0.0f, 0.0f, 0.0f, color);

	//drawModeText(draw_mode);
	displayTextString(ww/5 + ww/100, wh - wh/24, 0.0f, 0.0f, 0.0f, "draw_mode");
	displayTextString(ww/2 - ww/10, wh - wh/24, 0.0f, 0.0f, 0.0f, "fill");
	displayTextString(ww/2 - ww/100, wh - wh/24, 0.0f, 0.0f, 0.0f, "point_size");
	displayTextString(ww - (ww/5 * 2), wh - wh/24, 0.0f, 0.0f, 0.0f, "line_size");
	displayTextString(ww - ww/5, wh - wh/24, 0.0f, 0.0f, 0.0f, "mouse_coords");

	//display message that will go on message area
	displayText(ww/5 + ww/100, wh/24, 0.0f, 0.0f, 0.0f,message);

	//if text, show user written text in message area and by mouse pointer
	if(draw_mode == TEXT)
	{
		int l,i;

		l=strlen(sKeyBuff.c_str()); // see how many characters are in text string.
		glColor3f(0.0f,0.0f,0.0f);
		glRasterPos2i( ww/5 + ww/10, wh/24); // location to start printing text
		for( i=0; i < l; i++) // loop until i is greater then l
		{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, sKeyBuff.c_str()[i]); // Print a character on the screen
		glutPostRedisplay();
		}
		//text by mouse pointer
		glColor3f(r,g,b);
		glRasterPos2i( intMX, intMY); // location to start printing text where mouse is
		for( i=0; i < l; i++) // loop until i is greater then l
		{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, sKeyBuff.c_str()[i]); // Print a character on the screen
		glutPostRedisplay();
		}
	}




	// Pick 2D clipping window to match size of X window 
	// This choice avoids having to scale object coordinates
	// each time window is resized
	glMatrixMode(GL_MODELVIEW);
	glViewport(ww/5, wh/12, ww-ww/5,wh/2 + wh/3 + wh/60);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho((GLdouble)ww/5,(GLdouble)ww , wh/12, wh/2 + wh/3 + wh/10, -1.0, 1.0);

	
	picture->draw();

	

	glutSwapBuffers();

	// restore state
	glPopAttrib();
}

// setup window
int main(int argc, char** argv)
{
	int c_menu, f_menu, p_menu, l_menu, cc_menu;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(1000, 600);
	glutCreateWindow("Simple Paint");
	glutDisplayFunc(display);
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(key);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	glutPassiveMotionFunc(passive);
	myinit();

	// create a menu for the colors
	c_menu = glutCreateMenu(color_menu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Cyan", 4);
	glutAddMenuEntry("Magenta", 5);
	glutAddMenuEntry("Yellow", 6);
	glutAddMenuEntry("White", 7);
	glutAddMenuEntry("Black", 8);
	//create a menu for clear colors
	cc_menu = glutCreateMenu(clear_color_menu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Cyan", 4);
	glutAddMenuEntry("Magenta", 5);
	glutAddMenuEntry("Yellow", 6);
	glutAddMenuEntry("White", 7);
	glutAddMenuEntry("Black", 8);

	// create a sub menu for point size
	p_menu =glutCreateMenu(point_size_menu);
	glutAddMenuEntry("1 Pixel", 1);
	glutAddMenuEntry("2 Pixels", 2);
	glutAddMenuEntry("3 Pixels", 3);
	glutAddMenuEntry("4 Pixels", 4);
	glutAddMenuEntry("5 Pixels", 5);
	// create a sub menu for line size
	l_menu =glutCreateMenu(line_width_menu);
	glutAddMenuEntry("1 Pixel", 1);
	glutAddMenuEntry("2 Pixels", 2);
	glutAddMenuEntry("3 Pixels", 3);
	glutAddMenuEntry("4 Pixels", 4);
	glutAddMenuEntry("5 Pixels", 5);
	

	// create a shape option menu
	f_menu = glutCreateMenu(fill_menu);
	glutAddMenuEntry("fill on", 1);
	glutAddMenuEntry("fill off", 2);
	// create main menu
	glutCreateMenu(right_menu);
	glutAddMenuEntry("quit", 1);
	glutAddMenuEntry("clear", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);  //attach menu to right button
	// create options menu
	glutCreateMenu(middle_menu);
	glutAddSubMenu("Colors", c_menu);
	glutAddSubMenu("Clear Colors", cc_menu);
	glutAddSubMenu("Point Size", p_menu);
	glutAddSubMenu("Line Size", l_menu);
	glutAddSubMenu("Fill", f_menu);
	glutAttachMenu(GLUT_MIDDLE_BUTTON); // attach options menu to middle button

	glutMainLoop();
}