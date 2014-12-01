//
//  ofxOscEventListener.h
//  example_basic
//
//  Created by satcy on 12/1/14.
//
//
#pragma once

class ofxOscEventListener{
public:
    ofxOscEventListener(){}
    virtual ~ofxOscEventListener(){}
    virtual void onEventHandler(ofxOscMessage & m){}
};
