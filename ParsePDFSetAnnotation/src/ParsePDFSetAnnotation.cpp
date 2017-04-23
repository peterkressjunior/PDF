//============================================================================
// Name        : ParsePDFSetAnnotation.cpp
// Author      : Peter Kress
// Version     :
// Copyright   : Parse PDF text object and set an annotation
// Description : PDF Parser in C++, Ansi-style
//============================================================================

/*
 * Include the standard headers for cout to write
 * some output to the console.
 */
#include <iostream>
#include <string>
#include <stack>
#include "TestTokenizer.h"
#include <vector>

/*
 * Now include all podofo header files, to have access
 * to all functions of podofo and so that you do not have
 * to care about the order of includes.
 *
 * You should always use podofo.h and not try to include
 * the required headers on your own.
 */
#include "podofo.h"

/*
 * All podofo classes are member of the PoDoFo namespace.
 */
using namespace PoDoFo;

void PrintHelp()
{
    std::cout << "This is a example application for the PoDoFo PDF library." << std::endl
              << "It creates a small PDF file containing the text >Hello World!<" << std::endl
              << "Please see http://podofo.sf.net for more information" << std::endl << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  examplehelloworld [outputfile.pdf]" << std::endl << std::endl;
}

std::string c_strToString(const std::string &s)
{
    return s;
}

void PDFTextContent( const char* pszFilename )
{
    try {
        PdfMemDocument document(pszFilename);

        EPdfVersion pdfVersion = document.GetPdfVersion();

        std::cout << "document.GetPdfVersion(): " << pdfVersion << std::endl;

        bool isEncrypted = document.GetEncrypted();

        std::cout << "document.GetEncrypted(): " << isEncrypted << std::endl;

        // Objects

        const PdfVecObjects& vecObjects = document.GetObjects();

        size_t objectCount = vecObjects.GetObjectCount();

        std::cout << "document.GetObjectCount(): " << objectCount << std::endl;

        int pageCount = document.GetPageCount();

        std::cout << "document.GetPageCount(): " << pageCount << std::endl;

        for (int i=0; i < pageCount; i++){
            PdfPage *pdfPage = document.GetPage(i);

            PdfContentsTokenizer *pdfContentsTokenizer = new PdfContentsTokenizer(pdfPage);


            const char* charVar;
            PdfVariant pdfVariant;
            EPdfContentsType pdfContentsType;

            std::stack<PdfVariant> myStack;
            while(pdfContentsTokenizer->ReadNext(pdfContentsType, charVar, pdfVariant)){
                if(pdfContentsType == ePdfContentsType_Variant){
                   if (pdfVariant.IsArray()/*IsHexString()*/){

                        std::cout << "\npdfContentsType: " << pdfContentsType << std::endl;
                        std::cout << "charVar: " << charVar << std::endl;
                        std::cout << "PdfVariant: " << pdfVariant.GetDataTypeString() << "\n" << std::endl;
                        //const PdfString &pdfString = pdfVariant.GetString();
                        //std::cout << "String: " << pdfString.GetString() << std::endl;
                        const PdfArray & pdfArray = pdfVariant.GetArray();
                        std::cout << "Array: " << std::endl;
                        for (unsigned int j=0; j< pdfArray.GetSize(); j++)
                        if (pdfArray[j].IsString()){
                            std::cout << pdfArray[j].GetString().GetString();
                        }
                        std::cout << "\n" << std::endl;
                        if (!myStack.empty() && myStack.top().IsReal()){
                            std::cout << "y: " <<  myStack.top().GetReal() << std::endl;
                            myStack.pop();
                        }
                        if (!myStack.empty() && myStack.top().IsReal()){
                            std::cout << "x: " <<  myStack.top().GetReal() << std::endl;
                            myStack.pop();
                        }
                        while ( ! myStack.empty() ){ myStack.pop(); } //leere den Stappel
                        std::cout << "\n------------" << std::endl;
                    }
                    myStack.push( pdfVariant );
                }
            }
        }

    } catch ( const PdfError & e ) {
        e.PrintErrorMsg();
    }
}

int main( int argc, char* argv[] )
{
    /*
     * Check if a filename was passed as commandline argument.
     * If more than 1 argument or no argument is passed,
     * a help message is displayed and the example application
     * will quit.
     */
    if( argc != 2 )
    {
        PrintHelp();
        return -1;
    }

    /*
     * All podofo functions will throw an exception in case of an error.
     *
     * You should catch the exception to either fix it or report
     * back to the user.
     *
     * All exceptions podofo throws are objects of the class PdfError.
     * Thats why we simply catch PdfError objects.
     */
    try {
        /*
         * Call the drawing routing which will create a PDF file
         * with the filename of the output file as argument.
         */
    	//PDFTextContent( argv[1] );
    	TestTokenizer testTokenizer = TestTokenizer( argv[1] );
    	testTokenizer.parsePDF();
    	vector<TextObject> textObjects = testTokenizer.findTextObject("402");
    	cout << "Anzahl der Funde="<< textObjects.size() << endl;  // gezählt wird nur der erste Fund pro "Zeile", also pro String
    	for (unsigned int i=0; i<textObjects.size(); i++){
    		cout << " \nPosition: pagenumber=" << textObjects.at(i).getPageNumber();
    		cout << " x=" << textObjects.at(i).getPosX();
    		cout << " y=" << textObjects.at(i).getPosY() << endl;
    	}
    	PdfMemDocument document( argv[1] );
    	PdfPainter painter;
    	PdfPage *pdfPage;
    	pdfPage = document.GetPage(textObjects.at(0).getPageNumber()-1); // technische seitenzahl stimmt nicht mit der natürlichen überein.
    	painter.SetPage(pdfPage);
		PdfAnnotation * pAnn = pdfPage->CreateAnnotation(ePdfAnnotation_FileAttachement,
				PdfRect(textObjects.at(0).getPosX(),textObjects.at(0).getPosY(),10,12)
		);
		PdfFileSpec filespec2("Blatt.pdf", true, &document);
		pAnn->SetFileAttachement(filespec2);
		painter.FinishPage();
		document.Write( strcat(argv[1],"_neu.pdf"));
    } catch( const PdfError & eCode ) {
        /*
         * We have to check if an error has occurred.
         * If yes, we return and print an error message
         * to the commandline.
         */
        eCode.PrintErrorMsg();
        return eCode.GetError();
    }
    return 0;
}
