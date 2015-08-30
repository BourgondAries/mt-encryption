#include <cassert>
#include <ctime>
#include <iostream>
#include <random>

char fromBitMessageCharacter(const std::vector<bool> &character)
{
	char to_return = 0b00000000;
	for (int iterator = 0; iterator < 8; ++iterator)
		to_return |= (0b00000001 << iterator) & (static_cast<char>(character[iterator]) << iterator);

	return to_return;
}

std::string fromBitMessage(std::vector<bool> message)
{
	std::string to_return;
	while (message.empty() == false)
	{
		std::vector<bool> eight_bits;
		for (int iterator = 0; iterator < 8; ++iterator)
		{
			eight_bits.emplace_back(message.at(0));
			message.erase(message.begin());
		}
		to_return.push_back(fromBitMessageCharacter(eight_bits));
	}
	return to_return;
}

std::vector<bool> toBitMessage(const char &message)
{
	std::vector<bool> bit_string;
	bit_string.emplace_back(message & 0b00000001);
	bit_string.emplace_back(message & 0b00000010);
	bit_string.emplace_back(message & 0b00000100);
	bit_string.emplace_back(message & 0b00001000);

	bit_string.emplace_back(message & 0b00010000);
	bit_string.emplace_back(message & 0b00100000);
	bit_string.emplace_back(message & 0b01000000);
	bit_string.emplace_back(message & 0b10000000);
	return bit_string;
}

std::vector<bool> toBitMessage(const std::string &message)
{
	std::vector<bool> bit_string;
	for (char character : message)
	{
		std::vector<bool> character_bits = toBitMessage(character);
		for (bool bit : character_bits)
			bit_string.emplace_back(bit);
	}
	return bit_string;
}

std::string printBits(const std::vector<bool> &message)
{
	std::string total;
	int counter = 0;
	for (bool bit : message)
	{
		total += std::to_string(static_cast<int>(bit)) + ", ";
		if (++counter % 8 == 0)
			total += "\n";
	}
	return total;
}

std::string printTransition(const std::vector<bool> &from, const std::vector<bool> &to)
{
	std::string total;
	std::size_t multiplier = 0;
	while ((multiplier * 8 + 8) < from.size())
	{
		for (int iterator = 0; iterator < 8; ++iterator)
			total += std::to_string(static_cast<int>(from.at(iterator + multiplier * 8))) + ", ";
		total += "=> ";
		for (int iterator = 0; iterator < 8; ++iterator)
			total += std::to_string(static_cast<int>(to.at(iterator + multiplier * 8))) + ", ";
		++multiplier;
		total += "\n";
	}
	return total;
}

bool firstBitOf(uint64_t value)
{
	return 0b1 & value;
}

int main(int argc, char *argv[])
{
	std::string in_message;
	for (int iterator = 1; iterator < argc; ++iterator)
		in_message += argv[iterator];
	char in_value;
	while ((in_value = std::cin.get()) != -1)
		in_message.push_back(static_cast<char>(in_value));

	std::mt19937_64 engine(std::time(nullptr)); // Generate a key
	uint64_t key = 9145160492174859451;

	std::vector<bool> encrypted_message; // Prepare a vector for the encrypted bits

	////////////////////////////////////////////////////////////
	// Encryption
	////////////////////////////////////////////////////////////
	{
		engine = std::mt19937_64(key); // Seed the engine with the key

		std::vector<bool> message = toBitMessage(in_message); // Chop a message up into bits

		for (bool bit : message)
		{
			uint64_t random = engine();
			bool encrypted_bit = firstBitOf(random) != bit; // XOR the bit with the random number's first bit
			random ^= static_cast<uint64_t>(bit);
			engine = std::mt19937_64(random);
			encrypted_message.emplace_back(encrypted_bit);
		}
	}

	////////////////////////////////////////////////////////////
	// Decryption
	////////////////////////////////////////////////////////////
	{
		engine = std::mt19937_64(key); // Seed the engine with the key

		std::vector<bool> decrypted_message; // Prepare a vector for the dencrypted bits

		for (bool bit : encrypted_message)
		{
			uint64_t random = engine();
			bool decrypted_bit = firstBitOf(random) != bit; // XOR the bit with the random number's first bit
			random ^= static_cast<uint64_t>(decrypted_bit);
			engine = std::mt19937_64(random);
			decrypted_message.emplace_back(decrypted_bit);
		}

		std::cout << printTransition(decrypted_message, encrypted_message);
	}
}
