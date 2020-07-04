#pragma once

#include "ofMain.h"
#include <list>

//typedef struct pointer {
//	double sx;
//	double sy;
//	double x;
//	double y;
//	double acc = 1; 
//};
//
//class Water {
//public:
//	pointer S;
//	pointer E;
//	double gravity = 1;
//	
//	bool run;
//	bool erase;

//};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	/* WaterFall-related member variables Regions */

	// flag variables
	int draw_flag;
	int load_flag;
	int fall_flag;
	int erase_flag;

	// Line segment and dot related variables
	int num_of_line, num_of_dot;
	float dot_diameter;

	typedef struct pointer {
		double sx;
		double sy;
		double x;
		double y;
		double acc = 1;
	};

	typedef struct Water {
	public:
		pointer S;
		pointer E;
		double gravity = 1;

		bool run;
		bool erase;

		int code;
	};



	typedef struct Line {
		int x1;
		int y1;
		int x2;
		int y2;
		double grad;
	};
	typedef struct Dot {
		double x;
		double y;
		bool run;
		bool erase;
		int code;
	};
	Line* lines;
	Dot* dots;
	int cDots;
	int codeNum;
	double height;
	double maxheight;

	Water* water;
	void processOpenFileSelection(ofFileDialogResult openFileResult);
	void initializeWaterLines(); // 2nd week portion.
	int checkLine(Water, bool);

	ofImage image;

	list<Water> List;
	void makeNewWater();
	void NewinitializeWaterLines(Water& w);

};


