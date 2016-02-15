#ifndef _MARK_CHAIN_
#define _MARK_CHAIN_

#include <sstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <algorithm>


class RandomInititalizer
{
public:
    RandomInititalizer()
    {
        std::srand(std::time(0));
    }
};

//key is a string with n (chain order) words separated with comma: "word,word,word..."
class MarkovChain
{
public:
    
    typedef unsigned short ChainOrderType;
    
    explicit MarkovChain(ChainOrderType chainOrder = 1)
        :m_order(chainOrder)
    {}
    
    ChainOrderType getN() const
    {
        return m_order;
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
        sout << m_order << std::endl;
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
            
            ChainOrderType chainOrder = -1;
            try
            {
                chainOrder = static_cast<unsigned short>(std::stoi(line));
            }
            catch(...)
            {
                throw std::runtime_error("Chain order should be a number in range [1.." + std::to_string(maxChainOrder()) + "]");
            }
            
            if( !(chainOrder >= 1 && chainOrder <= maxChainOrder()) )
            {
                throw std::runtime_error("Chain order should be a number in range [1.." + std::to_string(maxChainOrder()) + "]");
            }
            
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
            m_order = chainOrder;
            
        } catch (const std::exception &exc)
        {
            throw std::runtime_error(std::string("Chain cannot be deserialized: ") + exc.what());
        }
    }

    static ChainOrderType maxChainOrder()
    {
        return MaxChainOrder;
    }
    
private:
    
    ChainOrderType m_order;
    
    std::unordered_map<std::string, std::vector<std::string>> m_chain;
    
    static const ChainOrderType MaxChainOrder = 100;
};



#endif //_MARK_CHAIN_
