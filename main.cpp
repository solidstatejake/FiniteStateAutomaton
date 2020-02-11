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

static constexpr char ALPHABET[] =
        { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
          'v', 'w', 'x', 'y', 'z',
          '0', '9', '8', '7', '6', '5', '4', '3', '2', '1' };

constexpr std::ostream_iterator<std::string> COUT( std::cout, " " );


std::vector<std::string> split(std::string original_str, const char &delim);

std::vector<std::string> split(std::string original_str, const char &delim, std::vector<std::string> &split_string);

void parse_file(const std::string &file_name, std::vector<std::vector<std::string>> &data_vector);

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
    State start_state;
    std::vector<State> end_states;
    std::vector<State> states;
    std::vector<Transition> transitions;
};


int main(int argc, char* argv[]) {

  Automaton automaton;
  const std::string in_file_handle = argv[ 1 ];
  std::vector<std::string> split_str;
  std::vector<std::vector<std::string>> data_vector;
  std::ostream_iterator<std::string> screen( std::cout, " " );

  //TODO change 3 to 2 when finished.
  if ( argc != 2 ) {
    std::cerr << "Error:\t Three arguments were not detected." << "\n"
              << "Usage:\t this_file_name\t automaton_specs.txt\tautomaton_config_string" << "\n"
              << "Halting with exit code 1." << "\n";

    return 1;
  }

  parse_file( in_file_handle, data_vector );
//
//  for ( auto split_string : data_vector ) {
//    for (const auto &word : split_string ) {
//      std::cout << word << "\n";
//    }
//  }


  return 0;
}

/*
 * Description: Read input file, line by line to parse definition of finite automaton. Store each line as a split string
 *              in a vector split_string, and then store split_string within data_vector.
 * Parameters:
 *    @std::string original_str               : The string to be split.
 *    @char delim                             : The delimiter along which to split string.
 */
void parse_file(const std::string &file_name, std::vector<std::vector<std::string>> &data_vector) {

  std::ifstream in_file{ file_name };
  std::string data;
  std::vector<std::string> split_string;

  if ( !in_file ) {
    std::cerr << "Failure in opening file." << "\n"
              << "Halting with exit code 1." << "\n";
    exit( 1 );
  }

  while ( !in_file.eof() ) {
    getline( in_file, data, ' ' );
    data_vector.push_back( split( data, '~') );
  }

  in_file.close();
}

/*
 * Description: Partitions a given string @original_str by provided delimiter @delim.
 * Parameters:
 *    @std::string original_str               : The string to be split.
 *    @char delim                             : The delimiter along which to split string.
 */
std::vector<std::string> split(std::string original_str, const char &delim) {
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
std::vector<std::string> split(std::string original_str, const char &delim, std::vector<std::string> &split_string) {
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
 * Description: Using data from input file create the desired automaton by specifying states and transitions.
 * Parameters:
 *    @Automaton &automaton                               : A reference to the automaton in main().
 *    @std::vector<std::vector<std::string>> &data_vector : The vector containing the parsed data file (containing the
 *                                                          specifications of the automaton).
 */
void create_automaton(Automaton &automaton, std::vector<std::vector<std::string>> &data_vector){
  std::vector<std::string> split_line;

  for ( auto line : data_vector ) {
    for (const auto &word : line ) {

      split_line = split(word, ' ');

      for(const auto &i : split_line) std::cout << i << "\n";

      if(word == "state"){


      }else if (word == "transition"){


      }

    }
  }

}