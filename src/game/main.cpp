#include "poker.h"
#include <algorithm>
#include <array>
#include <iostream>

int main() {
	Player player;
	
	std::array<std::array<bool, 15>, 4> arr;
	for (int i = 0; i < 4; i++) {
		arr.at(i).fill(false);
	}

	arr.at(0).at(10) = true;
	arr.at(0).at(11) = true;
	arr.at(0).at(12) = true;
	arr.at(0).at(13) = true;
	arr.at(0).at(14) = true;

	player.evaluate_hand(arr);
	std::cout << static_cast<int>(player.get_hand_evaluation()) << std::endl;
}

