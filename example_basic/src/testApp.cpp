#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    osc.setup(12000);
    ofAddListener(ofxOscEvent::packetIn, this, &testApp::onPacketIn);
}

void testApp::onPacketIn(ofxOscMessage & m){
    cout << m.getAddress() << " ";
    for ( int i=0; i<m.getNumArgs(); i++ ) {
        cout << m.getArgType(i);
    }
    cout << endl;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
