#ifndef CIMAGE
#define CIMAGE


#include "CShape.h"
#include <iostream>
#include <fstream>

class CImage
{
	CShape * head, *tail, *curr, *prev;

	public:
		CImage(){head = tail = curr = prev = NULL;}
		~CImage(){ clear();}

		void insertFigure(CShape * newShape)
		{
			if(!head)
				head = newShape;
			else
			{
				tail->nextShape = newShape;
			}
			tail = newShape;
			printf(" new pic in list \n");
		}

		void draw()
		{
			curr = head;
			while(curr)
			{
				curr->draw();
				curr = curr->nextShape;
			}
		}

		CShape * isPicked(int px, int py)
		{
			CShape * rtn = NULL;

			curr = head;
			while(curr)
			{

				if (curr->isPicked(px, py))
				{
					printf(" yes 2 %d\n", curr->getType());
					return curr;
				}
				curr = curr->nextShape;
			}
			return rtn;
		}

		void clear()
		{
			printf("In the clear\n");
			while(curr = head)
			{head = head->nextShape; delete curr;}
			printf("deleted pic\n");
		}

		//load last saved image
		void load(char * filename, CImage * pic)
		{
			CImage image;
			CShape * currShape = CNONE;
			CCoord * currPtr;
			int type, nbrVerts, x, y, z = 0;
			GLfloat pointSize, lineWidth;
			std::string txt;
			float r, g, b, ang;
			bool shapef;
			std::ifstream is;
			is.open(filename);

			//while (!is.eof())
			while(is)
			{
				is >> type;
				std::cout<<type<<"\n";
				if(type == CPOINT)
				{
					std::cout<<"this is a point\n";
					currShape = new CPoint();
					is >> nbrVerts;
					std::cout<< nbrVerts<<"\n";
					is >> pointSize;
					std::cout<< pointSize<<"\n";
					currShape->ptSize = pointSize;
					is >> x >> y >> z >> r >> g >> b;
					std::cout<< x <<" " << y << " "<< z<< "\n";
					currPtr = new CCoord(x, y, z, r, g, b, 1.0f);
					std::cout<<currPtr->x<<"\n";
					currShape->insertVertex(currPtr);
					currShape->nbrVert = nbrVerts;
					pic->insertFigure(currShape);
					currShape  = CNONE;
					std::cout<< currShape<<"\n";
				}
				else if(type == CLINE)
				{
					std::cout<<"this is a line\n";
					currShape = new CLine();
					is >> nbrVerts;
					is >> lineWidth;
					currShape->lWidth = lineWidth;
					for(int i = 0; i < nbrVerts; i++)
					{
						is >> x >> y >> z >> r >> g >> b;
						currPtr = new CCoord(x, y, z, r, g, b, 1.0f);
						currShape->insertVertex(currPtr);
					}
					currShape->nbrVert = nbrVerts;
					pic->insertFigure(currShape);
					currShape  = CNONE;
				}
				else if(type == CLINESTR)
				{
					currShape = new CLineStr();
					is >> nbrVerts;
					is >> lineWidth;
					currShape->lWidth = lineWidth;
					for(int i = 0; i < nbrVerts; i++)
					{
						is >> x >> y >> z >> r >> g >> b;
						currPtr = new CCoord(x, y, z, r, g, b, 1.0f);
						currShape->insertVertex(currPtr);
					}
					currShape->nbrVert = nbrVerts;
					pic->insertFigure(currShape);
					currShape  = CNONE;
				}
				else if(type == CCIRCLE)
				{
					currShape = new CCircle();
					is >> nbrVerts;
					is >> lineWidth;
					currShape->lWidth = lineWidth;
					is >> shapef;
					currShape->shapeFill = shapef;
					for(int i = 0; i < nbrVerts; i++)
					{
						is >> x >> y >> z >> r >> g >> b;
						currPtr = new CCoord(x, y, z, r, g, b, 1.0f);
						currShape->insertVertex(currPtr);
					}
					currShape->nbrVert = nbrVerts;
					pic->insertFigure(currShape);
					currShape  = CNONE;
				}
				else if(type == CRECT)
				{
					currShape = new CRect();
					is >> nbrVerts;
					is >> lineWidth;
					currShape->lWidth = lineWidth;
					is >> shapef;
					currShape->shapeFill = shapef;
					for(int i = 0; i < nbrVerts; i++)
					{
						is >> x >> y >> z >> r >> g >> b;
						currPtr = new CCoord(x, y, z, r, g, b, 1.0f);
						currShape->insertVertex(currPtr);
					}
					currShape->nbrVert = nbrVerts;
					pic->insertFigure(currShape);
					currShape  = CNONE;
				}
				else if(type == CARC)
				{
					currShape = new CArc();
					is >> nbrVerts;
					is >> lineWidth;
					currShape->lWidth = lineWidth;
					is >> shapef;
					currShape->shapeFill = shapef;
					is >> ang;
					currShape->angle = ang;
					for(int i = 0; i < nbrVerts; i++)
					{
						is >> x >> y >> z >> r >> g >> b;
						currPtr = new CCoord(x, y, z, r, g, b, 1.0f);
						currShape->insertVertex(currPtr);
					}
					currShape->nbrVert = nbrVerts;
					pic->insertFigure(currShape);
					currShape  = CNONE;
				}
				else if(type == CTRI)
				{
					currShape = new CTri();
					is >> nbrVerts;
					is >> lineWidth;
					currShape->lWidth = lineWidth;
					is >> shapef;
					currShape->shapeFill = shapef;
					for(int i = 0; i < nbrVerts; i++)
					{
						is >> x >> y >> z >> r >> g >> b;
						currPtr = new CCoord(x, y, z, r, g, b, 1.0f);
						currShape->insertVertex(currPtr);
					}
					currShape->nbrVert = nbrVerts;
					pic->insertFigure(currShape);
					currShape  = CNONE;
				}
				else if(type == CPOLY)
				{
					currShape = new CPoly();
					is >> nbrVerts;
					is >> lineWidth;
					currShape->lWidth = lineWidth;
					is >> shapef;
					currShape->shapeFill = shapef;
					for(int i = 0; i < nbrVerts; i++)
					{
						is >> x >> y >> z >> r >> g >> b;
						currPtr = new CCoord(x, y, z, r, g, b, 1.0f);
						currShape->insertVertex(currPtr);
					}
					currShape->nbrVert = nbrVerts;
					pic->insertFigure(currShape);
					currShape  = CNONE;
				}
				else if(type == CTEXT)
				{
					currShape = new Ctext();
					is >> nbrVerts;
					is >> x >> y >> z >> r >> g >> b;
					currPtr = new CCoord(x, y, z, r, g, b, 1.0f);
					currShape->insertVertex(currPtr);
					currShape->nbrVert = nbrVerts;
					std::getline(is,txt);
					currShape->text = txt;
					pic->insertFigure(currShape);
					currShape  = CNONE;
				}
			}
			is.close();
			std::cout<<"Stream is closed\n";
		}

		//save current image
		void save(char * filename)
		{
			int sType;
			std::ofstream myFile;
			myFile.open(filename);
			

			curr = head;
			while(curr)
			{
				sType = curr->getType();
				if(sType == CPOINT)
				{
					myFile << sType <<" ";
					std::cout<<"type is "<< sType;
					myFile << curr->nbrVert<<" ";
					myFile << curr->ptSize<<" ";
					std::cout<<" Here is x " <<curr->vertHead->x<<"\n";
					myFile << curr->vertHead->x<<" "<<curr->vertHead->y<<" "<<curr->vertHead->z<<" "<<curr->vertHead->r<<" "
						<<curr->vertHead->g<<" "<<curr->vertHead->b<<" ";
				}
				else if(sType == CLINE || sType == CLINESTR)
				{
					myFile << sType <<" ";
					myFile << curr->nbrVert<<" ";
					myFile << curr->lWidth<<" ";
					for(CCoord * ptr = curr->vertHead; ptr; ptr=ptr->nextCoord)
					{
						myFile << ptr->x<<" "<<ptr->y<<" "<<ptr->z<<" "<<ptr->r<<" "
						<<ptr->g<<" "<<ptr->b<<" ";
					}
				}
				else if(sType == CCIRCLE)
				{
					myFile << sType <<" ";
					myFile << curr->nbrVert<<" ";
					myFile << curr->lWidth<<" ";
					myFile << curr->shapeFill<<" ";
					for(CCoord * ptr = curr->vertHead; ptr; ptr=ptr->nextCoord)
					{
						myFile << ptr->x<<" "<<ptr->y<<" "<<ptr->z<<" "<<ptr->r<<" "
						<<ptr->g<<" "<<ptr->b<<" ";
					}
				}
				else if(sType == CRECT)
				{
					myFile << sType <<" ";
					myFile << curr->nbrVert<<" ";
					myFile << curr->lWidth<<" ";
					myFile << curr->shapeFill<<" ";
					for(CCoord * ptr = curr->vertHead; ptr; ptr=ptr->nextCoord)
					{
						myFile << ptr->x<<" "<<ptr->y<<" "<<ptr->z<<" "<<ptr->r<<" "
						<<ptr->g<<" "<<ptr->b<<" ";
					}
				}
				else if(sType == CARC)
				{
					myFile << sType <<" ";
					myFile << curr->nbrVert<<" ";
					myFile << curr->lWidth<<" ";
					myFile << curr->shapeFill<<" ";
					myFile << curr->angle<<" ";
					for(CCoord * ptr = curr->vertHead; ptr; ptr=ptr->nextCoord)
					{
						myFile << ptr->x<<" "<<ptr->y<<" "<<ptr->z<<" "<<ptr->r<<" "
						<<ptr->g<<" "<<ptr->b<<" ";
					}
				}
				else if(sType == CTRI)
				{
					myFile << sType <<" ";
					myFile << curr->nbrVert<<" ";
					myFile << curr->lWidth<<" ";
					myFile << curr->shapeFill<<" ";
					for(CCoord * ptr = curr->vertHead; ptr; ptr=ptr->nextCoord)
					{
						myFile << ptr->x<<" "<<ptr->y<<" "<<ptr->z<<" "<<ptr->r<<" "
						<<ptr->g<<" "<<ptr->b<<" ";
					}
				}
				else if(sType == CPOLY)
				{
					myFile << sType <<" ";
					myFile << curr->nbrVert<<" ";
					myFile << curr->lWidth<<" ";
					myFile << curr->shapeFill<<" ";
					for(CCoord * ptr = curr->vertHead; ptr; ptr=ptr->nextCoord)
					{
						myFile << ptr->x<<" "<<ptr->y<<" "<<ptr->z<<" "<<ptr->r<<" "
						<<ptr->g<<" "<<ptr->b<<" ";
					}
				}
				else if(sType == CTEXT)
				{
					myFile << sType <<" ";
					
					std::cout<<"type is "<< sType<<" ";
					myFile << curr->nbrVert<<" ";
					std::cout<<" Here is x " <<curr->vertHead->x<<"\n";
					myFile << curr->vertHead->x<<" "<<curr->vertHead->y<<" "<<curr->vertHead->z<<" "<<curr->vertHead->r<<" "
						<<curr->vertHead->g<<" "<<curr->vertHead->b<<" ";
					myFile << curr->text<<'\n';
				}

				curr = curr->nextShape;
			}
			myFile.close();
		}
};

#endif