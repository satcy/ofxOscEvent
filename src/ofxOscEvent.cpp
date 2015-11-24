//
//  ofxOscEvent.cpp
//  example_basic
//
//  Created by satcy on 6/14/14.
//
//

#include "ofxOscEvent.h"


ofEvent<ofxOscMessage> ofxOscEvent::packetIn = ofEvent<ofxOscMessage>();

void ofxOscEvent::setup(int port){
    ofxOscReceiver::setup(port);
    enable();
}

void ofxOscEvent::setNonRealtime(bool enable, float _fps) {
    isNonRealtime = enable;
    time = ofGetElapsedTimef();
    last_thread_time = 0;
    fps = _fps;
    if ( !isNonRealtime ) {
        if(lock()) {
            messages.clear();
            unlock();
        }
    }
}

void ofxOscEvent::enable(){
    messages.clear();
    ofThread::startThread();
    ofRemoveListener(ofEvents().update, this, &ofxOscEvent::onUpdate);
    ofAddListener(ofEvents().update, this, &ofxOscEvent::onUpdate);
}

void ofxOscEvent::disable(){
    stopThread();
    ofRemoveListener(ofEvents().update, this, &ofxOscEvent::onUpdate);
}

void ofxOscEvent::addEventListener(const string & type, ofxOscEventListener * listener ){
    removeEventListener(type, listener);
    listeners[type].push_back(listener);
}

void ofxOscEvent::removeEventListener(const string & type, ofxOscEventListener * listener){
    vector<ofxOscEventListener*> list = listeners[type];
    if ( !list.empty() ) {
        list.erase(std::remove(list.begin(), list.end(), listener), list.end());
    }
}

void ofxOscEvent::addEventListener(const string & addr, void* target, std::function<void(const ofxOscMessage&)> fn){
    callback[addr][target] = fn;
}

void ofxOscEvent::removeEventListener(const string & addr, void* target){
    map<string, map<void*, std::function<void(const ofxOscMessage&)> > >::iterator it = callback.find(addr);
    if (it == callback.end()) return;
    
    map<void*, std::function<void(const ofxOscMessage&)> >& m = it->second;
    m.erase(target);
}



void ofxOscEvent::threadedFunction() {
    while(isThreadRunning()) {
        if(lock()) {
            updateThread();
            unlock();
            sleep(1);
        } else {
            ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
        }
    }
}

void ofxOscEvent::notify(const string & addr, ofxOscMessage & m){
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

void ofxOscEvent::onUpdate(ofEventArgs & e){
    if ( isNonRealtime ) {
        if(lock()) {
            time += 1.0 / fps;
            unlock();
        }
    } else {
        if(lock()) {
            for ( int i=0; i < messages.size(); i++ ) {
                notify(messages[i].m.getAddress(), messages[i].m);
            }
            messages.clear();
            unlock();
        }
    }
}

void ofxOscEvent::updateThread(){
    float t = ofGetElapsedTimef();
    while ( hasWaitingMessages() ) {
        
        ofxOscMessage m;
        getNextMessage(&m);
        
        string addr = m.getAddress();
        if ( !isNonRealtime ) {
            OscMessageWithTime mess;
            mess.m.copy(m);
            messages.push_back(mess);
        } else {
            OscMessageWithTime mess;
            mess.m.copy(m);
            mess.time = t;
            mess.isOld = false;
            messages.push_back(mess);
        }
        
    }
    if ( isNonRealtime ) {
        float sec = 1.0 / fps;
        for ( int i=0; i < messages.size(); i++ ) {
            if ( messages[i].time <= time ) {
                messages[i].isOld = true;
                notify(messages[i].m.getAddress(), messages[i].m);
            } else if ( messages[i].time > time + sec*2.0 ) {
                break;
            }
        }
        
        ofRemove(messages, isOld);
    } else {
        
    }
}
