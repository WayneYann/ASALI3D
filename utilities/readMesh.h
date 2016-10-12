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
    class readMesh 
    {
        public:

            readMesh();

            inline unsigned int              getXnumberOfPoints() {return Nx_;};
            inline unsigned int              getYnumberOfPoints() {return Ny_;};
            inline unsigned int              getZnumberOfPoints() {return Nz_;};
            
            inline std::vector<double>       getXpoints()         {return xMesh_;};
            inline std::vector<double>       getYpoints()         {return yMesh_;};
            inline std::vector<double>       getZpoints()         {return zMesh_;};
            
            inline std::vector<std::string>  getBoundariesName()  {return bName_;};
            
            inline std::vector<unsigned int> getBoundariesIndex() {return bNumber_;};
            
            void read();

        private:

            unsigned int Nx_;
            unsigned int Ny_;
            unsigned int Nz_;
            
            ASALI::readFromFile file_;
            
            std::vector<unsigned int> bNumber_;

            std::vector<std::string>  bName_;
            
            std::vector<double>       xMesh_;
            std::vector<double>       yMesh_;
            std::vector<double>       zMesh_;

    };
    
    readMesh::readMesh()
    {
    }

    void readMesh::read()
    {
        std::cout << "\nReading info.asali in mesh folder" << std::endl;
        //- Read info
        {
            file_.checkFile("mesh/info.asali");
            file_.setFile("mesh/info.asali");

            Nx_ = boost::lexical_cast<unsigned int>(file_.getAllFile()[0]);
            Ny_ = boost::lexical_cast<unsigned int>(file_.getAllFile()[1]);
            Nz_ = boost::lexical_cast<unsigned int>(file_.getAllFile()[2]);

            xMesh_.resize(Nx_);
            yMesh_.resize(Ny_);
            zMesh_.resize(Nz_);
        }

        std::cout << "\nReading points.x.asali in mesh folder" << std::endl;
        //- Read points
        {
            file_.checkFile("mesh/points.x.asali");
            file_.setFile("mesh/points.x.asali");
            
            for (unsigned int x=0;x<Nx_;x++)
            {
                xMesh_[x] = boost::lexical_cast<double>(file_.getAllFile()[x]);
            }
        }

        std::cout << "\nReading points.y.asali in mesh folder" << std::endl;
        //- Read points
        {
            file_.checkFile("mesh/points.y.asali");
            file_.setFile("mesh/points.y.asali");
            
            for (unsigned int y=0;y<Ny_;y++)
            {
                yMesh_[y] = boost::lexical_cast<double>(file_.getAllFile()[y]);
            }
        }
        
        std::cout << "\nReading points.z.asali in mesh folder" << std::endl;
        //- Read points
        {
            file_.checkFile("mesh/points.z.asali");
            file_.setFile("mesh/points.z.asali");
            
            for (unsigned int z=0;z<Nz_;z++)
            {
                zMesh_[z] = boost::lexical_cast<double>(file_.getAllFile()[z]);
            }
        }

        std::cout << "\nReading boundaries.asali in mesh folder" << std::endl;
        //- Read boundaries
        {
            file_.checkFile("mesh/boundaries.asali");
            file_.setFile("mesh/boundaries.asali");
            bNumber_.resize(6);
            bName_.resize(6);
            unsigned int counter = 0;
            for (unsigned int b=2;b<file_.getAllFile().size();b = b + 3)
            {
                bNumber_[counter] = boost::lexical_cast<unsigned int>(file_.getAllFile()[b]);
                bName_[counter]   = file_.getAllFile()[b-1];
                counter++;
            }
        }
    }
}
