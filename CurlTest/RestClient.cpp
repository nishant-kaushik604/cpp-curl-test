#include "RestClient.h"
#include <iostream>

// Constructor: Initialize curl globally and create a session
RestClient::RestClient() {
	// Initialize libcurl globally (only once per app)
	CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
	if (res != CURLE_OK)
	{
		std::cerr << "curl_global_init() failed: " << curl_easy_strerror(res) << std::endl;
		throw std::runtime_error("Failed to initialize libcurl");
	}

	// Create a curl easy handle
	curl = curl_easy_init();
	if (!curl)
	{
		curl_global_cleanup();
		throw std::runtime_error("Failed to initialize curl handle");
	}
}

// Destructor: Cleanup resources
RestClient::~RestClient() {
	if (curl) {
		curl_easy_cleanup(curl);	// Free curl handle
	}
	curl_global_cleanup();			// free global curl resources
}

// GET request
bool RestClient::get(const std::string& url, SuccessCallback onSuccess, FailureCallback onFailure)
{
	if (!curl)
	{
		return false;
	}
	
	// Set URL
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	// Set write callback to store response
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);	// Pass this to callback

	CURLcode res;
	return performRequest(res, onSuccess, onFailure);
}

// POST Request
bool RestClient::post(const std::string& url, const std::string& data, SuccessCallback onSuccess, FailureCallback onFailure)
{
	if (!curl)
	{
		return false;
	}

	// Set URL
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	// Set POST method
	curl_easy_setopt(curl, CURLOPT_POST, 1L);

	// Set Post data
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.length());

	// Set write callback to store response
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

	CURLcode res;
	return performRequest(res, onSuccess, onFailure);
}

// WriteCallback: Called by curl to store response data
size_t RestClient::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;	//Total bytes recieved
	RestClient* client = static_cast<RestClient*>(userp);	// Cast userp back to RestClient
	client->response.append((char*)contents, realsize);		// Append data to response string
	return realsize;	// Must return the number of bytes handled.
}

// Helper to perform the request and invoke callbacks
bool RestClient::performRequest(CURLcode& res, SuccessCallback onSuccess, FailureCallback onFailure)
{
	response.clear();	// clear previous response
	res = curl_easy_perform(curl);	// Execute the request

	if (res == CURLE_OK)
	{
		if (onSuccess)
		{
			onSuccess(response);	// Call success callback with response
		}
		return true;
	}
	else
	{
		if (onFailure)
		{
			onFailure(res, curl_easy_strerror(res));	// Call failure callback with error
		}
		return false;
	}
}



