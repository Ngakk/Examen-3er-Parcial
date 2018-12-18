#include "ofApp.h"

static bool shouldRemove(shared_ptr<ofxBox2dBaseShape>shape) {
	return !ofRectangle(0, -400, ofGetWidth(), ofGetHeight() + 400).inside(shape->getPosition());
}

//--------------------------------------------------------------
void ofApp::setup() {

	ofDisableAntiAliasing();
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);

	//of
	background.load("action2.gif");

	// Box2d
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createGround();
	box2d.setFPS(30.0);
	box2d.registerGrabbing();

	// load the shape we saved...
	/*auto pts = loadPoints("key.dat");
	auto poly = std::make_shared<ofxBox2dPolygon>();
	poly->addVertices(pts);
	poly->setPhysics(1.0, 0.3, 0.3);
	poly->triangulatePoly();
	poly->create(box2d.getWorld());
	polyShapes.push_back(poly);*/

	//Static shapes
	leftStatic.setup(box2d.getWorld(), 85, 498, 10, 36);
	bottomCheck.setup(box2d.getWorld(), 190, 430, 13, 30);
	hammerSupport.setup(box2d.getWorld(), 112, 341, 12, 12);

	//Polygons
	auto basePts = loadPoints("base.dat");
	base.addVertices(basePts);
	base.setPhysics(10000.0, 0.3, 0.3);
	base.triangulatePoly();
	base.create(box2d.getWorld());

	auto keyPts = loadPoints("key.dat");
	key.addVertices(keyPts);
	key.setPhysics(1.0, 0.3, 0.3);
	key.triangulatePoly();
	key.create(box2d.getWorld());

	auto jackPts = loadPoints("jack.dat");
	jack.addVertices(jackPts);
	jack.setPhysics(1.0, 0.3, 0.3);
	jack.triangulatePoly();
	jack.create(box2d.getWorld());

	auto hammerButtPts = loadPoints("hammerButt.dat");
	hammerButt.addVertices(hammerButtPts);
	hammerButt.setPhysics(2.0, 0.3, 0.3);
	hammerButt.triangulatePoly();
	hammerButt.create(box2d.getWorld());

	//Rects
	rect.setPhysics(5.0, 0.5, 0.5);
	rect.setup(box2d.getWorld(), 1000, 300, 50, 50);

	whippen.setPhysics(5.0, 0.5, 0.5);
	whippen.setup(box2d.getWorld(), 145, 525, 190, 50);

	whippenToJack.setPhysics(1.0, 0.5, 0.5);
	whippenToJack.setup(box2d.getWorld(), 152, 492, 13, 44);

	catcher.setPhysics(1.0, 0.5, 0.5);
	catcher.setup(box2d.getWorld(), 200, 315, 80, 15);
	catcher.setRotation(15);

	//Joints
	b2RevoluteJointDef leftWDef;
	leftWDef.Initialize(leftStatic.body, whippen.body, leftStatic.body->GetWorldCenter() + b2Vec2(0, 0.28));
	leftWhippen = (b2RevoluteJoint*)box2d.getWorld()->CreateJoint(&leftWDef);

	b2RevoluteJointDef toJackDef;
	toJackDef.Initialize(whippenToJack.body, jack.body, whippenToJack.body->GetWorldCenter() + b2Vec2(0, -0.3));
	toJack = (b2RevoluteJoint*)box2d.getWorld()->CreateJoint(&toJackDef);

	b2RevoluteJointDef toHammerButtDef;
	toHammerButtDef.Initialize(hammerSupport.body, hammerButt.body, hammerSupport.body->GetWorldCenter());
	toHammerButt = (b2RevoluteJoint*)box2d.getWorld()->CreateJoint(&toHammerButtDef);

	b2WeldJointDef whToDef;
	whToDef.collideConnected = false;
	whToDef.Initialize(whippenToJack.body, whippen.body, whippenToJack.body->GetWorldCenter() + b2Vec2(0, 0.35));
	whippenTo = (b2WeldJoint*)box2d.getWorld()->CreateJoint(&whToDef);

	b2WeldJointDef chToDef;
	chToDef.collideConnected = false;
	chToDef.Initialize(hammerButt.body, catcher.body, hammerButt.body->GetWorldCenter() + b2Vec2(0, 0));
	catcherTo = (b2WeldJoint*)box2d.getWorld()->CreateJoint(&chToDef);

	b2DistanceJointDef springDef;
	springDef.Initialize(whippen.body, jack.body, whippen.body->GetWorldCenter(), jack.body->GetWorldCenter());
	spring = (b2DistanceJoint*)box2d.getWorld()->CreateJoint(&springDef);
}

//--------------------------------------------------------------
vector <ofDefaultVertexType> ofApp::loadPoints(const std::string& file) {
	vector <ofDefaultVertexType> pts;
	vector <string>  ptsStr = ofSplitString(ofBufferFromFile(file).getText(), ",");
	for (int i = 0; i < ptsStr.size(); i += 2) {
		float x = ofToFloat(ptsStr[i]);
		float y = ofToFloat(ptsStr[i + 1]);
		pts.push_back(ofDefaultVertexType(x, y, 0));
	}
	return pts;
}


//--------------------------------------------------------------
void ofApp::update() {
	box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(240, 240, 240, 255);
	background.draw(-185, 0, background.getWidth() * (ofGetWindowHeight()/background.getHeight()), ofGetWindowHeight());

	//Dynamic draw
	ofSetColor(10, 20, 230, 150); 

	key.draw();
	jack.draw();
	rect.draw();
	whippen.draw();
	whippenToJack.draw();
	hammerButt.draw();
	catcher.draw();

	//Static draws
	base.draw();
	ofSetColor(186, 40, 120, 150);
	leftStatic.draw();
	bottomCheck.draw();
	hammerSupport.draw();
	for (int i = 0; i < polyShapes.size(); i++) {
		polyShapes[i]->draw();

		ofDrawCircle(polyShapes[i]->getPosition(), 3);
	}

	//Debug
	ofSetColor(255, 0, 0, 200);
	float seg = 10;
	float drawseg = 0;
	int strongRate = 5;
	int rateCount = 0;
	while (drawseg < ofGetWidth()) {
		if (rateCount % strongRate == 0)
			ofSetColor(0, 0, 0, 200);
		if (rateCount % (strongRate * 2) == 0)
			ofSetColor(0, 0, 0, 255);
		ofDrawLine(drawseg, 0, drawseg, ofGetHeight());

		ofSetColor(255, 0, 0, 200);

		drawseg += seg;
		rateCount++;
	}
	drawseg = 0;
	rateCount = 0;
	while (drawseg < ofGetHeight()) {
		if (rateCount % strongRate == 0)
			ofSetColor(0, 0, 0, 200);
		if (rateCount % (strongRate * 2) == 0)
			ofSetColor(0, 0, 0, 255);
		ofDrawLine(0, drawseg, ofGetWidth(), drawseg);

		ofSetColor(255, 0, 0, 200);

		drawseg += seg;
		rateCount++;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	box2d.grabShapeDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	box2d.grabShapeDown(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	box2d.grabShapeUp(x, y, button);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}
