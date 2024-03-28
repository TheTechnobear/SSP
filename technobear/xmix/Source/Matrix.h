#pragma once

#include <juce_core/juce_core.h>

#include <vector>

class Matrix {
public:
    explicit Matrix();

    ~Matrix() = default;

    void createDefault(unsigned in1, unsigned in2);

    enum Src { SRC_INPUT, SRC_MOD, SRC_WORK };

    struct Routing {
        Routing(Src s, unsigned sCh, unsigned dCh) : src_(s), srcChannel_(sCh), destChannel_(dCh) {}

        Src src_;
        unsigned srcChannel_;
        unsigned destChannel_;
    };

    struct ModuleRouting {
        void clear() { routes_.clear(); }
        void addRoute(Src s, unsigned sCh, unsigned dCh) {
            for (auto& r : routes_) {
                // avoid duplications
                if (s == r.src_ && sCh == r.srcChannel_ && dCh == r.destChannel_) { return; }
            }
            routes_.push_back(Routing(s, sCh, dCh));
        }
        void removeRoute(unsigned idx) {
            if (idx < routes_.size()) { routes_.erase(routes_.begin() + idx); }
        }
        std::vector<Routing> routes_;
    };

    struct OutputRouting {
        void clear() { routes_.clear(); }
        void addRoute(Src s, unsigned sCh, unsigned dCh) {
            for (auto& r : routes_) {
                // avoid duplications
                if (s == r.src_ && sCh == r.srcChannel_ && dCh == r.destChannel_) { return; }
            }
            routes_.push_back(Routing(s, sCh, dCh));
        }
        void removeRoute(unsigned idx) {
            if (idx < routes_.size()) { routes_.erase(routes_.begin() + idx); }
        }
        std::vector<Routing> routes_;
    };

    std::vector<ModuleRouting> modules_;
    OutputRouting output_;

    void getStateInformation(juce::MemoryOutputStream& outStream);
    void setStateInformation(juce::MemoryInputStream& inStream);
};
