#include "ofAppNoWindow.h"
#include "ofMain.h"
#include "ofxAbletonLink.h"
#include "ofxUnitTests.h"

class ofApp : public ofxUnitTestsApp {
    void run() override {
        const auto listenerCountBefore = ofEvents().update.size();

        {
            const ofxAbletonLink::Setting disabled{
                120.0,
                4.0,
                false,
                false,
            };
            ofxAbletonLink link{disabled};

            ofxTest(!link.isEnabled(), "Setting disables Link");
            ofxTest(!link.isPlayStateSync(), "Setting disables play-state sync");
            ofxTestEq(
                ofEvents().update.size(),
                listenerCountBefore + 1,
                "construction registers one update listener"
            );

            link.setBPM(128.0);
            ofxTestEq(link.getBPM(), 128.0, "setBPM updates the local tempo");

            link.setQuantum(7.0);
            ofxTestEq(link.getQuantum(), 7.0, "setQuantum updates the quantum");

            link.enablePlayStateSync();
            ofxTest(link.isPlayStateSync(), "play-state sync can be enabled");
        }

        ofxTestEq(
            ofEvents().update.size(),
            listenerCountBefore,
            "destruction removes the update listener"
        );

        ofEventArgs args;
        ofNotifyEvent(ofEvents().update, args);
        ofxTestEq(
            ofEvents().update.size(),
            listenerCountBefore,
            "update notification after destruction leaves no stale listener"
        );
    }
};

int main() {
    ofInit();
    auto window = std::make_shared<ofAppNoWindow>();
    auto app = std::make_shared<ofApp>();
    ofRunApp(window, app);
    return ofRunMainLoop();
}
