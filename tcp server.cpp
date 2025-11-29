#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345   // Port to listen on

// Function to calculate percentage
float calculatePercentage(int obtainedMarks, int totalMarks) {
    return (float(obtainedMarks) / float(totalMarks)) * 100;
}

int main() {
    int server_fd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buffer[1024];

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    // Accept a connection from a client
    addr_len = sizeof(client_addr);
    if ((new_sock = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Get client address info
    std::cout << "Client connected\n";

    // Receive data from the client
    int valread = read(new_sock, buffer, sizeof(buffer));
    if (valread < 0) {
        perror("Read failed");
        close(new_sock);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    buffer[valread] = '\0'; // Null-terminate the string

    std::cout << "Received from client: " << buffer << std::endl;

    // Parse the received message (assuming format: "Name: <name>, Marks: <marks1>, <marks2>, <total>")
    char name[50];
    int marks1, marks2, marks3, totalMarks;
    
    // Extract the values from the buffer
    sscanf(buffer, "Name: %[^,], Marks: %d, %d, %d, Total Marks: %d", name, &marks1, &marks2, &marks3, &totalMarks);

    // Calculate the total marks obtained
    int obtainedMarks = marks1 + marks2 + marks3;

    // Calculate percentage
    float percentage = calculatePercentage(obtainedMarks, totalMarks);

    // Send the percentage back to the client
    sprintf(buffer, "Percentage for %s: %.2f%%", name, percentage);
    send(new_sock, buffer, strlen(buffer), 0);

    std::cout << "Sent to client: " << buffer << "\n";

    // Close the connection
    close(new_sock);
    close(server_fd);
    return 0;
}
