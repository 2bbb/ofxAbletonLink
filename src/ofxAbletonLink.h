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
        bool is_playing{false};
        
    public:
        ofEvent<double> bpmChanged;
        ofEvent<std::size_t> numPeersChanged;
        ofEvent<bool> playStateChanged;
        
        struct Setting {
            Setting(double bpm = 120.0,
                    double quantum = 4.0,
                    bool enable = true,
                    bool playStateSync = true)
            : bpm(bpm)
            , quantum(quantum)
            , enable(enable)
            , playStateSync(playStateSync)
            {};
            
            double bpm{120.0};
            double quantum{4.0};
            bool enable{true};
            bool playStateSync{true};
        };
        AbletonLink(const Setting &setting = {})
        : link(setting.bpm)
        , bpm(setting.bpm)
        , quantum(setting.quantum)
        {
            setEnable(setting.enable);
            setPlayStateSync(setting.playStateSync);
            
            link.setTempoCallback([&](double bpm) {
                ofNotifyEvent(bpmChanged, bpm);
            });
            link.setNumPeersCallback([&](std::size_t peers) {
                ofNotifyEvent(numPeersChanged, peers);
            });
            link.setStartStopCallback([this](bool isPlaying) {
                ofNotifyEvent(playStateChanged, isPlaying);
            });
            ofAddListener(ofEvents().update, this, &AbletonLink::update, OF_EVENT_ORDER_BEFORE_APP);
        };
        
        OF_DEPRECATED_MSG("Use ofxAbletonLink(const ofxAbletonLinkSetting &)", AbletonLink(double bpm, double quantum = 4.0, bool enable = true))
        : AbletonLink(Setting{bpm, quantum, enable, true})
        {};
        
        inline void setup(const Setting &setting = {}) {
            setEnable(setting.enable);
            setBPM(setting.bpm);
            setQuantum(setting.quantum);
            setEnable(setting.enable);
            setPlayStateSync(setting.playStateSync);
        }
        
        OF_DEPRECATED_MSG("Use setup(const ofxAbletonLinkSetting &)", inline void setup(double bpm, double quantum = 4.0, bool enable = true))
        {
            setup(Setting{bpm, quantum, enable, true});
        }
        
        inline bool isEnabled() const { return link.isEnabled(); }
        inline void setEnable(bool enable) { link.enable(enable); }
        inline void enable() { setEnable(true); }
        inline void disable() { setEnable(false); }
        
        OF_DEPRECATED_MSG("Use isEnabled().", inline bool isLinkEnabled() const) { return isEnabled(); }
        OF_DEPRECATED_MSG("Use setEnable(bool enable).", inline void setLinkEnable(bool enable)) { setEnable(enable); }
        OF_DEPRECATED_MSG("Use enable().", inline void enableLink()) { setEnable(true); }
        OF_DEPRECATED_MSG("Use disable().", inline void disableLink()) { setEnable(false); }
        
        inline bool isPlayStateSync() const { return link.isStartStopSyncEnabled(); }
        inline void setPlayStateSync(bool bEnable) { link.enableStartStopSync(bEnable); };
        inline void enablePlayStateSync() { link.enableStartStopSync(true); };
        inline void disablePlayStateSync() { link.enableStartStopSync(false); };
        
        inline bool isPlaying() const
        { return link.captureAppSessionState().isPlaying(); };
        inline void setIsPlaying(bool isPlaying) {
            const auto &&time = getTime();
            auto &&sessionState = get_session_state();
            sessionState->setIsPlaying(isPlaying, time);
        }
        inline void play() { setIsPlaying(true); }
        inline void stop() { setIsPlaying(false); };

        double getBeat() const { return beat; }
        void setBeat(double beat) {
            const auto time = getTime();
            auto session_state = get_session_state();
            session_state->requestBeatAtTime(beat, time, quantum);
        }
        void setBeatForce(double beat) {
            const auto time = getTime();
            auto session_state = get_session_state();
            session_state->forceBeatAtTime(beat, time, quantum);
        }
        
        double getPhase() const { return phase; }
        
        double getBPM() const { return bpm; }
        void setBPM(double bpm) {
            this->bpm = bpm;
            const auto time = getTime();
            auto session_state = get_session_state();
            session_state->setTempo(bpm, time);
        }
        
        std::size_t getNumPeers() const { return link.numPeers(); }
        
        void setQuantum(double quantum) { this->quantum = quantum; }
        double getQuantum() const { return quantum; }
        
        std::chrono::microseconds getTime() const
        { return link.clock().micros(); }

    private:
        void update(ofEventArgs &) {
            const auto time = link.clock().micros();
            auto session_state = link.captureAppSessionState();
            
            beat = session_state.beatAtTime(time, quantum);
            phase = session_state.phaseAtTime(time, quantum);
            bpm = session_state.tempo();
            is_playing = session_state.isPlaying();
        };
        
        struct scoped_session_state {
            scoped_session_state() = delete;
            scoped_session_state(ableton::Link &link)
            : link(link)
            , sessionState(link.captureAppSessionState())
            {}
            ~scoped_session_state()
            { link.commitAppSessionState(sessionState); };

            ableton::Link &link;
            ableton::Link::SessionState sessionState;
            inline ableton::Link::SessionState *operator->()
            { return &sessionState; };
            inline const ableton::Link::SessionState *operator->() const
            { return &sessionState; };
        };
        inline scoped_session_state get_session_state()
        { return { link }; };
    };
};

using ofxAbletonLink = ofx::AbletonLink;
using ofxAbletonLinkSetting = ofxAbletonLink::Setting;
