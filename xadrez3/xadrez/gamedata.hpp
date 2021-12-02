#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>
#include <vector>

enum class State {Playing, Check, Checkmate, Stalemate};

struct GameData {
	State m_state{State::Playing};
	std::vector<int> attacker_index;
};

#endif