/*##############################################################################################
#                                                                                              #
#     #############       #############       #############       ####                ####     #
#    #             #     #             #     #             #     #    #              #    #    #
#    #    #####    #     #    #########      #    #####    #     #    #              #    #    #
#    #    #   #    #     #    #              #    #   #    #     #    #              #    #    #
#    #    #####    #     #    #              #    #####    #     #    #              #    #    #
#    #             #     #    #########      #             #     #    #              #    #    #
#    #             #     #             #     #             #     #    #              #    #    #
#    #    #####    #      #########    #     #    #####    #     #    #              #    #    #
#    #    #   #    #              #    #     #    #   #    #     #    #              #    #    #
#    #    #   #    #      #########    #     #    #   #    #     #    #########      #    #    #
#    #    #   #    #     #             #     #    #   #    #     #             #     #    #    #
#     ####     ####       #############       ####     ####       #############       ####     #
#                                                                                              #
#   Department of Energy                                                                       #
#   Politecnico di Milano                                                                      #
#   Author: Stefano Rebughini <stefano.rebughini@polimi.it>                                    #
#                                                                                              #
################################################################################################
#                                                                                              #
#   License                                                                                    #
#                                                                                              #
#   This file is part of ASALI.                                                                #
#                                                                                              #
#   ASALI is free software: you can redistribute it and/or modify                              #
#   it under the terms of the GNU General Public License as published by                       #
#   the Free Software Foundation, either version 3 of the License, or                          #
#   (at your option) any later version.                                                        #
#                                                                                              #
#   ASALI is distributed in the hope that it will be useful,                                   #
#   but WITHOUT ANY WARRANTY; without even the implied warranty of                             #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                              #
#   GNU General Public License for more details.                                               #
#                                                                                              #
#   You should have received a copy of the GNU General Public License                          #
#   along with ASALI. If not, see <http://www.gnu.org/licenses/>.                              #
#                                                                                              #
##############################################################################################*/

#include <string>
#include <iostream>
#include <math.h>
#include <ctime>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

namespace ASALI
{
    class readFromFile 
    {
        public:

            readFromFile();

            void setFile(const std::string file);
            void checkFile(const std::string file);
            

            double                             lookUpDouble(const std::string keyWord);

            double                             lookUpScalar(const std::string keyWord);

            int                                lookUpInteger(const std::string keyWord);

            unsigned int                       lookUpUnsignedInteger(const std::string keyWord);

            std::string                        lookUpString(const std::string keyWord);

            std::vector<std::string>           lookUpDictionary(const std::string keyWord);

            std::vector<std::vector<double> >  readMatrixFile(const std::string name, const int dim);


            std::vector<double>                lookUp3DVector(const std::string keyWord);
            std::vector<double>                readVectorFile(const std::string name, const int dim);

            bool                     searchForString(std::string keyWord);

            std::vector<std::string> getAllFile()                                            {return inputFile_;};

        private:

            std::vector<std::string> inputFile_;
    };
    
    readFromFile::readFromFile()
    {
    }

    void readFromFile::checkFile(const std::string file)
    {
        if ( !boost::filesystem::exists(file) )
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> cannot find " << file << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void readFromFile::setFile(const std::string file)
    {
        std::string dummyString;
        inputFile_.clear();
        const char *path = file.c_str();
        std::ifstream is(path);
        unsigned int k = 0;
        while (getline(is,dummyString))
        {
            while (getline(is,dummyString))
            {
                dummyString.erase(std::remove(dummyString.begin(), dummyString.end(),')'), dummyString.end());
                dummyString.erase(std::remove(dummyString.begin(), dummyString.end(),'('), dummyString.end());
                std::istringstream iss(dummyString);
                while (iss >> dummyString)
                {
                    k++;
                    inputFile_.resize(k);
                    inputFile_[k-1] = dummyString;
                }
            }
        }
    }
    
    std::vector<double> readFromFile::readVectorFile(const std::string name, const int dim)
    {
        std::vector<double> v(dim);
        std::ifstream input;
        std::string line;
        input.open(name,std::ios::in);

        int counter = 0;
        for (int i=0;i<(dim+2);i++)
        {
            std::getline(input, line);
            std::istringstream iss(line); 

            if ( i > 1 )
            {
                iss >> v[i-2];
            }
        }

        input.close();
        
        return v;
    }

    std::vector<std::vector<double> > readFromFile::readMatrixFile(const std::string name, const int dim)
    {
        std::vector<std::vector<double> > v(dim);
        std::ifstream input;
        std::string line;
        input.open(name,std::ios::in);

        int counter = 0;
        for (int i=0;i<(dim+2);i++)
        {
            std::getline(input, line);
            std::istringstream iss(line); 

            if ( i > 1 )
            {
                v[i-2].resize(3);
                iss >> v[i-2][0] >> v[i-2][1] >> v[i-2][2];
            }
        }

        input.close();
        
        return v;
    }


    std::vector<double> readFromFile::lookUp3DVector(const std::string keyWord)
    {
        std::vector<double> v(3);
        bool                check = false;
        
        std::istringstream iss(keyWord);
        std::vector<std::string> word{std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{}};
        
        std::string input;
        std::string output;

        unsigned int index = 0;

        for (unsigned int i=0;i<inputFile_.size();i++)
        {
            if ( inputFile_[i] == word[0])
            {
                index = i;
                for ( unsigned int k=0;k<word.size();k++)
                {
                    input  = input  + inputFile_[i+k];
                    output = output + word[k];
                }
                break;
            }
        }
        
        if ( input == output && output != "" )
        {
            v[0] = boost::lexical_cast<double>(inputFile_[index + word.size() + 1]);
            v[1] = boost::lexical_cast<double>(inputFile_[index + word.size() + 2]);
            v[2] = boost::lexical_cast<double>(inputFile_[index + word.size() + 3]);
            check = true;
        }

        if ( check = false )
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> cannot find word || " << keyWord << " || " << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }

        return v;
    }

    double readFromFile::lookUpDouble(const std::string keyWord)
    {
        double value = 0.;
        bool   check = false;

        std::istringstream iss(keyWord);
        std::vector<std::string> word{std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{}};

        std::string input;
        std::string output;

        unsigned int index = 0;

        for (unsigned int i=0;i<inputFile_.size();i++)
        {
            if ( inputFile_[i] == word[0])
            {
                index = i;
                for ( unsigned int k=0;k<word.size();k++)
                {
                    input  = input  + inputFile_[i+k];
                    output = output + word[k];
                }
                break;
            }
        }

        if ( input == output && output != "" )
        {
            value = boost::lexical_cast<double>(inputFile_[index + word.size()]);
            check = true;
        }

        if ( check = false )
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> cannot find word || " << keyWord << " || " << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }

        return value;
    }

    double readFromFile::lookUpScalar(const std::string keyWord)
    {
        double value = 0.;
        bool   check = false;

        std::istringstream iss(keyWord);
        std::vector<std::string> word{std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{}};

        std::string input;
        std::string output;

        unsigned int index = 0;

        for (unsigned int i=0;i<inputFile_.size();i++)
        {
            if ( inputFile_[i] == word[0])
            {
                index = i;
                for ( unsigned int k=0;k<word.size();k++)
                {
                    input  = input  + inputFile_[i+k];
                    output = output + word[k];
                }
                break;
            }
        }

        if ( input == output && output != "" )
        {
            value = boost::lexical_cast<double>(inputFile_[index + word.size() + 1]);
            check = true;
        }

        if ( check = false )
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> cannot find word || " << keyWord << " || " << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }

        return value;
    }

    int readFromFile::lookUpInteger(const std::string keyWord)
    {
        int  value = 0;
        bool check = false;


        std::istringstream iss(keyWord);
        std::vector<std::string> word{std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{}};

        std::string input;
        std::string output;

        unsigned int index = 0;

        for (unsigned int i=0;i<inputFile_.size();i++)
        {
            if ( inputFile_[i] == word[0])
            {
                index = i;
                for ( unsigned int k=0;k<word.size();k++)
                {
                    input  = input  + inputFile_[i+k];
                    output = output + word[k];
                }
                break;
            }
        }

        if ( input == output && output != "" )
        {
            value = boost::lexical_cast<int>(inputFile_[index + word.size()]);
            check = true;
        }

        if ( check = false )
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> cannot find word || " << keyWord << " || " << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }

        return value;
    }

    unsigned int readFromFile::lookUpUnsignedInteger(const std::string keyWord)
    {
        unsigned int  value = 0;
        bool          check = false;


        std::istringstream iss(keyWord);
        std::vector<std::string> word{std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{}};

        std::string input;
        std::string output;

        unsigned int index = 0;

        for (unsigned int i=0;i<inputFile_.size();i++)
        {
            if ( inputFile_[i] == word[0])
            {
                index = i;
                for ( unsigned int k=0;k<word.size();k++)
                {
                    input  = input  + inputFile_[i+k];
                    output = output + word[k];
                }
                break;
            }
        }

        if ( input == output && output != "" )
        {
            value = boost::lexical_cast<unsigned int>(inputFile_[index + word.size()]);
            check = true;
        }

        if ( check = false )
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> cannot find word || " << keyWord << " || " << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }

        return value;
    }

    std::string readFromFile::lookUpString(const std::string keyWord)
    {
        std::string value;
        bool        check = false;

        std::istringstream iss(keyWord);
        std::vector<std::string> word{std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{}};

        std::string input;
        std::string output;

        unsigned int index = 0;

        for (unsigned int i=0;i<inputFile_.size();i++)
        {
            if ( inputFile_[i] == word[0])
            {
                index = i;
                for ( unsigned int k=0;k<word.size();k++)
                {
                    input  = input  + inputFile_[i+k];
                    output = output + word[k];
                }
                break;
            }
        }

        if ( input == output && output != "" )
        {
            value =inputFile_[index + word.size()];
            check = true;
        }

        if ( check == false )
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> cannot find word || " << keyWord << " || " << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }

        return value;
    }
    
    bool readFromFile::searchForString(const std::string keyWord)
    {
        bool check = false;

        std::istringstream iss(keyWord);
        std::vector<std::string> word{std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{}};

        std::string input;
        std::string output;

        unsigned int index = 0;

        for (unsigned int i=0;i<inputFile_.size();i++)
        {
            if ( inputFile_[i] == word[0])
            {
                index = i;
                for ( unsigned int k=0;k<word.size();k++)
                {
                    input  = input  + inputFile_[i+k];
                    output = output + word[k];
                }
                break;
            }
        }

        if ( input == output && output != "" )
        {
            check = true;
        }

        return check;
    }

    std::vector<std::string> readFromFile::lookUpDictionary(const std::string keyWord)
    {
        std::vector<std::string> value;
        bool        check = false;

        std::istringstream iss(keyWord);
        std::vector<std::string> word{std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{}};

        std::string input;
        std::string output;

        unsigned int index = 0;

        for (unsigned int i=0;i<inputFile_.size();i++)
        {
            if ( inputFile_[i] == word[0])
            {
                index = i;
                for ( unsigned int k=0;k<word.size();k++)
                {
                    input  = input  + inputFile_[i+k];
                    output = output + word[k];
                }
                break;
            }
        }

        if ( input == output )
        {
            index = index + word.size() + 1;
            check = true;
        }

        if ( check = false )
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> cannot find dictionary || " << keyWord << " || " << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }
        else
        {
            check = false;
        }

        unsigned int k = 0;
        for (unsigned int i=index;i<inputFile_.size();i++)
        {
            if ( inputFile_[i] == "}" )
            {
                check = true;
                break;
            }
            value.resize(k+1);
            value[k] = inputFile_[i];
            k++;
        }

        if ( check = false )
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> cannot find dictionary || " << keyWord << " || " << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }

        return value;
    }

}
