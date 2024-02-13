#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <functional>

#include "puzzle.hpp"





typedef unsigned long long ull;
typedef unsigned char uc;

namespace puzzle {
    class state {
        public:
            static const int board_size = 16;
            enum direction {UP, DOWN, LEFT, RIGHT, SIZE = 4};


        private:
            ull _board;
            std::shared_ptr <state> _parent;
            uc _cost;
            std::function <uc(const state&)> _heuristic_f;
            uc _heuristic;
            size_t _hash_value;
            bool _is_open;
            int _heap_index;

            ull _swap (int i, int j) const;
            state _move (direction d) const;
            void _set_position (ull value, int index);
            void _calculate_hash ();


        public:
            state ();
            state (
                ull board, 
                std::function <uc(const state&)> heuristic_f
            );
            state (
                const int board[state::board_size],
                std::function <uc(const state&)> heuristic_f
            );
            ~state() = default;

            // operators
            unsigned operator [] (int index) const;
            bool operator < (const state &state_) const;
            bool operator == (const state &state_) const { return (this->_board == state_._board); };
            friend std::ostream& operator << (std::ostream &os, const state &state_);
            
            // getters
            std::shared_ptr <state> parent () const { return this->_parent; }
            uc cost () const { return this->_cost; }
            std::function <uc(const state&)> heuristic () const { return this->_heuristic_f; }
            uc f () const { return (this->_cost + this->_heuristic); }
            size_t hash_value () const { return this->_hash_value; }
            bool is_open () { return this->_is_open; }
            int heap_index () const { return this->_heap_index; }
            std::vector <std::shared_ptr <state>> adjacent ();

            // setters
            void set_parent (std::shared_ptr <state> parent_) { this->_parent = parent_; }
            void set_cost (uc cost_) { this->_cost = cost_; }
            void set_heuristic (std::function <uc(const state&)> heuristic_f) { this->_heuristic_f = heuristic_f; }
            void set_open () { this->_is_open = true; }
            void set_heap_index (int index) { this->_heap_index = index; }

            // checker methods
            bool is_null () { return (this->_board == 0ULL); }
            bool is_solvable ();
            bool is_solved () { return (this->_board == 0x123456789abcdef0); }

            // other
            void shuffle (int num_moves);
    };



    struct state_hash {
        public:
            size_t operator () (std::shared_ptr <state> state_) const {
                return state_->hash_value();
            }
    };

    struct state_equal {
        public:
            bool operator () (std::shared_ptr <state> state_a, std::shared_ptr <state> state_b) const {
                return (*state_a == *state_b);
            }
    };

    struct state_less {
        public:
            bool operator () (std::shared_ptr <state> state_a, std::shared_ptr <state> state_b) const {
                return (*state_a < *state_b);
            }
    };



    namespace heuristic {
        uc manhattan_distance (const state &state_);
        uc inversion_distance (const state &state_);
    }



    std::ostream& operator << (std::ostream &os, const puzzle::state &state_) {
        for (int i = 0; i < puzzle::state::board_size; i++) {
            os << state_[i] << ' ';
            if (i % 4 == 3)
                os << '\n';
        }

        return os;
    }
}



puzzle::state::state () {
    this->_board = 0ULL;
    this->_cost = 0;
    this->_heuristic_f = heuristic::manhattan_distance;
    this->_heuristic = 0;
    this->_parent = nullptr;
    this->_hash_value = 0;
    this->_is_open = false;
}

puzzle::state::state (ull board, std::function <uc(const state&)> heuristic_f) {
    this->_board = board;
    this->_cost = 0;
    this->_heuristic_f = heuristic_f;
    this->_heuristic = this->_heuristic_f(*this);
    this->_parent = nullptr;
    this->_is_open = false;
    this->_calculate_hash();
}

puzzle::state::state (const int board[state::board_size], std::function <uc(const state&)> heuristic_f) {
    this->_board = 0ULL;
    for (int i = 0; i < state::board_size; i++)
        this->_set_position(board[i], i);

    this->_cost = 0;
    this->_heuristic_f = heuristic_f;
    this->_heuristic = this->_heuristic_f(*this);
    this->_parent = nullptr;
    this->_is_open = false;
    this->_calculate_hash();
}



unsigned puzzle::state::operator [] (int index) const {
    return (unsigned)(this->_board << (4 * index) >> 60);
}

bool puzzle::state::operator < (const state &state_) const {
    if ((this->_cost + this->_heuristic) < state_.f())
        return true;
    
    if ((this->_cost + this->_heuristic) == state_.f() && this->_heuristic < state_._heuristic)
        return true;
        
    return false;
}



std::vector <std::shared_ptr <puzzle::state>> puzzle::state::adjacent () {
    std::vector<std::shared_ptr<state>> adjacent;

    for (int i = 0; i < direction::SIZE; i++) {
        state adj = this->_move((direction)i);
        if (!adj.is_null())
            adjacent.push_back(std::shared_ptr<state>(new state(adj)));
    }

    return adjacent;
}



bool puzzle::state::is_solvable () {
    int inversions = 0;
    for (int i = 0; i < state::board_size; i++) {
        for (int j = i + 1; j < 16; j++) {
            int value_at_i = this->operator[](i);
            int value_at_j = this->operator[](j);

            if (value_at_i == 0 || value_at_j == 0)
                continue;
            
            if (value_at_i > value_at_j)
                inversions++;
        }
    }

    int empty_position = 0;
    while (this->operator[](empty_position) != 0)
        empty_position++;

    return (empty_position / 4 % 2 != inversions % 2);
}



void puzzle::state::shuffle (int num_moves) {
    std::mt19937 generator;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);
    std::uniform_int_distribution <int> uniform {0, state::direction::SIZE - 1 };

    while (num_moves--) {
        state state_ = this->_move((direction)uniform(generator));
        if (!state_.is_null()) {
            this->_board = state_._board;
            this->_cost = state_._cost;
            this->_heuristic = state_._heuristic;
            this->_parent = state_._parent;
        }
    }
}



ull puzzle::state::_swap (int i, int j) const {
    ull board = this->_board;
    ull value_at_i = this->operator[](i);
    ull value_at_j = this->operator[](j);

    board &= ~(0xf000000000000000 >> (4 * i)); // clear value at i
    board &= ~(0xf000000000000000 >> (4 * j)); // clear value at j
    board |= (value_at_i << (64 - 4 * j - 4)); // insert value at j to i-th position
    board |= (value_at_j << (64 - 4 * i - 4)); // insert value at i to j-th position

    return board;
}

puzzle::state puzzle::state::_move (direction d) const {
    int empty = 0;
    while (this->operator[](empty) != 0)
        empty++;

    switch (d) {
        case UP:
            if (empty < 12)
                return state(this->_swap(empty, empty + 4), this->_heuristic_f);
            break;
        case DOWN:
            if (empty > 3)
                return state(this->_swap(empty, empty - 4), this->_heuristic_f);
            break;
        case LEFT:
            if (empty % 4 < 3)
                return state(this->_swap(empty, empty + 1), this->_heuristic_f);
            break;
        case RIGHT:
            if (empty % 4 > 0)
                return state(this->_swap(empty, empty - 1), this->_heuristic_f);
            break;
    }

    return state();
}

void puzzle::state::_set_position (ull value, int index) {
    this->_board &= ~(0xf000000000000000 >> (4 * index));
    this->_board |= (value << (64 - 4 * index - 4));
}

void puzzle::state::_calculate_hash () {
    size_t h = 0;
    size_t mod = (1LL << 56) - 5;
    for (int i = 0; i < state::board_size; i++)
        h = (h * 4 + this->operator[](i)) % mod;

    h = h ^ (h >> 21);
    h = h ^ (h << 37);
    h = h ^ (h >> 4);
    h = h * 4768777513237032717;
    h = h ^ (h << 20);
    h = h ^ (h >> 41);
    h = h ^ (h <<  5);
    this->_hash_value = h;
}



uc puzzle::heuristic::manhattan_distance (const state &state_) {
    uc distance = 0;
    for (int i = 0; i < state::board_size; i++) {
        unsigned value_at_i = (uc)state_[i];
        if (value_at_i == 0)
            continue;
        distance += (uc)(abs((value_at_i - 1) / 4 - i / 4) + abs((value_at_i - 1) % 4 - i % 4));
    }

    return distance;
} 

uc puzzle::heuristic::inversion_distance (const state &state_) {
    uc vertical = 0;
    uc horizontal = 0;

    for (int i = 0; i < state::board_size; i++) {
        unsigned value_at_i = state_[i];
        if (value_at_i != 0) {
            for (int j = 0; j < i; j++) {
                if (value_at_i < (uc)state_[j])
                    vertical++;
            }
        }
    }
    vertical /= 3;

    int unpacked[16];
    int idx = 0;
    for (int i = 0; i < 4; i++) 
        for (int j = 0; j < 4; j++) 
            unpacked[idx++] = j * 4 + i;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int value = state_[i * 4 + j] - 1;
            if (value != -1) {
                idx = 0;
                for (int k = 0; k < state::board_size; k++) {
                    if (unpacked[k] == value) {
                        idx = k;
                        break;
                    }
                }
                horizontal += (uc)abs(idx - (j * 4 + i));
            }
        }
    }
    horizontal /= 3;

    return (vertical + horizontal);
} 