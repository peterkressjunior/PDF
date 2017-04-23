/*
 * TestTokenizer.h
 *
 *  Created on: 13.07.2011
 *      Author: Peter Kress
 */
#ifndef TESTTOKENIZER_H_
#define TESTTOKENIZER_H_

#include <stack>
#include "podofo.h"
#include "TextObject.h"
#include <string>
#include <vector>

using namespace std;
using namespace PoDoFo;

class TestTokenizer {
public:
	TestTokenizer(const char* pszFilename);
	virtual ~TestTokenizer();
	void parsePDF();
	vector<TextObject> findTextObject(string searchString);
protected:
	void handleBT();
	void handleET();
	void handleTf();
	void handleTd();
	void handleTD();
	void handleTm();
	void handleTAsterisk();
	void handleTL();
	void handleTJ();
	void handleTj();
	void handleApostroph();
	void handleQuotation();
	double calculateNewE(double k_e, double k_a, double k_b, double tx, double ty);
	double calculateNewF(double k_f, double k_c, double k_d, double tx, double ty);
private:
	const char* pszFilename;
	stack<PdfVariant> pdfVariants;
	vector<TextObject> pdfTextObjects;
	double k_a;
	double k_b;
	double k_c;
	double k_d;
	double k_e;
	double k_f;
	double t_x;
	double t_y;
	double TL;
	PdfArray array;
	PdfString pdfString;
	bool BT;
	bool ET;
	double fontSize;
	PdfName fontName;
	string textString;
};

#endif /* TESTTOKENIZER_H_ */
