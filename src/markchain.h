#ifndef _MARK_CHAIN_
#define _MARK_CHAIN_

#include <iostream>
#include <string>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <sstream>


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
    
    void add(const std::string &key, const std::string &word)
    {
        m_chain[key].push_back(word);
    }
    
    std::string getOneRandom(const std::string &key) const
    {
        static RandomInititalizer ri;
        
        std::string result;
        
        auto it = m_chain.find(key);
        if(it != m_chain.end())
        {
            const std::vector<std::string> &nextwords = it->second;
            result = nextwords[rand() % nextwords.size()];
        }
        return result;
    }
    
    std::ostream &serialize(std::ostream &sout) const
    {
        sout << m_n << std::endl;
        for(auto p: m_chain)
        {
            sout << p.first;
            for(std::string s: p.second)
            {
                sout << " " << s;
            }
            sout << std::endl;
        }
        
        return sout;
    }
    
    void deserialize(std::istream &sin)
    {
        try
        {
            std::unordered_map<std::string, std::vector<std::string>> chain;
            std::string line;
            std::getline(sin, line);
            int n = std::stoi(line);
            while(std::getline(sin, line))
            {
                std::stringstream ss(line);
                std::vector<std::string> tokens;
                std::copy(std::istream_iterator<std::string>(ss),
                     std::istream_iterator<std::string>(),
                     std::back_inserter(tokens));
                chain[tokens[0]] = std::vector<std::string>(tokens.begin() + 1, tokens.end());
            }
            std::swap(chain, m_chain);
            m_n = n;
        } catch (...)
        {
            throw std::runtime_error("Chain file cannot be serialized");
        }
    }
    
    void printchain()
    {
        for(auto it = m_chain.begin(); it != m_chain.end(); ++it)
        {
            std::cout << "[" + it->first << "]: {";
            for(std::string s: it->second)
            {
                std::cout << s << " ";
            }
            std::cout << "}" << std::endl;
        }
    }

    
private:
    
    unsigned short m_n;
    std::unordered_map<std::string, std::vector<std::string>> m_chain;
};



#endif //_MARK_CHAIN_
