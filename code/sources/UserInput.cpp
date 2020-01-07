#include <RandomServices.hpp>
#include "UserInput.hpp"

/**
 * Asks the user whether he wants to use random mazes from "input/". Returns true if that is the case.
 * 
 * @return a boolean; true when the user wants to randomly add mazes, false when he does not
 */
bool UserInput::useRandomPreBuiltMazes() {
    int input;
    printf("Should we use random, pre-built mazes?\n");
    printf("  Options: (0) False, (1) True.\n");
    do {
        printf("  Choice: "), scanf("%d", &input);
    } while (input != 0 && input != 1);
    return input == 1;
}

/**
 * If you chose the option to add random mazes from "input/". This function asks how many mazes you whish to add.
 * 
 * @return the amount of random mazes you whish to add
 */
int UserInput::randomPreBuiltMazesAmount() {
    int input;
    printf("How many runs should be performed? Each run has a random maze assigned to it.\n");
    printf("  Options: An integer (greater than zero).\n");
    do {
        printf("  Choice: "), scanf("%d", &input);
    } while (input <= 0);
    return input;
}

/**
 * This adds randomAmount amount of mazes which are randomly chosen from the "input/" folder.
 * 
 * @return a vector of strings representing mazes
 */
std::vector<std::string> UserInput::randomMazeSelection() {
    int mazeIndex, randomMaze, mazeAmount;
    std::string current;
    std::vector<std::string> mazes;
    mazeAmount = UserInput::randomPreBuiltMazesAmount();
    for (mazeIndex = 0; mazeIndex < mazeAmount; mazeIndex++) {
        randomMaze = RandomServices::discreteUniformSample(Maze::MAZES_AVAILABLE - 1) + 1;
        mazes.push_back("input/maze-" + std::to_string(randomMaze) + ".in");
    }
    return mazes;
}

/**
 * Creates a vector of strings which represents the chosen/created mazes as selected by the user.
 * 
 * @return a vector of strings representing mazes
 */
std::vector<std::string> UserInput::selectedMazes() {
    std::string currentString;
    std::vector<std::string> mazes;
    if (UserInput::useRandomPreBuiltMazes()) {
        /* Randomly generate numerous pre-built mazes. */
        return UserInput::randomMazeSelection();
    }
    printf("Which mazes would you like to use?\n");
    printf("  Options: Pre-built maze in input/ (excluding .in), or self-built maze (by typing '-').\n");
    printf("  Multiple entries possible. Stop by typing 'done'.\n");
    do {
        printf("  Choice: ");
        std::cin >> currentString;
        if (currentString == "-") {
            /* Input maze. */
            mazes.emplace_back("");
        } else if (currentString != "done") {
            /* Pre-built maze. */
            mazes.push_back("input/" + currentString + ".in");
        }
    } while (currentString != "done");
    return mazes;
}

/**
 * Asks the user whether he wants to add all players to the experiment. Returns true if that is the case.
 * 
 * @return a boolean; true when the user wants to use all players, false when he does not
 */
bool UserInput::useAllPlayers() {
    int input;
    printf("Use all available algorithms?\n");
    printf("  Options: (0) False, (1) True.\n");
    do {
        printf("  Choice: "), scanf("%d", &input);
    } while (input != 0 && input != 1);
    return input == 1;
}

/**
 * Returns a player type based on the provided integer parameter. The parameter corresponds to the user menu.
 * 
 * @param i integer the user chose that corresponds with a certain type of player
 * @return player type according to the provided parameter
 */
Player::Types UserInput::playerTypeFromInteger(int i) {
    switch (i) {
        case 0:
            return Player::Types::SynchronousPolicyIteration;
        case 1:
            return Player::Types::AsynchronousPolicyIteration;
        case 2:
            return Player::Types::SynchronousValueIteration;
        case 3:
            return Player::Types::AsynchronousValueIteration;
        case 4:
            return Player::Types::MonteCarloExploringStarts;
        case 5:
            return Player::Types::MonteCarloFirstVisit;
        default:
            return Player::Types::MonteCarloEveryVisit;
    }
}

/**
 * Creates a vector of player types as selected by the user.
 * 
 * @return a vector of player types
 */
std::vector<Player::Types> UserInput::selectedPlayers() {
    int input, playerTypeIndex;
    std::vector<Player::Types> selection;
    if (UserInput::useAllPlayers()) {
        /* Add all players to the queue once. */
        for (playerTypeIndex = 0; playerTypeIndex < Player::PLAYER_TYPE_AMOUNT; playerTypeIndex++) {
            selection.push_back(UserInput::playerTypeFromInteger(playerTypeIndex));
        }
        return selection;
    }
    printf("Which algorithms would you like to use?\n");
    printf("  Options:\n");
    printf("    (0) Synchronous Policy Iteration.\n");
    printf("    (1) Asynchronous Policy Iteration.\n");
    printf("    (2) Synchronous Value Iteration.\n");
    printf("    (3) Asynchronous Value Iteration.\n");
    printf("    (4) Monte Carlo Exploring Starts.\n");
    printf("    (5) Monte Carlo First Visit Control (yields soft policy).\n");
    printf("    (6) Monte Carlo Every Visit Control (yields soft policy).\n");
    printf("  Multiple entries possible. Stop by typing '-1'.\n");
    do {
        printf("  Choice: "), scanf("%d", &input);
        if (input >= 0 && input < Player::PLAYER_TYPE_AMOUNT) {
            selection.push_back(UserInput::playerTypeFromInteger(input));
        }
    } while (input != -1);
    return selection;
}

/**
 * Creates Experiment object with the players and mazes as selected by the user.
 * 
 * @return an Experiment with selected players and mazes
 */
Experiment UserInput::experimentFromUserInput() {
    return Experiment(UserInput::selectedPlayers(), UserInput::selectedMazes());
}
