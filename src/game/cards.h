#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

enum class Colors {
	Hearth = 0, 
	Diamond = 1, 
	Club = 2,
	Spade = 3
};

enum class CardValues {
	Jack = 11,
	Queen = 12,
	King = 13,
	Ace = 14
};

class Card {
	private:
	Colors color;
	int value;

	public:
	void load(const int &value, const int &color) {
		this->value = value;
		this->color = static_cast<Colors>(color);
	}

	public:
	Colors get_color() {
		return color;
	}

	public:
	int get_value() {
		return value;
	}
};

