#include "RestClient.h"
#include<iostream>

int main() {
	try {
		RestClient client;
		
		// Success callback
		auto onSuccess = [](const std::string& response) {
			std::cout << "Success Response: " << response << std::endl;
		};

		// Failure callback
		auto onFailure = [](CURLcode error, const std::string& errorMsg) {
			std::cerr << "Failed! Error code: " << error << ", Msg: " << errorMsg << std::endl;
		};

		// Test Get Request (e.g., a public api)
		std::cout << "Making GET request...\n";
		client.get("https://google.com", onSuccess, onFailure);

		// Test Post Request (e.g., a mock API)
		std::cout << "\nMaking POST request...\n";
		client.post("https://jsonplaceholder.typicode.com/posts", R"({"title": "foo", "body": "bar", "userId": 1})", onSuccess, onFailure);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}