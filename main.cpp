/*
 * Author:      Jacob Berg
 * Date:        February 10, 2020 @ 7:53 PM
 * Description: This is a program that simulates a partial, nondeterministic finite automaton. We say 'partial', because
 *              epsilon-transitions are not implemented (though, I don't believe it would be such a difficult task).
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <regex>
#include <string>

std::vector<std::string> split(std::string original_str, char delim);

std::vector<std::string> split(std::string original_str, char delim, std::vector<std::string> &split_string);

void parse_file(std::string file);

struct State {
    bool is_accept = false;
    bool is_start = false;
    long id = 0;
};

struct Transition {
    char symbol = ' ';
    State begin_state;
    State end_state;
};

struct Automaton {
    std::vector<State> states;
    static constexpr char alphabet[] =
            { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
              'v', 'w', 'x', 'y', 'z',
              '0', '9', '8', '7', '6', '5', '4', '3', '2', '1' };
    std::vector<Transition> transitions;


};


int main(int argc, char* argv[]) {

  Automaton automaton;
  std::string in_file_handle = "./input1.dat";
  std::vector<std::string> split_str;
  std::ostream_iterator<std::string> screen( std::cout, " " );

  parse_file(in_file_handle);
//  std::copy( split_str.begin(), split_str.end(), screen );
//  for ( auto i : split_str ) std::cout << i << std::endl;
//  parse_file(in_file);
//  std::cout << std::string::npos << std::endl;
  // TODO: Need to change 'input1.dat' to the argv[1]; the name of the file that Garrison will provide.

  return 0;
}


void parse_file(std::string file) {

  std::ifstream in_file{ file };
  std::string data;
  std::vector<std::string> data_vector( 1 );
  std::regex state_regex( "state" );
  std::regex start_regex( "start" );
  std::regex accept_regex( "accept" );
  std::regex start_accept_regex( "(accept|start){1,2}" );
  std::smatch start_match, accept_match, start_accept_match;

  if ( !in_file ) {
    std::cerr << "Failure in opening file." << "\n"
              << "Halting with exit code 1." << "\n";
    exit( 1 ); }

  getline( in_file, data );
  data_vector = split( data, '\t' );
  for ( auto i : data_vector ) std::cout << i << std::endl;
//
//  std::regex_search(data, start_match, start_regex);
//  for (int i = 0; i <= start_match.size(); i++){
//    std::cout << start_match[i] << "\n";
//  }


//  std::cout << std::boolalpha;
//
//  std::cout << "\n" << data << "\n\n";
//
//  std::cout <<"Were there any matches?" << "\n" << start_match.ready() << "\n\n";
//
//  std::cout << "How many matches were there?" << "\n" << start_match.size() << "\n\n";
//
//  std::cout << "What were the matches?" << "\n";


  in_file.close();
}

/*
 * Description: Partitions a given string @original_str by provided delimiter @delim.
 * Parameters:
 *    @std::string original_str               : The string to be split.
 *    @char delim                             : The delimiter along which to split string.
 */
std::vector<std::string>
split(std::string original_str, char delim) {
  std::vector<std::string> split_string;
  std::string spliced_string = original_str;

  if ( original_str.find_first_of( delim ) == std::string::npos ) {
    split_string.push_back( original_str );
    return split_string;
  } else {
    split_string.push_back( original_str.substr( 0, original_str.find_first_of( delim ) ) );
    spliced_string = original_str.erase( 0, original_str.find_first_of( delim ) + 1 );
  }
  return split( spliced_string, delim, split_string );

}

/*
 * Description: Partitions a given string @original_str by provided delimiter @delim.
 * Parameters:
 *    @std::string original_str               : The string to be split.
 *    @char delim                             : The delimiter along which to split string.
 *    @std::vector<std::string> &split_string : A reference to a vector which will contain the split string.
 */
std::vector<std::string>
split(std::string original_str, char delim, std::vector<std::string> &split_string) {
  std::string spliced_string = original_str;

  if ( original_str.find_first_of( delim ) == std::string::npos ) {
    split_string.push_back( original_str );
    return split_string;
  } else {
    split_string.push_back( original_str.substr( 0, original_str.find_first_of( delim ) ) );
    spliced_string = original_str.erase( 0, original_str.find_first_of( delim ) + 1 );
  }
  return split( spliced_string, delim, split_string );

}



















//  if (argc != 3) {
//    std::cerr << "Error:\t Three arguments were not detected." << "\n"
//              << "Usage:\t this_file_name\t automaton_specs.txt\tautomaton_config_string" << "\n"
//              << "Halting with exit code 1." << "\n";
//
//    return 1;
//  }