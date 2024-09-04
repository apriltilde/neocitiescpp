#include "neocities.h"
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// Callback function to write response data to a stringstream
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((stringstream*)userp)->write((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to perform cURL request
bool performCurlRequest(CURL *curl, const string& url, curl_mime *form, stringstream& responseStream, const string& userpwd) {
    CURLcode res;
    bool success = false;

    // Set common cURL options
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if (form) {
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
    }
    curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStream);

    // Perform the request
    res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK) {
        cerr << "cURL Error: " << curl_easy_strerror(res) << endl;
    } else {
        // Check HTTP response code
        long responseCode;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

        // Check for authentication error in the response body
        if (responseStream.str().find("invalid_auth") != string::npos) {
            cerr << "Authentication error: " << responseStream.str() << endl;
        } else {
            success = true;
        }
    }

    return success;
}

Neocities::Neocities(const std::string& user, const std::string& password)
    : user_(user), password_(password) {}

bool Neocities::upload(const std::vector<std::pair<std::string, std::string>>& files) {
    CURL *curl;
    curl_mime *form = nullptr;
    bool success = false;
    stringstream responseStream;

    curl = curl_easy_init();
    if (curl) {
        // Create a new mime form
        form = curl_mime_init(curl);

        // Add each file to the form
        for (const auto& file : files) {
            curl_mimepart *field = curl_mime_addpart(form);
            curl_mime_name(field, file.first.c_str()); // Filename with extension
            curl_mime_filedata(field, file.second.c_str()); // Path to the file
        }

        // Perform the cURL request
        string url = "https://neocities.org/api/upload";  // The URL for the upload endpoint
        success = performCurlRequest(curl, url, form, responseStream, user_ + ":" + password_);

        // Clean up
        curl_mime_free(form);
        curl_easy_cleanup(curl);
    }

    return success;
}

bool Neocities::deleteFiles(const std::vector<std::string>& filenames) {
    CURL *curl;
    curl_httppost *post = nullptr;
    curl_httppost *last = nullptr;
    stringstream responseStream;
    bool success = false;

    curl = curl_easy_init();
    if (curl) {
        // Create the POST data
        for (const auto& filename : filenames) {
            curl_formadd(&post, &last,
                CURLFORM_COPYNAME, "filenames[]",
                CURLFORM_COPYCONTENTS, filename.c_str(),
                CURLFORM_END);
        }

        // Perform the cURL request
        string url = "https://neocities.org/api/delete";
        success = performCurlRequest(curl, url, nullptr, responseStream, user_ + ":" + password_);

        // Clean up
        curl_formfree(post);
        curl_easy_cleanup(curl);
    }

    return success;
}

std::string Neocities::getInfo(const std::string& sitename) {
    CURL *curl;
    stringstream responseStream;
    std::string response;

    curl = curl_easy_init();
    if (curl) {
        // Construct the URL with sitename parameter
        char* encodedSitename = curl_easy_escape(curl, sitename.c_str(), sitename.length());
        string url = "https://neocities.org/api/info?sitename=" + string(encodedSitename);

        // Perform the cURL request
        if (performCurlRequest(curl, url, nullptr, responseStream, user_ + ":" + password_)) {
            response = responseStream.str();
        }

        // Clean up
        curl_free(encodedSitename);
        curl_easy_cleanup(curl);
    }

    return response;
}
