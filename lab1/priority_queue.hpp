#pragma once

#include <vector>
#include <climits>
#include <memory>

#include "state.hpp"





namespace puzzle {
    class priority_queue {
        private:
            std::vector <std::shared_ptr <state>> heap;

            void _swap (int i, int j);
            void _sift_up (int index);
            void _sift_down (int index);
        

        public:
            priority_queue () { this->heap.push_back(NULL); }
            
            bool empty () const { return (this->heap.size() == 1); }
            int size () const { return (this->heap.size() - 1); }

            int min_key () const;
            void push (std::shared_ptr <state> element);
            std::shared_ptr <state> pop ();
            void remove (std::shared_ptr <state> element);
            void update (std::shared_ptr <state> element);
    };
}




void puzzle::priority_queue::_swap (int i, int j) {
    std::shared_ptr <state> tmp = this->heap[i];
    this->heap[i] = this->heap[j];
    this->heap[j] = tmp;
    this->heap[i]->set_heap_index(i);
    this->heap[j]->set_heap_index(j);
}

void puzzle::priority_queue::_sift_up (int index) {
    int parent_index = index / 2;
    if ((parent_index >= 1) && (*(this->heap[index]) < *(this->heap[parent_index]))) {
        this->_swap(index, parent_index);
        this->_sift_up(parent_index);
    }
}

void puzzle::priority_queue::_sift_down (int index) {
    int child_index = index;
    if ((2 * index < (int)heap.size()) && (*(heap[2 * index]) < *(heap[child_index]))) {
        child_index = 2 * index;
    }
    if ((2 * index + 1 < (int)heap.size()) && (*(heap[2 * index + 1]) < *(heap[child_index]))) {
        child_index = 2 * index + 1;
    }
    if (index != child_index) {
        this->_swap(index, child_index);
        this->_sift_down(child_index);
    }
}



int puzzle::priority_queue::min_key () const {
    if (!empty()) {
        return heap[1]->f();
    }
    return INT_MAX;
}

void puzzle::priority_queue::push (std::shared_ptr <state> element) {
    this->heap.push_back(element);
    element->set_heap_index((int)heap.size() - 1);
    this->_sift_up((int)heap.size() - 1);
}

std::shared_ptr <puzzle::state> puzzle::priority_queue::pop () {
    if (!this->empty()) {
        std::shared_ptr <state> element = this->heap[1];
        this->_swap(1, (int)heap.size() - 1);
        this->heap.pop_back();
        if (1 < heap.size()) 
            this->_sift_down(1);

        return element;
    }
    return NULL;
}

void puzzle::priority_queue::remove (std::shared_ptr <state> element) {
    int index = element->heap_index();
    this->_swap(index, heap.size()-1);
    this->heap.pop_back();
    if (index < this->heap.size()) {
        this->_sift_up(index);
        this->_sift_down(index);
    }
}

void puzzle::priority_queue::update (std::shared_ptr <state> element) {
    int index = element->heap_index();
    this->_sift_up(index);
    this->_sift_down(index);
}