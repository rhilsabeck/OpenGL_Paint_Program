#include <GL/glut.h>
#include <windows.h>
#include "CShape.h"
#include <math.h>
#include <iostream>

#define PI 3.14159265

// ********************* CShape ********************

bool CShape::insertVertex(CCoord * vertPtr)
{
	if(nbrVert == 0)
		vertHead = vertTail = vertPtr;
	else
	{
		vertTail->nextCoord = vertPtr;
		vertTail = vertPtr;
	}
	nbrVert++;

	return true;
}

CShape::~CShape()
{
	CCoord * ptr;

	while(ptr = vertHead)
	{
		vertHead = vertHead->nextCoord;
		delete ptr;
	}
}

// ********************* CPoint ********************

void CPoint::draw()
{
	glPointSize(ptSize);
	glBegin(GL_POINTS);
		for(CCoord * ptr = vertHead; ptr; ptr=ptr->nextCoord)
		{
			glColor3f(ptr->r,ptr->g,ptr->b);
			glVertex2i(ptr->x, ptr->y);
		}
		glEnd();
		glFlush();
}

// ********************* CLine ********************

void CLine::draw()
{
	glLineWidth(lWidth);
	glBegin(GL_LINES);
		for(CCoord * ptr = vertHead; ptr; ptr=ptr->nextCoord)
		{
			glColor3f(ptr->r,ptr->g,ptr->b);
			glVertex2i(ptr->x, ptr->y);
		}
		glEnd();
		glFlush();
}

void CLine::mouseDrag(int x, int y)
{

		vertTail->x = x;
		vertTail->y = y;

	glutPostRedisplay();
}

// ********************* CLineStr ********************

void CLineStr::draw()
{
	glLineWidth(lWidth);
	glBegin(GL_LINE_STRIP);
		for(CCoord * ptr = vertHead; ptr; ptr=ptr->nextCoord)
		{
			glColor3f(ptr->r,ptr->g,ptr->b);
			glVertex2i(ptr->x, ptr->y);
		}
		glEnd();
		glFlush();
}

void CLineStr::mouseDrag(int x, int y)
{

		vertTail->x = x;
		vertTail->y = y;

	glutPostRedisplay();
}

// ********************* CCircle ********************

void CCircle::draw()
{
	int x = vertTail->x - vertHead->x;
    int y = vertTail->y - vertHead->y;
    radius = sqrt( (double)(x * x + y * y));
    //printf(" drag %f %f \n", x, y);
    
	float X1 = vertHead->x +radius;
	float Y1 = vertHead->y;
	if(shapeFill) glBegin(GL_TRIANGLE_FAN);
	else 
	{
		glLineWidth(lWidth);
		glBegin(GL_LINE_STRIP);	
	}
	for(float angle=0.0f;angle<=(2.0f*3.14159);angle+=0.01f)
	{
			glColor3f(vertHead->r,vertHead->g,vertHead->b);
			glVertex2f(X1, Y1);
			X1 = vertHead->x +(radius * (float)cos((double)angle));
			Y1 = vertHead->y +(radius * (float)sin((double)angle));
	}
	glEnd();
	//glFlush();
}

void CCircle::mouseDrag(int x, int y)
{

		vertTail->x = x;
		vertTail->y = y;

	glutPostRedisplay();
}

// ********************* CRect ********************

void CRect::draw()
{
	int tx, ty, bx, by;
	if(shapeFill) glBegin(GL_POLYGON);
    else
	{
		glLineWidth(lWidth);
		glBegin(GL_LINE_LOOP);
	}
	for(CCoord * ptr = vertHead; ptr; ptr=ptr->nextCoord)
		{
			tx= ptr->x, ty = ptr->y;
			glColor3f(ptr->r, ptr->g,ptr->b);
			if (ptr->nextCoord != NULL)
			{
				ptr=ptr->nextCoord;
				bx = ptr->x; by=ptr->y;

				glVertex2i(tx, ty);
				glVertex2i(tx, by);
				glVertex2i(bx, by);
				glVertex2i(bx, ty);
			}
		}
	glEnd();
	glFlush();		
}

void CRect::mouseDrag(int x, int y)
{

		vertTail->x = x;
		vertTail->y = y;

	glutPostRedisplay();
}

// ********************* CPoly ********************

void CPoly::draw()
{
	if(nbrVert <=2)
	{
		glLineWidth(lWidth);
		glBegin(GL_LINES);
		for(CCoord * ptr = vertHead; ptr; ptr=ptr->nextCoord)
		{
			glColor3f(ptr->r,ptr->g,ptr->b);
			glVertex2i(ptr->x, ptr->y);
		}
	glEnd();
	glFlush();
	}
	else
	{
	if(shapeFill) glBegin(GL_POLYGON);
    else glBegin(GL_LINE_LOOP);
		for(CCoord * ptr = vertHead; ptr; ptr=ptr->nextCoord)
		{
			glColor3f(ptr->r,ptr->g,ptr->b);
			glVertex2i(ptr->x, ptr->y);
		}
	glEnd();
	glFlush();
	}
}

void CPoly::mouseDrag(int x, int y)
{

		vertTail->x = x;
		vertTail->y = y;

	glutPostRedisplay();
}

// ********************* CTri ********************

void CTri::draw()
{
	if(nbrVert <=2)
	{
		glLineWidth(lWidth);
		glBegin(GL_LINES);
		for(CCoord * ptr = vertHead; ptr; ptr=ptr->nextCoord)
		{
			glColor3f(ptr->r,ptr->g,ptr->b);
			glVertex2i(ptr->x, ptr->y);
		}
	glEnd();
	glFlush();
	}
	else
	{
	if(shapeFill) glBegin(GL_POLYGON);
    else 
		{
			glLineWidth(lWidth);
			glBegin(GL_LINE_LOOP);
		}
		for(CCoord * ptr = vertHead; ptr; ptr=ptr->nextCoord)
		{
			glColor3f(ptr->r,ptr->g,ptr->b);
			glVertex2i(ptr->x, ptr->y);
		}
	glEnd();
	glFlush();
	}
}

void CTri::mouseDrag(int x, int y)
{

		vertTail->x = x;
		vertTail->y = y;

	glutPostRedisplay();
}

// ********************* CText ********************

void Ctext::draw()
{
	int l,i;

	l=strlen(text.c_str());

	for(CCoord * ptr = vertHead; ptr; ptr=ptr->nextCoord)
		{
		glColor3f(ptr->r,ptr->g,ptr->b);
		glRasterPos2i( ptr->x, ptr->y); // location to start printing text
		for( i=0; i < l; i++) // loop until i is greater then l
		{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text.c_str()[i]); // Print a character on the screen
		glutPostRedisplay();
		}
	}
}

// ********************* CArc ********************

void CArc::draw()
{
	int x = vertTail->x - vertHead->x;
    int y = vertTail->y - vertHead->y;
    radius = sqrt( (double)(x * x + y * y));
    //printf(" drag %f %f \n", x, y);
    
	float X1 = vertHead->x +radius;
	float Y1 = vertHead->y;
	if(shapeFill) glBegin(GL_TRIANGLE_FAN);
	else 
	{
		glLineWidth(lWidth);
		glBegin(GL_LINE_STRIP);	
	}

	if(angle < 0)
	{
		std::cout<<"Negative Angle:"<<angle<<"\n";
		for(float sAngle=0.0f;sAngle>=angle;sAngle-=0.01f)
			{
			glColor3f(vertHead->r,vertHead->g,vertHead->b);
			glVertex2f(X1, Y1);
			X1 = vertHead->x +(radius * (float)cos((double)sAngle));
			Y1 = vertHead->y +(radius * (float)sin((double)sAngle));
			}
	}
	else
	{	
		std::cout<<"Postive Angle: "<<angle<<"\n";
	for(float sAngle=0.0f;sAngle<=angle;sAngle+=0.01f)
	{
			glColor3f(vertHead->r,vertHead->g,vertHead->b);
			glVertex2f(X1, Y1);
			X1 = vertHead->x +(radius * (float)cos((double)sAngle));
			Y1 = vertHead->y +(radius * (float)sin((double)sAngle));
	}
	}
	
	glEnd();
	//glFlush();
}

void CArc::mouseDrag(int x, int y)
{
	double result;
	double resultR;

	vertTail->x = x;
	vertTail->y = y;
	result = atan2 (vertTail->y - vertHead->y,vertTail->x - vertHead->x) * (180/ PI);
		
	resultR = result * (PI/180);
	angle = resultR;

	glutPostRedisplay();
}

// ********************* CButton ********************

int CButton::selectedBtn = CNONE;

void CButton::draw()
{
	//printf(" %d = %d \n", btnType, selectedBtn);
	if(btnType != selectedBtn)
		glColor3f(0.0f, 0.6f, 0.6f);
	else
	{
		//printf("match \n");
		glColor3f(1.0f, 0.2f, 0.0f);
	}

	glBegin(GL_POLYGON);
		glVertex2i(x,y);
		glVertex2i(x,y-h);
		glVertex2i(x+w,y-h);
		glVertex2i(x+w,y);
	glEnd();
	glColor3f(0.0f,0.0f,0.0f);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
		glVertex2i(x,y);
		glVertex2i(x,y-h);
		glVertex2i(x+w,y-h);
		glVertex2i(x+w,y);
	glEnd();
	glFlush();

}

bool CButton::isPicked(int px, int py, int pz)
{
	if((px >= x && px <= x+w) && (py <= y && py >= y-h))
	{
		//printf("match %d \n", btnType);
		selectedBtn = btnType;
		return true;
	}
	else
	{
		return false;
	}
}

// ********************* CSlider ********************

void CSlider::draw()
{

	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2i(x,y);
		glVertex2i(x,y-h);
		glVertex2i(x+w,y-h);
		glVertex2i(x+w,y);
	glEnd();


}

bool CSlider::isPicked(int px, int py, int pz)
{
	if((px >= x && px <= x+w) && (py <= y && py >= y-h))
	{
		printf("match %d \n", sliderType);
		//selectedBtn = btnType;
		return true;
	}
	else
	{
		return false;
	}
}

