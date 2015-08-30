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

int main(int argc, char *argv[])
{
	std::string in_message;
	for (int iterator = 1; iterator < argc; ++iterator)
		in_message += argv[iterator];

	std::mt19937_64 engine(std::time(nullptr)); // Generate a key
	uint64_t key = 9145160492174859451;

	std::vector<bool> encrypted_message; // Prepare a vector for the encrypted bits

	////////////////////////////////////////////////////////////
	// Encryption
	////////////////////////////////////////////////////////////
	{
		engine = std::mt19937_64(key); // Seed the engine with the key

		std::string random = std::to_string(std::time(nullptr));
		std::vector<bool> message = toBitMessage(in_message); // Chop a message up into bits
		std::cout << fromBitMessage(message);

		for (bool bit : message)
		{
			uint64_t random = engine();
			bool encrypted_bit = 0b1 & (random ^ static_cast<decltype(random)>(bit)); // XOR the bit with the random number's first bit
			random ^= static_cast<uint64_t>(encrypted_bit);
			engine = std::mt19937_64(random);
			encrypted_message.emplace_back(encrypted_bit);
		}

		// Print the result
		int counter = 0;
		for (bool bit : encrypted_message)
		{
			std::cout << static_cast<int>(bit) << ", ";
			if (++counter % 8 == 0)
				std::cout << '\n';
		}
		std::cout << fromBitMessage(encrypted_message) << '\n';
	}

	std::cout << '\n';

	////////////////////////////////////////////////////////////
	// Decryption
	////////////////////////////////////////////////////////////
	{
		engine = std::mt19937_64(key); // Seed the engine with the key

		std::vector<bool> decrypted_message; // Prepare a vector for the dencrypted bits

		for (bool bit : encrypted_message)
		{
			uint64_t random = engine();
			bool decrypted_bit = 0b1 & (random ^ static_cast<decltype(random)>(bit)); // XOR the bit with the random number's first bit
			random ^= static_cast<uint64_t>(bit);
			engine = std::mt19937_64(random);
			decrypted_message.emplace_back(decrypted_bit);
		}

		// Print the result
		int counter = 0;
		for (bool bit : decrypted_message)
		{
			std::cout << static_cast<int>(bit) << ", ";
			if (++counter % 8 == 0)
				std::cout << '\n';
		}
		std::cout << fromBitMessage(decrypted_message) << '\n';
	}
}
