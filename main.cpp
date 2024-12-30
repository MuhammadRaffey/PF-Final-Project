#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <json.hpp>
#include <dotenv.h>
using namespace std;

using json = nlohmann::json;

// Callback function to handle cURL response
size_t writeCallback(char *data, size_t size, size_t nmemb, string *writerData)
{
    if (writerData == nullptr)
        return 0;
    writerData->append(data, size * nmemb);
    return size * nmemb;
}

// Function to fetch a response from OpenAI API
string fetchResponseFromAPI(const string &prompt)
{
    dotenv::init();
    const char *api_key = getenv("OPENAI_API_KEY");
    if (!api_key)
    {
        cerr << "Error: OPENAI_API_KEY not set in .env file" << endl;
        return "Error: Missing API Key";
    }
    string auth_header = "Authorization: Bearer " + string(api_key);

    CURL *curl = curl_easy_init();
    string response_data;

    if (curl)
    {
        // Use chat/completions API
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");

        // Set headers
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, auth_header.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set payload for chat API
        string payload = R"({
            "model": "gpt-4",
            "messages": [
                {"role": "user", "content": ")" + prompt + R"("}
            ],
            "temperature": 0.7
        })";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());

        // Set write callback
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            cerr << "cURL Error: " << curl_easy_strerror(res) << endl;
        }

        // Cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    else
    {
        cerr << "Failed to initialize cURL." << endl;
    }

    // Parse JSON response
    try
    {
        auto parsed = json::parse(response_data);
        if (parsed.contains("choices") && !parsed["choices"].empty())
        {
            return parsed["choices"][0]["message"]["content"];
        }
        else if (parsed.contains("error"))
        {
            cerr << "API Error: " << parsed["error"]["message"] << endl;
            return "Error: API returned an error.";
        }
        else
        {
            cerr << "Unexpected response format." << endl;
            return "Error: Unexpected response format.";
        }
    }
    catch (const exception &e)
    {
        cerr << "Error parsing JSON response: " << e.what() << endl;
        return "Error: Could not parse response.";
    }
}

// Function to save interaction
void saveToFile(const string &question, const string &response)
{
    ofstream file("interactions.txt", ios::app);
    if (file.is_open())
    {
        file << "Q: " << question << "\n";
        file << "A: " << response << "\n\n";
        file.close();
    }
    else
    {
        cerr << "Error opening file for writing." << endl;
    }
}

int main()
{
    string question, response;
    int choice;

    do
    {
        cout << "1. Ask a question\n2. View saved interactions\n3. Exit\nChoose an option: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            cout << "Enter your question: ";
            getline(cin, question);
            response = fetchResponseFromAPI(question);
            cout << "Response: " << response << endl;
            saveToFile(question, response);
            break;
        case 2:
        {
            ifstream file("interactions.txt");
            if (file.is_open())
            {
                cout << file.rdbuf();
                file.close();
            }
            else
            {
                cout << "No interactions saved yet." << endl;
            }
        }
        break;
        case 3:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 3);

    return 0;
}
