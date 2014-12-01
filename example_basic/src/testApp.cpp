#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    osc.setup(12000);
    
    //ofxOscEvent has 3 ways for handling oscmessage.
    // 1: 
    ofAddListener(ofxOscEvent::packetIn, this, &testApp::onPacketIn);
    
    // 2:
    osc.addEventListener("/test", this);
    
    // 3:
    osc.addEventListener("/test", this, ^(const ofxOscMessage& m){
        cout << "anonymous function \n";
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
