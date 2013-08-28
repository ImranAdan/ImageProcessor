#include <math.h>
#include <cmath>
#include <stack>
#include <wx/wx.h>
#include "window.h"
#include <wx/image.h>
#include <wx/dcmemory.h>
#include <wx/dcclient.h>
#include <wx/dc.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <algorithm>
using namespace std;

static const wxChar *FILETYPES = _T("All files (*.*)|*.*");

stack<wxImage*> *imageStack = new stack<wxImage*>();


IMPLEMENT_APP (BasicApplication);


bool BasicApplication::OnInit() {
	wxInitAllImageHandlers();
	MyFrame *frame = new MyFrame(_("C++ In Image Processing"), 50, 50, 1000, 700);
	frame->Show(TRUE);
	SetTopWindow(frame);
	return TRUE;
}

//###########################################################//
//-----------------------------------------------------------//
//----------		 CONSTRUCTOR 	--------------------//
//-----------------------------------------------------------//
//###########################################################//

MyFrame::MyFrame(const wxString title, int xpos, int ypos, int width,
		int height) :
		wxFrame((wxFrame *) NULL, -1, title, wxPoint(xpos, ypos),
				wxSize(width, height)) {


	/****************************
	 CREATE A NEW FILE MENU AND SET PARAMETERS
	 ************************************/
	fileMenu = new wxMenu;
	fileMenu->Append(LOAD_FILE_ID, _T("&Open File\tCtrl+O"));
	//Lab 5
	fileMenu->Append(LOAD_RAW_FILE_ID, _T("&Open from RAW file"));
	fileMenu->AppendSeparator();
	fileMenu->Append(SAVE_IMAGE_ID, _T("&Save image"));
	fileMenu->AppendSeparator();
	fileMenu->Append(EXIT_ID, _T("E&xit "));

	/****************************
	 CREATE A NEW EDIT MENU AND SET PARAMETERS
	 ************************************/

	editMenu = new wxMenu;
	editMenu->Append(UNDO_ID, _T("&Undo\tCtrl+Z"));
	//editMenu->AppendSeparator(); 
	//editMenu->Append(INVERT_IMAGE_ID, _T("&Sample Invert Image"));
	//editMenu->Append(SCALE_IMAGE_ID, _T("&Sample Scale image"));
	editMenu->AppendSeparator();
	editMenu->Append(MY_IMAGE_ID, _T("&Print Pixel Values  - RGB"));
	//editMenu->Append(MYTEST_IMAGE_ID, _T("&Dummy Function  - RGB"));
	editMenu->AppendSeparator();
	editMenu->Append(MY_SCALE_IMAGE_ID, _T("&Scale Image  - RGB"));
	editMenu->Append(MY_SHIFT_IMAGE_ID, _T("&Shift Image  - RGB"));


	/****************************
	 IMAGE CONVOLUTION MENU
	 ************************************/

	 imageConvolutionMenu = new wxMenu;
	 imageConvolutionMenu->Append(MY_AVERAGING_CONVOLUTION_ID, _T("&Averaging  - RGB"));
	 imageConvolutionMenu->Append(MY_WEIGHTED_AVERAGING_CONVOLUTION_ID, _T("&Weighted Averaging  - RGB"));
	 imageConvolutionMenu->AppendSeparator();
	 imageConvolutionMenu->Append(MY_FOUR_NEIGHBOUR_LAPLACIAN_CONVOLUTION_ID, _T("&4-neighbour Laplacian  - RGB"));
	 imageConvolutionMenu->Append(MY_EIGHT_NEIGHBOUR_LAPLACIAN_CONVOLUTION, _T("&8-neighbour Laplacian  - RGB"));
	 imageConvolutionMenu->AppendSeparator();
	 imageConvolutionMenu->Append(MY_FOUR_NEIGHBOUR_LAPLACIAN_ENHANCMENT_CONVOLUTION_ID, _T("&4-neighbour Laplacian Enhancment  - RGB"));
	 imageConvolutionMenu->Append(MY_EIGHT_NEIGHBOUR_LAPLACIAN_ENHANCMENT_CONVOLUTION_ID, _T("&8-neighbour Laplacian Enhancment  - RGB"));
	 imageConvolutionMenu->AppendSeparator();
	 imageConvolutionMenu->Append(MY_ROBERTS_CONVOLUTION_ID, _T("&Roberts G = |G1| + |G2| - RGB"));
	 imageConvolutionMenu->AppendSeparator();
	 imageConvolutionMenu->Append(MY_SOBEL_CONVOLUTION_ID, _T("&Sobel (|X| + |Y|) - RGB"));
	 imageConvolutionMenu->Append(MY_SOBEL_X_CONVOLUTION_ID, _T("&Sobel |X| - RGB"));
	 imageConvolutionMenu->Append(MY_SOBEL_Y_CONVOLUTION_ID, _T("&Sobel |Y| - RGB"));


	/****************************
	 IMAGE FILTERING MENU
	 ************************************/

	 imageFilteringMenu = new wxMenu;
	 imageFilteringMenu->Append(MY_SALT_AND_PEPPER_ADDER_ID,_T("&Add Salt and Pepper Noise - RGB"));
	 imageFilteringMenu->AppendSeparator();
	 imageFilteringMenu->Append(MY_MIN_FILTER_ID,_T("&Min Filter - RGB"));
	 imageFilteringMenu->Append(MY_MAX_FILTER_ID,_T("&Max Filter - RGB"));
	 imageFilteringMenu->Append(MY_MID_POINT_FILTER_ID,_T("&Mid Point Filter - RGB"));
	 imageFilteringMenu->Append(MY_MEDIAN_FILTER_ID,_T("&Median Filter - RGB"));

	/****************************
	 POINT PROCESSING MENU BAR
	 ************************************/
	 
	 pointProcessingMenu = new wxMenu;
	 pointProcessingMenu->Append(MY_NEGATIVE_LINEAR_PROCESSING_ID,_T("&Negative Linear Transformation - RGB"));
	 pointProcessingMenu->Append(MY_LOGARITHMIC_PROCESSING_ID,_T("&Logarithmic Function - RGB"));
	 pointProcessingMenu->Append(MY_POWER_LAW_PROCESSING_ID,_T("&Power Law - RGB"));
	 pointProcessingMenu->Append(MY_RANDOM_LOOKUP_PROCESSING_ID,_T("&Random Look-up Table - RGB"));

	/****************************
	 HISTOGRAM MENU BAR
	 ************************************/

	 histogramMenu = new wxMenu;
	 histogramMenu->Append(MY_FIND_HISTOGRAM_ID,_T("&Find Image Histogram - RGB"));
	 histogramMenu->Append(MY_NORMALISE_HISTOGRAM_ID,_T("&Normalise Histogram - RGB"));
	 histogramMenu->Append(MY_EQUALISE_HISTOGRAM_ID,_T("&Equalise Histogram - RGB"));
	 histogramMenu->AppendSeparator();
	 //histogramMenu->Append(MY_DISPLAY_HISTOGRAM_ID,_T("&Display Histogram"));

	 /****************************
	 THRESHOLDING MENU BAR
	 ************************************/
	thresholdMenu = new wxMenu;
	thresholdMenu->Append(MY_FIND_MEAN_STD_ID,_T("&Find Mean and Standard Deviation  GREY-SCALE ONLY"));
	thresholdMenu->AppendSeparator();
	thresholdMenu->Append(MY_SIMPLE_THRESHOLDING_ID,_T("&Apply Simple Thresholding  GREY-SCALE ONLY"));
	thresholdMenu->Append(MY_AUTOMATED_THRESHOLDING_ID,_T("&Apply Automated Thresholding  GREY-SCALE ONLY"));
	//thresholdMenu->Append(MY_ADAPETIVE_THRESHOLDING_ID,_T("&Apply Adaptive Thresholding\tGREY-SCALE ONLY"));
	
	/****************************
	 REGION OF INTEREST MENU BAR
	 ************************************/
	 roiMenu = new wxMenu;
	 roiMenu->Append(MY_ENABLE_ROI_ID, _T("&Enable Region of Interest"));
	 roiMenu->Append(MY_DISABLE_ROI_ID, _T("&Disable Region of Interest"));
		
	/****************************
	 ADD MENUES TO THE MAIN MENU BAR
	 ************************************/

	menuBar = new wxMenuBar;
	menuBar->Append(fileMenu, _T("&File"));
	menuBar->Append(editMenu, _T("&Edit Image"));
	menuBar->Append(imageConvolutionMenu, _T("&Image Convolution"));
	menuBar->Append(imageFilteringMenu, _T("&Order Statistical Filtering"));
	menuBar->Append(pointProcessingMenu, _T("&Point Processing"));
	menuBar->Append(histogramMenu, _T("&Histogram"));
	menuBar->Append(thresholdMenu, _T("&Thresholding"));
	menuBar->Append(roiMenu, _T("&ROI"));

	SetMenuBar (menuBar);
	CreateStatusBar(3);
	oldWidth = 0;
	oldHeight = 0;
	loadedImage = 0;

	/* initialise the variables that we added */
	imgWidth = 0;
	imgHeight = 0;
	stuffToDraw = 0;

}

//###########################################################//
//-----------------------------------------------------------//
//---------- DESTRUCTOR 		--------------------//
//-----------------------------------------------------------//
MyFrame::~MyFrame() {
	/* release resources */
	if (loadedImage) {
		loadedImage->Destroy();
		loadedImage = 0;
	}

}

//###########################################################//
//-----------------------------------------------------------//
//---------- IMAGE OPENING FUNCTIONS --------------------//
//-----------------------------------------------------------//
//###########################################################//

// DONOT EDIT
void MyFrame::OnOpenFile(wxCommandEvent & event) {
	wxFileDialog *openFileDialog = new wxFileDialog(this, _T("Open file"),
			_T(""), _T(""), FILETYPES, wxOPEN, wxDefaultPosition);
	if (openFileDialog->ShowModal() == wxID_OK) {
		wxString filename = openFileDialog->GetFilename();
		wxString path = openFileDialog->GetPath();
		printf("Loading image form file...");
		loadedImage = new wxImage(path); //Image Loaded form file
		if (loadedImage->Ok()) {
			stuffToDraw = ORIGINAL_IMG;    // set the display flag
			printf("Done! \n");
		} else {
			printf("error:...");
			loadedImage->Destroy();
			loadedImage = 0;
		}
		Refresh();
	}
}

/****************************
	OPEN RAW FILE:- READ THE ENTIRE RAW FILE INTO MEMORY. READ BACK FROM MEMORY 
	AND DISPLAY ON THE SCREEN
************************************/

void MyFrame::onOpenRawFile(wxCommandEvent & event) {
	wxFileDialog *openFileDialog = new wxFileDialog(this, _T("Open RAW file"),
			_T(""), _T(""), FILETYPES, wxOPEN, wxDefaultPosition);
	if (openFileDialog->ShowModal() == wxID_OK) {

		
		wxString filename = openFileDialog->GetFilename();
		wxString path = openFileDialog->GetPath();
		
		FILE *pFile;
		long lSize;
		char * buffer;
		size_t result;

		pFile =  fopen(path.ToAscii(), "rb");
		if(pFile==NULL){
			fputs("File Error", stderr);
			exit(1);
		}

		//Get FILE SIZE
		fseek(pFile, 0, SEEK_END);
		lSize = ftell(pFile);
		rewind (pFile);

  		// allocate memory to contain the whole file:
  		buffer = (char*) malloc (sizeof(char)*lSize);
  		if (buffer == NULL) {
			fputs ("Memory error",stderr); 
			exit (2);
		}

  		// copy the file into the buffer:
  		result = fread (buffer,1,lSize,pFile);
  		if (result != lSize) {
			fputs ("Reading error",stderr); 
			exit (3);
		}

  		/* the whole file is now loaded in the memory buffer. */
			
		int height = sqrt(lSize); 
		int width = sqrt(lSize);
		int imageMatrix [height][width];
		loadedImage = new wxImage(height, width, false);
	
		unsigned char r, g, b;
		for (int i = 0; i < width; i++){
			for (int j = 0; j < height; j++) {

				r = buffer[j*width+i];
				g = buffer[j*width+i];
				b = buffer[j*width+i];
				loadedImage->SetRGB(i, j, r, g, b);
			}
		}

		//Error catching
		if (loadedImage->Ok()) {
			stuffToDraw = ORIGINAL_IMG;    // set the display flag
			printf("Done! \n");
		} else {
			printf("error:...");
			loadedImage->Destroy();
			loadedImage = 0;
		}
	
		printf("[INFO] FINISHED OPENING RAW FILE.\n\n");
		Refresh();
	}
}


//###########################################################//
//-----------------------------------------------------------//
//---------- IMAGE PROCESSING FUNCTIONS --------------------//
//-----------------------------------------------------------//
//###########################################################//


	/****************************
	 SAMPLE IMAGE INVERTING:- INVERT THE IMAGE SUBTARCT 255 FROM EACH RGB PIXEL VALUE.
	
	 ************************************/
void MyFrame::OnInvertImage(wxCommandEvent & event) {

	printf("[INFO] Inverting...\n");
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;

	// GET ROI X AND Y CO-ORDINATES, ONLY APPLICABLE WHEN ROI FLAG IS SET TO TRUE
	int x = (roiFlag) ? roistartX: 0;
	int y = (roiFlag) ? roistartY: 0;

	for (int i = x; i < imgW; i++)
		for (int j = y; j < imgH; j++) {
			loadedImage->SetRGB(i, j, 255 - loadedImage->GetRed(i, j),
					255 - loadedImage->GetGreen(i, j),
					255 - loadedImage->GetBlue(i, j));
		}


	printf("[INFO] Finished inverting.\n\n");
	Refresh();
}

	/****************************
	 SAMPLE IMAGE SCALING:- SCALE THE IMAGE USING A  
		SCALING FACTOR. CONSTANT C = 2.0. 
	 ************************************/
void MyFrame::OnScaleImage(wxCommandEvent & event) {

	printf("[INFO] Scaling...\n");
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());
	// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;

	
	// GET ROI X AND Y CO-ORDINATES, ONLY APPLICABLE WHEN ROI FLAG IS SET TO TRUE
	int x = (roiFlag) ? roistartX: 0;
	int y = (roiFlag) ? roistartY: 0;

	for (int i = x; i < imgW; i++)
		for (int j = y; j < imgH; j++) {
			loadedImage->SetRGB(i, j, 2.0 * loadedImage->GetRed(i, j),
					2.0 * loadedImage->GetGreen(i, j),
					2.0 * loadedImage->GetBlue(i, j));
		}

	printf("[INFO] Finished scaling.\n\n");
	Refresh();
}

	/****************************

	 IMAGE SCALING:- SCALE THE IMAGE USING A SCALING FACTOR
		MULTIPLY EACH PIXEL VALUE WITH THE SCALING FACTOR.
		MULTIPLICATION WITH A POSTIVE INTEGER USUALLY RESULTS IN
		A BRIGHTER IMAGE. 
	 ************************************/


void MyFrame::OnMyScaleImageFunc(wxCommandEvent & event) {
	printf("[INFO] My Scaling function...\n");
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	float rescalingFactor = 1.5f;
	int r,g,b; 	
	// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;

	

	// GET ROI X AND Y CO-ORDINATES, ONLY APPLICABLE WHEN ROI FLAG IS SET TO TRUE 
	int x = (roiFlag) ? roistartX: 0;
	int y = (roiFlag) ? roistartY: 0;

	for (int i = x; i < imgW; i++)
		for (int j = y; j < imgH; j++) {
			r = round(rescalingFactor*loadedImage->GetRed(i, j));
			g = round(rescalingFactor*loadedImage->GetGreen(i, j));
			b = round(rescalingFactor*loadedImage->GetBlue(i, j));
			truncate(r,g,b);
			loadedImage->SetRGB(i, j, r,g,b);
		}

	printf("[INFO] Finished my scaling function.\n\n");
	Refresh();
}

	/****************************
	 IMAGE SHIFTING:- SHIFT THE IMAGE PIXEL VALE USING A CONSTANT VALUE.
	 ************************************/


void MyFrame::onMyShiftImageFunc(wxCommandEvent & event) {
	printf("[INFO] My Shift function...\n");
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	int shift = 10;
	int r,g,b; 

	// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;

	

	// GET ROI X AND Y CO-ORDINATES, ONLY APPLICABLE WHEN ROI FLAG IS SET TO TRUE
	int x = (roiFlag) ? roistartX: 0;
	int y = (roiFlag) ? roistartY: 0;

	for (int i = x; i < imgW; i++)
		for (int j = y; j < imgH; j++) {
			r = loadedImage->GetRed(i, j) - shift;
			g = loadedImage->GetGreen(i, j) - shift;
			b = loadedImage->GetBlue(i, j) - shift;
			truncate(r,g,b);
			loadedImage->SetRGB(i, j, r,g,b);
		}

	printf("[INFO] Finished my Shift function.\n\n");
	Refresh();
}




//###############################################################################################################################################################################################//
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------- IMAGE CONVOLUTION FUNCTIONS ---------------------------------------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//###################################################################################################################################################################################################//


/****************************
	AVERAGING :- AVERAGING IS USED TO SMOOTH IMAGE.
	AVERAGE MASK USES A 1/9 OPERATOR.
	IMAGE IS CONVOLUTED WITH A 3*3 MASK
************************************/

void MyFrame::onMyAveragingConvolution(wxCommandEvent & event){
	printf("%s\n", "[INFO] My Average Convolution Function");

	// SET UP THE IMAGE AND ADD TO UNDO STACK
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;
	

	// GET ROI X AND Y CO-ORDINATES, ONLY APPLICABLE WHEN ROI FLAG IS SET TO TRUE
	int x = (roiFlag) ? roistartX: 0;
	int y = (roiFlag) ? roistartY: 0;

	// SET UP CONVOLUTION MASK
	int maskWidth = 3; int maskHeight = 3; int sum = 9; 
	int mask [3][3] = {{1,1,1},{1,1,1},{1,1,1}};

	//SET THE RESCALING, ABSOLUTION AND TRUNCATING FALGS.
	bool rescaling = false; bool absolute = false; bool truncating = true;

	// APPLY CONVOLUTION
	wxImage* convImage = onMyConvoluteImageFunc(imgW, imgH, mask, maskHeight, maskWidth, sum, rescaling, truncating, absolute); // CONVOLUTE OVER IMAGE USING MASK

	showConvImage(convImage, imgW, imgH, x,y);
	delete convImage;

	printf("[INFO] Finished My function.\n\n");
	Refresh();
}


/****************************
	WEIGHTED AVERAGING:- WEIGHT AVERAGING IS USED TO SMOOTH IMAGE.
	WEIGHTED MASK USES A 1/16 OPERATOR.
	IMAGE IS CONVOLUTED WITH A 3*3 MASK
************************************/
void MyFrame::onMyWeightedAveragingConvolution(wxCommandEvent & event){
	printf("%s\n", "[INFO] My Weighted Average Convolution Function");

	// SET UP THE IMAGE AND ADD TO UNDO STACK
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;
	

	// GET ROI X AND Y CO-ORDINATES, ONLY APPLICABLE WHEN ROI FLAG IS SET TO TRUE
	int x = (roiFlag) ? roistartX: 0;
	int y = (roiFlag) ? roistartY: 0;

	// SET UP CONVOLUTION MASK
	int maskWidth = 3; int maskHeight = 3; int sum = 16; 
	int mask [3][3] = {{1,2,1},{2,4,2},{1,2,1}};

	//SET THE RESCALING, ABSOLUTION AND TRUNCATING FALGS.
	bool rescaling = false; bool absolute = false; bool truncating = true;

	wxImage* convImage = onMyConvoluteImageFunc(imgW, imgH, mask, maskHeight, maskWidth, sum, rescaling, truncating, absolute); // CONVOLUTE OVER IMAGE USING MASK

	showConvImage(convImage, imgW, imgH, x,y);
	delete convImage;

	printf("[INFO] Finished My function.\n\n");
	Refresh();
}

/****************************
	lAPLACIAN 4-NEIGHBOUR:- lAPLACIAN IS USED TO DETECT EDGE.
	AFTER lAPLACIAN IMAGE MUST BE RESACLED TO BRING TO VISIBLE RANGE
	IMAGE IS CONVOLUTED WITH A 3*3 MASK
************************************/
void MyFrame::onMyFourNeighbourLaplacianConvolution(wxCommandEvent & event){
	printf("%s\n", "[INFO] My Four Neighbour Laplacian Convolution Function");

	// SET UP THE IMAGE AND ADD TO UNDO STACK
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	// SET UP CONVOLUTION MASK
	int maskWidth = 3; int maskHeight = 3; int sum = 1; 
	int mask [3][3]= {{0,-1,0},{-1,4,-1},{0,-1,0}};

	// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;
	

	// GET ROI X AND Y CO-ORDINATES, ONLY APPLICABLE WHEN ROI FLAG IS SET TO TRUE
	int x = (roiFlag) ? roistartX: 0;
	int y = (roiFlag) ? roistartY: 0;


	//SET THE RESCALING, ABSOLUTION AND TRUNCATING FALGS.
	bool rescaling = true; bool absolute = false; bool truncating = false;

	// APPLY CONVOLUTION
	wxImage* convImage = onMyConvoluteImageFunc(imgW, imgH, mask, maskHeight, maskWidth, sum, rescaling, truncating, absolute); // CONVOLUTE OVER IMAGE USING MASK		

	//DISPLAY
	showConvImage(convImage, imgW, imgH, x,y);
	delete convImage;

	printf("[INFO] Finished My function.\n\n");
	Refresh();
}
/****************************
	lAPLACIAN 8-NEIGHBOUR:- lAPLACIAN IS USED TO DETECT EDGE.
	AFTER lAPLACIAN IMAGE MUST BE RESACLED TO BRING TO VISIBLE RANGE
	IMAGE IS CONVOLUTED WITH A 3*3 MASK
************************************/
void MyFrame::onMyEightNeighbourLaplacianConvolution(wxCommandEvent & event){
	printf("%s\n", "[INFO] My Eight Neighbour Laplacian Convolution Function");
	
	// SET UP THE IMAGE AND ADD TO UNDO STACK
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());
	
	// SET UP CONVOLUTION MASK
	int maskWidth = 3; int maskHeight = 3; int sum = 1; 
	int mask [3][3] = {{-1,-1,-1},{-1,8,-1},{-1,-1,-1}};

	// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;
	

	// GET ROI X AND Y CO-ORDINATES, ONLY APPLICABLE WHEN ROI FLAG IS SET TO TRUE
	int x = (roiFlag) ? roistartX: 0;
	int y = (roiFlag) ? roistartY: 0;


	bool rescaling = true; bool absolute = false; bool truncating = false;
	// APPLY CONVOLUTION
	wxImage* convImage = onMyConvoluteImageFunc(imgW, imgH, mask, maskHeight, maskWidth, sum, rescaling, truncating, absolute); // CONVOLUTE OVER IMAGE USING MASK		

	//DISPLAY
	showConvImage(convImage, imgW, imgH, x,y);
	delete convImage;
	
	printf("[INFO] Finished My function.\n\n");
	Refresh();
} 

/****************************
	lAPLACIAN 4-NEIGHBOUR ENHANCEMENT:- lAPLACIAN ENHANCMENT IS USED TO SHARPEN IMAGE.
	IMAGE IS CONVOLUTED WITH A 3*3 MASK
************************************/
void MyFrame::onMyFourNeighbourLaplacianEnhancmentConvolution(wxCommandEvent & event){
	printf("%s\n", "[INFO] My Four Neighbour Laplacian Enhancment Convolution Function");

	// SET UP THE IMAGE AND ADD TO UNDO STACK
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	// SET UP CONVOLUTION MASK
	int maskWidth = 3; int maskHeight = 3; int sum = 1; 
	int mask [3][3] = {{0,-1,0},{-1,5,-1},{0,-1,0}};


	// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;
	

	// GET ROI X AND Y CO-ORDINATES, ONLY APPLICABLE WHEN ROI FLAG IS SET TO TRUE
	int x = (roiFlag) ? roistartX: 0;
	int y = (roiFlag) ? roistartY: 0;


	bool rescaling = false; bool absolute = false; bool truncating = true;
	// APPLY CONVOLUTION
	wxImage* convImage = onMyConvoluteImageFunc(imgW, imgH, mask, maskHeight, maskWidth, sum, rescaling, truncating, absolute); // CONVOLUTE OVER IMAGE USING MASK		

	//DISPLAY
	showConvImage(convImage, imgW, imgH, x,y);
	delete convImage;

	printf("[INFO] Finished My function.\n\n");
	Refresh();
}

/****************************
	lAPLACIAN 8-NEIGHBOUR ENHANCEMENT:- lAPLACIAN ENHANCMENT IS USED TO SHARPEN IMAGE.
	IMAGE IS CONVOLUTED WITH A 3*3 MASK
************************************/
void MyFrame::onMyEightNeighbourLaplacianEnhancmentConvolution(wxCommandEvent & event){
	printf("%s\n", "[INFO] My Eight Neighbour Laplacian Enhancment Convolution Function");

	// SET UP THE IMAGE AND ADD TO UNDO STACK
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	// SET UP CONVOLUTION MASK
	int maskWidth = 3; int maskHeight = 3; int sum = 1; 
	int mask [3][3] = {{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};

	// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;
	
	// GET ROI X AND Y CO-ORDINATES, ONLY APPLICABLE WHEN ROI FLAG IS SET TO TRUE
	int x = (roiFlag) ? roistartX: 0;
	int y = (roiFlag) ? roistartY: 0;

	bool rescaling = false; bool absolute = false; bool truncating = true;

	// APPLY CONVOLUTION
	wxImage* convImage = onMyConvoluteImageFunc(imgW, imgH, mask, maskHeight, maskWidth, sum, rescaling, truncating, absolute); // CONVOLUTE OVER IMAGE USING MASK		

	//DISPLAY
	showConvImage(convImage, imgW, imgH, x,y);
	delete convImage;

	printf("[INFO] Finished My function.\n\n");
	Refresh();
}

/****************************
	ROBORTS:- USED FOR EDGE DETECTION, ROBERTS OPERATOR HIGHLIGHTS HIGH GRAIDENT REGION.
	IMAGE IS CONVOLUTED WITH A 3*3 MASK
************************************/
void MyFrame::onMyRobertsConvolution(wxCommandEvent & event){
	printf("%s\n", "[INFO] My Roberts Convolution Function");

	// SET UP THE IMAGE AND ADD TO UNDO STACK
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	// SET UP CONVOLUTION MASK
	int maskWidth = 3; int maskHeight = 3; int sum = 1; 
	int maskg1 [3][3] = {{0,0,0},{0,0,-1},{0,1,0}};
	int maskg2 [3][3] = {{0,0,0},{0,-1,0},{0,0,1}};

		// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;
	

	// GET ROI X AND Y CO-ORDINATES, ONLY APPLICABLE WHEN ROI FLAG IS SET TO TRUE
	int x = (roiFlag) ? roistartX: 0;
	int y = (roiFlag) ? roistartY: 0;

	bool rescaling = false; bool absolute = true; bool truncating = true;

	// APPLY CONVOLUTION TO G1 AND G2
	wxImage* g1 = onMyConvoluteImageFunc(imgW, imgH, maskg1, maskHeight, maskWidth, sum, rescaling, truncating, absolute); // CONVOLUTE OVER IMAGE USING MASK
	wxImage* g2 = onMyConvoluteImageFunc(imgW, imgH, maskg2, maskHeight, maskWidth, sum, rescaling, truncating, absolute); // CONVOLUTE OVER IMAGE USING MASK
	wxImage* g = new wxImage(imgW, imgH);

	// G = |G1| + |G2|
	int r,green,b;
	for (int i = x; i < imgW; i++){
		for (int j = y; j < imgH; j++)
		{
			r = (g1->GetRed(i,j) + g2->GetRed(i,j)); green = (g1->GetGreen(i,j) + g2->GetGreen(i,j)); b = (g1->GetBlue(i,j) + g2->GetBlue(i,j));
			truncate(r,green,b);
			g->SetRGB(i,j, r, green, b);
		}
	}

	showConvImage(g, imgW, imgH, x,y);
	delete g1; delete g2; delete g;
	printf("[INFO] Finished My function.\n\n");
	Refresh();
}

/****************************
	ROBORTS:- USED FOR EDGE DETECTION, ROBERTS OPERATOR HIGHLIGHTS HIGH GRAIDENT REGION.
	IMAGE IS CONVOLUTED WITH A 3*3 MASK
************************************/
void MyFrame::onMySobel(wxCommandEvent & event){
	printf("%s\n", "[INFO] My Roberts with absolute value conversion Convolution Function");

	// SET UP THE IMAGE AND ADD TO UNDO STACK
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	// SET UP CONVOLUTION MASK
	int maskWidth = 3; int maskHeight = 3; int sum = 1; 
	int maskX [3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
	int maskY [3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};

	// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;
	

	// GET ROI X AND Y CO-ORDINATES, ONLY APPLICABLE WHEN ROI FLAG IS SET TO TRUE
	int x = (roiFlag) ? roistartX: 0;
	int y = (roiFlag) ? roistartY: 0;

	bool rescaling = false; bool absolute = true; bool truncating = true;

	// APPLY CONVOLUTION- SOBEL = |SOBELX| + |SOBELY|
	wxImage* sobelX = onMyConvoluteImageFunc(imgW, imgH, maskX, maskHeight, maskWidth, sum, rescaling, truncating, absolute); // CONVOLUTE OVER IMAGE USING MASK
	wxImage* sobelY = onMyConvoluteImageFunc(imgW, imgH, maskY, maskHeight, maskWidth, sum, rescaling, truncating, absolute); // CONVOLUTE OVER IMAGE USING MASK
	wxImage* sobel = new wxImage(imgW, imgH);
	int r,g,b;
		for (int i = x; i < imgW; i++){
			for (int j = y; j < imgH; j++){
				r = (sobelX->GetRed(i,j) + sobelY->GetRed(i,j)); 
				g = (sobelX->GetGreen(i,j) + sobelY->GetGreen(i,j)); 
				b = (sobelX->GetBlue(i,j) + sobelY->GetBlue(i,j));
				truncate(r,g,b);
				sobel->SetRGB(i,j,r,g,b);
			}
		}
	



	showConvImage(sobel, imgW, imgH, x,y);
	delete sobelX; delete sobelY; delete sobel;

	printf("[INFO] Finished My function.\n\n");
	Refresh();
} 

/****************************
	SOBEL X:- USED FOR EDGE DETECTION, X-AXSIS SOBEL OPERATOR HIGHLIGHTS HIGH GRAIDENT REGION.
	IMAGE IS CONVOLUTED WITH A 3*3 MASK
************************************/

void MyFrame::onMySobelXConvolution(wxCommandEvent & event){
	printf("%s\n", "[INFO] My Sobel X Convolution Function");

	// SET UP THE IMAGE AND ADD TO UNDO STACK
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	// SET UP CONVOLUTION MASK
	int maskWidth = 3; int maskHeight = 3; int sum = 1; 
	int mask [3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};


		// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;
	

	// GET ROI X AND Y CO-ORDINATES, ONLY APPLICABLE WHEN ROI FLAG IS SET TO TRUE
	int x = (roiFlag) ? roistartX: 0;
	int y = (roiFlag) ? roistartY: 0;

	// Set Rescaling, Absolution and truncating
	bool rescaling = false; bool absolute = true; bool truncating = true;


	// APPLY CONVOLUTION
	wxImage* sobelX = onMyConvoluteImageFunc(imgW, imgH, mask, maskHeight, maskWidth, sum, rescaling, truncating, absolute); // CONVOLUTE OVER IMAGE USING MASK
	showConvImage(sobelX, imgW, imgH, x,y);
	delete sobelX;

	printf("[INFO] Finished My function.\n\n");
	Refresh();
}

/****************************
	SOBEL Y:- USED FOR EDGE DETECTION, Y-AXSIS SOBEL OPERATOR HIGHLIGHTS HIGH GRAIDENT REGION.
	IMAGE IS CONVOLUTED WITH A 3*3 MASK
************************************/
void MyFrame::onMySobelYConvolution(wxCommandEvent & event){
	printf("%s\n", "[INFO] My Sobel Y Convolution Function");

	// SET UP THE IMAGE AND ADD TO UNDO STACK
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	int maskWidth = 3; int maskHeight = 3; int sum = 1; 
	int mask [3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};

	// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;
	

	// GET ROI X AND Y CO-ORDINATES, ONLY APPLICABLE WHEN ROI FLAG IS SET TO TRUE
	int x = (roiFlag) ? roistartX: 0;
	int y = (roiFlag) ? roistartY: 0;

	// Set Rescaling, Absolution and truncating
	bool rescaling = false; bool absolute = true; bool truncating = true;


	// APPLY CONVOLUTION
	wxImage* sobelY = onMyConvoluteImageFunc(imgW, imgH, mask, maskHeight, maskWidth, sum, rescaling, truncating, absolute); // CONVOLUTE OVER IMAGE USING MASK
	showConvImage(sobelY, imgW, imgH, x,y);
	delete sobelY;


	printf("[INFO] Finished My function.\n\n");
	Refresh();
}

//###############################################################################################################################################################################################//
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------- END IMAGE CONVOLUTION FUNCTIONS ---------------------------------------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//###################################################################################################################################################################################################//

//###########################################################################################################################################################################//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------- IMAGE FILTERING FUNCTIONS ------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//#################################################################################################################################################################################//


/****************************
	ADD SALT AND PEPPER NOISE AT RANDOM LOCATIONS OF THE 
	IMAGE. SALT = 255, PEPPER = 0. 
************************************/
void MyFrame::onMySaltAndPepperAdder(wxCommandEvent &event){
	printf("%s\n", "[INFO] My salt and pepper function");

	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());
	
	int x,y;


	// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;

	// GET THE REGION BOUNDS
	int bounds = (roiFlag) ? imgW*imgH : imgWidth*imgHeight;

	// GET ROI X AND Y CO-ORDINATES, ONLY APPLICABLE WHEN ROI FLAG IS SET TO TRUE
	int x1 = (roiFlag) ? roistartX: 0;
	int y1 = (roiFlag) ? roistartY: 0;

	
	// SET SALT AND PEPPER
	for (int i = x1; i < imgW; i++){
			x = (rand() % (imgW+1-x1))+x1;
			y = (rand() % (imgH+1-y1))+y1;
			// GENERATE A RANDOM NUMBER TO DECIDED TO APPLY SALT OR PEPPER
			if(rand()%bounds>(bounds/2))
				loadedImage->SetRGB(x, y, 255, 255, 255);
			else
				loadedImage->SetRGB(x, y, 0, 0, 0);
	}

	Refresh();
	printf("%s\n", "[INFO] Finished my salt and pepper function\n");
} 


/****************************
	MIN FILTER:- FILTER USED TO REMOVE SALT NOISE AND
	INCREASE VISIBILITY OF PEPPER NOISE.  
	SET THE PIXEL VALUE TO THE MINIMUM PIXEL IN THE NEIGHBOURHOOD. 
************************************/
void MyFrame::onMyMinFilter(wxCommandEvent &event){
	printf("%s\n", "My Min Filter Function");

	// SET UP THE IMAGE AND TO UNDO STACK
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	// SET UP A MASK TO CREATE A 3*3 NEIGHBOURHOOD
	int maskWidth = 3; int maskHeight = 3;
	int mask[3][3] = {{1,1,1},{1,1,1},{1,1,1}};


	// SET UP RESCALING, TRUNCATING AND ABSOULTION.
	bool rescaling = false; bool truncating = false; bool absolute = false;
		
	// CONVOLUTE OVER IMAGE USING MASK, APPLYING FILTER 1 WHICH IS THE MIN FILTER. 
	onMyConvoluteFilterImageFunc(mask, maskHeight, maskWidth, 1, rescaling, truncating, absolute);

	Refresh();
	printf("%s\n", "[INFO] Finished my Min Filter Function\n");
} 

/****************************
	MAX FILTER:- FILTER USED TO REMOVE PEPPER NOISE AND
	INCREASE VISIBILITY OF  SALT NOISE.  
	SET THE PIXEL VALUE TO THE MAXIMUM PIXEL IN THE NEIGHBOURHOOD. 
************************************/
void MyFrame::onMyMaxFilter(wxCommandEvent &event){
	printf("%s\n", "[INFO] My Max Filter Function");

	// SET UP THE IMAGE AND ADD TO UNDO STACK
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	// SET UP A MASK TO CREATE A 3*3 NEIGHBOURHOOD
	int maskWidth = 3; int maskHeight = 3;
	int mask[3][3] = {{1,1,1},{1,1,1},{1,1,1}};

	// SET UP RESCALING, TRUNCATING AND ABSOULTION.
	bool rescaling = false; bool truncating = false; bool absolute = false;
		
	// CONVOLUTE OVER IMAGE USING MASK, APPLYING FILTER 2 WHICH IS THE MAX FILTER. 
	onMyConvoluteFilterImageFunc(mask, maskHeight, maskWidth, 2, rescaling, truncating, absolute);
	Refresh();

	printf("%s\n", "[INFO] Finished my max filter function\n");

} 

/****************************
	MID-POINT FILTER:- FILTER USED TO BALANCE PEPPER AND SALT NOISE 
	SET THE PIXEL VALUE TO THE MAXIMUM-MINIMYM/2 PIXEL IN THE NEIGHBOURHOOD. 
************************************/
void MyFrame::onMyMidPointFilter(wxCommandEvent &event){
	printf("%s\n", "[INFO] My Mid Point Filter Function");

	// SET UP THE IMAGE AND ADD TO UNDO STACK
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	// SET UP A MASK TO CREATE A 3*3 NEIGHBOURHOOD
	int maskWidth = 3; int maskHeight = 3;
	int mask[3][3] = {{1,1,1},{1,1,1},{1,1,1}};


	// Rescaling, Truncation or Absolution
	bool rescaling = false;
	bool truncating = false;
	bool absolute = false;
		
	// CONVOLUTE OVER IMAGE USING MASK, APPLYING FILTER 3 WHICH IS THE MID-POINT FILTER. 
	onMyConvoluteFilterImageFunc(mask, maskHeight, maskWidth, 3, rescaling, truncating, absolute);
	Refresh();

	printf("%s\n", "[INFO] Finished my Mid Point Filtere\n");
} 

/****************************
	MEDIAN FILTER:- FILTER USED TO REMOVE PEPPER AND SALT NOISE 
	SET THE PIXEL VALUE TO THE MIDDLE PIXEL IN THE SORTED NEIGHBOURHOOD. 
************************************/
void MyFrame::onMyMedianFilter(wxCommandEvent &event){
	printf("%s\n", "[INFO] My Median Point Filter Function");

	// SET UP THE IMAGE AND ADD TO UNDO STACK
	imageStack->push(loadedImage);
	loadedImage = new wxImage(bitmap.ConvertToImage());

	// SET UP A MASK TO CREATE A 3*3 NEIGHBOURHOOD
	int maskWidth = 3; int maskHeight = 3;
	int mask[3][3] = {{1,1,1},{1,1,1},{1,1,1}};

	// Rescaling, Truncation or Absolution
	bool rescaling = false;
	bool truncating = false;
	bool absolute = false;
		
	// CONVOLUTE OVER IMAGE USING MASK, APPLYING FILTER 4 WHICH IS THE MEDIAN FILTER. 
	onMyConvoluteFilterImageFunc(mask, maskHeight, maskWidth, 4, rescaling, truncating, absolute);

	Refresh();
	printf("%s\n", "[INFO] Finished my Median Filter Function\n");
}

//###########################################################################################################################################################################//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------- END IMAGE FILTERING FUNCTIONS ------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//#################################################################################################################################################################################//


//###########################################################################################################################################################################//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------- POINT PROCESSING FUNCTIONS ------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//#################################################################################################################################################################################//

/****************************
	NEGATIVE LINEAR TRANSFOMRATION:- NEAGTIVE TRANSFORMATION, SUBTRACTING 255 
	FROM EACH PIXEL VALUE. 
************************************/
void MyFrame::applyNegativeLinearProcessing(wxCommandEvent & event){
		printf("%s\n", "[INFO] Applying Negative Linear Transformation");

		// SET UP IMAGE AND ADD TO UNDO STACK 
		imageStack->push(loadedImage);
		loadedImage = new wxImage(bitmap.ConvertToImage());

		// GET IMAGE WIDTH AND HEIGHT
		int imgW = (roiFlag) ? roiendX : imgWidth;
		int imgH = (roiFlag) ? roiendY : imgHeight;


		// GET ROI X AND Y CO-ORDINATES
		int x = (roiFlag) ? roistartX: 0;
		int y = (roiFlag) ? roistartY: 0;

		int changedR[imgW][imgH];
		int changedG[imgW][imgH];
		int changedB[imgW][imgH];

		// APPLY NEGATIVE TRANSFORMATION BY SUBTRACTING 255 FROM EACH PIXEL
		for (int i = x; i < imgW; i++){
			for (int j = y; j < imgH; j++){
				changedR[i][j] = 255 - loadedImage->GetRed(i,j);
				changedG[i][j] = 255 - loadedImage->GetGreen(i,j);
				changedB[i][j] = 255 - loadedImage->GetBlue(i,j); 
			}
		}


	// APPLY TRUNCATION AS REQUIRED.
	for (int i = x; i < imgW; i++){
		for (int j = y; j < imgH; j++) {
					truncate(changedR[i][j], changedG[i][j], changedB[i][j]);
					loadedImage->SetRGB(i, j, changedR[i][j], changedG [i][j], changedB[i][j]);
			}
		}


		printf("%s\n", "[INFO] Done\n");
		Refresh();
}

/****************************
	LOGARITHMIC TRANSFOMRATION:- LOGARITHM OF THE PIXEL MULITPLYED BY A CONSTANT C.
	WHERE C = MAX/LOG(1+MAX), WHERE MAX IS THE MAX R,G,B VALUES IN THE IMAGE.
************************************/
	// Apply Logarithmic Transformation
	void MyFrame::applyLogarithmicProcessing(wxCommandEvent & event){
		printf("%s\n", "[INFO] Applying Logarithmic Transformation");
		imageStack->push(loadedImage);
		loadedImage = new wxImage(bitmap.ConvertToImage());


		// GET IMAGE WIDTH AND HEIGHT
		int imgW = (roiFlag) ? roiendX : imgWidth;
		int imgH = (roiFlag) ? roiendY : imgHeight;


		// GET ROI X AND Y CO-ORDINATES
		int x = (roiFlag) ? roistartX: 0;
		int y = (roiFlag) ? roistartY: 0;

		// DEFINE AND SET MAX AND MIN RGB VALUES
		int rmax,rmin,gmax,gmin,bmax,bmin; 
		rmax = gmax = bmax = 0;
		rmin = gmin = bmin = 255;

		int cR, cG, cB;

		int changedR[imgW][imgH];
		int changedG[imgW][imgH];
		int changedB[imgW][imgH];

		
		// FIND THE MAX RGB VALUES TO SET FOR OUR CONSTANTS
		for (int i = x; i < imgW; i++){
			for (int j = y; j < imgH; j++){
					setMaxAndMinRGB(loadedImage->GetRed(i,j), rmax, rmin, loadedImage->GetGreen(i,j), gmax, gmin, loadedImage->GetBlue(i,j), bmax, bmin);
				}
		}

		//SET CONSTANTS R, G, B
		cR = rmax/log(1+rmax);
		cG = gmax/log(1+gmax);
		cB = bmax/log(1+bmax);


		// APPLY LOGARITHM OF EACH PIXEL.
		for (int i = x; i < imgW; i++){
			for (int j = y; j < imgH; j++){
				changedR[i][j] = cR * log(1+ loadedImage->GetRed(i,j));
				changedG[i][j] = cG * log(1+ loadedImage->GetGreen(i,j));
				changedB[i][j] = cB * log(1+ loadedImage->GetBlue(i,j));
			}

		}

	// APPLY TRUNCATION AS REQUIRED.
	for (int i = x; i < imgW; i++){
		for (int j = y; j < imgH; j++) {
					truncate(changedR[i][j], changedG[i][j], changedB[i][j]);
					loadedImage->SetRGB(i, j, changedR[i][j], changedG [i][j], changedB[i][j]);
			}
		}

		printf("%s\n", "[INFO] Done\n");
		Refresh();
	}

/****************************
	POWER TRANSFOMRATION:- APPLYING POWER LAW TRANSFORMATION, PIXEL VALUE
	= CONSTANT * PIXEL VALUE ^ EXPONENT. WHERE CONSTANT = MAX^1-EXPONENT. 
	CONSTANT IS USED TO AVOID RESCALING.
************************************/
	void MyFrame::applyPowerLawProcessing(wxCommandEvent & event){
		printf("%s\n", "[INFO] Applying Power Law Transformation");
		imageStack->push(loadedImage);
		loadedImage = new wxImage(bitmap.ConvertToImage());

		// GET IMAGE WIDTH AND HEIGHT
		int imgW = (roiFlag) ? roiendX : imgWidth;
		int imgH = (roiFlag) ? roiendY : imgHeight;

		// GET ROI X AND Y CO-ORDINATES
		int x = (roiFlag) ? roistartX: 0;
		int y = (roiFlag) ? roistartY: 0;

		//DECLARE CONSTANTS R G B
		double cR, cG, cB;

		double exponent = 3.0;

		int changedR[imgW][imgH];
		int changedG[imgW][imgH];
		int changedB[imgW][imgH];

		// MAX AND MIN RGB VALUES
		int rmax,rmin,gmax,gmin,bmax,bmin;
		rmax = gmax = bmax = 0;
		rmin = gmin = bmin = 255;

		// FIND THE MAX RGB VALUES
		for (int i = x; i < imgWidth; i++){
			for (int j = y; j < imgHeight; j++){
					setMaxAndMinRGB(loadedImage->GetRed(i,j), rmax, rmin, loadedImage->GetGreen(i,j), gmax, gmin, loadedImage->GetBlue(i,j), bmax, bmin);
				}
		}


		// SET CONSTANTS
		cR = pow(rmax, 1-exponent);
		cG = pow(gmax, 1-exponent);
		cB = pow(bmax, 1-exponent);

		// Apply Power Law Transformation 
		for (int i = x; i < imgW; i++){
			for (int j = y; j < imgH; j++){
				changedR[i][j] = cR * pow(loadedImage->GetRed(i,j), exponent);
				changedG[i][j] = cG * pow(loadedImage->GetGreen(i,j), exponent);
				changedB[i][j] = cB * pow(loadedImage->GetBlue(i,j), exponent);
			}
		}


	// APPLY TRUNCATION AS REQUIRED.
	for (int i = x; i < imgW; i++){
		for (int j = y; j < imgH; j++) {
					truncate(changedR[i][j], changedG[i][j], changedB[i][j]);
					loadedImage->SetRGB(i, j, changedR[i][j], changedG[i][j], changedB[i][j]);
			}
		}

		Refresh();
		printf("%s\n", "[INFO] Done\n\n");
	}

/****************************
	RANDOM LOOK-UP TRANSFOMRATION:- MAP EACH PIXEL VALUE FROM 0 TO 255 IN THE 
	IMAGE TO A RANDOM VALUE IN THE LOOK UP TABLE. 
************************************/
void MyFrame::applyRandomLookUpProcessing(wxCommandEvent & event){
		printf("%s\n", "[INFO] Applying Random Look-Up Transformation");

		imageStack->push(loadedImage);
		loadedImage = new wxImage(bitmap.ConvertToImage());

		// GET IMAGE WIDTH AND HEIGHT
		int imgW = (roiFlag) ? roiendX : imgWidth;
		int imgH = (roiFlag) ? roiendY : imgHeight;


		// GET ROI X AND Y CO-ORDINATES
		int x = (roiFlag) ? roistartX: 0;
		int y = (roiFlag) ? roistartY: 0;

		int randomRValues[256];
		int randomGValues[256];
		int randomBValues[256];

		// POPULATE EACH VALUE FROM 0 TO 255 TO A RANDOM VALUE.
		for(int i = 0; i<256; ++i){
			randomRValues[i] = rand() % 256;
			randomGValues[i] = rand() % 256;
			randomBValues[i] = rand() % 256;
		}


		// Apply Random Look Up Transformation
		int r, g, b;
		for (int i = x; i < imgW; i++){
			for (int j = y; j < imgH; j++) {
					//GET RGB VALUES
					r = loadedImage->GetRed(i,j);
					g = loadedImage->GetGreen(i,j);
					b = loadedImage->GetBlue(i,j);

					// TRUNCATE PIXELS > 255 AND PIXELS < 0
					truncate(r,g,b);

					// MAP TO RANDOM LOOPUP TABLE
					r = randomRValues[r];
					g = randomGValues[g];
					b = randomBValues[b];

					loadedImage->SetRGB(i,j,r,g,b);
				}
			}

		Refresh();
		printf("%s\n", "[INFO] Done\n\n");
}



//###########################################################################################################################################################################//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------- END POINT PROCESSING FUNCTIONS ------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//#################################################################################################################################################################################//





//###########################################################################################################################################################################//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------- HISTOGRAM FUNCTIONS ------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//#################################################################################################################################################################################//


/****************************
	FIND IMAGE HISTOGRAM:- CALCULATE THE AMOUNT OF TIMES EACH PIXEL VALUE  
	APPEARS IN THE IMAGE. 
************************************/
	void MyFrame::findHistogram(wxCommandEvent &event){
		printf("%s\n", "[INFO] Finding Histogram");

		// GET IMAGE WIDTH AND HEIGHT
		int imgW = (roiFlag) ? roiendX : imgWidth;
		int imgH = (roiFlag) ? roiendY : imgHeight;


		// GET ROI X AND Y CO-ORDINATES
		int x = (roiFlag) ? roistartX: 0;
		int y = (roiFlag) ? roistartY: 0;

		// INITIALSE 0-255 
		int histogramRValues[256];
		int histogramGValues[256];
		int histogramBValues[256];

		// INITALISE THE HISTOGRAM OF ALL PIXELS TO 0.
		for(int i = 0; i<256; ++i){
			histogramRValues[i] = 0;
			histogramGValues[i] = 0;
			histogramBValues[i] = 0;
		}

		//INCREMENT THE VALUE OF THE HISTOGRAM
		for (int i = x; i < imgW; i++){
			for (int j = y; j < imgH; j++){
				histogramRValues[loadedImage->GetRed(i, j)]++;
				histogramGValues[loadedImage->GetGreen(i, j)]++;
				histogramBValues[loadedImage->GetBlue(i, j)]++;

			}
		}

		// PRINT THE DETAILS 
		cout << "Pixel\tR\tG\tB" << endl;
		for(int i = 0; i<256; i++){
			cout << i <<"\t"<<histogramRValues[i]<<"\t"<<histogramGValues[i]<<"\t"<<histogramBValues[i]<<endl;
		}


		printf("%s\n", "[INFO] Done\n\n");
	}

	/****************************
	NORMALISE HISTOGRAM:- CALCULATE THE NORMALISED HISTOGRAM 
	BY FINDING HISTOGRAM OF IMAGE AND DIVIDING THAT BY TOTAL NUMBER OF PIXEL
	NH = HISTOGRAM/TOTAL PIXELS. 
************************************/
	void MyFrame::normaliseHistogram(wxCommandEvent &event){
		printf("%s\n", "[INFO] Normalising Histogram");

		// GET IMAGE WIDTH AND HEIGHT
		int imgW = (roiFlag) ? roiendX : imgWidth;
		int imgH = (roiFlag) ? roiendY : imgHeight;


		// GET ROI X AND Y CO-ORDINATES
		int x = (roiFlag) ? roistartX: 0;
		int y = (roiFlag) ? roistartY: 0;
	
		// INITIALSE 0-255 
		float histogramRValues[256];
		float histogramGValues[256];
		float histogramBValues[256];

		// INITALISE THE HISTOGRAM OF ALL PIXELS TO 0.
		for(int i = 0; i<256; ++i){
			histogramRValues[i] = 0.0;
			histogramGValues[i] = 0.0;
			histogramBValues[i] = 0.0;
		}

		//INCREMENT THE VALUE OF THE HISTOGRAM
		for (int i = x; i < imgW; i++){
			for (int j = y; j < imgH; j++){
				histogramRValues[loadedImage->GetRed(i, j)]++;
				histogramGValues[loadedImage->GetGreen(i, j)]++;
				histogramBValues[loadedImage->GetBlue(i, j)]++;

			}
		}

		int total = imgWidth*imgHeight;
		cout << "Pixel\tR\tG\tB" << endl;
		for(int i = 0; i<256; i++){
			cout << i <<"\t"<<(histogramRValues[i]/total)<<"\t"<<(histogramGValues[i]/total)<<"\t"<<(histogramBValues[i]/total)<<endl;
		}

		printf("%s\n", "[INFO] Done\n\n");
	}

	/****************************
	EQUALISED HISTOGRAM TRANSFOMRATION:- TRANSFORMATION OF THE IMAGE USING IT'S EQUALISED HISTOGRAM.
************************************/
	void MyFrame::equaliseHistogram(wxCommandEvent &event){
		printf("%s\n", "[INFO] Equalising Histogram");
		imageStack->push(loadedImage);
		loadedImage = new wxImage(bitmap.ConvertToImage());

		// GET IMAGE WIDTH AND HEIGHT
		int imgW = (roiFlag) ? roiendX : imgWidth;
		int imgH = (roiFlag) ? roiendY : imgHeight;


		// GET ROI X AND Y CO-ORDINATES
		int x = (roiFlag) ? roistartX: 0;
		int y = (roiFlag) ? roistartY: 0;

		int totalPixels = (roiFlag) ? imgW*imgH : imgWidth*imgHeight;

		float histogramRValues[256];
		float histogramGValues[256];
		float histogramBValues[256];

		for(int i = 0; i<256; i++){
			histogramRValues[i] = 0;
			histogramGValues[i] = 0;
			histogramBValues[i] = 0;
		}

		// Assign Pixel Histogram
		for (int i = x; i < imgW; i++){
			for (int j = y; j < imgH; j++){
				histogramRValues[loadedImage->GetRed(i, j)]++;
				histogramGValues[loadedImage->GetGreen(i, j)]++;
				histogramBValues[loadedImage->GetBlue(i, j)]++;

			}
		}

		// Assign Cumulative Distribution
		for (int i = 1; i < 256; i++){
			histogramRValues[i] = histogramRValues[i] + histogramRValues[i-1];
			histogramGValues[i] = histogramGValues[i] + histogramGValues[i-1];
			histogramBValues[i] = histogramBValues[i] + histogramBValues[i-1];
		}


		// Map the values
		int r, g, b;
		for (int i = x; i < imgW; i++){
			for (int j = y; j < imgH; j++) {
					r = round((histogramRValues[loadedImage->GetRed(i,j)]* 255) / totalPixels);
					g = round((histogramGValues[loadedImage->GetGreen(i,j)]* 255) / totalPixels);
					b = round((histogramBValues[loadedImage->GetBlue(i,j)]* 255) / totalPixels);
					loadedImage->SetRGB(i, j, r,g,b);
			}
		}
		

		Refresh();
		printf("%s\n", "[INFO] Done\n\n");
	}

	// NOT YET IMPLEMENTED {MASTERS ONLY}
	void MyFrame::displayHistogram(wxCommandEvent &event){
		printf("%s\n", "[INFO] Displaying Histogram");
		printf("%s\n", "[INFO] Done\n\n");
	}


//###########################################################################################################################################################################//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------- END HISTOGRAM FUNCTIONS ------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//#################################################################################################################################################################################//

//###########################################################################################################################################################################//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------- THRESHOLDING FUNCTIONS ------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//#################################################################################################################################################################################//

/****************************
	MEAN AND STANDARD DEVIATION OF THE IMAGE.
	MEAN DEVIATION  = TOTAL(ALL PIXEL VALUES - MEAN)/TOTAL PIXELS
	STANDARD DEVIATION = SQUARE ROOT OF VARIANCE   
	FROM EACH PIXEL VALUE. 
************************************/
	void MyFrame::findMeanAndStandardDeviation(wxCommandEvent &event){
		printf("%s\n", "[INFO] Displaying Mean and Standard Deviation");

		// GET IMAGE WIDTH AND HEIGHT
		int imgW = (roiFlag) ? roiendX : imgWidth;
		int imgH = (roiFlag) ? roiendY : imgHeight;


		// GET ROI X AND Y CO-ORDINATES
		int x = (roiFlag) ? roistartX: 0;
		int y = (roiFlag) ? roistartY: 0;

		int totalPixels = (roiFlag) ? imgW*imgH : imgWidth*imgHeight;

		//DECLARE STANDARD DEVIATION VARIABLES
		int mean,  variance, meanDeviation, standardDeviation;
		int rSum, minusMeanSum, minusSTDMeanSum;

		for(int i = x; i<imgW; i++){
			for(int j = y; j<imgH; j++){
				rSum += loadedImage->GetRed(i, j);
			}
		}
		mean = rSum/totalPixels;

		int temp;
		printf("%s %d\n", "[INFO] Image Mean = ", meanDeviation);
		for(int i = x; i<imgW; i++){
			for(int j = y; j<imgH; j++){
				temp = loadedImage->GetRed(i,j) - mean;
				temp = abs(temp);
				minusMeanSum += temp;
			}
		}

		meanDeviation = minusMeanSum/totalPixels;
		printf("%s %d\n", "[INFO] Image Mean Deviation = ", meanDeviation);


		for(int i = x; i<imgW; i++){
			for(int j = y; j<imgH; j++){
				temp = loadedImage->GetRed(i,j) - mean;
				temp = abs(temp);
				temp = pow(temp, 2);
				minusSTDMeanSum += temp;
			}
		}

		variance = minusSTDMeanSum/totalPixels;
		standardDeviation = sqrt(variance);
		printf("%s %d\n", "[INFO] Image Standard Deviation = ", standardDeviation);

		printf("%s\n", "[INFO] Done\n");
	}

/****************************
	SIMPLE THRESHOLDING:- SELECT A THRESHOLD VALUE T 
	FOR EACH PIXEL VALUE > T ASSGIN AS OBJECT ELSE
	ASSGIN AS BACKGROUND. 
************************************/
	void MyFrame::applySimpleThresholding(wxCommandEvent &event){
		printf("%s\n", "Applying Simple Thresholding");

		imageStack->push(loadedImage);
		loadedImage = new wxImage(bitmap.ConvertToImage());

		const int THRESHOLD = 135;


		// GET IMAGE WIDTH AND HEIGHT
		int imgW = (roiFlag) ? roiendX : imgWidth;
		int imgH = (roiFlag) ? roiendY : imgHeight;


		// GET ROI X AND Y CO-ORDINATES
		int x = (roiFlag) ? roistartX: 0;
		int y = (roiFlag) ? roistartY: 0;

		for (int i = x; i < imgW; i++){
			for (int j = y; j < imgH; j++) {
					if(loadedImage->GetRed(i,j)>=THRESHOLD)
						loadedImage->SetRGB(i,j, 255,255,255);
					else
						loadedImage->SetRGB(i,j, 0,0,0);
				}
			}

		Refresh();
		printf("%s\n", "[INFO] Done\n");
	}

/****************************
	AOUTMATED THRESHOLDING:- SELECT AN INITIAL THRESHOLD T AND ITERATIVLEY  
	GO THROUHG THE IMAGE TO FIND BEST VALUE T. 
************************************/
	void MyFrame::applyAutomatedThresholding(wxCommandEvent &event){
		printf("%s\n", "Applying Automated Thresholding");
		imageStack->push(loadedImage);
		loadedImage = new wxImage(bitmap.ConvertToImage());

		
		// GET IMAGE WIDTH AND HEIGHT
		int imgW = (roiFlag) ? roiendX : imgWidth;
		int imgH = (roiFlag) ? roiendY : imgHeight;

		int totalPixels = (roiFlag) ? imgW*imgH : imgWidth*imgHeight;

		// GET ROI X AND Y CO-ORDINATES
		int x = (roiFlag) ? roistartX: 0;
		int y = (roiFlag) ? roistartY: 0;

		
		// INITIAL STEPS
		int w = imgWidth-1; int h = imgHeight-1;
		double backgroundAverage, objectAverage;

		//Get the four corners
		int topLeft = loadedImage->GetRed(0,0);
		int topRight = loadedImage->GetRed(0,h);
		int bottomLeft = loadedImage->GetRed(h,0);
		int bottomRight = loadedImage->GetRed(w,h);
		
		//FIND BACKGROUND AVERAGE
		backgroundAverage = (topLeft+topRight+bottomLeft+bottomRight)/4;
		
		// Find  object Average Intensity
		float initalT, currT, prevT;
		for (int i = 1; i < imgWidth-1; i++)
		{
			for (int j = 1; j < imgHeight-1; j++)
			{
				objectAverage+= loadedImage->GetRed(i,j);
			}
		}
		// GET INITIAL T
		objectAverage = abs(objectAverage/(totalPixels-4));
		initalT = backgroundAverage+objectAverage/2;
		currT = initalT;

		// Iterative Steps, GET A NEW T AND COMPARE TO PREVIOUS T, ONLY WHEN THEY ARE EQUAL STOP
		int bgpixel, objpixels;
		while(!(abs(currT - prevT) < 1)){

			backgroundAverage = objectAverage = 0;
			bgpixel = objpixels = 0;

			for (int i = x; i < imgW; i++){
				for (int j = y; j < imgH; j++) {
								if(loadedImage->GetRed(i,j)>=currT){
									objectAverage += loadedImage->GetRed(i,j);
									objpixels++;
								}
								else{
									backgroundAverage += loadedImage->GetRed(i,j);
									bgpixel++;
								}
							}
						}

				
				// GET THE NEW BACKGROUND AVERAGE
				backgroundAverage = backgroundAverage/bgpixel;

				//GET THE NEW OBJECT AVERAGE
				objectAverage = objectAverage/objpixels;

				// ASSGIN PREVIOUS AND CURRENT T VALUES 
				prevT = currT;
				currT = (backgroundAverage+objectAverage)/2;
		}

		// SET THERSHOLDING USING OPTIMAL T
		for (int i = x; i < imgW; i++){
			for (int j = y; j < imgH; j++) {
					if(loadedImage->GetRed(i,j)>=currT)
							loadedImage->SetRGB(i,j, 255,255,255);
					else
							loadedImage->SetRGB(i,j, 0,0,0);
								
			}
		}
		Refresh();
		printf("%s\n", "[INFO] Done\n");
	}


/****************************
	ADAPTIVE THRESHOLDING:-. MASTERS ONLY 
	NOT YET IMPLEMENTED
************************************/
	void MyFrame::applyAdapetiveThresholding(wxCommandEvent &event){
		printf("%s\n", "[INFO] Applying Adapetive Thresholding");
		printf("%s\n", "[INFO] Done\n");
	}

//###########################################################################################################################################################################//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------- END THRESHOLDING FUNCTIONS ------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//#################################################################################################################################################################################//

//###########################################################################################################################################################################//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------- REGION OF INTEREST  ------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//#################################################################################################################################################################################//



void MyFrame::enableRegionOfInterest(wxCommandEvent &event){
	printf("%s\n", "[INFO] ENABLING REGION OF INTEREST....\n PLEASE SELECT THE REGION OF INTEREST");
	//SET THE ROI FLAF TO TRUE
	roiFlag = true;
}
void MyFrame::disableRegionOfInterest(wxCommandEvent &event){
	printf("%s\n", "[INFO] DISABLING REGION OF INTEREST....\nREGION OF INTEREST OFF");
	// SET THE ROI FLAG TO FALSE 
		roiFlag = false;
		roistartX = roistartY = roiendX = roiendY = 0;
        cout<< "[INFO] startX= "<< roistartX << " startY= "<< roistartY << endl;
		cout<< "[INFO] endX "<< roiendX << " endY= "<< roiendY << endl;
}

//###########################################################################################################################################################################//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------- END REGION OF INTEREST ------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//#################################################################################################################################################################################//





//###########################################################################################################################################################################//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------- UTILITY METHODS ------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//#################################################################################################################################################################################//

/****************************
 IMAGE FILTERING CONVOLUTION
 ************************************/

void MyFrame::onMyConvoluteFilterImageFunc(int mask[][3], int maskWidth, int maskHeight, int filterId, bool rescaling, bool truncating, bool absolute) {
    printf("%s\n", "[INFO] My Filtring Convolution Function");


	// GET IMAGE WIDTH AND HEIGHT
	int imgW = (roiFlag) ? roiendX : imgWidth;
	int imgH = (roiFlag) ? roiendY : imgHeight;
	

	// GET ROI X AND Y CO-ORDINATES
	int k1 = (roiFlag) ? roistartX: 0;
	int l1 = (roiFlag) ? roistartY: 0;


	// MAX AND MIN RGB VALUES
	int rmax,rmin,gmax,gmin,bmax,bmin;

	// MAX AND MIN RGB VALUES IN THE NEIGHBOURHOOD
	int maxRNeighbour,minRNeighbour,maxGNeighbour,minGNeighbour,maxBNeighbour,minBNeighbour;

	// CURRENT MIN AND MAX VALUES, USED IN MIDPOINT FILTERS
	int maxCurrentRed;
	int minCurrentRed;
	int maxCurrentGreen;
	int minCurrentGreen;
	int maxCurrentBlue;
	int minCurrentBlue;

	// CURRENT RGB VALUES
	int currentRed,currentGreen,currentBlue; 

	int r,g,b,x,y;

	int changedR [imgW][imgH];
	int changedG [imgW][imgH];
	int changedB [imgW][imgH];

	int midPointArrayRed [9];
	int midPointArrayGreen [9];
	int midPointArrayBlue [9];

	for (int i = k1; i < imgW; i++) {
		for (int j = l1; j < imgH; j++) {

			// FOR MID POINT FILTERS
			currentRed = loadedImage->GetRed(i,j);
			maxCurrentRed = loadedImage->GetRed(i,j);
			minCurrentRed = loadedImage->GetRed(i,j);

			currentGreen = loadedImage->GetGreen(i,j);
			maxCurrentGreen = loadedImage->GetGreen(i,j);
			minCurrentGreen = loadedImage->GetGreen(i,j);

			currentBlue = loadedImage->GetBlue(i,j);
			maxCurrentBlue = loadedImage->GetBlue(i,j);
			minCurrentBlue = loadedImage->GetBlue(i,j);


			// ITERATE THROUGH THE NEIGHBOURHOOD
			for (int k = 0; k < maskWidth; k++) {
				for (int l = 0; l < maskHeight; l++) {

					x = i - maskWidth/2 + k;
					y = j - maskHeight/2 + l;

					// APPLY EACH RULE TO EACH FILTER
					switch(filterId) {
						case 1:
						// ASSGIN RGB TO MIN VALUES IN THE NEIGHBOURHOOD.
						if(loadedImage->GetRed(x,y) < currentRed)
							currentRed = loadedImage->GetRed(x,y);

						if(loadedImage->GetGreen(x,y) < currentGreen)
							currentGreen = loadedImage->GetGreen(x,y);

						if(loadedImage->GetBlue(x,y) < currentBlue)
							currentBlue = loadedImage->GetBlue(x,y);
						
						r = currentRed;
						g = currentBlue;
						b = currentGreen;

						break;

						case 2:
						// ASSGIN RGB TO MAX VALUES IN THE NEIGHBOURHOOD.
						if(loadedImage->GetRed(x,y) > currentRed)
							currentRed = loadedImage->GetRed(x,y);

						if(loadedImage->GetGreen(x,y) > currentGreen)
							currentGreen = loadedImage->GetGreen(x,y);

						if(loadedImage->GetBlue(x,y) > currentBlue)
							currentBlue = loadedImage->GetBlue(x,y);
						
						r = currentRed;
						g = currentBlue;
						b = currentGreen;

						break;

						case 3:
						// GET MIN AND MAX VALUES IN THE NEIGHBOURHOOD.
						if(loadedImage->GetRed(x,y) > currentRed){
							maxRNeighbour = loadedImage->GetRed(x,y);
							if(maxRNeighbour>maxCurrentRed)
								maxCurrentRed = maxRNeighbour;

						}
						if(loadedImage->GetRed(x,y) < currentRed){
							minRNeighbour = loadedImage->GetRed(x,y);
							if(minRNeighbour<minCurrentRed)
								minCurrentRed = minRNeighbour;
						}

						// Green

						if(loadedImage->GetGreen(x,y) > currentGreen){
							maxGNeighbour = loadedImage->GetGreen(x,y);
							if(maxGNeighbour>maxCurrentGreen)
								maxCurrentGreen = maxGNeighbour;

						}
						if(loadedImage->GetGreen(x,y) < currentGreen){
							minGNeighbour = loadedImage->GetGreen(x,y);
							if(minGNeighbour<minCurrentGreen)
								minCurrentGreen = minGNeighbour;
						}

						// Blue

						if(loadedImage->GetBlue(x,y) > currentBlue){
							maxBNeighbour = loadedImage->GetBlue(x,y);
							if(maxBNeighbour>maxCurrentBlue)
								maxCurrentBlue = maxBNeighbour;

						}
						if(loadedImage->GetBlue(x,y) < currentBlue){
							minBNeighbour = loadedImage->GetBlue(x,y);
							if(minBNeighbour<minCurrentBlue)
								minCurrentBlue = minBNeighbour;
						}

						break;

						case 4:
						// POPULATE MEDIAN ARRAY
						midPointArrayRed[k+l] = loadedImage->GetRed(x,y);
						midPointArrayGreen[k+l] = loadedImage->GetGreen(x,y);
						midPointArrayBlue[k+l] = loadedImage->GetBlue(x,y);
						break;

					}
					
				}

			}


			switch(filterId) {
								case 1:
								//ASSGIN RGB TO MIN VALUES
								changedR[i][j] = r;
								changedG[i][j] = g;
								changedB[i][j] = b;
								break;

								case 2:
								//ASSGIN RGB TO MAX VALUES
								changedR[i][j] = r;
								changedG[i][j] = g;
								changedB[i][j] = b;
								break;

								case 3:
								//ASSGIN RGB TO MIN+MAX/2 VALUES
								changedR[i][j] = (maxCurrentRed + minCurrentRed)/2;
								changedG[i][j] = (maxCurrentGreen + minCurrentGreen)/2;
								changedB[i][j] = (maxCurrentBlue + minCurrentBlue)/2;
								break;

								case 4:
								//SORT THE ARRAY AND SELECT THE MID INDEX VALUE
								std::sort(midPointArrayRed, midPointArrayRed+9);
								std::sort(midPointArrayGreen, midPointArrayGreen+9);
								std::sort(midPointArrayBlue, midPointArrayBlue+9);

								// Find the median 

								changedR[i][j] = midPointArrayRed[5];
								changedG[i][j] = midPointArrayGreen[5];
								changedB[i][j] = midPointArrayBlue[5];

								//reset the array
								for (int i = 0; i < 9; i++){
									midPointArrayRed[i] = 0;
									midPointArrayGreen[i] = 0;
									midPointArrayBlue[i] = 0;
								}	
								
								break;
			}


		}


	}


	for (int i = k1; i < imgW; i++){
		for (int j = l1; j < imgH; j++) {
			loadedImage->SetRGB(i, j, changedR[i][j], changedG[i][j], changedB[i][j]);
		}
	}
}

/****************************
 IMAGE CONVOLUTION
 ************************************/
wxImage* MyFrame::onMyConvoluteImageFunc(int w, int h, int mask[][3], int maskWidth, int maskHeight, int sum, bool rescaling, bool truncating, bool absolute) {
	printf("%s\n", "[INFO] My Convolution Function");
	wxImage* workingImage = new wxImage(w,h);
		// MAX AND MIN RGB VALUES
	int rmax,rmin,gmax,gmin,bmax,bmin; rmax = gmax = bmax = 0; rmin = gmin = bmin = 255;
	int tempR,tempG,tempB;
	int r,g,b,x,y;

	int changedR [w][h];
	int changedG [w][h];
	int changedB [w][h];

		// GET ROI X AND Y CO-ORDINATES
	int e = (roiFlag) ? roistartX: 0;
	int f = (roiFlag) ? roistartY: 0;

	for (int i = e; i < w; i++) {
		for (int j = f; j < h; j++) {
			r=g=b=0;
			for (int k = 0; k < maskWidth; k++) {
				for (int l = 0; l < maskHeight; l++) {

					// apply standard convolution
					x = i - maskWidth/2 + k;
					y = j - maskHeight/2 + l;

					tempR = loadedImage->GetRed(x,y) * mask[k][l];
					tempG = loadedImage->GetGreen(x,y) * mask[k][l];
					tempB = loadedImage->GetBlue(x,y) * mask[k][l];

					r += tempR;
					g += tempG;
					b += tempB;
				}
			}

			r = r/sum;
			g = g/sum;
			b = b/sum;

			changedR[i][j] = r;
			changedG[i][j] = g;
			changedB[i][j] = b;
		}

	}
	

	// Find Min and Max Values
	if(rescaling) {
		for (int i = e; i < w; i++)
		{
			for (int j = f; j < h; j++)
			{
				setMaxAndMinRGB(changedR[i][j], rmax, rmin, changedG[i][j], gmax, gmin, changedB[i][j], bmax, bmin);
			}
		}
	}


	for (int i = e; i < w; i++){
		for (int j = f; j < h; j++) {
			if(rescaling){
				r = rescale(changedR[i][j], rmax, rmin); 
				g = rescale(changedG[i][j], gmax, gmin);
				b = rescale(changedB[i][j], bmax, bmin);
				workingImage->SetRGB(i, j, r,g,b);
			}
			else if(absolute){
					absoluteValue(changedR[i][j], changedG[i][j], changedB[i][j]);
						if(truncating)
							truncate(changedR[i][j], changedG[i][j], changedB[i][j]);

					workingImage->SetRGB(i, j, changedR[i][j], changedG[i][j], changedB[i][j]);
			}
			else{
				truncate(changedR[i][j], changedG[i][j], changedB[i][j]);
				workingImage->SetRGB(i, j, changedR[i][j], changedG [i][j], changedB[i][j]);
			}
		}
	}
	printf("[INFO] Finished My Convolution Function.\n");
	return workingImage;
}

/****************************
 PRINT PIXEL VALUES 
 ************************************/
void MyFrame::showConvImage(wxImage* finalImage, int w, int h, int x, int y){
	for (int i = x; i < w; i++){
		for (int j = y; j < h; j++) {
				loadedImage->SetRGB(i, j, finalImage->GetRed(i,j), finalImage->GetGreen(i,j), finalImage->GetBlue(i,j));
		}
	}
}

/****************************
 ON MOUSE EVENT LISTENER:- 
 RECORDS MOUSES' X AND Y COORDINATES 
 ************************************/

void MyFrame::OnMouseEvent(wxMouseEvent &event) {
	//CREATE THE PAINT OBJECT
	wxPaintDC rectPaint(this);
	rectPaint.SetBrush(*wxTRANSPARENT_BRUSH); 
	rectPaint.SetPen( wxPen(wxColor(127,0,255), 1 ));

	// WHILE THE REGION FLAG IS SET TO TRUE
	if (roiFlag) {
	    if (event.LeftDown()) {
	    	// GET INITIAL X AND Y POSITIONS OF CURSOR
	        roistartX = event.GetX();
	        roistartY = event.GetY();

	        roiendX = roistartX;
	       	roiendY = roistartY;

	       	Refresh();
	    }
	        
	    if (event.Dragging()) {
	       // UPDATE THE END X AND Y POSIYIONS OF THE CURSORS AND DRAW ROI RECTANGLE
	       roiendX = event.GetX();
	       roiendY = event.GetY();
	       rectPaint.DrawRectangle(roistartX, roistartY,(roiendX-roistartX),(roiendY-roistartY));
	       Refresh();
	    }

			if(event.LeftUp()){

				if(roistartX > roiendX){
					int temp = roistartX;
					roistartX = roiendX;
					roiendX = temp;
				}

				if(roistartY > roiendY){
					int temp = roistartY;	
					roistartY = roiendY;
					roiendY = temp;
				}
				
				//ON MOUSE RELEASE DRAW THE FINAL RECTANGLE
				rectPaint.DrawRectangle(roistartX, roistartY,(roiendX-roistartX),(roiendY-roistartY));
				printf("[INFO] ROI: start X= %d start Y= %d endX = %d endy= %d\n", roistartX, roistartY, roiendX, roiendY);

			}

	    }
}


/****************************
 TRUNCATE PIXEL VALUES:- 
 TRUNCATE THE PIXEL VALUES THAT ARE ABOVE 255 AND 
 BELOW 0. 
 ************************************/

void MyFrame::truncate(int &r, int &g, int &b){
	if(r > 255 || r <0){
		if(r > 255)
			r = 255;
				else
					r = 0;
			}	

				if(g > 255 ||g <0){
					if(g > 255)
						g = 255;
					else
						g = 0;	
				}

				if(b > 255 || b <0){
					if(b > 255)
						b = 255;
					else
						b = 0;
				}
}

/****************************
 RESCALE PIXEL VALUES
 ************************************/

int MyFrame::rescale(int pv, int max, int min){
	int rescaledPv = 255*(pv - max)/(max-min);
	return rescaledPv;
}

/****************************
 ABSOULTE PIXEL VALUES
 ************************************/

void MyFrame::absoluteValue(int &r, int &g, int &b){
	r = abs(r);
	g = abs(g);
	b = abs(b);
}

/****************************
 SET THE MAX AND MIN RGB VALUES OF THE 
 IMAGE 
 ************************************/
void MyFrame::setMaxAndMinRGB(int r, int &rmax, int &rmin, int g, int &gmax, int &gmin, int b, int &bmax, int &bmin){
					if(r > rmax) rmax = r;
					if(r < rmin) rmin = r;
					
					if(g > gmax) gmax = g;
					if(g < gmin) gmin = g;

					if(b > bmax) bmax = b;
					if(b < bmin) bmin = b;
}
/****************************
	 MY FUNCTION:- PRINT THE RGB VALUES OF EACH PIXEL OF THE LOADED IMAGE
************************************/
void MyFrame::OnMyFunctionImage(wxCommandEvent & event) {

	printf("[INFO] My function...");
	loadedImage = new wxImage(bitmap.ConvertToImage());

	unsigned char r, g, b;

	for (int i = 0; i < imgWidth; i++)
		for (int j = 0; j < imgHeight; j++) {
			// GET THE RGB VALUES
			r = loadedImage->GetRed(i, j);   // red pixel value
			g = loadedImage->GetGreen(i, j); // green pixel value
			b = loadedImage->GetBlue(i, j); // blue pixel value

			printf("(%d,%d) [r = %x  | g = %x | b = %x] \n", i, j, r, g, b);

			// SAVE THE RGB VALUES
			loadedImage->SetRGB(i, j, r, g, b);
		}

	printf("[INFO] Finished My function.\n\n");
	Refresh();
}

	/****************************
	 Undo Function 
	
	 ************************************/
void MyFrame::onUndo(wxCommandEvent & event) {
	printf("[INFO] Undo.\n");
	if (!imageStack->empty())
	{
		loadedImage = imageStack->top();
		imageStack->pop();
	}
	printf("[INFO] Finished undoing.\n\n");
	Refresh();
}

	/****************************
	 TEST FUNCTION:- 
	
	 ************************************/
void MyFrame::onTestImage(wxCommandEvent & event) {
	printf("[INFO] Called onTestImage.\n");
	printf("[INFO] Finished My function.\n\n");
	Refresh();
}


//###########################################################################################################################################################################//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//---------- END UTILITY METHODS ------------------------------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//#################################################################################################################################################################################//



//###########################################################//
//-----------------------------------------------------------//
//---------- DO NOT MODIFY THE CODE BELOW--------------------//
//-----------------------------------------------------------//
//###########################################################//

//IMAGE SAVING
void MyFrame::OnSaveImage(wxCommandEvent & event) {

	wxFileDialog *SaveDialog = new wxFileDialog(this, _T("Save File As _?"), wxEmptyString, wxEmptyString,
		_T("PNG files (*.png)|*.png|JPEG Files (*.jpeg)|*.jpeg|Bitmap (*.bmp)|*.bmp"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
 
	// Creates a Save Dialog with 4 file types
	if (SaveDialog->ShowModal() == wxID_OK) // If the user clicked "OK"
	{
		wxString CurrentDocPath = SaveDialog->GetPath();
		// set the path of our current document to the file the user chose to save under
		loadedImage->SaveFile(CurrentDocPath); // Save the file to the selected path

	}
 
	// Clean up after ourselves
	SaveDialog->Destroy();
	printf("Finished Saving.\n\n");
}

// ON EXIT
void MyFrame::OnExit(wxCommandEvent & event) {
	Close (TRUE);
}

// PAINT THE IMAGE TO THE FRAME
void MyFrame::OnPaint(wxPaintEvent & event) {
	wxPaintDC dc(this);
	int cWidth, cHeight;
	GetSize(&cWidth, &cHeight);
	if ((back_bitmap == NULL) || (oldWidth != cWidth)
			|| (oldHeight != cHeight)) {
		if (back_bitmap != NULL)
			delete back_bitmap;
		back_bitmap = new wxBitmap(cWidth, cHeight);
		oldWidth = cWidth;
		oldHeight = cHeight;
	}
	wxMemoryDC *temp_dc = new wxMemoryDC(&dc);
	temp_dc->SelectObject(*back_bitmap);
	// We can now draw into the offscreen DC...
	temp_dc->Clear();
	if (loadedImage)
		temp_dc->DrawBitmap(wxBitmap(*loadedImage), 0, 0, false); //given bitmap xcoord y coord and transparency

	switch (stuffToDraw) {
	case NOTHING:
		break;
	case ORIGINAL_IMG:
		bitmap.CleanUpHandlers; // clean the actual image header
		bitmap = wxBitmap(*loadedImage); // Update the edited/loaded image
		break;
	}

// update image size
	imgWidth = (bitmap.ConvertToImage()).GetWidth();
	imgHeight = (bitmap.ConvertToImage()).GetHeight();

	temp_dc->SelectObject(bitmap); //given bitmap

	//end draw all the things
	// Copy from this DC to another DC.
	dc.Blit(0, 0, cWidth, cHeight, temp_dc, 0, 0);
	delete temp_dc; // get rid of the memory DC
}


// EVENT TABLE BELOW

//#########################################################################################################################################//

BEGIN_EVENT_TABLE (MyFrame, wxFrame)

//###########################################################//
//---------------------- FILE MENU EVENTS ---------------------//
//###########################################################//

EVT_MENU ( LOAD_FILE_ID, MyFrame::OnOpenFile)
EVT_MENU ( EXIT_ID, MyFrame::OnExit)
EVT_MENU ( LOAD_RAW_FILE_ID, MyFrame::onOpenRawFile)

//###########################################################//
//---------------------- EDIT MENU EVENTS ---------------------//
//###########################################################//

EVT_MENU ( UNDO_ID, MyFrame::onUndo)

// Provided Events
EVT_MENU ( INVERT_IMAGE_ID, MyFrame::OnInvertImage)
EVT_MENU ( SCALE_IMAGE_ID, MyFrame::OnScaleImage)
EVT_MENU ( SAVE_IMAGE_ID, MyFrame::OnSaveImage)


//My Events

// Lab 4 
EVT_MENU ( MY_IMAGE_ID, MyFrame::OnMyFunctionImage)//--->To be modified!
EVT_MENU ( MYTEST_IMAGE_ID, MyFrame::onTestImage)//--->To be modified!


// Lab 5 
EVT_MENU ( MY_SCALE_IMAGE_ID, MyFrame::OnMyScaleImageFunc) // My Scale Image.

// Lab 6 
EVT_MENU ( MY_SHIFT_IMAGE_ID, MyFrame::onMyShiftImageFunc) // My shift Image.
//EVT_MENU ( MY_CONVOLUTE_IMAGE_ID, MyFrame:: onMyConvoluteImageFunc) // My convolute Image.


//###########################################################//
//---------------------- CONVOLUTION MENU EVENTS ---------------------//
//###########################################################//

EVT_MENU ( MY_AVERAGING_CONVOLUTION_ID, MyFrame::onMyAveragingConvolution)
EVT_MENU ( MY_WEIGHTED_AVERAGING_CONVOLUTION_ID, MyFrame::onMyWeightedAveragingConvolution)

EVT_MENU ( MY_FOUR_NEIGHBOUR_LAPLACIAN_CONVOLUTION_ID, MyFrame::onMyFourNeighbourLaplacianConvolution)
EVT_MENU ( MY_EIGHT_NEIGHBOUR_LAPLACIAN_CONVOLUTION, MyFrame::onMyEightNeighbourLaplacianConvolution)

EVT_MENU ( MY_FOUR_NEIGHBOUR_LAPLACIAN_ENHANCMENT_CONVOLUTION_ID, MyFrame::onMyFourNeighbourLaplacianEnhancmentConvolution)
EVT_MENU ( MY_EIGHT_NEIGHBOUR_LAPLACIAN_ENHANCMENT_CONVOLUTION_ID, MyFrame::onMyEightNeighbourLaplacianEnhancmentConvolution)

EVT_MENU ( MY_ROBERTS_CONVOLUTION_ID, MyFrame::onMyRobertsConvolution)

EVT_MENU ( MY_SOBEL_CONVOLUTION_ID, MyFrame::onMySobel)
EVT_MENU ( MY_SOBEL_X_CONVOLUTION_ID, MyFrame::onMySobelXConvolution)
EVT_MENU ( MY_SOBEL_Y_CONVOLUTION_ID, MyFrame::onMySobelYConvolution)


//###########################################################//
//---------------------- FILTERING MENU EVENTS ---------------------//
//###########################################################//

EVT_MENU ( MY_SALT_AND_PEPPER_ADDER_ID, MyFrame::onMySaltAndPepperAdder)

EVT_MENU ( MY_MIN_FILTER_ID, MyFrame::onMyMinFilter)
EVT_MENU ( MY_MAX_FILTER_ID, MyFrame::onMyMaxFilter)
EVT_MENU ( MY_MID_POINT_FILTER_ID, MyFrame::onMyMidPointFilter)
EVT_MENU ( MY_MEDIAN_FILTER_ID, MyFrame::onMyMedianFilter)


//###########################################################//
//---------------------- POINT PROCESSING MENU EVENTS ---------------------//
//###########################################################/

EVT_MENU ( MY_NEGATIVE_LINEAR_PROCESSING_ID, MyFrame::applyNegativeLinearProcessing)
EVT_MENU ( MY_LOGARITHMIC_PROCESSING_ID, MyFrame::applyLogarithmicProcessing)
EVT_MENU ( MY_POWER_LAW_PROCESSING_ID, MyFrame::applyPowerLawProcessing)
EVT_MENU ( MY_RANDOM_LOOKUP_PROCESSING_ID, MyFrame::applyRandomLookUpProcessing)


//###########################################################//
//---------------------- HISTOGRAM MENU EVENTS ---------------------//
//###########################################################/

EVT_MENU ( MY_FIND_HISTOGRAM_ID, MyFrame::findHistogram)
EVT_MENU ( MY_NORMALISE_HISTOGRAM_ID, MyFrame::normaliseHistogram)
EVT_MENU ( MY_EQUALISE_HISTOGRAM_ID, MyFrame::equaliseHistogram)
EVT_MENU ( MY_DISPLAY_HISTOGRAM_ID, MyFrame::displayHistogram)

//###########################################################//
//---------------------- THRESHOLDING MENU EVENTS ---------------------//
//###########################################################/

EVT_MENU ( MY_FIND_MEAN_STD_ID, MyFrame::findMeanAndStandardDeviation)
EVT_MENU ( MY_SIMPLE_THRESHOLDING_ID, MyFrame::applySimpleThresholding)
EVT_MENU ( MY_AUTOMATED_THRESHOLDING_ID, MyFrame::applyAutomatedThresholding)
EVT_MENU ( MY_ADAPETIVE_THRESHOLDING_ID, MyFrame::applyAdapetiveThresholding)



//###########################################################//
//---------------------- ROI MENUE EVENTS ---------------------//
//###########################################################/

EVT_MENU ( MY_ENABLE_ROI_ID, MyFrame::enableRegionOfInterest)
EVT_MENU ( MY_DISABLE_ROI_ID, MyFrame::disableRegionOfInterest)

EVT_MOUSE_EVENTS(MyFrame::OnMouseEvent)

EVT_PAINT (MyFrame::OnPaint)

//###########################################################//
//----------------------END MY EVENTS -----------------------//
//###########################################################//
END_EVENT_TABLE()

//#########################################################################################################################################//
