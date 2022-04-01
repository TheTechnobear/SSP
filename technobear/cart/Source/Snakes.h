#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <memory>


class SnakeAlgo {
public:
    explicit SnakeAlgo() { ; }
    virtual ~SnakeAlgo() { ; }

    virtual const std::string &name() = 0;
    virtual unsigned getPosition(unsigned seq_step) = 0;
    static constexpr unsigned NSTEPS=16;
};

class Snakes {
public:
    explicit Snakes();

    unsigned size() { return snakes_.size();}
    std::string name(unsigned snake) {
        auto & s = snakes_[snake];
        if(s) {
            return s->name();
        }
        return "Invalid";
    }

    unsigned getPosition(unsigned snake, unsigned seq_step) {
        auto & s = snakes_[snake];
        if(s) {
            return s->getPosition(seq_step);
        }
        return 0;
    }

private:
    std::vector<std::unique_ptr<SnakeAlgo>> snakes_;
};
