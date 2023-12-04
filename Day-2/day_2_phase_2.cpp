//
// Euan W: 03/12/2023
//
// Solution for Advent of Code day 2 Phase 2
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
    string COLOUR_RED = "red";
    string COLOUR_BLUE = "blue";
    string COLOUR_GREEN = "green";
    array<string, 3> COLOURS = {COLOUR_RED, COLOUR_BLUE, COLOUR_GREEN};
    int RED_IN_BAG = 12;
    int GREEN_IN_BAG = 13;
    int BLUE_IN_BAG = 14;
}

class Roll
{
public:
    int red_dice;
    int blue_dice;
    int green_dice;
};

class Game
{
public:
    int id;
    vector<Roll> rolls;
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

vector<int> find_location_of_delimiter(string line, char delimiter)
{
    vector<int> delimiter_locations = {};
    for (int i = 0; i < line.size(); i++)
    {
        if (line[i] == delimiter)
        {
            delimiter_locations.push_back(i);
        }
    }
    return delimiter_locations;
}

int get_game_id(string line)
{
    int game_id;
    int colon_split_location = find_location_of_delimiter(line, ':')[0];
    string game_id_string = line.substr(0, colon_split_location);

    int space_split_location = find_location_of_delimiter(game_id_string, ' ')[0];
    string game_id_string_2 = game_id_string.substr(space_split_location, game_id_string.size() - space_split_location);

    game_id = stoi(game_id_string_2);
    return game_id;
}

int extract_number(string line, string colour)
{
    int location = line.find(colour);
    if (location != string::npos)
    {
        return stoi(line.substr(0, location));
    }
    else
    {
        return 0;
    }
}

Roll extract_roll_from_string(string string_roll)
{
    Roll roll = Roll();
    vector<int> colour_location_delimiters = find_location_of_delimiter(string_roll, ',');
    colour_location_delimiters.insert(colour_location_delimiters.begin(), 0); // Ensure we get the first colour too.

    // Get the size of the vector
    size_t num_delimiters = colour_location_delimiters.size();
    if (num_delimiters == 1) {
        colour_location_delimiters.push_back(100);
    }

    for (int i = 0; i < num_delimiters; i++)
    {
        string colour_string = string_roll.substr(colour_location_delimiters[i], colour_location_delimiters[i + 1] - colour_location_delimiters[i]);
        colour_string.erase(remove(colour_string.begin(), colour_string.end(), ','), colour_string.end()); // Remove any crap
        colour_string.erase(remove(colour_string.begin(), colour_string.end(), ' '), colour_string.end()); // Again

        // cout << "Checking " << colour_string;
        // cout << "Found colour " << colour_string << endl;
        if (colour_string.find(constants::COLOUR_RED) != string::npos)
        {
            // cout << "Found a red value" << endl;
            roll.red_dice = extract_number(colour_string, constants::COLOUR_RED);
        }
        else if (colour_string.find(constants::COLOUR_GREEN) != string::npos)
        {
            // cout << "       Found a green value" << endl;
            roll.green_dice = extract_number(colour_string, constants::COLOUR_GREEN);
        }
        else if (colour_string.find(constants::COLOUR_BLUE) != string::npos)
        {
            // cout << "Found a blue value" << endl;
            roll.blue_dice = extract_number(colour_string, constants::COLOUR_BLUE);
        }
    }

    // cout << "Returing roll: Blue: " << roll.blue_dice  << " Green: " << roll.green_dice << " Red: " << roll.red_dice << endl;

    return roll;
}

vector<Roll> get_rolls(string line)
{
    vector<Roll> rolls = {};

    // Split the line into rolls
    vector<int> roll_delimiter_locations = find_location_of_delimiter(line, ';');
    roll_delimiter_locations.insert(roll_delimiter_locations.begin(), -1); // Ensure we get the first roll too.

    // Get the size of the vector
    size_t num_delimiters = roll_delimiter_locations.size();

    // For each delimiter location split
    for (size_t i = 0; i < num_delimiters; i++)
    {
        string roll_string = line.substr(roll_delimiter_locations[i] + 1, roll_delimiter_locations[i + 1] - roll_delimiter_locations[i]);
        // cout << roll_string << endl;
        rolls.push_back(extract_roll_from_string(roll_string));
    }

    return rolls;
}

int find_minimum_dice_by_colour(Game game, string colour) {
    int biggest_value = 0;
    for (int i = 0; i < game.rolls.size(); i++) {
        Roll roll = game.rolls[i];
        if (colour == constants::COLOUR_RED) {
            if (roll.red_dice > biggest_value) {
                biggest_value = roll.red_dice;
            }
        }
        if (colour == constants::COLOUR_GREEN) {
            if (roll.green_dice > biggest_value) {
                biggest_value = roll.green_dice;
            }
        }
        if (colour == constants::COLOUR_BLUE) {
            if (roll.blue_dice > biggest_value) {
                biggest_value = roll.blue_dice;
            }
        }
    }

    cout << "Min " << colour << " is " << biggest_value << endl;

    return biggest_value;
}

int find_power_of_minimum_dice(Game game) {
    int red_minimum = find_minimum_dice_by_colour(game, constants::COLOUR_RED);
    int green_minimum = find_minimum_dice_by_colour(game, constants::COLOUR_GREEN);
    int blue_minimum = find_minimum_dice_by_colour(game, constants::COLOUR_BLUE);
    
    int power = 1;
    if (red_minimum > 0) {
        power = power * red_minimum;
    }

    if (green_minimum > 0) {
        power = power * green_minimum;
    }

    if (blue_minimum > 0) {
        power = power * blue_minimum;
    }

    return power;
}

int main()
{
    string filename = "input.txt";
    vector<string> line_list = read_lines_from_file(filename);
    // vector<string> line_list = {"Game 57: 2 red, 3 blue; 1 red, 8 blue; 9 red, 11 green, 8 blue; 1 blue, 13 green, 6 red; 7 red, 8 green, 2 blue", "Game 88: 9 green, 1 red, 5 blue; 8 red, 7 green, 14 blue; 8 red, 9 blue, 10 green; 4 red, 10 blue; 10 red, 4 green, 19 blue; 9 red, 1 green", "Game 40: 17 green; 12 green, 6 blue; 1 red, 3 blue, 6 green; 1 red, 5 blue, 8 green"};
    int total = 0;

    for (int i = 0; i < line_list.size(); i++)
    {
        cout << endl << "Processing line: " << line_list[i] << endl;

        // Intialise our game.
        Game game = Game();
        game.id = get_game_id(line_list[i]);

        // Remove our Game ID: from the line.
        string line = line_list[i].replace(0, line_list[i].find(':') + 1, "");

        // Get our rolls.
        vector<Roll> rolls = get_rolls(line);
        game.rolls = rolls;

        // Get the power of each min roll
        int power = find_power_of_minimum_dice(game);
        total += power;
    }

    cout << "Total of ids: " << total << endl;
};