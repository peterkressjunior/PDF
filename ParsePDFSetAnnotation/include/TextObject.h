/*
 * TextObject.h
 *
 *  Created on: 15.07.2011
 *      Author: Peter Kress
 */

#ifndef TEXTOBJECT_H_
#define TEXTOBJECT_H_

#include <string>
using namespace std;

class TextObject {
public:
	TextObject();
	TextObject(unsigned int pageNumber, double k_x, double k_y, string textString);
	virtual ~TextObject();
	void setPageNumber(unsigned int pageNumber);
	void setPosX(double posX);
	void setPosY(double posY);
	void setTextString(string textString);
	unsigned int getPageNumber();
	double getPosX();
	double getPosY();
	string getTextString();
private:
	int pageNumber;
	double posX;
	double posY;
	string textString;

};

#endif /* TEXTOBJECT_H_ */
