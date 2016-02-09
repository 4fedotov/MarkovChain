
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

class Learner
{
public:
    Learner()
    :Learner(1)
    {}
    Learner(unsigned short n)
    :m_chain(n)
    {
        if(n == 0)
        {
            throw runtime_error("Chain order can not be zero");
        }
        if(n > MaxChainOrder)
        {
            throw runtime_error("Chain order should be not more than " + to_string(MaxChainOrder));
        }
    }
    
    void accumulate(const string &str)
    {
        stringstream buf(str);
        
        for(string token; getline(buf, token, ' '); )
        {
            token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
            
            if(token.empty())
                continue;
            
            //cout << "push token \"" << token << "\"" << endl;
            m_accum.push_back(token);
            
            if(m_accum.size() == (m_chain.getN() + 1))
            {
                string key_string;
                stringstream ss;
                copy(m_accum.begin(), m_accum.begin() + m_chain.getN() - 1, ostream_iterator<string>(ss, ","));
                key_string = ss.str();
                key_string += *(m_accum.begin() + m_chain.getN() - 1);
                
                //cout << "add to chain: " << key_string << " " << m_accum.back() << endl;
                m_chain.add(key_string, m_accum.back());
                m_accum.pop_front();
            }
        }
        
    }
    
    void store(ostream &out)
    {
        getChain().serialize(out);
    }
    
    MarkovChain &getChain()
    {
        return m_chain;
    }
    
private:
    string m_buffer;
    deque<string> m_accum;

    MarkovChain m_chain;
    
    const unsigned short MaxChainOrder = 100;
};

void learn(const vector<string> &urls, unsigned short chain_order)
{
    Learner l(chain_order);
    
    if(urls.empty())
    {
        throw runtime_error("Url list is empty");
    }
    
    for(string url: urls)
    {
        FILE *pipe = popen((string("curl -s ") + url).c_str(), "r");
        if (pipe == NULL)
        {
            throw runtime_error("Couldn't open pipe");
        }
        char output[10000]; //todo check overwriting
        while (fgets(output, sizeof(output), pipe) != NULL)
        {
            l.accumulate(Utils::preprocess(output));
        }
        pclose(pipe);
    }

    l.store(cout);
}


int main()
{
    //input:
    //chain_order
    //url1
    //url2
    //...
/*
    string s;
    cin >> s;
    
    s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
    s.erase(std::remove_if(s.begin(), s.end(), ::ispunct), s.end());

    
    cout << icu::UnicodeString(s.c_str()).toLower() << endl;
    
    return 0;*/
    
    unsigned short chain_order = 0;
    cin >> chain_order;
    
    vector<string> urls;
    std::copy(std::istream_iterator<string>(cin),
              std::istream_iterator<string>(),
              std::back_inserter(urls));
    
    try
    {
        learn(urls, chain_order);
    }
    catch(const exception &exc)
    {
        cerr << "Error while learn: " << exc.what() << endl;
        return -1;
    }
    
    return 0;
}

