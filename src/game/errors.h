#include <iostream>

/* C++ is so shit, that enums can't be fuckin 
 * translated into std::string, so one has three options
 * 1. Use a hash map and lose shit ton of computing power
 * 2. Use external libary (doesn't fit this use case)
 * 3. Rely on macro hell 
 * With this said, welcome to my macro hell */

#define ShufflingError "Shuffling did not work correctly."
#define QueueClearError "Queue was not cleared correctly."
#define NoCardsInQueue "Cards queue is empty."
#define StreakEndOutOfBounds "Streak end is out of bounds."
#define CardsWereNotAssigned "Cards were not assigned."

