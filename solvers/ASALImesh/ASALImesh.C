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

#include "readFromFile.h"
#include "readOptions.h"
#include "write.h"
#include "layout.h"
#include "functions.h"


int main( int argc, char** argv )
{
    ASALI::logo();
    
    #include "memoryAllocation.H"

    //- Check and read mesh file
    {
        meshFile.checkFile("mesh/mesh.asali");
        meshFile.setFile("mesh/mesh.asali");
    }

    //- Check and read options file
    {
        optionFile.read();
    }

    //- Points
    {
        double dim = ASALI::conversionToMeter(meshFile.lookUpString("Unit dimensions"));
        dict.clear();
        dict = meshFile.lookUpDictionary("Discretization");
        ASALI::checkDict(dict,3,"Discretization");
        Nx   = boost::lexical_cast<unsigned int>(dict[0]);
        Ny   = boost::lexical_cast<unsigned int>(dict[1]);
        Nz   = boost::lexical_cast<unsigned int>(dict[2]);

        dict = meshFile.lookUpDictionary("Points");
        ASALI::checkDict(dict,6,"Points");

        xMesh.resize(Nx);
        {
            xMesh[0]    = boost::lexical_cast<double>(dict[0])*dim;
            xMesh[Nx-1] = boost::lexical_cast<double>(dict[3])*dim;
            
            double d    = (xMesh[Nx-1]-xMesh[0])/double(Nx - 1);
            
            for (unsigned int i=1;i<(Nx-1);i++)
            {
                xMesh[i] = xMesh[i-1] + d;
            }
        }

        yMesh.resize(Ny);
        {
            yMesh[0]    = boost::lexical_cast<double>(dict[1])*dim;
            yMesh[Ny-1] = boost::lexical_cast<double>(dict[4])*dim;
            
            double d    = (yMesh[Ny-1]-yMesh[0])/double(Ny - 1);
            
            for (unsigned int i=1;i<(Ny-1);i++)
            {
                yMesh[i] = yMesh[i-1] + d;
            }
        }

        zMesh.resize(Nz);
        {
            zMesh[0]    = boost::lexical_cast<double>(dict[2])*dim;
            zMesh[Nz-1] = boost::lexical_cast<double>(dict[5])*dim;
            
            double d    = (zMesh[Nz-1]-zMesh[0])/double(Nz - 1);
            
            for (unsigned int i=1;i<(Nz-1);i++)
            {
                zMesh[i] = zMesh[i-1] + d;
            }
        }
    }
    
    //- Boundaries
    {
        bMesh.resize(6);
        dict.clear();

        dict = meshFile.lookUpDictionary("Boundaries");
        ASALI::checkDict(dict,12,"Boundaries");
        std::vector<std::string> boundaries(6);
        {
            boundaries[0] = meshFile.lookUpString("up");
            boundaries[1] = meshFile.lookUpString("down");
            boundaries[2] = meshFile.lookUpString("front");
            boundaries[3] = meshFile.lookUpString("back");
            boundaries[4] = meshFile.lookUpString("left");
            boundaries[5] = meshFile.lookUpString("right");
        }

        std::vector<std::string> names(6);
        {
            for (unsigned int b=0;b<6;b++)
            {
                names[b] = boundaries[b];
            }

            std::sort( names.begin(), names.end() );
            names.erase( std::unique( names.begin(), names.end() ), names.end() );
        }

        for (unsigned int b=0;b<6;b++)
        {
            for (unsigned int j=0;j<names.size();j++)
            {
                if ( boundaries[b] == names[j] )
                {
                    bMesh[b].first  = boundaries[b];
                    bMesh[b].second = j;
                }
            }
        }
    }

    //- Write
    {
        writeOnFile.setWritePrecision(optionFile.getWritePrecision());
        writeOnFile.writeMeshBoundaries(bMesh);
        writeOnFile.writeMeshPoints(xMesh,yMesh,zMesh);
    }

    ASALI::end();

    return 0;
}
