
#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

// ------------------------------------------------- App
class ofApp : public ofBaseApp {

public:

	~ofApp() {
		box2d.getWorld()->DestroyJoint(leftWhippen);
		box2d.getWorld()->DestroyJoint(toJack);
		box2d.getWorld()->DestroyJoint(toHammerButt);
		box2d.getWorld()->DestroyJoint(toDumperBody);
		box2d.getWorld()->DestroyJoint(whippenTo);
		box2d.getWorld()->DestroyJoint(catcherTo);
		box2d.getWorld()->DestroyJoint(hammerToButt);
		box2d.getWorld()->DestroyJoint(whippenTo2);
		box2d.getWorld()->DestroyJoint(damperTo);
		box2d.getWorld()->DestroyJoint(damperTo2);
		box2d.getWorld()->DestroyJoint(damperTo3);
		box2d.getWorld()->DestroyJoint(damperTo4);
	}

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	// little helper function to load
	// point that we saved
	std::vector<ofDefaultVertexType> loadPoints(const std::string& file);

	ofPolyline                            shape;
	ofxBox2d                              box2d;
	vector <shared_ptr<ofxBox2dPolygon>>  polyShapes;

	//Static shapes
	ofxBox2dPolygon base;
	ofxBox2dRect leftStatic;
	ofxBox2dRect bottomCheck;
	ofxBox2dRect hammerSupport, hammerRest;
	ofxBox2dRect dumperSupport;

	//Not static
	ofxBox2dPolygon key, jack, hammerButt;

	ofxBox2dRect rect, whippen, whippenToJack, catcher, hammerBody, cucha, dumperBody, damperHeadUp, damperHeadDown, damperHead, damperJoint;
	b2RevoluteJoint *leftWhippen, *toJack, *toHammerButt, *toDumperBody;
	b2WeldJoint *whippenTo, *catcherTo, *hammerToButt, *whippenTo2, *damperTo, *damperTo2, *damperTo3, *damperTo4;

	b2DistanceJoint *spring;

	ofImage background;
};
