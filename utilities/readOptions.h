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
    class readOptions 
    {
        public:

            readOptions();

            inline double       getTimeStep()        {return dt_;};
            inline double       getIntegrationTime() {return tf_;};
            
            inline unsigned int getWritePrecision()  {return WP_;};
            inline unsigned int getWriteStep()       {return WS_;}; 
            
            inline std::string  getStart()           {return start_;};
            
            void read();
            
            void check();
            
            double        readTimeFolder();


        private:
        
            unsigned int WP_;
            unsigned int WS_;

            double dt_;
            double tf_;
            
            std::string start_;

            ASALI::readFromFile file_;
            
            std::vector<std::vector<double> > fieldVector_;
            
            std::vector<double>               fieldScalar_;
    };
    
    readOptions::readOptions()
    {
        if ( !boost::filesystem::exists("mesh"))
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> || mesh || folder does not exist" << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }
        else
        {
            if ( !boost::filesystem::is_directory("mesh") )
            {
                std::cout << "\nASALI::FatalERROR" << std::endl;
                std::cout << "--> || mesh || folder does not exist" << std::endl;
                std::cout << "\n" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        if ( !boost::filesystem::exists("options"))
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> || options || folder does not exist" << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }
        else
        {
            if ( !boost::filesystem::is_directory("options") )
            {
                std::cout << "\nASALI::FatalERROR" << std::endl;
                std::cout << "--> || options || folder does not exist" << std::endl;
                std::cout << "\n" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        if ( !boost::filesystem::exists("0"))
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> || 0 || folder does not exist" << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }
        else
        {
            if ( !boost::filesystem::is_directory("0") )
            {
                std::cout << "\nASALI::FatalERROR" << std::endl;
                std::cout << "--> || 0 || folder does not exist" << std::endl;
                std::cout << "\n" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    void readOptions::read()
    {
        file_.checkFile("options/write.asali");
        file_.setFile("options/write.asali");

        dt_    = file_.lookUpDouble("TimeStep");
        tf_    = file_.lookUpDouble("IntegrationTime");
        WP_    = file_.lookUpUnsignedInteger("WritePrecision");
        WS_    = file_.lookUpUnsignedInteger("WriteStep");
        start_ = file_.lookUpString("Start");
    }
    
    void readOptions::check()
    {
        if ( start_ != "restart" && start_ != "new" )
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> wrong || Start || option in file options/write.asali (new/restart)" << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        } 
    }
    
    
    double readOptions::readTimeFolder()
    {
        double timeFolder = -1.;
        for(auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator("."), {}))
        {
            std::string dummyString = entry.path().string();
            dummyString.erase(0,2);
            if ( boost::filesystem::is_directory(dummyString) )
            {
                if ( dummyString != "mesh" && dummyString != "options" )
                {
                    timeFolder = std::max(timeFolder,boost::lexical_cast<double>(dummyString));
                }
            }
        }
        return timeFolder;
    }
}
