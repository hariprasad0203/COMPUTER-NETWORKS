#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 100
#define MAX_BITS 1024
#define FRAME_SIZE 7

void ascii_to_bin(char ch, char *b) {
    for (int i = 7; i >= 0; i--) {
        b[7 - i] = (ch & (1 << i)) ? '1' : '0';
    }
    b[8] = '\0';
}

void str_to_bits(const char *str, char *bits) {
    int len = strlen(str);
    char b[9];
    bits[0] = '\0';
    for (int i = 0; i < len; i++) {
        ascii_to_bin(str[i], b);
        strcat(bits, b);
    }
}

        int ask_parity_scheme() {
    int choice;
    printf("Choose parity scheme--(1: Even Parity 2: Odd Parity) : ");
    scanf("%d", &choice);
    return (choice == 2) ? 1 : 0; /* 0 = even, 1 = odd */
}

char compute_parity_bit(int ones_in_data, int scheme) {
    if (scheme == 0)
        return (ones_in_data % 2 == 0) ? '0' : '1';
    else
        return (ones_in_data % 2 == 0) ? '1' : '0';
}

int check_parity_error(int total_ones, int scheme) {
    if (scheme == 0) return (total_ones % 2 != 0);
    else return (total_ones % 2 == 0);
}

int main() {
    char input[MAX_STR];
    char full_msg[MAX_BITS];

    printf("\n~~~SENDER SIDE~~~\n");
    printf("Enter string for ascii convertion: ");
    scanf("%s", input);

    int scheme = ask_parity_scheme();
    str_to_bits(input, full_msg);
    int total_bits = strlen(full_msg);

    printf("converted string into binary: %s\n", full_msg);

    // Calculate number of 7-bit frames needed
    int num_frames = (total_bits + FRAME_SIZE - 1) / FRAME_SIZE;

    char tx_frames[100][16]; // Stores transmitted frames (7 bits + 1 parity + null)

    printf("\n--- Frame Generation ---\n");
    for (int i = 0; i < num_frames; i++) {
        char current_frame[8] = {0};
        int ones = 0;

        // Extract 7 bits (or pad with 0s if last frame is incomplete)
        for (int j = 0; j < FRAME_SIZE; j++) {
            int idx = i * FRAME_SIZE + j;
            if (idx < total_bits) {
                current_frame[j] = full_msg[idx];
            } else {
                current_frame[j] = '0'; // Padding
            }
            if (current_frame[j] == '1') ones++;
        }
        current_frame[FRAME_SIZE] = '\0';

        char p_bit = compute_parity_bit(ones, scheme);
        sprintf(tx_frames[i], "%s%c", current_frame, p_bit);

        printf("Frame %d: Data = %s | Parity [%s] = %c | Sent = %s\n",
               i + 1, current_frame, scheme == 0 ? "EVEN" : "ODD", p_bit, tx_frames[i]);
    }

    printf("\n---- RECEIVER SIDE ----\n");
    printf("Do you want to add transmission error?\n");
    printf("Enter Frame number to corrupt (1 to %d), or -1 for NO error: ", num_frames);
    int f_err;
    scanf("%d", &f_err);

    if (f_err >= 1 && f_err <= num_frames) {
        printf("Enter bit position to flip inside Frame %d (0 to %d): ", f_err, FRAME_SIZE);
        int b_err;
        scanf("%d", &b_err);
        if (b_err >= 0 && b_err <= FRAME_SIZE) {
            int f_idx = f_err - 1;
            tx_frames[f_idx][b_err] = (tx_frames[f_idx][b_err] == '0') ? '1' : '0';
            printf("Bit flipped in Frame %d at position %d.\n", f_err, b_err);
        }
    } else {
        printf("No error was introduced to frame error.\n");
    }

    printf("\n--- Receiver Verification ---\n");
    int global_error_flag = 0;
    for (int i = 0; i < num_frames; i++) {
        int r_ones = 0;
        int frame_len = strlen(tx_frames[i]);

        for (int j = 0; j < frame_len; j++) {
            if (tx_frames[i][j] == '1') r_ones++;
        }

        int has_error = check_parity_error(r_ones, scheme);
        printf("Received Frame %d: %s -> Total 1s: %d -> Status: %s\n",
               i + 1, tx_frames[i], r_ones, has_error ? "error detected!!!!" : "ok");

        if (has_error) global_error_flag = 1;
    }

    printf("\nThe Final Result: %s\n", global_error_flag ? "Transmission Failed (Errors found)" : "TRANSMISSION SUCCESSFUL");

    return 0;
}
