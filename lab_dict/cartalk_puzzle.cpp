/**
 * @file cartalk_puzzle.cpp
 * Holds the function which solves a CarTalk puzzler.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include <fstream>

#include "cartalk_puzzle.h"

using namespace std;

/**
 * Solves the CarTalk puzzler described here:
 * http://www.cartalk.com/content/wordplay-anyone.
 * @return A vector of (string, string, string) tuples
 * Returns an empty vector if no solutions are found.
 * @param d The PronounceDict to be used to solve the puzzle.
 * @param word_list_fname The filename of the word list to be used.
 */
vector<std::tuple<std::string, std::string, std::string>> cartalk_puzzle(PronounceDict d,
                                                                         const string &word_list_fname)
{
    vector<std::tuple<std::string, std::string, std::string>> ret;

    ifstream inputFile(word_list_fname);
    if (!inputFile.is_open())
    {
        return vector<std::tuple<std::string, std::string, std::string>>();
    }
    string line;
    while (getline(inputFile, line))
    {
        if (line.size() >= 3)
        {
            if (d.homophones(line, line.substr(1)) && d.homophones(line, line.substr(0, 1) + line.substr(2)))
            {
                ret.push_back(std::make_tuple(line, line.substr(1), line.substr(0, 1) + line.substr(2)));
            }
        }
    }
    return ret;
}
