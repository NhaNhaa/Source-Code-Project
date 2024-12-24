#include<iostream>
#include "Bracket.h"
#include <fstream>
#include <vector>

using namespace std;

int main() {
    BracketGenerate bracket;

    int n;
    cout << "Enter the number of participants: ";
    cin >> n;

    bracket.LoadParticipants("name.txt", n);

    int group;
    cout << "How many groups do you want?: ";
    cin >> group;

    // Group participants randomly and display group information
    bracket.Grouping(group);

    // Generate the tournament bracket
    bracket.generateBracket();

    // Display the bracket in 2D format
    bracket.displayBracket();

    // Simulate and display the tournament winner
    bracket.simulateTournament();

    return 0;
}
