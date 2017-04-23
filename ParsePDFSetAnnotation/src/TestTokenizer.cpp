/*
 * TestTokenizer.cpp
 *
 *  Created on: 13.07.2011
 *      Author: Peter Kress
 */

#include "TestTokenizer.h"
#include "podofo.h"
#include <iostream>
#include <string>
#include <stack>
#include "TextObject.h"
#include <vector>

using namespace PoDoFo;

TestTokenizer::TestTokenizer(const char* pszFilename) {
	this->pszFilename = pszFilename;
}

TestTokenizer::~TestTokenizer() {
	//
}

void TestTokenizer::parsePDF(){
	 try {
		        PdfMemDocument document(pszFilename);
		        int pageCount = document.GetPageCount();
		        PdfPage *pdfPage;
		        PdfContentsTokenizer *pdfContentsTokenizer;

	            const char* charVar;
	            PdfVariant pdfVariant;
	            EPdfContentsType pdfContentsType;

	            document.GetFontLibrary();

	            for (int i=0; i < pageCount; i++){
		        	pdfPage = document.GetPage(i);
		            pdfContentsTokenizer = new PdfContentsTokenizer(pdfPage);
		            while(pdfContentsTokenizer->ReadNext(pdfContentsType, charVar, pdfVariant)){
		            	pdfVariants.push(pdfVariant);
		            	if (!pdfVariants.empty() && strcmp(charVar, "BT")==0){
		            		handleBT();
		            	}
		            	if (this->BT && !pdfVariants.empty() && pdfContentsType == ePdfContentsType_Keyword){ // 0=Parameter und 1=Variant(???)
		            		if(strcmp(charVar, "Tf")==0){
		            			handleTf();
		            		}
		            		if (strcmp(charVar, "Td")==0){
		            			handleTd();
		            		}
		            		if (strcmp(charVar, "Tm")==0){
		            			handleTm();
		            		}
		            		if (strcmp(charVar, "TL")==0){
		            			handleTL();
		            		}
		            		if (strcmp(charVar, "T*")==0){
		            			handleTAsterisk();
		            		}
		            		// show content operators
		            		if(strcmp(charVar, "Tj")== 0){
		            			handleTj();
		            			pdfTextObjects.push_back(
		            				TextObject(pdfPage->GetPageNumber(), this->k_e,this->k_f,this->textString)
		            			);
		            		}
							if (strcmp(charVar, "TJ")==0){
								handleTJ();
								pdfTextObjects.push_back(
									TextObject(pdfPage->GetPageNumber(), this->k_e,this->k_f,this->textString)
								);
							}
							if (strcmp(charVar, "'")==0){
								handleApostroph();
								pdfTextObjects.push_back(
									TextObject(pdfPage->GetPageNumber(), this->k_e,this->k_f,this->textString)
								);
							}
							if (strcmp(charVar, "\"")==0){
								handleQuotation();
								pdfTextObjects.push_back(
									TextObject(pdfPage->GetPageNumber(), this->k_e,this->k_f,this->textString)
								);
							}
		            	}
		            	if (!pdfVariants.empty() && strcmp(charVar, "ET")==0){
		            		handleET();
		            	}
		            }
		        }
		    } catch ( const PdfError & e ) {
		        e.PrintErrorMsg();
		    }
}

void TestTokenizer::handleBT(){
	this->ET = false;
	this->BT = true;
	k_a = 0;
	k_b = 0;
	k_c = 0;
	k_d = 0;
	k_e = 0;
	k_f = 0;
	t_x = 0;
	t_y = 0;
	TL = 0;
	fontSize = 0;
	std::cout << "BT" << std::endl;
}

void TestTokenizer::handleTf(){
	if (pdfVariants.top().IsNumber()){
		this->fontSize = pdfVariants.top().GetNumber();
	}
	if (pdfVariants.top().IsReal()){
		this->fontSize = pdfVariants.top().GetNumber();
	}
	pdfVariants.pop();
	if (pdfVariants.top().IsName()){
		this->fontName = pdfVariants.top().GetName();
	}else{
		pdfVariants.pop();
		if (pdfVariants.top().IsName()){
			this->fontName = pdfVariants.top().GetName();
		}
	}
	std::cout << "/" << fontName.GetName() << " " << fontSize << " " << "Tf" << std::endl;
}

void TestTokenizer::handleET(){
	if (!BT){
		std::cout << "ERROR: Text object is ending without a begin!";
	}
	this->ET = true;
	this->BT = false;
	std::cout << "ET" << std::endl;
}

/**
 * ACHTUNG: funktioniert nur richtig, falls 'pdfContentsType=0'
 * vor dem Aufruf dieser Methode muss sichergestellt werden, dass
 * 'pdfContentsType=0' gefilltert wird.
 *  Der Token muss also ein 'keyword' sein und kein 'variant' im Sinne von PoDoFo/PDF
 */
void TestTokenizer::handleTd(){
	pdfVariants.pop();
	if (pdfVariants.top().IsReal()){
		this->t_y = pdfVariants.top().GetReal();
	}
	if (pdfVariants.top().IsNumber()){
		this->t_y = pdfVariants.top().GetNumber();
	}
	pdfVariants.pop();
	if (pdfVariants.top().IsReal()){
		this->t_x = pdfVariants.top().GetReal();
	}
	if (pdfVariants.top().IsNumber()){
		this->t_x = pdfVariants.top().GetNumber();
	}
	std::cout << t_x << " " << t_y << " " << "Td" << " ";
	std::cout << "\t\t\t--> calculated Tm: ";
	k_e = calculateNewE(k_e, k_a, k_b, t_x, t_y);
	k_f = calculateNewF(k_f, k_c, k_d, t_x, t_y);
	std::cout << k_a << " " << k_b << " " << k_c << " " << k_d << " " << k_e << " " << k_f << " " << "Tm" << std::endl;
}

void TestTokenizer::handleTD(){
	pdfVariants.pop();
	if (pdfVariants.top().IsReal()){
		this->t_y = pdfVariants.top().GetReal();
	}
	if (pdfVariants.top().IsNumber()){
		this->t_y = pdfVariants.top().GetNumber();
	}

	this->TL = (-1)*t_y;  //!! Side effect! BITCHES!

	pdfVariants.pop();
	if (pdfVariants.top().IsReal()){
		this->t_x = pdfVariants.top().GetReal();
	}
	if (pdfVariants.top().IsNumber()){
		this->t_x = pdfVariants.top().GetNumber();
	}
	std::cout << t_x << " " << t_y << " " << "TD" << " ";
	std::cout << "\t\t\t--> calculated Tm: ";
	k_e = calculateNewE(k_e, k_a, k_b, t_x, t_y);
	k_f = calculateNewF(k_f, k_c, k_d, t_x, t_y);
	std::cout << k_a << " " << k_b << " " << k_c << " " << k_d << " " << k_e << " " << k_f << " " << "Tm" << std::endl;
}

void TestTokenizer::handleTm(){
	pdfVariants.pop();
	if (pdfVariants.top().IsReal()){
		this->k_f = pdfVariants.top().GetReal();
	}
	if (pdfVariants.top().IsNumber()){
		this->k_f = pdfVariants.top().GetNumber();
	}
	pdfVariants.pop();
	if (pdfVariants.top().IsReal()){
		this->k_e = pdfVariants.top().GetReal();
	}
	if (pdfVariants.top().IsNumber()){
		this->k_e = pdfVariants.top().GetNumber();
	}
	pdfVariants.pop();
	if (pdfVariants.top().IsReal()){
		this->k_d = pdfVariants.top().GetReal();
	}
	if (pdfVariants.top().IsNumber()){
		this->k_d = pdfVariants.top().GetNumber();
	}
	pdfVariants.pop();
	if (pdfVariants.top().IsReal()){
		this->k_c = pdfVariants.top().GetReal();
	}
	if (pdfVariants.top().IsNumber()){
		this->k_c = pdfVariants.top().GetNumber();
	}
	pdfVariants.pop();
	if (pdfVariants.top().IsReal()){
		this->k_b = pdfVariants.top().GetReal();
	}
	if (pdfVariants.top().IsNumber()){
		this->k_b = pdfVariants.top().GetNumber();
	}
	pdfVariants.pop();
	if (pdfVariants.top().IsReal()){
		this->k_a = pdfVariants.top().GetReal();
	}
	if (pdfVariants.top().IsNumber()){
		this->k_a = pdfVariants.top().GetNumber();
	}
	std::cout << k_a << " " << k_b << " " << k_c << " " << k_d << " " << k_e << " " << k_f << " " << "Tm" << std::endl;
}

void TestTokenizer::handleTAsterisk(){ //T*
	k_e = calculateNewF(k_e, k_a, k_b, 0, TL); //current leading
	k_f = calculateNewF(k_f, k_c, k_d, 0, TL); //current leading
	std::cout << "T*" << " ";
	std::cout << "\t\t\t--> calculated Tm: ";
	std::cout << k_a << " " << k_b << " " << k_c << " " << k_d << " " << k_e << " " << k_f << " " << "Tm" << std::endl;
}

void TestTokenizer::handleTL(){
	pdfVariants.pop();
	if (pdfVariants.top().IsNumber()){
		this->TL = pdfVariants.top().GetNumber();
	}
	if (pdfVariants.top().IsReal()){
		this->TL = pdfVariants.top().GetReal();
	}
	std::cout << TL << " " << "TL" << std::endl;
}

void TestTokenizer::handleTJ(){
	if (pdfVariants.top().IsArray()){
		this->array = pdfVariants.top().GetArray();
		textString = ""; // leere String
		for (unsigned int i=0; i<array.GetSize(); i++ ){
			if(array[i].GetDataType()==3){
				pdfString = array[i].GetString();
				textString = textString + pdfString.GetString();
			}
		}
	}
	std::cout << "[(" << textString << ")]" << " " << "TJ" << std::endl;
}

void TestTokenizer::handleTj(){
	if (pdfVariants.top().IsString()){
		this->pdfString = pdfVariants.top().GetString();
		this->textString = pdfString.GetString();
	}
	if (pdfVariants.top().IsHexString()){
		this->pdfString = pdfVariants.top().GetString();
		this->textString = pdfString.GetString();
	}
	std::cout << "(" << textString << ")" << " " << "Tj" << std::endl;
}

void TestTokenizer::handleApostroph(){
	k_f = calculateNewF(k_f, k_c, k_d, 0, (-1)*TL);
	if (pdfVariants.top().IsString()){
		this->pdfString = pdfVariants.top().GetString();
		this->textString = pdfString.GetString();
	}
	if (pdfVariants.top().IsHexString()){
		this->pdfString = pdfVariants.top().GetString();
		this->textString = pdfString.GetString();
	}
	std::cout << "(" << textString << ")" << " " << "'" << std::endl;
}

void TestTokenizer::handleQuotation(){
	k_f = calculateNewF(k_f, k_c, k_d, 0, (-1)*TL);
	if (pdfVariants.top().IsString()){
		this->pdfString = pdfVariants.top().GetString();
		this->textString = pdfString.GetString();
	}
	if (pdfVariants.top().IsHexString()){
		this->pdfString = pdfVariants.top().GetString();
		this->textString = pdfString.GetString();
	}
	std::cout << "(" << textString << ")" << " " << "\"" << std::endl;
}


double TestTokenizer::calculateNewE(double k_e, double k_a, double k_b, double tx, double ty){
	return k_e + k_a*tx - k_b*ty;
}

double TestTokenizer::calculateNewF(double k_f, double k_c, double k_d, double tx, double ty){
	return k_f - k_c*tx + k_d*ty;
}

vector<TextObject> TestTokenizer::findTextObject(string searchString){
	TextObject textObject;
	vector<TextObject> textObjects;
	for (unsigned int i=0; i<this->pdfTextObjects.size(); i++){
		textObject = this->pdfTextObjects.at(i);
		if(textObject.getTextString().find(searchString)!=std::string::npos){
			textObjects.push_back(textObject);
		}
    }
	return textObjects;
}
