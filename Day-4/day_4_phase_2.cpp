//
// Euan W: 12/12/2023
//
// Solution for Advent of Code day 4 Phase 2
//


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <map>

using namespace std;

bool check_number_in_array(int number, vector<int> array) {
    for (int i = 0; i < array.size(); i++) {
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

    int count_matches() {
        int found_count = 0;
        for (int i = 0; i < winning_numbers.size(); i++) {
            if (check_number_in_array(winning_numbers[i], our_numbers)) {
                found_count += 1;
            }
        };
        return found_count;
    }

    int calculate_score(){
        int number_of_matches = count_matches();
        if (number_of_matches >= 1) {
            _score = 1;
            for (int i = 0; i < number_of_matches - 1; i++) {
               _score = _score * 2;
            }
        }
        return _score;
    };
 
    private:
        int _score = 0;
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

bool check_string_is_numerical( string s ) {
   for( int i = 0; i < s.length(); i++ ) {
      if( !isdigit( s[i] )) {
         return false;
      }
   }
   return true;
}

vector<int> split_number_string(string line){
    vector<int> numbers = {};
    vector<int> split_locations = find_location_of_delimiter(line, ' ');
    split_locations.insert(split_locations.begin(), -1); // Ensure we get the first roll too.

    for (int i = 0; i < split_locations.size(); i++) {
        string potential_number = line.substr(split_locations[i] + 1, split_locations[i + 1] - split_locations[i]);
        if (potential_number == "") {
            continue;
        }

        // Remove any whitespace
        vector<int> space_locations = find_location_of_delimiter(potential_number, ' ');
        for (int j = 0; j < space_locations.size(); j++) {
            potential_number.erase(space_locations[j], 1);
        }

        if (potential_number.size() == 0) {
            continue;
        }
        if (check_string_is_numerical(potential_number)){
            numbers.push_back(stoi(potential_number));
        }

    } 
    return numbers;
}

Card split_number_list(string line, Card new_card) {
    int delimiter_split_location = find_location_of_delimiter(line, '|')[0];

    string winning_numbers_string = line.substr(0, delimiter_split_location);
    cout << winning_numbers_string << endl;
    
    string our_number_string = line.substr(delimiter_split_location + 1, line.size());
    cout << our_number_string << endl;
    
    new_card.winning_numbers = split_number_string(winning_numbers_string);
    new_card.our_numbers = split_number_string(our_number_string);
    
    return new_card;
}

int main() {
    string filename = "input_test.txt";
    vector<string> line_list = read_lines_from_file(filename);

    int total = 0;
    
    // Populate map of ids to card
    map<int, vector<Card>> id_to_card_list;
    for (string line: line_list) {
        cout << "Processing line: " << line << endl;
        Card new_card = Card();
        
        new_card.id = get_card_id(line);
        int colon_split_location = find_location_of_delimiter(line, ':')[0];
        line = line.substr(colon_split_location + 1, line.size());
        new_card = split_number_list(line, new_card);
        id_to_card_list[new_card.id] = {new_card};
    }

    int total_scratchcard_number = 0;
    map<int, vector<Card>>::iterator iterator = id_to_card_list.begin();
    while (iterator != id_to_card_list.end()) {
        int card_id = iterator->first;
        cout << "Card id: " << card_id << " has " << iterator->second.size() << " cards" << endl;
        int card_matches = iterator->second[0].count_matches();
        for (Card card_to_read: iterator->second) {
            total_scratchcard_number++;
            cout << "   Reading " << card_to_read.id << " " << card_to_read.count_matches() << endl; 
            for (int j = 0; j <= card_matches; j++) {
                int next_card_id = card_id + j;
                cout << "   Adding match to " << next_card_id << endl;
                if (next_card_id < id_to_card_list.end()->first) {
                    cout << id_to_card_list[next_card_id][0].id << endl;
                    id_to_card_list[next_card_id].push_back(id_to_card_list[next_card_id][0]);
                    cout << "added" << endl;
                }
            }
        }        
        iterator++;
    }
    cout << "Total scratch cards: " << total_scratchcard_number << endl;
}