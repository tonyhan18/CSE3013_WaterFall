#include "ofApp.h"
#define dis 2
#define plus 0.001
//initialize work

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(15); // Limit the speed of our program to 15 frames per second

						// We still want to draw on a black background, so we need to draw
						// the background before we do anything with the brush
	ofBackground(255, 255, 255);
	ofSetLineWidth(4);

	draw_flag = 0;
	load_flag = 0;
	dot_diameter = 20.0f;
	cDots = 0;
	erase_flag = 0;
	fall_flag = 0;

	//물의 번호를 매기는 변수
	codeNum = 0;

	//물이 차오른 높이를 나타내는 변수
	height = 40;
	maxheight = 40;

	ofSetBackgroundAuto(false);
}

//--------------------------------------------------------------
void ofApp::update() {
	ofSetColor(127, 23, 31);  // Set the drawing color to brown

							  // Draw shapes for ceiling and floor
	ofDrawRectangle(0, 0, ofGetWidth(), 40); // Top left corner at (50, 50), 100 wide x 100 high
	ofDrawRectangle(0, 728, ofGetWidth(), 40); // Top left corner at (50, 50), 100 wide x 100 high


	if (draw_flag) {
		ofSetLineWidth(4);

		for (int i = ofGetHeight() - 40; i > (ofGetHeight() - maxheight) && i > 40; i -= 4) {
			if (i < ofGetHeight() - height) {
				ofSetColor(255);
			}
			else {
				ofSetColor(0, 0, 255);
			}
			ofDrawLine(0, i, ofGetWidth(), i);
			cout << "height : " << height << endl;
			cout << "maxheight : " << maxheight << endl;
		}


		/* Draw Star */
		ofSetColor(255);
		ofDrawLine(ofGetWidth() / 2 - 40, 80, ofGetWidth() / 2, 120);
		ofDrawLine(ofGetWidth() / 2, 120, ofGetWidth() / 2 + 40, 120);
		ofDrawLine(ofGetWidth() / 2 + 40, 120, ofGetWidth() / 2, 160);
		ofDrawLine(ofGetWidth() / 2, 160, ofGetWidth() / 2 + 40, 200);
		ofDrawLine(ofGetWidth() / 2 + 40, 200, ofGetWidth() / 2 - 40, 160);

		ofDrawLine(ofGetWidth() / 2 - 40, 80, ofGetWidth() / 2 - 80, 120);
		ofDrawLine(ofGetWidth() / 2 - 80, 120, ofGetWidth() / 2 - 120, 120);
		ofDrawLine(ofGetWidth() / 2 - 120, 120, ofGetWidth() / 2 - 80, 160);
		ofDrawLine(ofGetWidth() / 2 - 80, 160, ofGetWidth() / 2 - 120, 200);
		ofDrawLine(ofGetWidth() / 2 - 120, 200, ofGetWidth() / 2 - 40, 160);

		ofDrawCircle(ofGetWidth() / 2 - 60, 120, 10);
		ofDrawCircle(ofGetWidth() / 2 - 20, 120, 10);


		for (int i = 0; i < num_of_dot; ++i) {
			if (cDots == i) ofSetColor(255, 0, 0);
			else ofSetColor(0);

			if (dots[i].x<0 || dots[i].x>ofGetWidth()) continue;
			if (dots[i].y<0 || dots[i].y>ofGetHeight()) continue;

			ofDrawCircle(dots[i].x, dots[i].y, dot_diameter);
		}

		ofSetColor(127, 23, 31);
		for (int i = 0; i < num_of_line; ++i) {
			if (lines[i].x1<0 || lines[i].x1>ofGetWidth()) continue;
			if (lines[i].x2<0 || lines[i].x2>ofGetWidth()) continue;
			if (lines[i].y1<0 || lines[i].y1>ofGetHeight()) continue;
			if (lines[i].y2<0 || lines[i].y2>ofGetHeight()) continue;

			ofDrawLine(lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2);
		}

	}



}

//--------------------------------------------------------------
int ofApp::checkLine(Water w, bool RE) {
	for (int i = 0; i < num_of_line; ++i) {
		if (RE == true) {
			if (lines[i].x1 < w.S.x && w.S.x < lines[i].x2 && lines[i].y1 < w.S.y && w.S.y < lines[i].y2) return i;
			if (lines[i].x1 < w.S.x && w.S.x < lines[i].x2 && lines[i].y1 > w.S.y&& w.S.y > lines[i].y2) return i;
		}
		if (RE == false) {
			if (lines[i].x1 < w.E.x && w.E.x < lines[i].x2 && lines[i].y1 < w.E.y && w.E.y < lines[i].y2) return i;
			if (lines[i].x1 < w.E.x && w.E.x < lines[i].x2 && lines[i].y1 > w.E.y&& w.E.y > lines[i].y2) return i;
		}

	}
	return -1;
}

//--------------------------------------------------------------
void ofApp::draw() {
	/*Fall Water*/
	for (list<Water>::iterator iter = List.begin(); iter != List.end(); ++iter) {
		int l;
		double y = 0;
		ofSetLineWidth(4);

		/*Water가 운용중일 때*/
		if (iter->run) {
			ofSetColor(0, 0, 255);
			/*우선적으로 그림 그리기*/

			if (iter->S.y <= ofGetHeight() - height + 5) {
				/*cout << "Selcted Water Draw Current Point is (" << iter->S.x << ", " << iter->S.y << ")" << endl;*/
				ofDrawLine(iter->S.sx, iter->S.sy, iter->S.x, iter->S.y);


				iter->S.sx = iter->S.x;
				iter->S.sy = iter->S.y;

				/*다음에 올 라인 위치 결정하기*/
				//박스 안에 있을 경우
				if ((l = checkLine(*iter, true)) != -1) {

					y = lines[l].grad * (iter->S.x - lines[l].x2) + lines[l].y2;
					if (iter->S.acc == 1) iter->S.acc = (lines[l].grad < 0) ? -iter->S.acc : iter->S.acc;

					if (y - iter->S.y < 9.8) {
						iter->S.x += iter->S.acc;
						iter->S.acc += (lines[l].grad * 3) / (iter->gravity);

						if ((y - iter->S.y < 4))y = lines[l].grad * (iter->S.x - lines[l].x2) + lines[l].y2;
						while (y - iter->S.y > dis) {
							iter->S.y += plus;
						}
					}
					else {
						iter->S.y += iter->gravity;
						iter->S.acc = 1;
					}
				}
				//박스 외부에 있을 경우
				else {
					iter->S.y += iter->gravity;
					iter->S.acc = 1;
				}
			}
			else if (height <= ofGetHeight() - height + 5) {
				height += 0.5;
				maxheight = height;

			}
			//if (iter->S.y > ofGetHeight() && iter->erase) iter->run = false;

		}


		/*삭제모드일때*/
		if (iter->erase) {
			ofSetColor(255);
			/*우선적으로 그림 그리기*/

			if (iter->E.y <= ofGetHeight() - height + 10) {
				//cout << "Selcted Water Erase Current Point is (" << iter->E.x << ", " << iter->E.y << ")" << endl;
				ofDrawLine(iter->E.sx, iter->E.sy, iter->E.x, iter->E.y);


				iter->E.sx = iter->E.x;
				iter->E.sy = iter->E.y;


				/*다음에 올 라인 위치 결정하기*/
				//박스 안에 있을 경우
				if ((l = checkLine(*iter, false)) != -1) {

					y = lines[l].grad * (iter->E.x - lines[l].x2) + lines[l].y2;
					if (iter->E.acc == 1) iter->E.acc = (lines[l].grad < 0) ? -iter->E.acc : iter->E.acc;
					//&& y - iter->S.y>1
					if (y - iter->E.y < 9.8) {


						iter->E.x += iter->E.acc;
						iter->E.acc += (lines[l].grad * 3) / (iter->gravity);

						if ((y - iter->E.y < 4)) y = lines[l].grad * (iter->E.x - lines[l].x2) + lines[l].y2;
						while (y - iter->E.y > dis) {
							iter->E.y += plus;
						}
					}
					else {
						iter->E.y += iter->gravity;
						iter->E.acc = 1;
					}
				}
				//박스 외부에 있을 경우
				else {
					iter->E.y += iter->gravity;
					iter->E.acc = 1;
				}
			}
			else {
				iter->run = false;
				iter->erase = false;
				iter = List.erase(iter);

				if (List.empty()) {
					//cout << "empty" << endl;
					break;
				}
				else iter = List.begin();
			}
		}
	}



	if (height > 40) {
		maxheight -= 0.32;
		height -= 0.4;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'v') {
		// HACK: only needed on windows, when using ofSetAutoBackground(false)
		glReadBuffer(GL_FRONT);
		ofSaveScreen("savedScreenshot_" + ofGetTimestampString() + ".png");
	}
	if (key == 'q') {
		// Reset flags
		draw_flag = 0;

		// Free the dynamically allocated memory exits.
		cout << "Dynamically allocated memory has been freed." << endl;
		free(lines);
		free(dots);

		_Exit(0);
	}
	if (key == 'd') {
		if (!load_flag) return;

		for (int i = 0; i < num_of_dot; ++i) {
			cout << "Selcted Dot Coordinate is (" << dots[i].x << ", " << dots[i].y << ")" << endl;
		}

		draw_flag = 1;
		draw();
	}
	if (key == 's') {
		// 2nd week portion.
		if (!load_flag) return;

		/*현재 호수가 작동중이지 않을 경우에만 새로운 물을 제작*/
		if (!dots[cDots].run) {
			dots[cDots].erase = false;
			dots[cDots].run = true;
			makeNewWater();
		}
	}
	if (key == 'e') {
		// 2nd week portion.
		if (!load_flag) return;

		if (!dots[cDots].erase) {
			dots[cDots].run = false;
			dots[cDots].erase = true;

			for (list<Water>::iterator iter = List.begin(); iter != List.end(); ++iter) {
				if (iter->code == dots[cDots].code) {
					iter->erase = true;
					break;
				}
			}

		}

	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == 'l') {
		// Open the Open File Dialog
		ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a only txt for Waterfall");

		// Check whether the user opened a file
		if (openFileResult.bSuccess) {
			ofLogVerbose("User selected a file");

			// We have a file, so let's check it and process it
			processOpenFileSelection(openFileResult);
			load_flag = 1;
		}
	}

	/* COMSIL1-TODO 4: This is selection dot control part.
	You can select dot in which water starts to flow by left, right direction key (<- , ->).
	*/

	if (key == OF_KEY_RIGHT) {
		cDots = (cDots + 1) % num_of_dot;
		cout << "Selcted Dot Coordinate is (" << dots[cDots].x << ", " << dots[cDots].y << ")" << endl;
	}
	if (key == OF_KEY_LEFT) {
		cDots = (cDots + (num_of_dot - 1)) % num_of_dot;
		cout << "Selcted Dot Coordinate is (" << dots[cDots].x << ", " << dots[cDots].y << ")" << endl;
	}

	draw();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
	//Path to the comma delimited file
	//string fileName = "input.txt";

	string fileName = openFileResult.getName();
	ofFile file(fileName);

	if (!file.exists()) cout << "Target file does not exists." << endl;
	else cout << "We found the target file." << endl;

	ofBuffer buffer(file);

	int input_type = 0;

	//count lines 0 to num_of_line
	int lineNum = 0;
	int dotNum = 0;
	int waterNum = 0;

	// Read file line by line
	for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
		string line = *it;

		// Split line into strings
		vector<string> words = ofSplitString(line, " ");

		if (words.size() == 1) {
			if (input_type == 0) { // Input for the number of lines.
				num_of_line = atoi(words[0].c_str());
				cout << "The number of line is: " << num_of_line << endl;
				lines = (Line*)malloc(sizeof(Line) * num_of_line);
				input_type = 1;
			}
			else { // Input for the number of dots.
				num_of_dot = atoi(words[0].c_str());
				cout << "The number of dot is: " << num_of_dot << endl;
				dots = (Dot*)malloc(sizeof(Dot) * num_of_dot);
				input_type = 0;
			}
		}
		else if (words.size() >= 2) {
			int x1, y1, x2, y2;
			if (input_type == 1) { // Input for actual information of lines
				x1 = atoi(words[0].c_str());
				y1 = atoi(words[1].c_str());
				x2 = atoi(words[2].c_str());
				y2 = atoi(words[3].c_str());



				lines[lineNum].x1 = x1;
				lines[lineNum].x2 = x2;
				lines[lineNum].y1 = y1;
				lines[lineNum].y2 = y2;
				lines[lineNum++].grad = (double)(y2 - y1) / (x2 - x1);
			}
			else { // Input for actual information of dots.
				x1 = atoi(words[0].c_str());
				y1 = atoi(words[1].c_str());


				dots[dotNum].x = x1;
				dots[dotNum].y = y1;
				dots[dotNum].run = false;
				dots[dotNum++].erase = false;
			}
		} // End of else if.
	} // End of for-loop (Read file line by line).

	lineNum--;
	dotNum--;
}

void ofApp::initializeWaterLines() {
	if (!water[cDots].run) {
		water[cDots].S.sx = water[cDots].S.x = dots[cDots].x;
		water[cDots].S.sy = water[cDots].S.y = dots[cDots].y + dot_diameter;
		water[cDots].S.acc = 1;
	}
	if (!water[cDots].erase) {
		water[cDots].E.sx = water[cDots].E.x = dots[cDots].x;
		water[cDots].E.sy = water[cDots].E.y = dots[cDots].y + dot_diameter;
		water[cDots].E.acc = 1;
	}

	water[cDots].gravity = 9.8;
}

void ofApp::makeNewWater() {
	Water* temp = new Water;
	NewinitializeWaterLines(*temp);
	List.push_front(*temp);
}

void ofApp::NewinitializeWaterLines(Water& w) {
	dots[cDots].code = codeNum;
	w.code = codeNum++;
	w.run = true;
	w.erase = false;

	//생성 위치 초기화
	w.S.sx = w.S.x = dots[cDots].x;
	w.S.sy = w.S.y = dots[cDots].y + dot_diameter;
	w.S.acc = 1;

	//삭제 위치 초기화
	w.E.sx = w.E.x = dots[cDots].x;
	w.E.sy = w.E.y = dots[cDots].y + dot_diameter;
	w.E.acc = 1;

	//물의 가속도 조절
	w.gravity = 9.8;
}
