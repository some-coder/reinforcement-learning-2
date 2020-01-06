#include <utility>
#include <fstream>
#include "Datum.hpp"

Datum::Datum(int id, int mazeWidth, int mazeHeight, std::string mazeIdentifier, std::vector<Player::Types> players,
        std::map<Player::Types, std::vector<double>> timings,
        std::map<Player::Types, std::map<std::tuple<int, int, Maze::Actions>, double>> policies) {
    this->id = id;
    this->mazeWidth = mazeWidth;
    this->mazeHeight = mazeHeight;
    this->mazeIdentifier = std::move(mazeIdentifier);
    this->players  = std::move(players);
    this->timings  = std::move(timings);
    this->policies = std::move(policies);
}

Datum::Datum(int id) {
    this->id = id;
    this->mazeWidth = this->mazeHeight = 0;
}

Datum::~Datum() = default;

std::string Datum::singlePlayerTimings(Player::Types type) {
    int timingIndex;
    std::string playerTimings;
    for (timingIndex = 0; timingIndex < (int)this->timings[type].size(); timingIndex++) {
        playerTimings.append("\"" + std::to_string(this->id) + "\",");
        playerTimings.append("\"" + this->mazeIdentifier + "\",");
        playerTimings.append("\"" + Player::playerTypeAsString(type) + "\",");
        playerTimings.append("\"" + std::to_string(timingIndex) + "\",");
        playerTimings.append("\"" + std::to_string(this->timings[type][timingIndex]) + "\"\n");
    }
    return playerTimings;
}

std::string Datum::playerTimings() {
    int playerIndex;
    std::string playerTimings;
    for (playerIndex = 0; playerIndex < (int)this->players.size(); playerIndex++) {
        playerTimings.append(this->singlePlayerTimings(this->players[playerIndex]));
    }
    return playerTimings;
}

std::string Datum::singlePlayerPolicy(Player::Types type) {
    int xIndex, yIndex, actionIndex;
    std::string playerPolicy;
    std::tuple<int, int, Maze::Actions> stateActionPair;
    for (xIndex = 0; xIndex < this->mazeWidth; xIndex++) {
        for (yIndex = 0; yIndex < this->mazeHeight; yIndex++) {
            for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
                stateActionPair = std::make_tuple(xIndex, yIndex, (Maze::Actions)actionIndex);
                playerPolicy.append("\"" + std::to_string(this->id) + "\",");
                playerPolicy.append("\"" + this->mazeIdentifier + "\",");
                playerPolicy.append("\"" + Player::playerTypeAsString(type) + "\",");
                playerPolicy.append("\"" + std::to_string(xIndex) + "\",");
                playerPolicy.append("\"" + std::to_string(yIndex) + "\",");
                playerPolicy.append("\"" + Maze::actionAsString((Maze::Actions)actionIndex) + "\",");
                playerPolicy.append("\"" + std::to_string(this->policies[type][stateActionPair]) + "\"\n");
            }
        }
    }
    return playerPolicy;
}

std::string Datum::playerPolicies() {
    int playerIndex;
    std::string playerPolicies;
    for (playerIndex = 0; playerIndex < (int)this->players.size(); playerIndex++) {
        playerPolicies.append(this->singlePlayerPolicy(this->players[playerIndex]));
    }
    return playerPolicies;
}

void Datum::writeDatumToFiles() {
    std::ofstream output;
    output.open("output/data/timings.csv");
    output << this->playerTimings();
    output.close();
    output.open("output/data/policies.csv");
    output << this->playerPolicies();
    output.close();
}