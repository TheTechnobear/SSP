
#include "Snakes.h"

class ReneEnumSnake : public SnakeAlgo {
public:
    explicit ReneEnumSnake(const std::string &name, const unsigned *pattern) :
        name_(name), pattern_(pattern) {

        // nxtPattern - for a cpos find next
        for (int i = 0; i < NSTEPS; i++) {
            unsigned ni = (NSTEPS - 1 + i) % NSTEPS;
            unsigned ns = pattern_[i];
            nxtPattern_[ni] = ns;
        }
    }

    const std::string &name() { return name_; }

    unsigned findNext(unsigned cpos) {
        return nxtPattern_[cpos];
    }

protected:
    std::string name_;
    const unsigned *pattern_;
    unsigned nxtPattern_[16];
};


class LinearAlgo : public SnakeAlgo {
public:
    explicit LinearAlgo() { ; }

    const std::string &name() { return name_; }

    unsigned findNext(unsigned cpos) {
        return (cpos + 1) % 16;
    }

    std::string name_ = "Linear";
};


Snakes::Snakes() {
    static constexpr unsigned renepatterns[16][16] = {
        { // pattern 1
            0,  1,  2,  3,
            4,  5,  6,  7,
            8,  9,  10, 11,
            12, 13, 14, 15
        },

        { // pattern 2
            0,  1,  2,  3,
            7,  6,  5,  4,
            8,  9,  10, 11,
            15, 14, 13, 12
        },

        { // pattern 3
            3,  7,  11, 15,
            2,  6,  10, 14,
            1,  5,  9,  13,
            0,  4,  8,  12
        },

        { // pattern 4
            0,  4,  8,  12,
            12, 9,  5,  1,
            2,  6,  10, 14,
            15, 11, 7,  3

        },

        { // pattern 5
            0,  1,  2,  3,
            7,  11, 15, 14,
            13, 12, 8,  4,
            5,  6,  10, 9
        },
        { // pattern 6
            12, 13, 14, 15,
            11, 7,  3,  2,
            1,  0,  4,  8,
            9,  10, 6,  5
        },
        { // pattern 7
            0,  1,  4,  8,
            5,  2,  3,  6,
            9,  12, 13, 10,
            7,  11, 14, 14
        },
        { // pattern 8
            0,  5,  10, 15,
            14, 9,  4,  1,
            6,  11, 7,  2,
            8,  13, 12, 3
        },
        { // pattern 9
            0,  12, 13, 3,
            2,  14, 15, 1,
            7,  8,  9,  5,
            4,  10, 11, 6
        },
        { // pattern 10
            0,  7,  10, 14,
            4,  1,  8,  11,
            12, 5,  2,  9,
            15, 13, 6,  4
        },
        { // pattern 11
            0,  1,  8,  9,
            6,  7,  14, 15,
            4,  5,  12, 13,
            2,  3,  10, 11,
        },
        { // pattern 12
            0,  8,  1,  9,
            6,  14, 7,  15,
            4,  12, 5,  13,
            2,  10, 3,  11
        },
        { // pattern 13
            0,  1,  2,  3,
            4,  5,  6,  7,
            12, 14, 14, 15,
            8,  9,  10, 11
        },
        { // pattern 14
            0,  3,  11, 10,
            2,  5,  12, 12,
            4,  7,  15, 14,
            6,  1,  9,  8
        },
        { // pattern 15  - same as 14!
            0,  3,  11, 10,
            2,  5,  12, 12,
            4,  7,  15, 14,
            6,  1,  9,  8
        },

        { // pattern 16 - same as 14!
            0,  3,  11, 10,
            2,  5,  12, 12,
            4,  7,  15, 14,
            6,  1,  9,  8
        }

    };

//        snakes_.push_back(std::make_unique<LinearAlgo>());
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 1", renepatterns[0]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 2", renepatterns[1]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 3", renepatterns[2]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 4", renepatterns[3]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 5", renepatterns[4]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 6", renepatterns[5]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 7", renepatterns[6]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 8", renepatterns[7]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 9", renepatterns[8]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 10", renepatterns[9]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 11", renepatterns[10]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 12", renepatterns[11]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 13", renepatterns[12]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 14", renepatterns[13]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 15", renepatterns[14]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 16", renepatterns[15]));
}

