#include <iostream>
#include <curl/curl.h>

// Callback function to handle recieved data
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	size_t realsize = size * nmemb;
	std::cout << (char*)contents;
	return realsize;
}

int main() {
	CURL* curl;
	CURLcode res;

	// Initialize libcurl globally (required once per application)
	res = curl_global_init(CURL_GLOBAL_DEFAULT);

	if (res != CURLE_OK)
	{
		std::cerr << "curl_global_init() failed: " << curl_easy_strerror(res) << std::endl;
		return 1;
	}

	// Initialize a curl session
	curl = curl_easy_init();
	if (!curl) {
		std::cerr << "curl_easy_init() failed" << std::endl;
		curl_global_cleanup();
		return 1;
	}

	// set url to fetch
	curl_easy_setopt(curl, CURLOPT_URL, "https://google.com");

	// set the callback function to handle the response
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

	// perform the request
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
	}
	else {
		std::cout << "\nRequest Succeeded!" << std::endl;
	}

	// Cleanup
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return (res == CURLE_OK) ? 0 : 1;
}