#include "ofMain.h"
#include "ofxAbletonLink.h"

class ofApp : public ofBaseApp {
    ofxAbletonLink link;
    
    void bpmChanged(double &bpm) {
        ofLogNotice("bpmChanged") << bpm;
    }
    
    void numPeersChanged(std::size_t &peers) {
        ofLogNotice("numPeersChanged") << peers;
    }
    
    void playStateChanged(bool &state) {
        ofLogNotice("playStateChanged") << (state ? "play" : "stop");
    }
public:
    void setup() {
        link.setup();
        ofSetBackgroundColor(0, 0, 0);
        ofSetVerticalSync(true);
        ofSetFrameRate(60);
        ofAddListener(link.bpmChanged, this, &ofApp::bpmChanged);
        ofAddListener(link.numPeersChanged, this, &ofApp::numPeersChanged);
        ofAddListener(link.playStateChanged, this, &ofApp::playStateChanged);
    }
    
    void draw() {
        float x = ofGetWidth() * link.getPhase() / link.getQuantum();
        
        ofSetColor(255, 0, 0);
        ofDrawLine(x, 0, x, ofGetHeight());
        
        std::stringstream ss("");
        ss
            << "bpm:   " << link.getBPM() << std::endl
            << "beat:  " << link.getBeat() << std::endl
            << "phase: " << link.getPhase() << std::endl
            << "peers: " << link.getNumPeers() << std::endl
            << "play?: " << (link.isPlaying() ? "play" : "stop");
        
        ofSetColor(255);
        ofDrawBitmapString(ss.str(), 20, 20);
    }
    void keyPressed(int key) {
        if(key == OF_KEY_LEFT) {
            if(20 < link.getBPM()) link.setBPM(link.getBPM() - 1.0);
        } else if(key == OF_KEY_RIGHT) {
            link.setBPM(link.getBPM() + 1.0);
        } else if(key == 'b') {
            link.setBeat(0.0);
        } else if(key == 'B') {
            link.setBeatForce(0.0);
        } else if(key == ' ') {
            link.setIsPlaying(!link.isPlaying());
        }
    }
};

int main() {
    ofSetupOpenGL(320, 320, OF_WINDOW);
    ofRunApp(new ofApp);
}
