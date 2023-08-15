#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

const std::string API_KEY = "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";

std::string sendMessageToOpenAI(const std::string& message) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        return "";
    }

    struct hostent* server = gethostbyname("api.openai.com");
    if (server == NULL) {
        perror("Host not found");
        close(sock);
        return "";
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(443);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection error");
        close(sock);
        return "";
    }

    std::string postData = "prompt=" + message;
    std::string request = "POST /v1/engines/davinci/completions HTTP/1.1\r\n"
                          "Host: api.openai.com\r\n"
                          "Content-Type: application/x-www-form-urlencoded\r\n"
                          "Content-Length: " + std::to_string(postData.size()) + "\r\n"
                          "Authorization: Bearer " + API_KEY + "\r\n\r\n" + postData;

    send(sock, request.c_str(), request.size(), 0);

    std::string response;
    char buffer[4096];
    while (true) {
        int bytesRead = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            break;
        }
        response.append(buffer, bytesRead);
    }
    close(sock);
    return response;
}

int main() {
    // Simulate receiving a user message from the IRC channel
    std::string userMessage = "Tell me a joke";

    // Send user message to OpenAI API and get response
    std::string response = sendMessageToOpenAI(userMessage);

    // Simulate sending the response back to the IRC channel
    std::cout << "Bot: " << response << std::endl;

    return 0;
}
