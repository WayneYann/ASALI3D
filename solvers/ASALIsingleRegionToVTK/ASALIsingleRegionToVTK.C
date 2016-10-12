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

#include "functions.h"
#include "layout.h"
#include "readFromFile.h"
#include "readFields.h"
#include "readOptions.h"
#include "readMesh.h"
#include "write.h"


int main( int argc, char** argv )
{
    ASALI::logo();

    //- Check options
    std::string timeFolder;
    {
        std::vector<std::string> input(argc-1);
        for (unsigned int i=0;i<(argc-1);i++)
        {
            input[i] = std::string(argv[i+1]);
        }

        timeFolder = ASALI::lookUpTime(input);
    }

    #include "mesh.H"
    #include "options.H"
    #include "createFields.H"

    //- Set field
    {
        fieldFile.setWritePrecision(WP);
        fieldFile.setDimensions(Nx,Ny,Nz);
        fieldFile.setBoundaries(bMeshName);
        fieldFile.setPoints(xMesh,yMesh,zMesh);
        fieldFile.setTimeFolder(boost::lexical_cast<double>(timeFolder));
    }

    //- Set writing options
    {
        writeOnFile.setDimensions(Nx,Ny,Nz);
        writeOnFile.setPoints(xMesh,yMesh,zMesh);
        writeOnFile.setBoundaries(bMeshIndex);
        writeOnFile.setWritePrecision(WP);
        writeOnFile.setTimeFolder(boost::lexical_cast<double>(timeFolder),"none");
    }

    //- Read scalar fields
    {
        p = fieldFile.readScalarField("p");
        T = fieldFile.readScalarField("T");
    }

    //- Read vector field
    {
        U    = fieldFile.readVectorField("U");
        magU = fieldFile.magnitudine(U);
    }

    //- Read temperature
    {
        writeOnFile.writeVTKfield("p",p);
        writeOnFile.writeVTKfield("T",T);
        writeOnFile.writeVTKfield("magU",magU);
    }


    ASALI::end();

    return 0;
}
