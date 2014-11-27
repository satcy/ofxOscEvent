//
//  ofxOscEvent.h
//  example
//
//  Created by satcy on 6/13/14.
//
//
#pragma once
#include "ofMain.h"
#include "ofxOsc.h"

class ofxOscEvent : public ofxOscReceiver {
private:
  
public:
    static ofEvent<ofxOscMessage> packetIn;
    
    void setup(int port){
        ofxOscReceiver::setup(port);
        enable();
    }
    
    void enable(){
        ofRemoveListener(ofEvents().update, this, &ofxOscEvent::onUpdate);
        ofAddListener(ofEvents().update, this, &ofxOscEvent::onUpdate);
    }
    
    void disable(){
        ofRemoveListener(ofEvents().update, this, &ofxOscEvent::onUpdate);
    }
    
protected:
    
    void notify(const string & addr, ofxOscMessage & m){
        ofNotifyEvent(packetIn, m);
    }
    
    void onUpdate(ofEventArgs & e){
        while (hasWaitingMessages())
		{
			ofxOscMessage m;
			getNextMessage(&m);
			
			string addr = m.getAddress();
			notify(addr, m);
		}
    }
};
