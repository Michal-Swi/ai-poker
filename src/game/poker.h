#include <algorithm>
#include <array>
#include <memory>
#include <queue>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>
#include "errors.h"
#include "./player.h"

enum class Moves {
	Call, Raise, Check
};

class Poker {
	private:
	std::vector<std::unique_ptr<Card>> cards;
	std::queue<std::unique_ptr<Card>> cards_queue;	

	public:
	Poker() {
		for (int i = 0; i < 4; i++) {
			for (int j = 2; j <= 14; j++) {
				std::unique_ptr<Card> card = std::make_unique<Card>();
				card->load(j, i);

				cards.push_back(std::move(card));
			}
		}
	}

	private:
	std::unique_ptr<Card> get_next_card() {
		std::unique_ptr<Card> next_card = std::make_unique<Card>();
		
		if (cards_queue.empty()) {
			throw std::runtime_error(NoCardsInQueue);
		}

		next_card = std::move(cards_queue.front());
		cards_queue.pop();

		return std::move(next_card);
	}

	private:
	std::vector<std::unique_ptr<Card>> table;

	private:
	void first_round() {
		table.clear();

		for (int i = 0; i < 3; i++) {
			table.push_back(std::move(get_next_card()));
		}
	}

	private:
	void next_round() {
		table.push_back(std::move(get_next_card()));
	}

	private:
	std::vector<std::unique_ptr<Player>> players;
	std::vector<HandEval> player_hand_evaluation;

	private:
	void create_players(const int &number_of_players) {
		for (int i = 0; i < number_of_players; i++) {
			auto player = std::make_unique<Player>();
			player->assign_cards(std::move(get_next_card()), 
					std::move(get_next_card()));
			players.push_back(std::move(player));
		}
	}

	private:
	std::array<std::array<bool, 15>, 4> poker_values_arr;

	private:
	void evaluate_table() {
		for (int i = 0; i < 4; i++) {
			poker_values_arr.at(i).fill(false);
		}

		for (auto &table_card : table) {
			int color_num = static_cast<int>
				(table_card->get_color());
			int value = table_card->get_value();

			poker_values_arr.at(color_num).at(value) = true;
		}
	}

	private:
	void evaluate_hands() {
		evaluate_table();

		for (auto &player : players) {
			player->evaluate_hand(poker_values_arr);
			HandEval hand = player->get_hand_evaluation();
			player_hand_evaluation.push_back(hand);
		}
	}

	private:
	void shuffle_cards() {
		std::random_device random_device;
		std::mt19937 engine(random_device());

		std::shuffle(cards.begin(), cards.end(), engine);
		std::cout << "Shuffled" << std::endl;

		for (auto &card : cards) {
			cards_queue.push(std::move(card));
		}

		if (!cards.empty()) {
			throw std::runtime_error(ShufflingError);
		}

		cards.clear(); // Deletes nullptrs left
	}

	private:
	void end_round() {
		if (!cards.empty()) {
			throw std::runtime_error(ShufflingError);
		}

		while (!cards_queue.empty()) {
			auto card = std::move(cards_queue.front());
			cards_queue.pop();

			cards.push_back(std::move(card));
		}

		if (!cards_queue.empty()) {
			throw std::runtime_error(QueueClearError);
		}
	}

	public:
	void load() {	
		shuffle_cards();
	}
};

