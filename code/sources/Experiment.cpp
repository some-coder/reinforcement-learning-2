#include <RandomServices.hpp>
#include <ExploitPlayer.hpp>
#include "Experiment.hpp"

/**
 * Constructs an experiment.
 *
 * @param selectedPlayers The player types to use.
 * @param runNumber The number of runs involved.
 */
Experiment::Experiment(std::vector<Player::Types> selectedPlayers, int runNumber) {
    this->selectedPlayers = std::move(selectedPlayers);
    this->runNumber = runNumber;
}

/**
 * Constructs an experiment.
 *
 * @param selectedPlayers The player types to use.
 * @param selectedMazes The maze identifiers of the mazes to use.
 */
Experiment::Experiment(std::vector<Player::Types> selectedPlayers, std::vector<std::string> selectedMazes) {
    this->selectedPlayers = std::move(selectedPlayers);
    this->selectedMazes   = std::move(selectedMazes);
    this->runNumber = (int)this->selectedMazes.size();
}

/**
 * Destructs the experiment.
 */
Experiment::~Experiment() = default;

/**
 * Obtains the maze identifier of the given run.
 *
 * @param runIndex The index of the run of which to get the maze identifier.
 * @return The maze identifier.
 */
std::string Experiment::runMazeIdentifier(int runIndex) {
    int mazeIndex;
    if (this->selectedMazes.empty()) {
        mazeIndex = RandomServices::discreteUniformSample(Maze::MAZES_AVAILABLE - 1) + 1;
        return "input/maze-" + std::to_string(mazeIndex) + ".in";
    } else {
        return this->selectedMazes[runIndex];
    }
}

/**
 * Clears the files "output/data/timings.csv" and "output/data/policies.csv".
 * 
 * There might still be data in "output/data/timings.csv" and "output/data/policies.csv" 
 * from a previous experiment. This function clears these files.
 */
void Experiment::clearOldData() {
    printf("  (1/6) Removing old data.\n");
    std::ofstream output;
    output.open("output/data/timings.csv", std::ofstream::trunc);
    output.close();
    output.open("output/data/average-policies.csv", std::ofstream::trunc);
    output.close();
    output.open("output/data/exploitation.csv", std::ofstream::trunc);
    output.close();
    output.open("output/data/progression.csv", std::ofstream::trunc);
    output.close();
}

/**
 * Compute the average policy of some player, in some maze.
 *
 * @param mazeIdentifier The identifier of the maze.
 * @param type The player type.
 * @return The average policy.
 */
std::map<std::tuple<int, int, Maze::Actions>, double> Experiment::averagePolicy(const std::string& mazeIdentifier,
        Player::Types type) {
    int datumIndex, count;
    std::map<std::tuple<int, int, Maze::Actions>, double> policy;
    std::map<std::tuple<int, int, Maze::Actions>, double> average;
    count = 0;
    for (datumIndex = 0; datumIndex < (int)this->data.size(); datumIndex++) {
        policy = this->data[datumIndex].getPolicy(type);
        if (this->data[datumIndex].getMazeIdentifier() == mazeIdentifier) {
            count++;
            if (average.empty()) {
                /* Simply copy the policy. */
                average = policy;
            } else {
                /* Add to the probability sum for the state action pairs. */
                for (auto const& a : average) {
                    average[a.first] += policy[a.first];
                }
            }
        }
    }
    for (auto const& a : average) {
        /* Correct for the number of times the map was present in the experiment. */
        average[a.first] /= (double)count;
    }
    return average;
}

/**
 * Conduct the experiment by running all runs.
 */
void Experiment::conductRuns() {
    int runIndex;
    printf("  (2/6) Conducting runs.\n");
    for (runIndex = 0; runIndex < this->runNumber; runIndex++) {
        Experiment::reportProgress(runIndex, this->runNumber);
        Run run = Run(runIndex, this->runMazeIdentifier(runIndex), this->selectedPlayers);
        this->data.push_back(run.conductRun());
    }
    printf("\n");
}

/**
 * Determines whether the given maze identifier is already present.
 *
 * @param mazeIdentifier The maze identifier to check for.
 * @return The question's answer.
 */
bool Experiment::mazeIdentifierAlreadyPresent(std::string mazeIdentifier) {
    int identifierIndex;
    for (identifierIndex = 0; identifierIndex < (int)this->mazeIdentifiers.size(); identifierIndex++) {
        if (this->mazeIdentifiers[identifierIndex] == mazeIdentifier) {
            return true;
        }
    }
    return false;
}

/**
 * Stringify the maze identifiers.
 *
 * @return The maze identifiers, stringified.
 */
std::vector<std::string> Experiment::getMazeIdentifiers() {
    int datumIndex;
    std::string current;
    std::vector<std::string> identifiers;
    printf("  (3/6) Assigning maze identifiers.\n");
    for (datumIndex = 0; datumIndex < (int)this->data.size(); datumIndex++) {
        Experiment::reportProgress(datumIndex, (int)this->data.size());
        current = this->data[datumIndex].getMazeIdentifier();
        if (!this->mazeIdentifierAlreadyPresent(current)) {
            identifiers.push_back(current);
        }
    }
    printf("\n");
    return identifiers;
}

/**
 * Reports the progress on some sector of the experiment.
 *
 * @param current The current progress, expressed as an integer.
 * @param maximum The goal, expressed as an integer.
 */
void Experiment::reportProgress(int current, int maximum) {
    printf("\r    [%.1lf%%]", (((double)current + 1.0) / maximum) * 100.0);
    fflush(stdout);
}

/**
 * Computes the average policies of the players on each individual maze.
 */
void Experiment::getAveragePolicies() {
    int mazeIdentifierIndex, playerTypeIndex;
    std::string mazeIdentifier;
    Player::Types playerType;
    printf("  (4/6) Taking the averages of policies.\n");
    for (mazeIdentifierIndex = 0; mazeIdentifierIndex < (int)this->mazeIdentifiers.size(); mazeIdentifierIndex++) {
        Experiment::reportProgress(mazeIdentifierIndex, (int)this->mazeIdentifiers.size());
        mazeIdentifier = this->mazeIdentifiers[mazeIdentifierIndex];
        for (playerTypeIndex = 0; playerTypeIndex < (int)this->selectedPlayers.size(); playerTypeIndex++) {
            playerType = this->selectedPlayers[playerTypeIndex];
            this->averagePolicies[std::make_tuple(mazeIdentifier, playerType)] = this->averagePolicy(mazeIdentifier,
                    playerType);
        }
    }
    printf("\n");
}

/**
 * Obtains the policies in converted form, ready for output.
 *
 * @param m The maze to convert a player's policy on it for.
 * @param type The player whose policy to convert.
 * @return The converted policy.s
 */
std::map<State*, std::vector<double>> Experiment::convertedPolicy(Maze *m, Player::Types type) {
    int stateIndex, actionIndex;
    State *s;
    std::map<State*, std::vector<double>> converted;
    std::tuple<std::string, Player::Types> outerKey;
    std::tuple<int, int, Maze::Actions> innerKey;
    for (stateIndex = 0; stateIndex < (int)m->getStates()->size(); stateIndex++) {
        s = m->getState(stateIndex);
        std::vector<double> actionProbabilities;
        for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
            outerKey = std::make_tuple(m->getMazeIdentifier(), type);
            innerKey = std::make_tuple(s->getX(), s->getY(), (Maze::Actions)actionIndex);
            actionProbabilities.push_back(this->averagePolicies[outerKey][innerKey]);
        }
        converted[s] = actionProbabilities;
    }
    return converted;
}

/**
 * Employs exploit players to evaluate a player's policy on a maze.
 *
 * @param mazeIdentifierIndex The identifier index to the maze to evaluate.
 * @param type The player type to evaluate for.
 */
void Experiment::evaluateAveragePolicy(int mazeIdentifierIndex, Player::Types type) {
    int evaluationIndex;
    std::string mazeIdentifier = this->mazeIdentifiers[mazeIdentifierIndex];
    Maze maze = Maze(mazeIdentifier);
    std::vector<double> rewards;
    std::map<State*, std::vector<double>> m;
    for (evaluationIndex = 0; evaluationIndex < EVALUATION_RUNS; evaluationIndex++) {
        m = this->convertedPolicy(&maze, type);
        ExploitPlayer p = ExploitPlayer(&maze, m);
        p.solveMaze();
        rewards.push_back(p.getRewardTotal());
    }
    this->averagePolicyRewards[std::make_tuple(maze.getMazeIdentifier(), type)] = rewards;
}

/**
 * Employs exploit players to evaluate all player's policies on all mazes.
 */
void Experiment::evaluateAveragePolicies() {
    int mazeIdentifierIndex, playerIndex;
    printf("  (5/6) Evaluating average policies.\n");
    for (mazeIdentifierIndex = 0; mazeIdentifierIndex < (int)this->mazeIdentifiers.size(); mazeIdentifierIndex++) {
        Experiment::reportProgress(mazeIdentifierIndex, (int) this->mazeIdentifiers.size());
        for (playerIndex = 0; playerIndex < (int) this->selectedPlayers.size(); playerIndex++) {
            this->evaluateAveragePolicy(mazeIdentifierIndex, this->selectedPlayers[playerIndex]);
        }
    }
    printf("\n");
}

/**
 * Converts a player's average policy reward on a maze to a string.
 *
 * @param mazeIdentifier The identifier to the maze to stringify for.
 * @param type The player type to stringify for.
 * @return The average policy reward of a player on a maze, as a string.
 */
std::string Experiment::averagePolicyRewardAsString(std::string mazeIdentifier, Player::Types type) {
    int rewardsIndex;
    std::vector<double> *rewards;
    std::string s;
    rewards = &(this->averagePolicyRewards[std::make_tuple(mazeIdentifier, type)]);
    for (rewardsIndex = 0; rewardsIndex < (int)rewards->size(); rewardsIndex++) {
        s.append("\"" + mazeIdentifier + "\",");
        s.append("\"" + Player::playerTypeAsStringShortened(type) + "\",");
        s.append("\"" + std::to_string((*rewards)[rewardsIndex]) + "\"\n");
    }
    return s;
}

/**
 * Writes the average policy rewards to output.
 */
void Experiment::writeAveragePoliciesRewards() {
    int mazeIdentifierIndex, playerIndex;
    std::ofstream output;
    output.open("output/data/exploitation.csv");
    for (mazeIdentifierIndex = 0; mazeIdentifierIndex < (int)this->mazeIdentifiers.size(); mazeIdentifierIndex++) {
        for (playerIndex = 0; playerIndex < (int)this->selectedPlayers.size(); playerIndex++) {
            output << this->averagePolicyRewardAsString(this->mazeIdentifiers[mazeIdentifierIndex],
                    this->selectedPlayers[playerIndex]);
        }
    }
    output.close();
}

/**
 * Writes data of the experiment to "output/data/".
 * 
 * Using the Datum class it writes the data in data[] to the output directory. The timings
 * (how long it takes for a player to complete an epoch) are written to "output/data/timings.csv".
 * The policies of the player for every epoch are written to "output/data/policies.csv".
 */
void Experiment::writeData() {
    int datumIndex;
    printf("  (6/6) Writing data to output.\n");
    for (datumIndex = 0; datumIndex < (int)this->data.size(); datumIndex++) {
        Experiment::reportProgress(datumIndex, (int)this->data.size());
        this->data[datumIndex].writeDatumToFiles();
    }
    printf("\n");
}

/**
 * Runs the experiment with provided player types and mazes, writes timing results to "output/data/timings.csv".
 * 
 * This function runs the experiment using the Run class. It uses the playerSelection to details what player algorithm(s)
 * should be run in this experiment (playerSelection provided when Experiment object was created). It uses mazeSelection to detail what maze(s)
 * the player should navigate (mazeSelection provided when Experiment object was created). Before we run the experiment we clear the files in "output/data/".
 * The time it takes for an algorithm to complete an epoch is written to "output/data/timings.csv". The policies of the player are written to "output/data/policies.csv"
 */
void Experiment::conductExperiment() {
    printf("\nEXPERIMENT IN PROGRESS\n");
    this->clearOldData();
    this->conductRuns();
    this->mazeIdentifiers = this->getMazeIdentifiers();
    this->getAveragePolicies();
    this->evaluateAveragePolicies();
    this->writeAveragePoliciesRewards();
    this->writeData();
}

