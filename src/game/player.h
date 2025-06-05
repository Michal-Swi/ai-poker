#include <array>
#include <memory>
#include <utility>
#include "cards.h"

using unique_ptr_pair = std::pair<std::unique_ptr<Card>, std::unique_ptr<Card>>;

enum class HandEval {
	Nothing = 0,
	OnePair = 1,
	TwoPair = 2,
	ThreeOfAKind = 3,
	Straight = 4, 
	Flush = 5,
	FullHouse = 6,
	FourOfAKind = 7,
	StraightFlush = 8,
	RoyalFlush = 9
};

class Player {
	private:
	std::unique_ptr<Card> card1, card2;
	int cash;

	private:
	HandEval hand_evaluation;

	public:
	void evaluate_hand(std::array<Colors, 4> color_arr, std::array<int, 15> table_vals) {
			
	}

	HandEval get_hand_evaluation() {
		return hand_evaluation;
	}

	public:
	void assign_cards(std::unique_ptr<Card> card1, std::unique_ptr<Card> card2) {
		this->card1 = std::move(card1);
		this->card2 = std::move(card2);
	}

	public:
	unique_ptr_pair return_cards() {
		return std::make_pair(std::move(card1), std::move(card2));
	}
};

