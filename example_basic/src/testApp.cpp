#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    osc.setup(12000);
    
    //ofxOscEvent has 3 ways for handling oscmessage.
    // 1: 
    ofAddListener(ofxOscEvent::packetIn, this, &testApp::onPacketIn);
    
    // 2:
    osc.addEventListener("/test", this);
    
    // 3:
    // if you wanna change class members in this way, use class pointer in global scope.
    static testApp* self = this;
    osc.addEventListener("/test", this, ^(const ofxOscMessage& m){
        cout << "anonymous function \n";
//        self->some_member = m.getArgAsInt32(0);
    });
}

void testApp::printOutOscMessage(const ofxOscMessage & m){
    cout << m.getAddress() << " ";
    for ( int i=0; i<m.getNumArgs(); i++ ) {
        ofxOscArgType type = m.getArgType(i);
        cout << type << ":";
        switch ( type ) {
            case OFXOSC_TYPE_FLOAT:
                cout << m.getArgAsFloat(i) << ", ";
                break;
            case OFXOSC_TYPE_INT32:
                cout << m.getArgAsInt32(i) << ", ";
                break;
            case OFXOSC_TYPE_INT64:
                cout << m.getArgAsInt64(i) << ", ";
                break;
            case OFXOSC_TYPE_STRING:
                cout << m.getArgAsString(i) << ", ";
                break;
            default:
                break;
        }
    }
    if ( m.getAddress() == "/count" ) {
        count = m.getArgAsInt32(0);
    }
    cout << endl;
}

void testApp::onPacketIn(ofxOscMessage & m){
    cout << "of event: \n";
    printOutOscMessage(m);
}

void testApp::onEventHandler(ofxOscMessage & m){
    cout << "onEventHandler: \n";
    printOutOscMessage(m);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10,10);
    
    ofDrawBitmapString(ofToString(count), 10,20);
    ofRect(0, 30, count%500, 10);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if ( key == ' ' ) {
        osc.setNonRealtime(!osc.getNonRealTime());
        if ( osc.getNonRealTime() ) {
            ofSetFrameRate(15);
        } else {
            ofSetFrameRate(60);
        }
    }
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
