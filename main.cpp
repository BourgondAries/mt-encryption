#include <cassert>
#include <ctime>
#include <iostream>
#include <random>

int main()
{
	std::mt19937_64 engine(std::time(nullptr)); // Generate a key
	uint64_t key = engine();

	std::vector<bool> encrypted_message; // Prepare a vector for the encrypted bits

	////////////////////////////////////////////////////////////
	// Encryption
	////////////////////////////////////////////////////////////
	{
		engine = std::mt19937_64(key); // Seed the engine with the key

		std::vector<bool> message // Create the ASCII message "abc", LSB first.
		{
			1, 0, 0, 0, 0, 1, 1, 0,
			0, 1, 0, 0, 0, 1, 1, 0,
			1, 1, 0, 0, 0, 1, 1, 0,
		};

		for (bool bit : message)
		{
			uint64_t random = engine();
			bool encrypted_bit = 0b1 & (random ^ static_cast<decltype(random)>(bit)); // XOR the bit with the random number's first bit
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
	}
}
