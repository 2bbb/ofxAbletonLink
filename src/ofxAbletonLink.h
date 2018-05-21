//
//  ofxAbletonLink.h
//
//  Created by ISHII 2bit on 2016/11/28.
//
//

#pragma once

#include "ofConstants.h"

#if defined(TARGET_OSX) || defined(TARGET_OF_IOS)
#   define LINK_PLATFORM_MACOSX 1
#elif defined(TARGET_LINUX)
#   define LINK_PLATFORM_LINUX 1
#else
#   define LINK_PLATFORM_WINDOWS 1
#endif

#include <ableton/Link.hpp>
#include "ofEvent.h"
#include "ofEvents.h"
#include "ofEventUtils.h"

namespace ofx {
    class AbletonLink {
        ableton::Link link;
        double beat{0.0};
        double phase{0.0};
        double bpm{120.0};
        double quantum{4.0};
        
    public:
        ofEvent<double> bpmChanged;
        ofEvent<std::size_t> numPeersChanged;
        
        AbletonLink(double bpm = 120.0, double quantum = 4.0, bool enable = true)
        : link(bpm)
        , bpm(bpm)
        , quantum(quantum)
        {
            link.enable(enable);
            
            link.setTempoCallback([&](double bpm) {
                ofNotifyEvent(bpmChanged, bpm);
            });
            link.setNumPeersCallback([&](std::size_t peers) {
                ofNotifyEvent(numPeersChanged, peers);
            });
            ofAddListener(ofEvents().update, this, &AbletonLink::update, OF_EVENT_ORDER_BEFORE_APP);
        };
        
        void setup(double bpm = 120.0, double quantum = 4.0, bool enable = true) {
            setBPM(bpm);
            setQuantum(quantum);
            setLinkEnable(enable);
        }
        
        bool isLinkEnabled() const { return link.isEnabled(); }
        void setLinkEnable(bool enable) { link.enable(enable); }
        void enableLink() { link.enable(true); }
        void disableLink() { link.enable(false); }
        
        double getBeat() const { return beat; }
        void setBeat(double beat) {
            const auto time = link.clock().micros();
            auto session_state = link.captureAppSessionState();
            session_state.requestBeatAtTime(beat, time, quantum);
            link.commitAppSessionState(session_state);
        }
        void setBeatForce(double beat) {
            const auto time = link.clock().micros();
            auto session_state = link.captureAppSessionState();
            session_state.forceBeatAtTime(beat, time, quantum);
            link.commitAppSessionState(session_state);
        }
        
        double getPhase() const { return phase; }
        void setPhase(double phase) {
            const auto time = link.clock().micros();
            auto session_state = link.captureAppSessionState();
        }
        
        double getBPM() const { return bpm; }
        void setBPM(double bpm) {
            this->bpm = bpm;
            const auto time = link.clock().micros();
            auto session_state = link.captureAppSessionState();
            session_state.setTempo(bpm, time);
            link.commitAppSessionState(session_state);
        }
        
        std::size_t getNumPeers() const { return link.numPeers(); }
        
        void setQuantum(double quantum) { this->quantum = quantum; }
        double getQuantum() const { return quantum; }
        
    private:
        void update(ofEventArgs &) {
            const auto time = link.clock().micros();
            auto session_state = link.captureAppSessionState();
            
            beat = session_state.beatAtTime(time, quantum);
            phase = session_state.phaseAtTime(time, quantum);
            bpm = session_state.tempo();
        };
    };
};

using ofxAbletonLink = ofx::AbletonLink;
