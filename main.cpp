#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Function to make an API request
std::string fetchResponseFromAPI(const std::string &prompt)
{
    CURL *curl = curl_easy_init();
    std::string response_data;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/completions");
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Authorization: Bearer YOUR_API_KEY");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        std::string payload = R"({"model": "text-davinci-003", "prompt": ")" + prompt + R"(", "max_tokens": 150})";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());

        auto writeCallback = [](char *data, size_t size, size_t nmemb, std::string *writerData)
        {
            if (writerData == nullptr)
                return size_t(0);
            writerData->append(data, size * nmemb);
            return size * nmemb;
        };

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    auto parsed = json::parse(response_data);
    return parsed["choices"][0]["text"];
}

// Function to save interaction
void saveToFile(const std::string &question, const std::string &response)
{
    std::ofstream file("interactions.txt", std::ios::app);
    if (file.is_open())
    {
        file << "Q: " << question << "\n";
        file << "A: " << response << "\n\n";
        file.close();
    }
}

int main()
{
    std::string question, response;
    int choice;

    do
    {
        std::cout << "1. Ask a question\n2. View saved interactions\n3. Exit\nChoose an option: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice)
        {
        case 1:
            std::cout << "Enter your question: ";
            std::getline(std::cin, question);
            response = fetchResponseFromAPI(question);
            std::cout << "Response: " << response << std::endl;
            saveToFile(question, response);
            break;
        case 2:
            std::ifstream file("interactions.txt");
            if (file.is_open())
            {
                std::cout << file.rdbuf();
                file.close();
            }
            else
            {
                std::cout << "No interactions saved yet." << std::endl;
            }
            break;
        case 3:
            std::cout << "Exiting..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Try again." << std::endl;
        }
    } while (choice != 3);

    return 0;
}
