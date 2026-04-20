#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 256
#define MAX_FILE_CONTENT 255

int server_fd = -1;

void handle_sigint(int sig) {
    (void)sig;
    if (server_fd != -1) {
        close(server_fd);
    }
    write(STDOUT_FILENO, "\nServer shutting down gracefully...\n", 36);
    _exit(0);
}

void send_message(int client_socket, const char *message) {
    send(client_socket, message, strlen(message), 0);
}

int is_valid_filename(const char *filename) {
    if (filename == NULL || strlen(filename) == 0) return 0;
    if (strstr(filename, "..") != NULL || strchr(filename, '/') != NULL) return 0;
    return 1;
}

void handle_client(int client_socket) {
    char filename[BUFFER_SIZE];
    memset(filename, 0, sizeof(filename));

    recv(client_socket, filename, sizeof(filename) - 1, 0);
    filename[strcspn(filename, "\r\n")] = '\0';

    if (!is_valid_filename(filename)) {
        send_message(client_socket, "ERROR: Invalid file name.\n");
        close(client_socket);
        return;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        send_message(client_socket, "ERROR: File not found.\n");
        close(client_socket);
        return;
    }

    char content[MAX_FILE_CONTENT + 2];
    size_t bytes_read = fread(content, 1, MAX_FILE_CONTENT + 1, file);
    fclose(file);

    if (bytes_read > MAX_FILE_CONTENT) {
        send_message(client_socket, "ERROR: File too large (max 255 chars).\n");
        close(client_socket);
        return;
    }

    content[bytes_read] = '\0';
    send_message(client_socket, content);

    close(client_socket);
}

int main() {
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sa.sa_flags = SA_RESTART;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("Server running on port %d...\n", PORT);

    while (1) {
        int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        handle_client(client_socket);
    }

    return 0;
}
