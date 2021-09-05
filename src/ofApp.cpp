#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(false);
    ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight() - 70);
    ofSetWindowPosition(0, 30);
    ofSetFrameRate(60);
    ofSetEscapeQuitsApp(false);

}

//--------------------------------------------------------------
void ofApp::update(){
    manager.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor::black);

    manager.draw();
    manager.drawGui();
}

