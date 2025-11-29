#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"  // Server IP address (localhost)
#define PORT 12345   // Server's port

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected to server\n";

    // Send student information (name, marks, total marks)
    // Format: "Name: John, Marks: 85, 90, 95, Total Marks: 300"
    const char* student_info = "Name: John, Marks: 85, 90, 95, Total Marks: 300";
    send(client_fd, student_info, strlen(student_info), 0);
    std::cout << "Sent to server: " << student_info << "\n";

    // Receive the percentage from the server
    int valread = read(client_fd, buffer, sizeof(buffer));
    if (valread < 0) {
        perror("Read failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    buffer[valread] = '\0'; // Null-terminate the string
    std::cout << "Received from server: " << buffer << "\n";

    // Close the connection
    close(client_fd);
    return 0;
}
