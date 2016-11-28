#include "ofMain.h"
#include "ofxAbletonLink.h"

class ofApp : public ofBaseApp {
    ofxAbletonLink link;
public:
    void setup() {
        ofSetColor(255, 0, 0);
        ofSetBackgroundColor(0, 0, 0);
    }
    void update() {
        ofLogNotice()
            << "bpm:   " << link.getBPM() << std::endl
            << "beat:  " << link.getBeat() << std::endl
            << "phase: " << link.getPhase() << std::endl
            << "peers: " << link.getNumPeers() << std::endl;
    }
    void draw() {
        float x = ofGetWidth() * link.getPhase() / link.getQuantum();
        ofDrawLine(x, 0, x, ofGetHeight());
    }
    
    void exit() {}
    
    void keyPressed(int key) {
        if(key == OF_KEY_LEFT) {
            if(20 < link.getBPM()) link.setBPM(link.getBPM() - 1.0);
        } else if(key == OF_KEY_RIGHT) {
            link.setBPM(link.getBPM() + 1.0);
        } else if(key == 'b') {
            link.setBeat(0.0);
        } else if(key == 'B') {
            link.setBeatForce(0.0);
        }
    }
};

int main() {
    ofSetupOpenGL(1280, 720, OF_WINDOW);
    ofRunApp(new ofApp);
}
