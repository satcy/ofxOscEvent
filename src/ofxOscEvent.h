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
    
    void addEventListener(const string & type, ofxOscEventListener * listener ){
        removeEventListener(type, listener);
        listeners[type].push_back(listener);
    }
    
    void removeEventListener(const string & type, ofxOscEventListener * listener){
        vector<ofxOscEventListener*> list = listeners[type];
        if ( !list.empty() ) {
            list.erase(std::remove(list.begin(), list.end(), listener), list.end());
        }
    }
	
	void addEventListener(const string & addr, void* target, std::function<void(const ofxOscMessage&)> fn){
		callback[addr][target] = fn;
	}
	
	void removeEventListener(const string & addr, void* target){
		map<string, map<void*, std::function<void(const ofxOscMessage&)> > >::iterator it = callback.find(addr);
		if (it == callback.end()) return;
		
		map<void*, std::function<void(const ofxOscMessage&)> >& m = it->second;
		m.erase(target);
	}
    
protected:
    
    void notify(const string & addr, ofxOscMessage & m){
        ofNotifyEvent(packetIn, m);//OF buildin Event system
        
        //dispatching to extended ofxOscEventListener class
        vector<ofxOscEventListener*> list = listeners[addr];
        if ( !list.empty() ) {
            for ( int i=0; i<list.size(); i++ ){
                list[i]->onEventHandler(m);
            }
        }
		
        //dispatching to anonymous function target
		map<string, map<void*, std::function<void(const ofxOscMessage&)> > >::iterator it = callback.find(addr);
		if (it != callback.end())
		{
			typedef map<void*, std::function<void(const ofxOscMessage&)> > TargetList;
			TargetList& TL = it->second;
			TargetList::iterator target_it = TL.begin();
			while (target_it != TL.end())
			{
				const std::function<void(const ofxOscMessage&)> &fn = target_it->second;
				fn(m);
				
				target_it++;
			}
		}
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
protected:
    map< string, vector<ofxOscEventListener * > > listeners;
	
	map<string, map<void*, std::function<void(const ofxOscMessage&)> > > callback;
};
