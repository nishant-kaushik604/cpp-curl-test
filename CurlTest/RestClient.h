#pragma once

#ifndef REST_CLIENT_H
#define REST_CLIENT_H

#include<string>
#include<functional>
#include<curl/curl.h>

// Callback types for success and failures
using SuccessCallback = std::function<void(const std::string& response)>;
using FailureCallback = std::function<void(CURLcode error, const std::string& errorMsg)>;

class RestClient
{
public:
	// Constructor: Initializes curl resources
	RestClient();

	// Destructor: Clean up curl resources
	~RestClient();

	// Perform a GET request
	bool get(const std::string& url, SuccessCallback onSuccess, FailureCallback onFailure);

	// Perform a POST request with data
	bool post(const std::string& url, const std::string& data, SuccessCallback onSuccess, FailureCallback onFailure);

private:
	CURL* curl;				// Curl handle
	std::string response;	// Stores the response from the server

	// Static callback for curl to write response data
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

	// Helper to perform the request and handle result
	bool performRequest(CURLcode& res, SuccessCallback onSuccess, FailureCallback onFailure);
};
#endif // !REST_CLIENT_H


