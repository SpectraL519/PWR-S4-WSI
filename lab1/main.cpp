#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <memory>

#include "puzzle.hpp"
#include "state.hpp"



int main (int argc, char *argv[]) {
    std::function <unsigned char(puzzle::state)> heuristic_f;
    if (argc < 2) 
        heuristic_f = puzzle::heuristic::manhattan_distance;
    else {
        std::string heuristic = argv[1];
        if (heuristic == "manhattan") 
            heuristic_f = puzzle::heuristic::manhattan_distance;
        else if (heuristic == "inversion") {
            heuristic_f = puzzle::heuristic::inversion_distance;
        }
        else {
            std::cerr << "Errror: Invalid value of <heuristic> - must be 'manhattan' or 'inversion'!";
            return 1;
        }
    }

    // initialize random board
    std::mt19937 generator;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);
    std::uniform_int_distribution <int> uniform { 1, puzzle::state::board_size - 1 };

    int board[puzzle::state::board_size];
    std::vector <bool> selected(puzzle::state::board_size, false);
    for (int i = 0; i < puzzle::state::board_size - 1; i++) {
        int element;
        do { element = uniform(generator); } while (selected[element]);
        board[i] = element;
        selected[element] = true;
    }
    board[puzzle::state::board_size - 1] = 0;


    std::shared_ptr <puzzle::state> init(new puzzle::state(board, heuristic_f));
    std::cout << "Initial permutation:\n" << *init << '\n';

    if (init->is_solvable()) {
        std::cout << "Solving...\n";
        auto start = std::chrono::high_resolution_clock::now();

        auto [result, visited] = puzzle::solve(init);

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        int size = result.size() - 1;
        for (int i = size; i >= 0; i--) 
            std::cout << size - i << ":\n" << result[i] << '\n';
        std::cout << "# visited nodes: " << visited << '\n';
        std::cout << "Solution path length: " << size << '\n';
        std::cout << "Solving time: " << (float)duration.count() / 1000 << "s\n";
    }
    else
        std::cout << "Initial permutation is not solvable!\n";

    return 0;
}