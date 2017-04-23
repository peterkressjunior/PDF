/*
 * TextObject.cpp
 *
 *  Created on: 15.07.2011
 *      Author: Peter Kress
 */

#include "TextObject.h"
#include <string>

using namespace std;

TextObject::TextObject() {
	this->pageNumber = 0;
	this->posX = 0;
	this->posY = 0;
	this->textString = "";
}

TextObject::TextObject(unsigned int pageNumber, double k_x, double k_y, string textString) {
	this->pageNumber = pageNumber;
	this->posX = k_x;
	this->posY = k_y;
	this->textString = textString;
}

TextObject::~TextObject() {
	this->pageNumber = 0;
	this->posX = 0;
	this->posY = 0;
	this->textString = "";
}

void TextObject::setPosX(double posX) {
	this->posX = posX;
}
void TextObject::setPosY(double posY) {
	this->posY = posY;
}
void TextObject::setTextString(string textString) {
	this->textString = textString;
}

void TextObject::setPageNumber(unsigned int pageNumber){
	this->pageNumber = pageNumber;
}

unsigned int TextObject::getPageNumber(){
	return this->pageNumber;
}

double TextObject::getPosX() {
	return this->posX;
}
double TextObject::getPosY() {
	return this->posY;
}
string TextObject::getTextString() {
	return this->textString;
}
