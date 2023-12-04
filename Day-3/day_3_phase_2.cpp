//
// Euan W: 03/12/2023
//
// Solution for Advent of Code day 3 Phase 2
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <map>

using namespace std;
namespace constants
{
    char DOT = '.';
    char STAR = '*';
}


class GridCharacter
{
    public:
        char value; // Single character value. 
        bool part_of_number = false; // Set to true if this character is part of a bigger number.
        int number_index; // If this character is part of a bigger number, this is the index that this number sits in the number. 
        int number_value; // If this character is part of a bigger number, this value is set to the overall value.
        short int number_id; // Id of the parent number
};


vector<string> read_lines_from_file(string &filename)
{
    string file_text;
    vector<string> line_list;
    ifstream line_list_file(filename);
    if (line_list_file.is_open())
    {
        while (getline(line_list_file, file_text))
        {
            line_list.push_back(file_text);
        }
    }
    else
    {
        cout << "Unable to open file" << endl;
    }
    line_list_file.close();
    return line_list;
};

bool check_symbol(char value, bool consider_dot_symbol) {
    /* Returns true if the character is a symbol */
    if (isdigit(value)) {
        return false;
    }
    
    if (value == constants::DOT) {
        if (consider_dot_symbol) { 
            return true;
        }
        else {
            return false;
        }
    }
    
    return true;
}
 
int find_overall_number_value(string line, int start_index) {
    int index = 0;
    char character_to_test = line[start_index];
    string number_string = "";

    while (isdigit(character_to_test) && index <= line.size()) {
        character_to_test = line[start_index + index];
        number_string += line[start_index + index];
        index++;
    }

    index = index - 1;
    int number_int = stoi(number_string.substr(0, index));
    cout << "Found number " << number_int << " with length " << index << " at position " << start_index << endl;

    return number_int;
}

tuple<vector<GridCharacter>, int> assemble_line_vector(string line, int number_id) {
    vector<GridCharacter> line_vector;
    bool number = false;
    int number_index = 0;
    int number_value = 0;

    for (int char_index = 0; char_index < line.size(); char_index++) {
        char character = line[char_index];
        cout << "Checking " << character << " " << number << endl;

        GridCharacter grid_character = GridCharacter();
        grid_character.value = character;

        if (isdigit(character)) {
            grid_character.part_of_number = true;
            if (number == false) {
                // We have found the start of a number
                number_id++;
                number = true;
                number_index = 0;
                number_value = find_overall_number_value(line, char_index);
            }
            else {
                // We have found the middle of a number, set to current number value
                number_index++;
                grid_character.number_value = number_value;
            }
            grid_character.number_value = number_value;
            grid_character.number_index = number_index;
            grid_character.number_id = number_id;
        }
        else if (check_symbol(character, true)) {
            // cout << "Found symbol character " << character << endl;
            if (number) {
                // We have found the end of a number
                number = false;
                number_index = 0; 
                number_value = 0;
            }
            grid_character.part_of_number = false;
        }
        line_vector.push_back(grid_character);
    }

    return make_tuple(line_vector, number_id);
}

vector<vector<GridCharacter>> unpack_lines_into_grid(vector<string> line_list) {
    vector<vector<GridCharacter>> grid;
    short int number_id = 0;

    for (string line : line_list) {
        vector<GridCharacter> line_vector;
        tie(line_vector, number_id) = assemble_line_vector(line, number_id);
        grid.push_back(line_vector);
    }
    return grid;
}

bool check_neighbouring_number_already_counted(vector<GridCharacter> neighbouring_numbers, GridCharacter character) {
    // Checks if the character is part of a number that is in the neighbouring_number_ids list
    for (GridCharacter grid_character : neighbouring_numbers) {
        if (grid_character.number_id == character.number_id) {
            return true;
        }
    }
    return false;
}

vector<GridCharacter> check_character(GridCharacter character_to_check, vector<GridCharacter> neighbouring_numbers) {
    // cout << "Checking character " << character_to_check.value;
    if (character_to_check.part_of_number && !check_neighbouring_number_already_counted(neighbouring_numbers, character_to_check)) {
        cout << "Found number " << character_to_check.number_value << endl;
        neighbouring_numbers.push_back(character_to_check);
    }
    return neighbouring_numbers;
}

vector<GridCharacter> check_above_bellow_line(vector<GridCharacter> line, int character_index, vector<GridCharacter> neighbouring_numbers, bool isInline) {
    cout << "Starting above bellow with char index " << character_index << endl;
    
    // Check to left
    if (character_index != 0) {
        GridCharacter character_to_check = line[character_index - 1];
        cout << "Checking character " << character_index - 1 << " the left: " << character_to_check.value << endl;
        neighbouring_numbers = check_character(character_to_check, neighbouring_numbers);
    }

    cout << "Checking above with char index " << character_index << endl;
    // Check directly above
    if (!isInline) {
        GridCharacter character_to_check = line[character_index];
        cout << "Checking character " << character_index << " above: " << character_to_check.value << endl;
        neighbouring_numbers = check_character(character_to_check, neighbouring_numbers);
    }

    // Check to the right
    if (character_index < line.size()) {
        cout << "Checking right with char index " << character_index << endl;
        GridCharacter character_to_check = line[character_index + 1];
        cout << "Checking character " << character_index + 1 << " the right: " << character_to_check.value << endl;
        neighbouring_numbers = check_character(character_to_check, neighbouring_numbers);
    }

    return neighbouring_numbers;
}

int check_for_neighbouring_numbers(vector<vector<GridCharacter>> grid, int line_index, int char_index) {
    cout << "Checking line " << line_index << endl;
    vector<GridCharacter> neighbouring_numbers = {};

    // Check line above
    if (line_index != 0) {
        cout << "Checking line above" << endl;
        vector<GridCharacter> line = grid[line_index - 1];
        neighbouring_numbers = check_above_bellow_line(line, char_index, neighbouring_numbers, false);
    }
   
    // Check neighbouring characters
    cout << "Checking neighbouring line" << endl;
    vector<GridCharacter> line = grid[line_index];
    neighbouring_numbers = check_above_bellow_line(line, char_index, neighbouring_numbers, true);

    // Check line bellow
    if (line_index + 1 < grid.size()) {
        cout << "Checking line bellow" << endl;
        line = grid[line_index + 1];
        neighbouring_numbers = check_above_bellow_line(line, char_index, neighbouring_numbers, false);
    }

    // Found too many gears, return nothing.
    if (neighbouring_numbers.size() == 2) {
        int first_number = neighbouring_numbers[0].number_value;
        int second_number = neighbouring_numbers[1].number_value;
        cout << "FOUND PAIR: " << first_number << " * " << second_number << " = " << first_number * second_number << endl;
        return (first_number * second_number);
    }
    else {
        return 0;
    }
}

int main()
{
    string filename = "input_full.txt";
    vector<string> line_list = read_lines_from_file(filename);
    vector<vector<GridCharacter>> grid = unpack_lines_into_grid(line_list);

    int total = 0;    
    // Iterate through each character in grid, check for nearby
    for (int line_index = 1; line_index < grid.size(); line_index++) {
        for (int char_index = 0; char_index < grid[line_index].size(); char_index++) {
            GridCharacter character_to_test = grid[line_index][char_index];
            if (character_to_test.value == constants::STAR) {
                cout << "Found star at (" << char_index << "," << line_index << ")" << endl;
                total += check_for_neighbouring_numbers(grid, line_index, char_index);
            }
        }
    }
    cout << "Total sum of numbers found is " << total << endl;
};