// File CShape.h
// Author: S. Renk 10/07
// Shape classes that form a graphics library for library for a Paint program.

#ifndef _CShape
#define _CShape

//#include <windows.h>

#include <stdio.h>
#include <math.h>
#include <string>

// define constants for all shapes

#define CNONE 0
#define CPOINT 1
#define CLINE 2
#define CLINESTR 3
#define CCIRCLE 4
#define CRECT 5
#define CARC 6
#define CTEXT 7
#define CTRI 8
#define CPOLY 9

#define CLOAD 10
#define CSAVE 11
#define CSLIDER 12

#define CBUTTON 15


 

// ********************** CCord class ********************

class CCoord // basic vertex class

{ public:
      int x, y, z; // vertex cartesian coord (z=0 for 2D)
      float r,g,b,a; // vertex color (a=1.0f)
      CCoord * nextCoord; // ptr. to next vertex in a linked list
      CCoord(int nx=0, int ny=0, int nz=0, float nr=0, float ng=0, float nb=0, float na=0)
      { nextCoord = NULL; x=nx; y=ny; z=nz; r=nr; g=ng; b=nb;a=na;}
};

 

// ********************* CShape class ********************

class CShape // abstract base class for all Shapes

{ protected:
      int type; // 0- point, 1-line, ...
      bool good;
      //int nbrVert; // # vertices, if = 0 bad figure


public:
      CCoord * vertHead, * vertTail; // list of vertices in figure
      CShape * nextShape; // points to next shape in list
	  int nbrVert; // # vertices, if = 0 bad figure
	  bool shapeFill;
	  GLfloat ptSize;
	  GLfloat lWidth;
	  std::string text;
	  double angle;
      CShape(int ntype){ vertHead=vertTail=NULL; nextShape=NULL; nbrVert=0; type=ntype; good=false; shapeFill=false;
	  ptSize =0;lWidth=0;text="";angle=0;}
      ~CShape(); // delete linked list of verts
	  int getType(){return type;}
      virtual void draw() = 0; //Draws the figure
      virtual bool isPicked(int x, int y, int z=0) = 0; // true if (x,y,z) is inside figure
      virtual bool insertVertex(CCoord * vert)=0; // place in vertex list
	  virtual void mouseDrag(int x, int y) = 0; // mouse drag for rubberbanding
      //virtual void read() = 0; // read object from file -- I'd actually overload << & >>
      //virtual void write() = 0; // write object to file
};

 extern CShape * Sptr;

// ******************** CPoint class ********************

class CPoint : public CShape // A list of points
{
//private:
	//float size;			//size of all pts in list
public:
      CPoint():CShape(CPOINT){} // pass type to base
      void draw(); // draw a set of points
      bool isPicked(int x, int y, int z=0){return false;}
      bool insertVertex(CCoord * vertex){ CShape::insertVertex(vertex); return good = true;}
      void mouseDrag(int x, int y){} // do nothing
	  //void read();
	  //void write();
};

 

// ******************** CLine class *********************

class CLine : public CShape // A list of lines. Every pair is a line.
{
public:
      CLine():CShape(CLINE){} // pass type to base
      void draw();            // draw a set of lines
      bool isPicked(int x, int y, int z=0){return false;}
      bool insertVertex(CCoord * vertex)
      {     CShape::insertVertex(vertex);
            return good = (nbrVert > 1);
      }
      void mouseDrag(int x, int y); //mouse drag function
	  //void read();
	  //void write();
};

// ******************** CLineStr class *********************
class CLineStr : public CShape
{
public:
      CLineStr():CShape(CLINESTR){} // pass type to base
      void draw();            // draw a set of lines
      bool isPicked(int x, int y, int z=0){return false;}
      bool insertVertex(CCoord * vertex)
      {     CShape::insertVertex(vertex);
            return good = (nbrVert > 1);
      }
      void mouseDrag(int x, int y); //mouse drag function
	  //void read();
	  //void write();

};

// ******************* CCircle class ********************

class CCircle : public CShape // A single circle
{
protected:
	double radius;

public:
      CCircle():CShape(CCIRCLE){radius = 10;} // pass type to base
      void draw(); // draw a set of lines
      bool isPicked(int x, int y, int z=0){return false;}
      bool insertVertex(CCoord * vertex)
      {     CShape::insertVertex(vertex);
            return good = (nbrVert > 1);
      }
	  void mouseDrag(int x, int y); //mouse drag function
	  //void read();
	  //void write();
};

 

// ***************** CRect class ***********************

class CRect: public CShape
{
	public:
		CRect():CShape(CRECT){}
		void draw();
		bool isPicked(int x, int y, int z=0){return false;}
		bool insertVertex(CCoord * vertex){CShape::insertVertex(vertex); return good = (nbrVert > 1);}
		void mouseDrag(int x, int y);
		//void read();
		//void write();
};



// ***************** CPoly class ***********************

class CPoly: public CShape
{
	public:
      CPoly():CShape(CPOLY){} // pass type to base
      void draw();            // draw a set of lines
      bool isPicked(int x, int y, int z=0){return false;}
      bool insertVertex(CCoord * vertex)
      {     CShape::insertVertex(vertex);
            return good = (nbrVert > 1);
      }
	  void mouseDrag(int x, int y);//mouse drag function
	  //void read();
	  //void write();

};

// ***************** CArc class ***********************

class CArc: public CShape
{
protected:
	double radius;

public:
      CArc():CShape(CARC){radius = 10;} // pass type to base
      void draw(); // draw a set of lines
      bool isPicked(int x, int y, int z=0){return false;}
      bool insertVertex(CCoord * vertex)
      {     CShape::insertVertex(vertex);
            return good = (nbrVert > 1);
      }
	  void mouseDrag(int x, int y); //mouse drag function
	  //void read();
	  //void write();

};

// ***************** CTri class ***********************

class CTri: public CShape
{
	public:
	  CTri():CShape(CTRI){} // pass type to base
      void draw();            // draw a set of lines
      bool isPicked(int x, int y, int z=0){return false;}
      bool insertVertex(CCoord * vertex)
      {     CShape::insertVertex(vertex);
            return good = (nbrVert > 1);
      }
	  void mouseDrag(int x, int y); //mouse drag function
	  //void read();
	  //void write();

};

// ***************** CText class ***********************

class Ctext: public CShape
{
public:
      Ctext():CShape(CTEXT){} // pass type to base
      void draw(); // draw a set of points
      bool isPicked(int x, int y, int z=0){return false;}
      bool insertVertex(CCoord * vertex){ CShape::insertVertex(vertex); return good = true;}
      void mouseDrag(int x, int y){} // do nothing
	  //void read();
	  //void write();
};

// ***************** CButton class ***********************

//#define BTN_HEIGHT 40
//#define BTN_WIDTH  100

class CButton: public CShape
{
	int btnType, x, y, w, h;
	static int selectedBtn;

	public:
		CButton(int nx, int ny, int btn, int nw, int nh):CShape(CBUTTON)
			{x=nx, y=ny; btnType=btn; w = nw; h = nh;}
		int getBtnType(){return btnType;}
		void draw();
		bool isPicked(int x, int y, int z=0);
		bool insertVertex(CCoord * vertex){CShape::insertVertex(vertex); return good = (nbrVert >1);}
		void mouseDrag(int x, int y){}
		//void read(){}
		//void write(){}
};

// ***************** CSlider class ***********************

class CSlider: public CShape
{
	//int sliderType, x, y, w, h, min, max, cPos;
	//static int selectedBtn;

	public:
		int sliderType, x, y, w, h, min, max, cPos;
		CSlider(int nx, int ny, int sType, int nw, int nh, int nMin, int nMax, int ncPos):CShape(CSLIDER)
			{x=nx, y=ny; sliderType=sType; w = nw; h = nh; min = nMin; max = nMax; cPos = ncPos;}
		int getSliderType(){return sliderType;}
		void draw();
		bool isPicked(int x, int y, int z=0);
		bool insertVertex(CCoord * vertex){CShape::insertVertex(vertex); return good = (nbrVert >1);}
		void mouseDrag(int x, int y){}
		//void read(){}
		//void write(){}
};

#endif
