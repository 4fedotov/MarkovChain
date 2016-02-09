#ifndef _MARK_CHAIN_
#define _MARK_CHAIN_

#include <iostream>
#include <string>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <sstream>

using namespace std;

class RandomInititalizer
{
public:
    RandomInititalizer()
    {
        std::srand(std::time(0));
    }
};

//key is a string with n words separated with comma: "word,word,word..."
class MarkovChain
{
public:
    MarkovChain()
    :MarkovChain(1)
    {}
    
    MarkovChain(unsigned short n)
    :m_n(n)
    {}
    
    unsigned short getN() const
    {
        return m_n;
    }
    
    void add(const string &key, const string &word)
    {
        m_chain[key].push_back(word);
    }
    
    string getOneRandom(const string &key) const
    {
        static RandomInititalizer ri;
        
        string result;
        
        auto it = m_chain.find(key);
        if(it != m_chain.end())
        {
            const vector<string> &nextwords = it->second;
            result = nextwords[rand() % nextwords.size()];
        }
        return result;
    }
    
    ostream &serialize(ostream &sout) const
    {
        sout << m_n << endl;
        for(auto p: m_chain)
        {
            sout << p.first;
            for(string s: p.second)
            {
                sout << " " << s;
            }
            sout << endl;
        }
        
        return sout;
    }
    
    void deserialize(istream &sin)
    {
        try
        {
            unordered_map<string, vector<string>> chain;
            string line;
            std::getline(sin, line);
            int n = stoi(line);
            while(std::getline(sin, line))
            {
                stringstream ss(line);
                vector<string> tokens;
                copy(istream_iterator<string>(ss),
                     istream_iterator<string>(),
                     back_inserter(tokens));
                chain[tokens[0]] = vector<string>(tokens.begin() + 1, tokens.end());
            }
            swap(chain, m_chain);
            m_n = n;
        } catch (...)
        {
            throw runtime_error("Chain file cannot be serialized");
        }
    }
    
    void printchain()
    {
        for(auto it = m_chain.begin(); it != m_chain.end(); ++it)
        {
            cout << "[" + it->first << "]: {";
            for(string s: it->second)
            {
                cout << s << " ";
            }
            cout << "}" << endl;
        }
    }

    
private:
    
    unsigned short m_n;
    unordered_map<string, vector<string>> m_chain;
};



#endif //_MARK_CHAIN_
