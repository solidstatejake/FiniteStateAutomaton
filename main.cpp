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

const std::ostream_iterator<std::string> COUT( std::cout, "\n" );


struct State {
    bool is_accept = false;
    bool is_start = false;
    long id = 0;
};

struct Transition {
    std::string symbol;
    State begin_state;
    State end_state;
};

struct Automaton {
    State start_state;
    std::vector<State> end_states;
    std::vector<State> states;
    std::vector<Transition> transitions;
};

std::vector<std::string> split(const std::string original_str, const char &delim);

std::vector<std::string>
split(const std::string original_str, const char &delim, std::vector<std::string> &split_string);

void parse_file(const std::string &file_name, std::vector<std::string> &data_vector);

void create_automaton(Automaton &automaton, std::vector<std::string> &data_vector);

void handle_state_line(Automaton &automaton,
                       std::smatch matches,
                       const std::string &current_line,
                       const std::regex &id_pattern,
                       const std::regex &start_pattern,
                       const std::regex &accept_pattern);

void handle_transition_line(Automaton &automaton,
                            std::smatch matches,
                            const std::regex &transition_function_pattern,
                            const std::string &current_line
);

int main(int argc, char* argv[]) {

  Automaton automaton;
  const std::string in_file_handle = argv[ 1 ];
  std::vector<std::string> split_str;
  std::vector<std::string> data_vector;
  std::ostream_iterator<std::string> screen( std::cout, " " );

  //TODO change 3 to 2 when finished.
  if ( argc != 2 ) {
    std::cerr << "Error:\t Three arguments were not detected." << "\n"
              << "Usage:\t this_file_name\t automaton_specs.txt\tautomaton_config_string" << "\n"
              << "Halting with exit code 1." << "\n";

    return 1;
  }

  parse_file( in_file_handle, data_vector );

  create_automaton( automaton, data_vector );
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
void parse_file(const std::string &file_name, std::vector<std::string> &data_vector) {

  std::ifstream in_file{ file_name };
  std::string data;
  std::vector<std::string> split_string;

  if ( !in_file ) {
    std::cerr << "Failure in opening file." << "\n"
              << "Halting with exit code 1." << "\n";
    exit( 1 );
  }

  //Data is coming in line by line.
  while ( !in_file.eof() ) {
    getline( in_file, data );
    data_vector.push_back( data );
  }

  in_file.close();
}

/*
 * Description: Partitions a given string @original_str by provided delimiter @delim.
 * Parameters:
 *    @std::string original_str               : The string to be split.
 *    @char delim                             : The delimiter along which to split string.
 */
std::vector<std::string> split(const std::string original_str, const char &delim) {
  std::vector<std::string> split_string;
  std::string spliced_string = original_str;

  if ( original_str.find_first_of( delim ) == std::string::npos ) {
    split_string.push_back( original_str );
    return split_string;
  } else {
    split_string.push_back( original_str.substr( 0, original_str.find_first_of( delim ) ) );
    spliced_string.erase( 0, original_str.find_first_of( delim ) + 1 );
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
split(const std::string original_str, const char &delim, std::vector<std::string> &split_string) {
  std::string spliced_string = original_str;

  if ( original_str.find_first_of( delim ) == std::string::npos ) {
    split_string.push_back( original_str );
    return split_string;
  } else {
    split_string.push_back( original_str.substr( 0, original_str.find_first_of( delim ) ) );
    spliced_string.erase( 0, original_str.find_first_of( delim ) + 1 );
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
void create_automaton(Automaton &automaton, std::vector<std::string> &data_vector) {
  std::regex state_pattern{ "state" },
          transition_pattern{ "transition" },
          transition_function_pattern{ R"(\d\t\w\t\d)" },
          id_pattern{ "[[:digit:]]+" },
          start_pattern{ "start" },
          accept_pattern{ "accept" },
          start_and_accept_pattern{ R"((start\taccept)|(accept\tstart))" };

  std::smatch matches;

  for ( const std::string &current_line : data_vector ) {

    std::regex_search( current_line, matches, state_pattern );


    handle_state_line( automaton, matches, current_line, id_pattern, start_pattern, accept_pattern );


    std::regex_search( current_line, matches, transition_pattern );

    handle_transition_line( automaton, matches, transition_function_pattern, current_line );
  }
}

/*
* Author:      Jacob Berg
* Date:        February 12, 2020 @ 6:32PM
* Description: Determines if line read from data read from input file is a state line, then updates
 *             the automata's list of states for each state line. State lines will only consist of
 *             start or accept states.
*/
void handle_state_line(Automaton &automaton,
                       std::smatch matches,
                       const std::string &current_line,
                       const std::regex &id_pattern,
                       const std::regex &start_pattern,
                       const std::regex &accept_pattern) {

  if ( !matches.empty() ) {
    State new_state;
    //find id number
    std::regex_search( current_line, matches, id_pattern );
    if ( !matches.empty() ) new_state.id = std::stol( matches.str( 0 ) );
    //check if start state
    std::regex_search( current_line, matches, start_pattern );
    if ( !matches.empty() ) new_state.is_start = true;
    //check if accept state
    std::regex_search( current_line, matches, accept_pattern );
    if ( !matches.empty() ) new_state.is_accept = true;

    //Handle appending new_state to automaton.
    automaton.states.push_back( new_state );
    if ( new_state.is_start ) automaton.start_state = new_state;
    if ( new_state.is_accept ) automaton.end_states.push_back( new_state );
  }

}


/*
 * Author:      Jacob Berg
 * Date:        February 12, 2020 @ 11:43PM
 * Description: Determines if line read from data read from input file is a transition line, then updates
 *              the automata's list of transitions for each transition line. If a transition contains a
 *              state which was not already a part of the automata's states, then it adds it to the set of
 *              states.
*/
void handle_transition_line(Automaton &automaton,
                            std::smatch matches,
                            const std::regex &transition_function_pattern,
                            const std::string &current_line) {
  std::vector<std::string> split_line = split( current_line, '\t' );
  if ( !matches.empty() ) {
    split_line.erase( split_line.begin() );

    State transition_fn_begin_state, transition_fn_end_state;
    Transition new_transition;
    long arg1 = std::stol( split_line[ 0 ] ),
            arg3 = std::stol( split_line[ 2 ] );
    std::string arg2 = split_line[ 1 ];

    std::regex_search( current_line, matches, transition_function_pattern );
    //Determine if either of states in current transition are NOT already registered in the automaton.
    std::vector<long> state_ids;

    for ( const auto &current_automaton_state : automaton.states )
      state_ids.push_back( current_automaton_state.id );

    std::sort( state_ids.begin(), state_ids.end() );

    //Create new states (neither start nor accept) where missing.
    for ( auto current_arg: { arg1, arg3 } ) {
      if ( !std::binary_search( state_ids.begin(), state_ids.end(), current_arg ) ) {
        State new_state;
        new_state.id = current_arg;
        automaton.states.push_back( new_state );
      }
      state_ids.push_back( current_arg );
      std::sort( state_ids.begin(), state_ids.end() );
    }

    //Done adding states to automaton.
    //BEGIN transitions.

    transition_fn_begin_state.id = arg1;
    transition_fn_end_state.id = arg3;
    new_transition.symbol = arg2;
    new_transition.begin_state = transition_fn_begin_state;
    new_transition.end_state = transition_fn_end_state;
    automaton.transitions.push_back( new_transition );
  }
}

void handle_input_string(const std::string input) {


}


