#include <memory>
#include <utility>
#include "cards.h"

using unique_ptr_pair = std::pair<std::unique_ptr<Card>, std::unique_ptr<Card>>;

class Player {
	private:
	std::unique_ptr<Card> card1, card2;
	int cash;

	public:
	Player(const int &cash) : cash(cash) {}  

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

