//
//  ofxAbletonLink.h
//
//  Created by ISHII 2bit on 2016/11/28.
//
//

#pragma once

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
        
        bool isLinkEnabled() const { return link.isEnabled(); }
        void setLinkEnable(bool enable) { link.enable(enable); }
        void enableLink() { link.enable(true); }
        void disableLink() { link.enable(false); }
        
        double getBeat() const { return beat; }
        void setBeat(double beat) {
            const auto time = link.clock().micros();
            auto timeline = link.captureAppTimeline();
            timeline.requestBeatAtTime(beat, time, quantum);
            link.commitAppTimeline(timeline);
        }
        void setBeatForce(double beat) {
            const auto time = link.clock().micros();
            auto timeline = link.captureAppTimeline();
            timeline.forceBeatAtTime(beat, time, quantum);
            link.commitAppTimeline(timeline);
        }
        
        double getPhase() const { return phase; }
        void setPhase(double phase) {
            const auto time = link.clock().micros();
            auto timeline = link.captureAppTimeline();
        }
        
        double getBPM() const { return bpm; }
        void setBPM(double bpm) {
            this->bpm = bpm;
            const auto time = link.clock().micros();
            auto timeline = link.captureAppTimeline();
            timeline.setTempo(bpm, time);
            link.commitAppTimeline(timeline);
        }
        
        std::size_t getNumPeers() const { return link.numPeers(); }
        
        void setQuantum(double quantum) { this->quantum = quantum; }
        double getQuantum() const { return quantum; }
        
    private:
        void update(ofEventArgs &) {
            const auto time = link.clock().micros();
            auto timeline = link.captureAppTimeline();
            
            beat = timeline.beatAtTime(time, quantum);
            phase = timeline.phaseAtTime(time, quantum);
            bpm = timeline.tempo();
        };
    };
};

using ofxAbletonLink = ofx::AbletonLink;