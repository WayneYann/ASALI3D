reset
echo '/*##############################################################################################'
echo '#                                                                                              #'
echo '#     #############       #############       #############       ####                ####     #'
echo '#    #             #     #             #     #             #     #    #              #    #    #'
echo '#    #    #####    #     #    #########      #    #####    #     #    #              #    #    #'
echo '#    #    #   #    #     #    #              #    #   #    #     #    #              #    #    #'
echo '#    #    #####    #     #    #              #    #####    #     #    #              #    #    #'
echo '#    #             #     #    #########      #             #     #    #              #    #    #'
echo '#    #             #     #             #     #             #     #    #              #    #    #'
echo '#    #    #####    #      #########    #     #    #####    #     #    #              #    #    #'
echo '#    #    #   #    #              #    #     #    #   #    #     #    #              #    #    #'
echo '#    #    #   #    #      #########    #     #    #   #    #     #    #########      #    #    #'
echo '#    #    #   #    #     #             #     #    #   #    #     #             #     #    #    #'
echo '#     ####     ####       #############       ####     ####       #############       ####     #'
echo '#                                                                                              #'
echo '#   Department of Energy                                                                       #'
echo '#   Politecnico di Milano                                                                      #'
echo '#   Author: Stefano Rebughini <stefano.rebughini@polimi.it>                                    #'
echo '#                                                                                              #'
echo '################################################################################################'
echo '#                                                                                              #'
echo '#   License                                                                                    #'
echo '#                                                                                              #'
echo '#   This file is part of ASALI.                                                                #'
echo '#                                                                                              #'
echo '#   ASALI is free software: you can redistribute it and/or modify                              #'
echo '#   it under the terms of the GNU General Public License as published by                       #'
echo '#   the Free Software Foundation, either version 3 of the License, or                          #'
echo '#   (at your option) any later version.                                                        #'
echo '#                                                                                              #'
echo '#   ASALI is distributed in the hope that it will be useful,                                   #'
echo '#   but WITHOUT ANY WARRANTY; without even the implied warranty of                             #'
echo '#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                              #'
echo '#   GNU General Public License for more details.                                               #'
echo '#                                                                                              #'
echo '#   You should have received a copy of the GNU General Public License                          #'
echo '#   along with ASALI. If not, see <http://www.gnu.org/licenses/>.                              #'
echo '#                                                                                              #'
echo '##############################################################################################*/'
echo ' '
echo ' '

source asali.bash


#g++ myProgram.cpp -o myProgram $(pkg-config --cflags --libs cantera)

export BOOST_LIBS='-Wl,--start-group -Wl,-Bstatic -lboost_program_options -lboost_system -lboost_filesystem -lboost_regex -lboost_date_time -Wl,--end-group -Wl,-Bdynamic'

export SUNDIALS_LIBS='-Wl,--start-group -Wl,-Bstatic -lsundials_ida -lsundials_nvecserial -Wl,--end-group -Wl,-Bdynamic'

mkdir exe

#ASALImesh
rm -f exe/ASALImesh
g++ -Ofast -m64 -std=c++11 -Wno-write-strings -fpermissive solvers/ASALImesh/ASALImesh.C -Iutilities -Iinclude -I$EIGEN -I$BOOST/include/ -I$RAPIDXML -L$BOOST/lib/ -lgfortran -Wl,--no-as-needed -ldl -lpthread -lm $BOOST_LIBS -o exe/ASALImesh

#ASALImeshToVTK
rm -f exe/ASALImeshToVTK
g++ -Ofast -m64 -std=c++11 -Wno-write-strings -fpermissive solvers/ASALImeshToVTK/ASALImeshToVTK.C -Iutilities -Iinclude -I$EIGEN -I$BOOST/include/ -I$RAPIDXML -L$BOOST/lib/ -lgfortran -Wl,--no-as-needed -ldl -lpthread -lm $BOOST_LIBS -o exe/ASALImeshToVTK

#ASALIsingleRegion
rm -f exe/ASALIsingleRegion
g++ -Ofast -m64 -std=c++11 -Wno-write-strings -fpermissive solvers/ASALIsingleRegion/ASALIsingleRegion.C -Iutilities -Iinclude -I$CANTERA -I$EIGEN -I$BOOST/include/ -I$SUNDIALS/include -I$RAPIDXML -L$BOOST/lib/ -L$SUNDIALS/lib -lgfortran -Wl,--no-as-needed -ldl -lpthread -lm $BOOST_LIBS $SUNDIALS_LIBS -o exe/ASALIsingleRegion

#ASALIsingleRegion
rm -f exe/ASALIsingleRegionToVTK
g++ -Ofast -m64 -std=c++11 -Wno-write-strings -fpermissive solvers/ASALIsingleRegionToVTK/ASALIsingleRegionToVTK.C -Iutilities -Iinclude -I$CANTERA -I$EIGEN -I$BOOST/include/ -I$SUNDIALS/include -I$RAPIDXML -L$BOOST/lib/ -L$SUNDIALS/lib -lgfortran -Wl,--no-as-needed -ldl -lpthread -lm $BOOST_LIBS $SUNDIALS_LIBS -o exe/ASALIsingleRegionToVTK

