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
#include <map>
#include <unordered_map>
#include <iterator>
#include <string>

struct State {
    bool is_accept = false;
    bool is_start = false;
    int id = 0;
    std::map<std::string, std::vector<int> > transitions; // <std::string symbol, std::vector<long> end_states>
};

struct Automaton {
    State start_state;
    std::vector<State> accept_states;
    std::vector<State> states;
};

struct Output {
    bool is_accept = false;
    std::vector<int> final_states;
};


/*
 * Description: Read input file, line by line to parse definition of finite automaton. Store each line as a split string
 *              in a vector split_string, and then store split_string within data_vector.
 * Parameters:
 *    @std::string original_str               : The string to be split.
 *    @char delim                             : The delimiter an int which to split string.
 */
void parse_file(
        const std::string &file_name,
        std::vector<std::string> &data_vector
) {

  std::ifstream in_file{ file_name };
  std::string data;

  if ( !in_file ) {
    std::cerr << "Failure in opening file." << "\n"
              << "Halting with exit code 1." << "\n";
    exit( 1 );
  }

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
 *    @std::vector<std::string> &split_string : A reference to a vector which will contain the split string.
 */
std::vector<std::string> split(
        const std::string &original_str,
        const char &delim,
        std::vector<std::string> &split_string
) {
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
 */
std::vector<std::string> split(
        const std::string &original_str,
        const char &delim
) {
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
* Author:      Jacob Berg
* Date:        February 12, 2020 @ 6:32PM
* Description: Determines if line read from data read from input file is a state line, then updates
 *             the automata's list of states for each state line. State lines will only consist of
 *             start or accept states.
*/
void handle_state_line(
        Automaton &automaton,
        std::smatch matches,
        const std::string &current_line,
        const std::regex &id_pattern,
        const std::regex &start_pattern,
        const std::regex &accept_pattern
) {

  if ( !matches.empty() ) {
    State new_state;
    //find id number
    std::regex_search( current_line, matches, id_pattern );
    if ( !matches.empty() ) new_state.id = std::stoi( matches.str( 0 ) );
    //check if start state
    std::regex_search( current_line, matches, start_pattern );
    if ( !matches.empty() ) new_state.is_start = true;
    //check if accept state
    std::regex_search( current_line, matches, accept_pattern );
    if ( !matches.empty() ) new_state.is_accept = true;

    //Handle appending new_state to automaton.
    automaton.states.push_back( new_state );
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
void handle_transition_line(
        Automaton &automaton,
        std::smatch matches,
        const std::regex &transition_function_pattern,
        const std::string &current_line
) {
  std::vector<std::string> split_line = split( current_line, '\t' );


  //Remove word 'transition' from line.
  split_line.erase( split_line.begin() );

  int begin_state_arg = std::stoi( split_line[ 0 ] ), end_state_arg = std::stoi( split_line[ 2 ] );
  std::string symbol_arg = split_line[ 1 ];

  std::regex_search( current_line, matches, transition_function_pattern );

  //Determine if either of states in current transition are NOT already registered in the automaton.
  std::vector<int> state_ids;

  for ( const auto &current_automaton_state : automaton.states ) state_ids.push_back( current_automaton_state.id );

  std::sort( state_ids.begin(), state_ids.end() );

  //Create new states (neither start nor accept) where missing.
  for ( auto current_arg: { begin_state_arg, end_state_arg } ) {
    if ( !std::binary_search( state_ids.begin(), state_ids.end(), current_arg ) ) {
      State new_state;
      new_state.id = current_arg;
      automaton.states.push_back( new_state );
    }
    state_ids.push_back( current_arg );
    std::sort( state_ids.begin(), state_ids.end() );
  }

  //Add transitions to maps
  for ( auto &current_automaton_state : automaton.states ) {
    if ( current_automaton_state.id == begin_state_arg ) {
      current_automaton_state.transitions[ symbol_arg ].push_back( end_state_arg );
    }
    state_ids.push_back( current_automaton_state.id );
  }

}


/*
 * Description: Using data from input file create the desired automaton by specifying states and transitions.
 * Parameters:
 *    @Automaton &automaton                               : A reference to the automaton in main().
 *    @std::vector<std::vector<std::string>> &data_vector : The vector containing the parsed data file (containing the
 *                                                          specifications of the automaton).
 */
void create_automaton(
        Automaton &automaton,
        std::vector<std::string> &data_vector
) {
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

    if ( !matches.empty() ) handle_transition_line( automaton, matches, transition_function_pattern, current_line );
  }
}


/*
 * Author: Jacob Berg
 * Date: February 13, 2020 @ 3:40PM
 * Description: After having processed data from input file (i.e. determining all states, whether they are start or
 *              accept, and the outward transitions each possesses, we update the automatons start_state and
 *              accept_states fields for easier access.
 */
void config_start_and_accept_states(Automaton &automaton) {
  for ( const auto &state: automaton.states ) {
    if ( state.is_start ) automaton.start_state = state;
    if ( state.is_accept ) automaton.accept_states.push_back( state );
  }
}

void process_configuration_sequence(
        const std::vector<State> &automaton_states,
        State &current_state,
        const std::string &input_string,
        Output &output
) {

  std::vector<State> endpoints;
  std::string input_string_cpy;

  if ( input_string.empty() ) {
    output.final_states.push_back( current_state.id );
    if ( current_state.is_accept ) output.is_accept = true;
    return;
  }

  auto itr = current_state.transitions.find( std::string( 1, input_string.front() ) );

  for ( const auto &state : automaton_states ) {
    for ( const auto &transition_endpoint : itr->second ) {

      if ( transition_endpoint == state.id ) {
        endpoints.push_back( state );
        break;
      }

    }
  }
  input_string_cpy = input_string.substr( 1, input_string.length() );
  for ( auto &state : endpoints )
    process_configuration_sequence(
            automaton_states,
            state,
            input_string_cpy,
            output
    );


}

int main(int argc, char* argv[]) {

  static Automaton automaton;
  Output output;
  std::vector<std::string> data_vector;

  if ( argc != 3 ) {
    std::cerr << "Error:\t Three arguments were not detected." << "\n"
              << "Arguments detected were" << "\n";
    for ( int i = 0; i < argc; i++ ) {
      std::cout << argv[ i ] << "\n";
    }
    std::cout << "Usage:\t this_file_name\t automaton_specs.txt\tautomaton_config_string" << "\n"
              << "Halting with exit code 1." << "\n";

    exit( 1 );
  }

  const std::string in_file_handle = argv[ 1 ];
  const auto input_string = new std::string( argv[ 2 ] );

  parse_file( in_file_handle, data_vector );
  create_automaton( automaton, data_vector );
  config_start_and_accept_states( automaton );
  process_configuration_sequence(
          automaton.states,
          automaton.start_state,
          *input_string,
          output
  );

  std::sort( output.final_states.begin(), output.final_states.end() );
  auto itr = std::unique( output.final_states.begin(), output.final_states.end() );
  output.final_states.resize(
          static_cast<unsigned long>(std::distance( output.final_states.begin(), itr ))
  );

  if ( output.is_accept ) {
    std::cout << "accept\t";

    for ( const auto &final_state_id : output.final_states ) {
      for ( const auto &automaton_accept_state : automaton.accept_states ) {
        if ( final_state_id == automaton_accept_state.id ) {
          std::cout << final_state_id << " ";
          break;
        }
      }
    }
  } else {
    std::cout << "reject\t";
    for ( const auto &final_state_id : output.final_states ) std::cout << final_state_id << " ";
  }

  std::cout << "\n";

  return 0;
}

