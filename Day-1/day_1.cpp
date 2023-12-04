//
// Euan W: 01/12/2023
//
// Solution for Advent of Code day 1, a series of strings are given, each containing a number, find the first and last number in each string and add them together.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <map>

using namespace std;
namespace constants {
    array<string, 18> number_strings = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    map<string, string> number_map = {
        {"one", "1"},
        {"two", "2"},
        {"three", "3"},
        {"four", "4"},
        {"five", "5"},
        {"six", "6"},
        {"seven", "7"},
        {"eight", "8"},
        {"nine", "9"},
        {"1", "1"},
        {"2", "2"},
        {"3", "3"},
        {"4", "4"},
        {"5", "5"},
        {"6", "6"},
        {"7", "7"},
        {"8", "8"},
        {"9", "9"}
    };
}

vector<string> read_lines_from_file(string& filename) {
    string file_text;
    vector<string> line_list;
    ifstream line_list_file(filename);
    if (line_list_file.is_open()) {
        while (getline(line_list_file, file_text)) {
            line_list.push_back(file_text);
        }
    }
    else {
        cout << "Unable to open file" << endl;
    }
    line_list_file.close();
    return line_list;
};

string find_digit_in_string(string line, bool first) {
    string digit;
    string original_line = line;
    int digit_index;

    if (first) {
        digit_index = 10000;
    }
    else {
        digit_index = 0;
    }
    
    for (int i = 0; i < constants::number_strings.size(); i++) {
        cout << "Trying to find " << constants::number_strings[i] << " in " << line << endl;
        int location = line.find(constants::number_strings[i]);
        if (location != std::string::npos) {
            cout << "   Found " << constants::number_strings[i] << " at position " << location << " current digit is " << digit << " at " << digit_index << endl;
            if (first && location <= digit_index) {
                cout << "   Location of " << constants::number_strings[i] << " is closer to start than current, " << digit << " at " << digit_index << " replacing." << endl;
                digit = constants::number_strings[i];
                digit_index = location;
            }
            else if (!first && location >= digit_index) {
                cout << "   Location of " << constants::number_strings[i] << " is closer to end than current, " << digit << " at " << digit_index << " replacing." << endl;
                digit = constants::number_strings[i];
                digit_index = location;
            }
            line.replace(location, 1, "X");
            i--;  // If we found the digit, we need to check again if it exists further along in the string
        }
        else {
            line = original_line;
        }
    }
    return constants::number_map[digit];
}

/* Finds first and last integer in line, regardless of whether its a string representation (IE: "one" or 1), returns as integer value */
int calculate_value(string& line) {
    cout << "Checking line " << line << endl;
    string first_digit;
    string last_digit;
    
    // cout << "Finding first digit." << endl << endl;
    first_digit = find_digit_in_string(line, true);

    // cout << "Finding last digit." << endl << endl;
    last_digit = find_digit_in_string(line, false);

    if (first_digit == "") {
        cout << "   No first digit found." << endl;
    }
    else if (last_digit == "") {
        cout << "   No last digit found." << endl;
    }
    else {
        cout << "   First digit is " << first_digit << ", last digit is " << last_digit << endl;
    }

    string number_string = first_digit + last_digit;
    return stoi(number_string);
}

int main() {
    // Get our lines.
    string filename = "line_list.txt";
    vector<string> line_list = read_lines_from_file(filename);

    // For each line in list, 
    int total = 0;
    for (int i = 0; i < line_list.size(); i++) {
        total += calculate_value(line_list[i]);
    }
    cout << "Total: " << total << endl;
};