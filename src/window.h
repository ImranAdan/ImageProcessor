class BasicApplication: public wxApp {
public:
	virtual bool OnInit();
};

class MyFrame: public wxFrame {
protected:

	wxMenuBar *menuBar; //main menu bar

	/****************************
	 CREATE A FEW MENUS
	 ************************************/

	wxMenu *fileMenu; //file menu
	wxMenu *editMenu; //edit menu
	wxMenu *imageConvolutionMenu; // image convolution menu
	wxMenu *imageFilteringMenu; //image filtering menu
	wxMenu *pointProcessingMenu; //point processing menu
	wxMenu *histogramMenu; // histogram menu.
	wxMenu *thresholdMenu; // threshold menu.
	wxMenu *roiMenu;






	/****************************
		ROI VARAIABLES
	 ************************************/
	 bool roiFlag;
	 int roistartX;
	 int roistartY;
	 int roiendX;
	 int roiendY;

	/****************************
	 NO NEED TO MODIFY YET!
	 ************************************/

	wxBitmap *back_bitmap; // offscreen memory buffer for drawing
	wxToolBar *toolbar; //tollbar not necessary to use
	int oldWidth, oldHeight; // save old dimensions

	wxBitmap bitmap;  //structure for the edited image
	wxImage *loadedImage; // image loaded from file
	int imgWidth, imgHeight; // image dimensions
	int stuffToDraw;



	/****************************
	UTILITY FUNCTIONS
	 ************************************/
	void truncate(int &r, int &g, int &b);
	int rescale(int pv, int max, int min);
	void absoluteValue(int &r, int &g, int &b);
	void setMaxAndMinRGB(int r, int &rmax, int &rmin, int g, int &gmax, int &gmin, int b, int &bmax, int &bmin);

//	void setPixels(int** rs, int** gs, int** bs, int initalI, int initalJ, int loopw, int looph)

	/****************************

	 FILE Menu Message Handlers:
	 1) Open from RAW
	 ************************************/

	// File:- Image Opening Functions
	void onOpenRawFile(wxCommandEvent & event);
	void OnSaveImage(wxCommandEvent & event);


	
	/****************************

	 Edit Menu Message Handlers:
	 1) Open from RAW
	 ************************************/
	void onUndo(wxCommandEvent & event);
	void OnInvertImage(wxCommandEvent & event);
	void OnScaleImage(wxCommandEvent & event);
	// Lab 4	
	void OnMyFunctionImage(wxCommandEvent & event); //---> To be modified!
	void onTestImage(wxCommandEvent & event);
	//Lab 5 
	void OnMyScaleImageFunc(wxCommandEvent & event); // SCALE IMAGE	
	// Lab 6 
	void onMyShiftImageFunc(wxCommandEvent & event); // SHIFT IMAGE



	/****************************

	 Image Convolution Menu Message Handlers:
	 ************************************/



	//Averaging
	void onMyAveragingConvolution(wxCommandEvent & event); // Apply Average Convolution 
	void onMyWeightedAveragingConvolution(wxCommandEvent & event); // Apply Weighted Average Convolution

	//Laplacian
	void onMyFourNeighbourLaplacianConvolution(wxCommandEvent & event); // Apply 4-Neighbour Laplacian Convolution
	void onMyEightNeighbourLaplacianConvolution(wxCommandEvent & event); // Apply 8-Neighbour Laplacian Convolution

	//Laplacian Enhancment
	void onMyFourNeighbourLaplacianEnhancmentConvolution(wxCommandEvent & event); // Apply 4-Neighbour Laplacian Enhancment Convolution
	void onMyEightNeighbourLaplacianEnhancmentConvolution(wxCommandEvent & event); // Apply 8-Neighbour Laplacian Enhancment Convolution

	//Roberts
	void onMyRobertsConvolution(wxCommandEvent & event); // Apply Roberts Convolution

	//Sobel
	void onMySobel(wxCommandEvent & event); // Apply Roberts with Absolute value Convolution
	void onMySobelXConvolution(wxCommandEvent & event); // Apply SobelX with absolute value Convolution
	void onMySobelYConvolution(wxCommandEvent & event); // Apply SobelY with absolute value Convolution

	// APPLY CONVOLUTION
	wxImage* onMyConvoluteImageFunc(int w, int h, int mask[][3], int maskWidth, int maskHeight, int sum, bool rescaling, bool truncating, bool absolute);

	// PRINT CONVOLUTED SECTION
	void showConvImage(wxImage* finalImage, int w, int h, int x, int y);


	/****************************
	FILTERING FUNCTIONS
	 ************************************/
	
	void onMySaltAndPepperAdder(wxCommandEvent &event); // ADD SALT AND PEPPER
	void onMyMinFilter(wxCommandEvent &event); //APPLY MIN FILTER
	void onMyMaxFilter(wxCommandEvent &event); //APPLY MAX FILTER 
	void onMyMidPointFilter(wxCommandEvent &event); //APPLY MID POINT FILTER
	void onMyMedianFilter(wxCommandEvent &event);// APPLY MEDIAN FILTER

	// APPLY CONVOLUTION
	void onMyConvoluteFilterImageFunc(int mask[][3], int maskWidth, int maskHeight, int maskId , bool rescaling, bool truncating, bool absolute);


	/****************************
	POINT PROCESSING FUNCTIONS
	 ************************************/
	void applyNegativeLinearProcessing(wxCommandEvent &event);
	void applyLogarithmicProcessing(wxCommandEvent &event);
	void applyPowerLawProcessing(wxCommandEvent &event);
	void applyRandomLookUpProcessing(wxCommandEvent &event);


	/****************************
	HISTOGRAM FUNCTIONS
	 ************************************/

	void findHistogram(wxCommandEvent &event);
	void normaliseHistogram(wxCommandEvent &event);
	void equaliseHistogram(wxCommandEvent &event);
	void displayHistogram(wxCommandEvent &event);

	/****************************
	THRESHOLD FUNCTIONS
	 ************************************/
	void findMeanAndStandardDeviation(wxCommandEvent &event);
	void applySimpleThresholding(wxCommandEvent &event);
	void applyAutomatedThresholding(wxCommandEvent &event);
	void applyAdapetiveThresholding(wxCommandEvent &event);


	/****************************
	REGION OF INTEREST FUNCTIONS
	 ************************************/
	void enableRegionOfInterest(wxCommandEvent &event);
	void disableRegionOfInterest(wxCommandEvent &event);
	void OnMouseEvent(wxMouseEvent &event);

public:

	/****************************
	 CONSTRUCTOR
	 ************************************/

	MyFrame(const wxString title, int xpos, int ypos, int width, int height);
	virtual ~MyFrame();

	/****************************
	 MEMBER FUNCTIONS
	 ************************************/
	void OnExit(wxCommandEvent & event);
	void OnOpenFile(wxCommandEvent & event);
	void OnPaint(wxPaintEvent & event);

	DECLARE_EVENT_TABLE()

};

enum {
	NOTHING = 0, ORIGINAL_IMG,
};

enum {




	/****************************
	 ENUMS CORRESPOND TO CALLED EVENTS/FUNCTIONS
	 ************************************/

	// File Opening
	EXIT_ID = wxID_HIGHEST + 1,
	LOAD_FILE_ID,
	LOAD_RAW_FILE_ID,


	// Edit
	UNDO_ID,
	INVERT_IMAGE_ID,
	SCALE_IMAGE_ID,
	SAVE_IMAGE_ID,
		// Lab 4
	MY_IMAGE_ID, //--->To be modified!
	MYTEST_IMAGE_ID,
		// Lab 5 
	MY_SCALE_IMAGE_ID,
	MY_SHIFT_IMAGE_ID,



	// Image Convolution
	//Lab 6
	MY_CONVOLUTE_IMAGE_ID,
	MY_AVERAGING_CONVOLUTION_ID,
	MY_WEIGHTED_AVERAGING_CONVOLUTION_ID,
	MY_FOUR_NEIGHBOUR_LAPLACIAN_CONVOLUTION_ID,
	MY_EIGHT_NEIGHBOUR_LAPLACIAN_CONVOLUTION,
	MY_FOUR_NEIGHBOUR_LAPLACIAN_ENHANCMENT_CONVOLUTION_ID,
	MY_EIGHT_NEIGHBOUR_LAPLACIAN_ENHANCMENT_CONVOLUTION_ID,
	MY_ROBERTS_CONVOLUTION_ID,
	MY_SOBEL_CONVOLUTION_ID,
	MY_SOBEL_X_CONVOLUTION_ID,
	MY_SOBEL_Y_CONVOLUTION_ID,


	//Image Filtering 
	MY_SALT_AND_PEPPER_ADDER_ID, 
	MY_MIN_FILTER_ID,
	MY_MAX_FILTER_ID,
	MY_MID_POINT_FILTER_ID,
	MY_MEDIAN_FILTER_ID,

	//Point Processing 
	MY_NEGATIVE_LINEAR_PROCESSING_ID, 
	MY_LOGARITHMIC_PROCESSING_ID,
	MY_POWER_LAW_PROCESSING_ID,
	MY_RANDOM_LOOKUP_PROCESSING_ID,

	//HISTOGRAM 
	MY_FIND_HISTOGRAM_ID, 
	MY_NORMALISE_HISTOGRAM_ID,
	MY_EQUALISE_HISTOGRAM_ID,
	MY_DISPLAY_HISTOGRAM_ID,

	//THRESHOLDING
	MY_FIND_MEAN_STD_ID, 
	MY_SIMPLE_THRESHOLDING_ID,
	MY_AUTOMATED_THRESHOLDING_ID,
	MY_ADAPETIVE_THRESHOLDING_ID,

	//Region of Interest
	MY_ENABLE_ROI_ID,
	MY_DISABLE_ROI_ID
	
};
