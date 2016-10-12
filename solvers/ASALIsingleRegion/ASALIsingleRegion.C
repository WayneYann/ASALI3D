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
#include "system.h"
#include "daeInterface.h"
#include "layout.h"
#include "readFromFile.h"
#include "readFields.h"
#include "readOptions.h"
#include "readMesh.h"
#include "write.h"


int main( int argc, char** argv )
{
    ASALI::logo();

    #include "mesh.H"
    #include "options.H"
    #include "createFields.H"

    //- Set field
    {
        fieldFile.setWritePrecision(WP);
        fieldFile.setDimensions(Nx,Ny,Nz);
        fieldFile.setBoundaries(bMeshName);
        fieldFile.setPoints(xMesh,yMesh,zMesh);
        fieldFile.setTimeFolder(folder);
    }

    //- Read pressure
    {
        p = fieldFile.readScalarField("p");
    }

    //- Read velocity
    {
        U = fieldFile.readVectorField("U");
    }

    //- Read temperature
    {
        T = fieldFile.readScalarField("T");
    }
    
    //- Change Dimensions
    {
        rho.resize(Ntot);
        MWmix.resize(Ntot);
    }

    //- set equations
    ASALI::system       *eqn;
    ASALI::daeInterface *dae;
    ASALI::write         writeOnFile;
    {
        eqn = new ASALI::system(Ntot);
        dae = new ASALI::daeInterface();

        dae->setEquations(eqn);
        dae->setAlgebraic(eqn->getAlgebraic());
        dae->setTollerance(1.e-12,1.e-09);
        dae->setBandDimensions(2.,2.);

        writeOnFile.setDimensions(Nx,Ny,Nz);
        writeOnFile.setPoints(xMesh,yMesh,zMesh);
        writeOnFile.setBoundaries(bMeshIndex);
        writeOnFile.setWritePrecision(WP);
    }
    
    unsigned int counter = 0;
    double       t0      = folder;
    double       tf      = t0;
    while ( (ttot - t0) > dt )
    {
        #include "properties.H"
        tf = t0 + dt;

        std::cout << "\nTime:" << t0 << " --> " << tf << std::endl;

        //y0 = yf;
        //dae->setInitialConditions(t0,y0);
        //dae->solve(tf,yf);

        if (counter == Ws || (ttot - tf) < dt )
        {
            writeOnFile.setTimeFolder(tf,"make");
            writeOnFile.writeField("T",T);
            writeOnFile.writeField("p",p);
            writeOnFile.writeField("U",U);
            counter = 0;
        }
        else
        {
            counter++;
        }

        t0 = tf;
    }
    
    delete dae;
    delete eqn;

    ASALI::end();

    return 0;
}
