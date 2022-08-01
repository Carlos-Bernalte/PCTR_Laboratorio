#include <ctype.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <definitions.h>


/* Message queue management */
void open_message_queue(const char *mq_name, mode_t mode, mqd_t *q_handler);
/* Task management */
void count_digits(mqd_t q_handler_words, int *n_digits);
void send_count(mqd_t q_handler_number_digits, int *n_digits);
/******************** Main function ********************/
int main(int argc, char *argv[]) {
    mqd_t q_handler_words, q_handler_number_digits;
    int n_digits;
    mode_t mode_read_only = O_RDONLY;
    mode_t mode_write_only = O_WRONLY;
    /* Open message queues */
    open_message_queue(MQ_WORDS, mode_read_only, &q_handler_words);
    open_message_queue(MQ_NUMBER_DIGITS, mode_write_only, &q_handler_number_digits);
    /* Task management */
    while (TRUE) {
        count_digits(q_handler_words, &n_digits);
        send_count(q_handler_number_digits, &n_digits);
    }
    return EXIT_SUCCESS;
}


/******************** Message queue management ********************/
void open_message_queue(const char *mq_name, mode_t mode, mqd_t *q_handler) {
    *q_handler = mq_open(mq_name, mode);
}
/******************** Task management ********************/
void count_digits(mqd_t q_handler_words, int *n_digits) {
    char word[MAX_LINE_SIZE];
    int i = 0;
    /* Wait for a word to count digits */
    mq_receive(q_handler_words, word, sizeof(word), NULL);

    *n_digits = 0;
    while (word[i]) {
        if (isdigit(word[i++])) {
        ++*n_digits;
        }
    }
}
void send_count(mqd_t q_handler_number_digits, int *n_digits) {
    mq_send(q_handler_number_digits, (const char *)n_digits, sizeof(int), 0);
}