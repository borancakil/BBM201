#include "Leaderboard.h"
#include "iostream"
#include "fstream"


void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).


    if (!head_leaderboard_entry || new_entry->score > head_leaderboard_entry->score) {
        new_entry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    }
    else {
        LeaderboardEntry* current = head_leaderboard_entry;
        while (current->next_leaderboard_entry && new_entry->score <= current->next_leaderboard_entry->score) {
            current = current->next_leaderboard_entry;
        }
        new_entry->next_leaderboard_entry = current->next_leaderboard_entry;
        current->next_leaderboard_entry = new_entry;
    }

    // Keep only the top 10 all-time high-scores
    int count = 0;
    LeaderboardEntry* prev = nullptr;
    LeaderboardEntry* current = head_leaderboard_entry;

    while (current && count < 10) {
        prev = current;
        current = current->next_leaderboard_entry;
        count++;
    }

    if (prev) {
        prev->next_leaderboard_entry = nullptr;
        delete current; // Free memory for entries beyond the top 10
    }
}

void Leaderboard::write_to_file(const string& filename) {
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions

    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    LeaderboardEntry* current = head_leaderboard_entry;
    while (current) {
        outFile << current->player_name << " " << current->score << std::endl;
        current = current->next_leaderboard_entry;
    }

    outFile.close();
}

void Leaderboard::read_from_file(const string& filename) {
    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Clear existing leaderboard entries
    while (head_leaderboard_entry) {
        LeaderboardEntry* next = head_leaderboard_entry->next_leaderboard_entry;
        delete head_leaderboard_entry;
        head_leaderboard_entry = next;
    }

    // Read entries from the file
    LeaderboardEntry* prev = nullptr;
    while (!inFile.eof()) {
        LeaderboardEntry* new_entry = new LeaderboardEntry(0 , 0 , " ");
        inFile >> new_entry->player_name >> new_entry->score;

        if (inFile.eof()) {
            delete new_entry;
            break;
        }

        if (!head_leaderboard_entry) {
            head_leaderboard_entry = new_entry;
        }
        else {
            prev->next_leaderboard_entry = new_entry;
        }

        prev = new_entry;
    }

    inFile.close();
}


void Leaderboard::print_leaderboard() {
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
    LeaderboardEntry* current = head_leaderboard_entry;
    int rank = 1;

    std::cout << "===== Leaderboard =====" << std::endl;
    while (current) {
        std::cout << rank << ". " << current->player_name << " - " << current->score << " - " << current->last_played << std::endl;
        current = current->next_leaderboard_entry;
        rank++;
    }
    std::cout << "=======================" << std::endl;
}

Leaderboard::~Leaderboard() {
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
    LeaderboardEntry* current = head_leaderboard_entry;
    while (current) {
        LeaderboardEntry* next = current->next_leaderboard_entry;
        delete current;
        current = next;
    }
}
