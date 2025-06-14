#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <charconv>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <ranges>
#include "cards.h"
#include "errors.h"

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
	using poker_cards_arr = std::array<std::array<bool, 15>, 4>;
	
	private:
	std::unique_ptr<Card> card1, card2;
	int cash;

	private:
	HandEval hand_evaluation;
	int pairs;
	int streak, start, end; // For straight and flush
	int threes;
	bool four, same_colour;

	public:
	Player() : hand_evaluation(HandEval::Nothing),
			   pairs(0),
			   threes(0),
			   four(false),
			   same_colour(false),
			   streak(0),
			   start(0),
			   end(0)
	{} 

	private:
	bool card_value_exists(const int &value, const poker_cards_arr &poker_cards_arr) {
		for (int i = 0; i < 4; i++) {
			if (poker_cards_arr.at(i).at(value)) {
				return true;
			}
		}

		return false;
	}

	private:
	bool straight_or_royal_flush(const int &dominant_colour, const poker_cards_arr &poker_cards_arr) {
		if (streak < 5) {
			return false;
		}

		if (dominant_colour < 0) {
			return false;
		}

		if (start == 0 or end == 0) {
			return false;
		}

		// Should not be possible
		if (end > 14) {
			throw std::runtime_error(StreakEndOutOfBounds);
		}
		
		for (int i = start; i <= end; i++) {
			if (!poker_cards_arr.at(dominant_colour).at(i)) {
				return false;
			}
		}

		return true;
	}

	private:
	// For evaluating weather we have one of the following; pair, ..., four of a kind 
	int get_amount_of_same_values(const int &value, const poker_cards_arr &poker_cards_arr) {
		int amount_of_same_values = 0;

		for (int i = 0; i < 4; i++) {
			if (poker_cards_arr.at(i).at(value)) {
				amount_of_same_values++;
			}
		}

		return amount_of_same_values;
	}

	private:
	void check_max_streak 
		(const int &curr_start, const int &curr_end, const int &curr_streak) {
		if (streak > curr_streak) {
			return;
		}

		end = curr_end;
		start = curr_start;
		streak = curr_streak;
	}

	public:
	void evaluate_hand(poker_cards_arr poker_cards_arr) {
		if (card1 == nullptr) {
			throw std::runtime_error(CardsWereNotAssigned);
		}

		if (card2 == nullptr) {
			throw std::runtime_error(CardsWereNotAssigned);
		}

		poker_cards_arr.at(static_cast<int>(card1->get_color())).at(card1->get_value()) = true;
		poker_cards_arr.at(static_cast<int>(card2->get_color())).at(card2->get_value()) = true;

		int curr_streak = 0, curr_start = 0, curr_end = -1;
		for (int i = 2; i <= 14; i++) {
			bool exists_flag = card_value_exists(i, poker_cards_arr);

			if (curr_streak == 0 and exists_flag) {
				curr_start = i;
				curr_streak = 0;	
			}

			if (exists_flag) {
				curr_streak++;
				curr_end = i;
			} else {
				check_max_streak(curr_start, curr_end, curr_streak);
				curr_streak = 0;
			}

			switch (get_amount_of_same_values(i, poker_cards_arr)) {
				case 2:
					pairs++;
					break;
				case 3:
					threes++;
					break;
				case 4:
					four = true;
					break;
			}
		}

		check_max_streak(curr_start, curr_end, curr_streak);

		int dominant_colour = -1;
		for (int i = 0; i < 4; i++) {
			// Error here exists only if compiling with C++ pre 20 
			// To avoid this error add -std=c++20 flag to your compiler
			if (std::ranges::count(poker_cards_arr.at(i), true) >= 5) {
				dominant_colour = i;
				same_colour = true;
				break;
			}
		}

		if (pairs == 1) {
			hand_evaluation = HandEval::OnePair;
		} else if (pairs >= 2) {
			hand_evaluation = HandEval::TwoPair;
		}

		if (threes) {
			hand_evaluation = HandEval::ThreeOfAKind;
		}

		if (streak >= 5) {
			hand_evaluation = HandEval::Straight;
		}

		if (same_colour) {
			hand_evaluation = HandEval::Flush;
		}

		if (threes >= 1 and pairs >= 1) {
			hand_evaluation = HandEval::FullHouse;
		}

		if (four) {
			hand_evaluation = HandEval::FourOfAKind;
		}
		
		const bool flush_flag = 
			straight_or_royal_flush(dominant_colour, poker_cards_arr);

		if (streak >= 5 and flush_flag) {
			hand_evaluation = HandEval::StraightFlush;
		}

		if (streak >= 5 and flush_flag 
			and start == 10 and end == 14) {
			hand_evaluation = HandEval::RoyalFlush;
		}
	}

	std::string get_next_round_input() {
		std::string next_round_input;
		std::cin >> next_round_input;
		
		return next_round_input;
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

