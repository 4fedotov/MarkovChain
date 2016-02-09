#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>
#include <locale>
#include <queue>
#include <unordered_map>
#include <sstream>

#include "markchain.h"
#include "utils.h"

using namespace std;

class Generator
{
public:
    Generator()
    {}
    
    template<typename Iterator>
    string createKey(const Iterator begin, Iterator end)
    {
        string key_string;
        stringstream ss;
        copy(begin, end - 1, ostream_iterator<string>(ss, ","));
        key_string = ss.str();
        key_string += *(end - 1);
        return key_string;
    }
    
    void run(const MarkovChain &chain, const vector<string> &start_words, unsigned long need_words, ostream &out)
    {
        
        if(chain.getN() > start_words.size())
        {
            throw runtime_error("Number of start words is less than chain order");
        }
        
        deque<string> accum;
    
        // start form n last words
        copy(start_words.end() - chain.getN(), start_words.end(), back_inserter(accum));
    
        while(need_words--)
        {
            string key_string = createKey(accum.begin(), accum.end());
          
            string val = chain.getOneRandom(key_string);
        
            if(val.empty())
                break;
            
            out << val << " ";
            
            accum.push_back(val);
            accum.pop_front();
        }
    }
};

void help()
{
    cerr << "Usage: <app> start_words_filename chain_filename need_words" << endl;
}

int main(int argc, char *argv[])
{
    //check command line args
    if(argc < 4)
    {
        help();
        return -1;
    }
    //read n words, chain file name and number of words need to generate
    string start_words_filename(argv[1]);
    string chain_filename(argv[2]);
    
    unsigned long need_words = 0;
    try
    {
        need_words = stoul(argv[3]);
    }
    catch(const exception &exc)
    {
        cerr << "Words number couldn't be parsed: " << exc.what() << endl;
        return -1;
    }
    
    //create markov chain from file, check errors
    MarkovChain chain;
    ifstream fchain;
    try
    {
        fchain.open(chain_filename, ios::in);
    }
    catch(const runtime_error &exc)
    {
        cerr << "Chain file couldn't be opened: " << exc.what() <<endl;
        return -1;
    }
    
    if(fchain.is_open())
    {
        try
        {
            chain.deserialize(fchain);
        } catch (const runtime_error &exc)
        {
            cerr << exc.what() <<endl;
            return -1;
        }
    }
    else
    {
        cerr << "Chain file cannot be opened" << endl;
        return -1;
    }
    fchain.close();
    
    vector<string> start_words;
    ifstream fstartwords;
    try
    {
        fstartwords.open(start_words_filename, ios::in);
    }
    catch(const runtime_error &exc)
    {
        cerr << "Start words file cannot be opened" << exc.what() <<endl;
        return -1;
    }

    if(fstartwords.is_open())
    {
        string line;
        while(getline(fstartwords, line))
        {
            line = Utils::preprocess(line);
            stringstream ss(line);
            string word;
            while(getline(ss, word, ' '))
            {
                word.erase(remove_if(word.begin(), word.end(), ::isspace), word.end());
                if(!word.empty())
                    start_words.push_back(word);
            }
        }
        if(start_words.empty())
        {
            cerr << "No start words were read" << endl;
            return -1;
        }
    }
    else
    {
        cerr << "Start words file cannot be opened" << endl;
        return -1;
    }
    fstartwords.close();
    
    //generate to cout
    Generator g;
    try
    {
        g.run(chain, start_words, need_words, cout);
    }
    catch(const runtime_error &exc)
    {
        cerr << exc.what() << endl;
        return -1;
    }

    return 0;
}






