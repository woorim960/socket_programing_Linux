// headers/functions.h

void set_logfile(char log_file[], char log_path[]);
void print_log(char log_path[]);
char* get_ip();
void save_log(char* data, RECEIVE_RECORD msg, int sock_fd, char* port);
