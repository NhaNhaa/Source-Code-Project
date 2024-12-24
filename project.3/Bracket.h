#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
#include <iomanip>

using namespace std;

class Node {
public:
    string name;
    Node* left;
    Node* right;

    Node(string name) : name(name), left(nullptr), right(nullptr) {}
};

class BracketGenerate {
private:
    vector<string> participants;
    vector<vector<string>> groups;
    vector<Node*> grouproot;
    vector<string> groupWinners;
    vector<string> groupLosers;

    Node* root;
    Node* upperRoot;
    Node* lowerRoot;

    // Helper function to simulate a match and determine the winner
    string simulateMatch(const string& participant1, const string& participant2) {
        return (participant1 < participant2) ? participant1 : participant2;
    }

    // Helper function to simulate group matches and find the winner
    string simulateGroupMatches(const vector<string>& group, vector<string>& losers) {
        if (group.size() < 2) return group.empty() ? "" : group[0];

        string winner = group[0];
        for (size_t i = 1; i < group.size(); i++) {
            winner = simulateMatch(winner, group[i]);
        }

        // Identify losers
        for (const auto& member : group) {
            if (member != winner) {
                losers.push_back(member);
            }
        }

        return winner;
    }

    // Helper function to generate random dates and times within the same year and month
    string generateRandomDate() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis_day(1, 31);
        uniform_int_distribution<> dis_hour(0, 23);
        uniform_int_distribution<> dis_minute(0, 59);

        int day = dis_day(gen);
        int month = 12;
        int year = 2024;
        int hour = dis_hour(gen);
        int minute = dis_minute(gen);

        stringstream date;
        date << setfill('0') << setw(2) << day << "/" << setw(2) << month << "/" << year;
        date << " " << setw(2) << hour << ":" << setw(2) << minute;
        return date.str();
    }

    // Recursive function to build the bracket tree
    Node* buildBracket(queue<Node*>& matchQueue) {
        while (matchQueue.size() > 1) {
            Node* left = matchQueue.front();
            matchQueue.pop();
            Node* right = matchQueue.front();
            matchQueue.pop();

            string winner = simulateMatch(left->name, right->name);
            Node* parent = new Node(winner);
            parent->left = left;
            parent->right = right;
            matchQueue.push(parent);
        }
        return matchQueue.front();
    }

    // Helper function to display the bracket with proper spacing
    void print2DUtil(Node* root, int space) const {
        if (root == nullptr) return;

        space += 10;

        print2DUtil(root->right, space);

        // Print current node after space
        cout << endl;
        for (int i = 10; i < space; i++) cout << " ";
        cout << root->name << "\n";

        print2DUtil(root->left, space);
    }

public:
    BracketGenerate() : root(nullptr), upperRoot(nullptr), lowerRoot(nullptr) {}

    // Load participants from file
    void LoadParticipants(string name, int n) {
        ifstream readfile("name.txt");
        if (!readfile) {
            cerr << "Unable to open file." << endl;
            return;
        }

        int count = 0;
        while (getline(readfile, name) && count < n) {
            participants.push_back(name);
            count++;
        }
        readfile.close();
    }

    // Print the list of participants
    void printParticipants() {
        cout << "Participants:" << endl;
        for (const string& name : participants) {
            cout << name << ", ";
        }
        cout << endl;
    }

    // Group participants based on user input for the number of groups
    void Grouping(int totalGroups) {
        if (totalGroups <= 0) {
            cout << "Invalid number of groups. Please enter a positive number." << endl;
            return;
        }

        random_device rd;
        mt19937 gen(rd());
        shuffle(participants.begin(), participants.end(), gen);

        groups.clear();  // Clear any existing groups
        int membersPerGroup = participants.size() / totalGroups;

        groups.resize(totalGroups);

        for (int i = 0; i < participants.size(); i++) {
            groups[i % totalGroups].push_back(participants[i]);
        }

        // Generate random dates for the groups
        vector<string> dates;
        for (int i = 0; i < totalGroups; i++) {
            dates.push_back(generateRandomDate());
        }

        // Sort the dates in ascending order
        sort(dates.begin(), dates.end());

        // Output the groups with sorted dates
        for (int i = 0; i < totalGroups; i++) {
            cout << "Group " << i + 1 << " (Date: " << dates[i] << "): ";
            for (const string& name : groups[i]) {
                cout << name << " ";
            }
            cout << endl;

            string groupWinner = simulateGroupMatches(groups[i], groupLosers);
            cout << "Group " << i + 1 << " winner: " << groupWinner << endl;
            groupWinners.push_back(groupWinner);
        }
    }

    // Generate tournament bracket (higher and lower bracket)
    void generateBracket() {
        // Participants for upper bracket: Winners from each group
        vector<string> upperParticipants = groupWinners;
        queue<Node*> matchQueue;

        for (const auto& winner : upperParticipants) {
            matchQueue.push(new Node(winner));
        }

        upperRoot = buildBracket(matchQueue);

        // Participants for lower bracket: Losers from each group
        vector<string> lowerParticipants = groupLosers;
        queue<Node*> lowerMatchQueue;

        for (const auto& loser : lowerParticipants) {
            lowerMatchQueue.push(new Node(loser));
        }

        lowerRoot = buildBracket(lowerMatchQueue);
    }

    // Simulate the entire tournament and determine the champion
    void simulateTournament() {
        if (!upperRoot || !lowerRoot) {
            cerr << "Bracket not generated." << endl;
            return;
        }

        // Simulate the final match between the two bracket winners
        string upperChampion = simulateMatch(upperRoot->left->name, upperRoot->right->name);
        string lowerChampion = simulateMatch(lowerRoot->left->name, lowerRoot->right->name);
        
        cout << "Upper Bracket Champion: " << upperChampion << endl;
        cout << "Lower Bracket Champion: " << lowerChampion << endl;

        // Final match between the two champions
        string champion = simulateMatch(upperChampion, lowerChampion);
        cout << "Tournament Champion: " << champion << endl;
    }

    // Display the tournament bracket in 2D format
    void displayBracket() const {
        if (!upperRoot || !lowerRoot) {
            cerr << "Bracket not generated." << endl;
            return;
        }
        
        cout << "\nUpper Bracket:\n";
        print2DUtil(upperRoot, 0);

        cout << "\nLower Bracket:\n";
        print2DUtil(lowerRoot, 0);
    }

    // Delete all dynamically allocated nodes to avoid memory leaks
    void deleteNodes(Node* node) {
        if (node != nullptr) {
            deleteNodes(node->left);
            deleteNodes(node->right);
            delete node;
        }
    }

    // Destructor to clean up dynamically allocated memory
    ~BracketGenerate() {
        deleteNodes(upperRoot);
        deleteNodes(lowerRoot);
    }
};

#endif
