
#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

// ------------------------------------------------- App
class ofApp : public ofBaseApp {

public:

	~ofApp() {
		box2d.getWorld()->DestroyJoint(leftWhippen);
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

	//Not static
	ofxBox2dPolygon key, jack, hammerButt;
	ofxBox2dRect rect, whippen, whippenToJack, catcher;
	b2RevoluteJoint *leftWhippen, *toJack, *toHammerButt;
	b2WeldJoint *whippenTo, *catcherTo;
	b2DistanceJoint *spring;

	ofImage background;
};
