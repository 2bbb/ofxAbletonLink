#include "ofAppNoWindow.h"
#include "ofMain.h"
#include "ofApp.h"

int main() {
    ofInit();
    auto window = std::make_shared<ofAppNoWindow>();
    auto app = std::make_shared<ofApp>();
    ofRunApp(window, app);
    return ofRunMainLoop();
}
