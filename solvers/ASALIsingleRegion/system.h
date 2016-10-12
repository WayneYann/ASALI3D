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

#include <ida/ida.h>
#include <ida/ida_dense.h>
#include <ida/ida_band.h>
#include <nvector/nvector_serial.h>
#include <sundials/sundials_math.h>
#include <sundials/sundials_dense.h>    /* definitions DlsMat DENSE_ELEM */
#include <sundials/sundials_types.h>

namespace ASALI
{
    class system 
    {
        public:

            system(const int NEQ);
            
            int getSystem(double& t, std::vector<double>& y, std::vector<double>& dy);

            inline std::vector<bool> getAlgebraic() const {return A_;}
            
            inline unsigned int numberOfEquations() const {return NEQ_;};
            
            void algebraic();

        private:

            std::vector<bool> A_;

            int NEQ_;

    };
    
    system::system(const int NEQ):
    NEQ_(NEQ)
    {
    }
    
    int system::getSystem(double& t, std::vector<double>& y, std::vector<double>& dy)
    {
        dy[0] = -0.04*y[0] + 1.0e4*y[1]*y[2];
        dy[1] = -(-0.04*y[0] + 1.0e4*y[1]*y[2]) - 3.0e7*y[1]*y[1];
        dy[2] = y[0] + y[1] + y[2] - 1.;

        return 0;
    }

    void system::algebraic()
    {
        A_.clear();
        A_.resize(NEQ_);

        A_[0] = false;
        A_[1] = false;
        A_[2] = true;
    }
}
