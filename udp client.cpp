

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"  // Server IP address (localhost)
#define PORT 12345   // Server's port
#define MAX_LEN 1024 // Buffer size

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[MAX_LEN];

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Send student information (name, marks, total marks)
    const char* student_info = "Name: John, Marks: 85, 90, 95, Total Marks: 300";

    // Send the data to the server
    sendto(client_fd, student_info, strlen(student_info), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    std::cout << "Sent to server: " << student_info << "\n";

    // Receive the percentage from the server
    int len = recvfrom(client_fd, buffer, sizeof(buffer), 0, nullptr, nullptr);
    if (len < 0) {
        perror("Receive failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    buffer[len] = '\0'; // Null-terminate the string
    std::cout << "Received from server: " << buffer << "\n";

    // Close the socket
    close(client_fd);
    return 0;
}
