#include <algorithm>
#include <memory>
#include <queue>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>
#include "./cards.h"
#include "errors.h"

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

