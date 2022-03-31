#pragma once

#include <algorithm>
#include <string>
#include <vector>



class SnakeAlgo {
public:
    explicit SnakeAlgo() { ; }
    virtual ~SnakeAlgo() { ; }

    virtual const std::string &name() = 0;
    virtual unsigned findNext(unsigned cpos) = 0;
    virtual unsigned findPrev(unsigned cpos) = 0;

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

    unsigned findNext(unsigned snake, unsigned cpos) {
        auto & s = snakes_[snake];
        if(s) {
            return s->findNext(cpos);
        }
        return 0;
    }

    unsigned findPrev(unsigned snake, unsigned cpos) {
        auto & s = snakes_[snake];
        if(s) {
            return s->findPrev(cpos);
        }
        return 0;
    }

private:
    std::vector<std::unique_ptr<SnakeAlgo>> snakes_;
};
