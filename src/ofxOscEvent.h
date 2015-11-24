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
#include "ofxOscEventListener.h"

#if __cplusplus < 201103L
#include <tr1/functional>
namespace std {
	using tr1::function;
	using tr1::bind;
	
	using tr1::placeholders::_1;
	using tr1::placeholders::_2;
	using tr1::placeholders::_3;
	using tr1::placeholders::_4;
	using tr1::placeholders::_5;
	using tr1::placeholders::_6;
	using tr1::placeholders::_7;
	using tr1::placeholders::_8;
	using tr1::placeholders::_9;
	using tr1::placeholders::_10;
}
#endif

class ofxOscEvent : public ofxOscReceiver, ofThread {
private:
    class OscMessageWithTime{
    public:
        OscMessageWithTime(){}
        ~OscMessageWithTime(){
            m.clear();
        }
        ofxOscMessage m;
        float time;
        bool isOld;
    };
public:
    static ofEvent<ofxOscMessage> packetIn;
    
    static bool isOld(OscMessageWithTime & m){
        return m.isOld;
    }
    
    ofxOscEvent() : isNonRealtime(false), fps(30) {
        time = ofGetElapsedTimef();
    }
    
    void setup(int port);
    
    //be true for dispatching per frame from cued osc messages.
    void setNonRealtime(bool enable, float _fps = 30.0);
    bool getNonRealTime(){
        return isNonRealtime;
    }
    
    void enable();
    
    void disable();
    
    void addEventListener(const string & type, ofxOscEventListener * listener );
    void removeEventListener(const string & type, ofxOscEventListener * listener);
    
    void addEventListener(const string & addr, void* target, std::function<void(const ofxOscMessage&)> fn);
    void removeEventListener(const string & addr, void* target);
    
    void threadedFunction();
    
    void notify(const string & addr, ofxOscMessage & m);
    
    const vector<OscMessageWithTime> & getMessages() { return messages; }
    
protected:
    
    void onUpdate(ofEventArgs & e);
    
    void updateThread();
    
protected:
    map< string, vector<ofxOscEventListener * > > listeners;
	
	map<string, map<void*, std::function<void(const ofxOscMessage&)> > > callback;
    
    vector<OscMessageWithTime> messages;
    
    float time;
    float last_thread_time;
    bool isNonRealtime;
    float fps;
    
};
