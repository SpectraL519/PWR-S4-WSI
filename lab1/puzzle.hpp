#pragma once

#include <memory>
#include <unordered_set>
#include <cmath>

#include "state.hpp"
#include "priority_queue.hpp"





namespace puzzle {
    std::pair <std::vector <state>, ull> solve (std::shared_ptr <state> inittial_state);
}



std::pair <std::vector <puzzle::state>, ull> puzzle::solve (std::shared_ptr <state> inittial_state) {
    priority_queue open;
    std::unordered_set <std::shared_ptr <state>, state_hash, state_equal> hash;
    std::shared_ptr <state> solution = nullptr;

    open.push(inittial_state);
    hash.insert(inittial_state);
    ull visited = 0;

    while (!open.empty()) {
        std::shared_ptr <state> state_ = open.pop();
        visited++;

        if (state_->is_solved()) {
            solution = state_;
            break;
        }

        for (std::shared_ptr <state> adj : state_->adjacent()) {
            auto it = hash.find(adj);

            uc new_cost = state_->cost() + 1;
            if (it != hash.end()) {
                adj = *it;
                if (new_cost < adj->cost()) {
                    adj->set_parent(state_);
                    adj->set_cost(new_cost);

                    if (adj->is_open()) 
                        open.update(adj);
                    else 
                        open.push(adj);
                }
            }
            else {
                adj->set_open();
                adj->set_parent(state_);
                adj->set_cost(new_cost);
                
                hash.insert(adj);
                open.push(adj);
            }
        }
    }

    std::vector <state> result;
    while (solution != nullptr) {
        result.push_back(*solution);
        solution = solution->parent();
    }

    return std::make_pair(result, visited);
}