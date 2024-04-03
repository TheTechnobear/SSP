#pragma once

#include <juce_core/juce_core.h>

#include <vector>

class Matrix {
public:
    explicit Matrix();

    ~Matrix() = default;

    struct Jack {
        Jack(unsigned mod, unsigned idx) : modIdx_(mod), chIdx_(idx) {}
        unsigned modIdx_;
        unsigned chIdx_;

        int operator==(const Jack& b) {
            if (modIdx_ == b.modIdx_ && chIdx_ == b.chIdx_) { return 1; }
            return 0;
        }
    };

    struct Wire {
        Wire(const Jack& src, const Jack& dest) : src_(src), dest_(dest) {}
        Jack src_;
        Jack dest_;

        int operator==(const Wire& b) {
            if (src_ == b.src_ && dest_ == b.dest_) { return 1; }
            return 0;
        }
    };


    void clear() { connections_.clear(); }

    void connect(const Jack& src, const Jack& dest) { connections_.push_back(Wire(src, dest)); }

    void disconnect(const Jack& src, const Jack& dest) {
        int idx = 0;
        for (auto& c : connections_) {
            if (c.src_ == src && c.dest_ == dest) {
                disconnect(idx);
                return;
            }
            idx++;
        }
    }

    void disconnect(int idx) {
        if (idx < connections_.size()) { connections_.erase(connections_.begin() + idx); }
    }

    std::vector<Wire> connections_;

    void getStateInformation(juce::MemoryOutputStream& outStream);
    void setStateInformation(juce::MemoryInputStream& inStream);
};
