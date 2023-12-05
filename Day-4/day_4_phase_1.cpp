//
// Euan W: 04/12/2023
//
// Solution for Advent of Code day 4 Phase 1
//


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <map>

using namespace std;

bool check_number_in_array(int number, vector<int> array) {
    for (int i = 0; array.size(); i++) {
        if (array[i] == number) {
            return true;
        }
    }
    return false;
}


class Card {
    public:
        int id;  // Id of the card
        vector<int> winning_numbers = {};
        vector<int> our_numbers = {};

    private:
        int _number_of_matches;

    void calculate_score(){
        for (int i = 0; i < winning_numbers.size(); i++) {
            if (check_number_in_array(winning_numbers[i], our_numbers)) {
                _number_of_matches++;
            }
        };
    };
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

int get_card_id(string line)
{
    int game_id;
    int colon_split_location = find_location_of_delimiter(line, ':')[0];
    string card_id_string = line.substr(0, colon_split_location);

    vector<int> space_split_locations = find_location_of_delimiter(card_id_string, ' ');  // We expect more than one space sometimes.
    int last_location = space_split_locations[space_split_locations.size() - 1];
    
    string card_id_string_2 = card_id_string.substr(last_location, card_id_string.size() - last_location);

    game_id = stoi(card_id_string_2);
    return game_id;
}

int main() {
    string filename = "input.txt";
    vector<string> line_list = read_lines_from_file(filename);
    vector<Card> card_list = {};

    for (string line: line_list) {
        Card new_card = Card();
        new_card.id = get_card_id(line);
        card_list.push_back(new_card);
        cout << new_card.id << endl;
    }
}