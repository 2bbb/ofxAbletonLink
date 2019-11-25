//
//  ofxAbletonLinkThreaded.h
//
//  Created by ISHII 2bit on 2016/11/28.
//
//

#pragma once

#include <atomic>

#include <ableton/Link.hpp>
#include "ofEvent.h"
#include "ofEvents.h"
#include "ofEventUtils.h"
#include "ofLog.h"

namespace ofx {
    class AbletonLinkThreaded {
        std::atomic_bool finish{false};
        ableton::Link link;
        std::thread thread;
        
    public:
        AbletonLinkThreaded()
        : link(120.0f)
        , thread([&]() {
            while(!finish) {
                const auto time = link.clock().micros();
                auto timeline = link.captureAppTimeline();
                const auto beats = timeline.beatAtTime(time, 4.0);
                const auto phase = timeline.phaseAtTime(time, 4.0);
                ofLogVerbose("ofxAbletonLinkThreaded") << "info\n"
                << "  beats: " << beats << std::endl
                << "  phase: " << phase << std::endl
                << "  tempo: " << timeline.tempo() << std::endl
                << "  peers: " << link.numPeers() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }) {
            link.enable(true);
            ofAddListener(ofEvents().exit, this, &AbletonLinkThreaded::exit, OF_EVENT_ORDER_BEFORE_APP);
        };

        void exit(ofEventArgs &) {
            finish = true;
            thread.join();
        }
    };
};

using ofxAbletonLinkThreaded = ofx::AbletonLinkThreaded;
