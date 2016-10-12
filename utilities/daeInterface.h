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
#include <sundials/sundials_dense.h>
#include <sundials/sundials_types.h>

namespace ASALI
{
    #define Ith(v,i)    NV_Ith_S(v,i-1)

    class daeInterface 
    {
        public:

            daeInterface();
            
            int solve(const double tf, std::vector<double>& yf);
            
            void setEquations(ASALI::system* eq);
            
            void setInitialConditions(double t0, std::vector<double> y0);
            
            void setAlgebraic(const std::vector<bool> algebraic);
            
            void setTollerance(const double absTol, const double relTol);
            
            void setBandDimensions(const double upperBand, const double lowerBand);

            ~daeInterface(void);

        private:

            void *ida_mem_;

            int NEQ_;

            double relTol_;
            double absTol_;
            double upperBand_;
            double lowerBand_;
            double t0_;

            std::vector<bool>  algebraic_;

            N_Vector yIDA_;
            N_Vector dyIDA_;
            N_Vector y0IDA_;
            N_Vector dy0IDA_;
            N_Vector algebraicIDA_;

            ASALI::system* eq_;

            int checkFlag(void *flagvalue, const char *funcname, int opt);

    };
    
    daeInterface::daeInterface()
    {
        yIDA_         = NULL;
        dyIDA_        = NULL;
        y0IDA_        = NULL;
        dy0IDA_       = NULL;
        algebraicIDA_ = NULL;
        ida_mem_      = NULL;

        upperBand_    = 0.;
        lowerBand_    = 0.;
        relTol_       = 1.e-07;
        absTol_       = 1.e-12;
        t0_           = 0.;
    }

    void daeInterface::setEquations(ASALI::system* eq)
    {
        eq_  = eq;
        NEQ_ = eq_->numberOfEquations();

        y0IDA_ = N_VNew_Serial(NEQ_);
        if (checkFlag((void *)y0IDA_, "N_VNew_Serial", 0))        exit(-1);

        dy0IDA_ = N_VNew_Serial(NEQ_);
        if (checkFlag((void *)dy0IDA_, "N_VNew_Serial", 0))       exit(-1);

        yIDA_ = N_VNew_Serial(NEQ_);
        if (checkFlag((void *)yIDA_, "N_VNew_Serial", 0))         exit(-1);

        dyIDA_ = N_VNew_Serial(NEQ_);
        if (checkFlag((void *)dyIDA_, "N_VNew_Serial", 0))        exit(-1);

        algebraicIDA_ = N_VNew_Serial(NEQ_);
        if (checkFlag((void *)algebraicIDA_, "N_VNew_Serial", 0)) exit(-1);

        algebraic_.resize(NEQ_);
    }
    
    void daeInterface::setBandDimensions(const double upperBand, const double lowerBand)
    {
        upperBand_ = upperBand;
        lowerBand_ = lowerBand;
    }
    
    void daeInterface::setTollerance(const double absTol, const double relTol)
    {
        absTol_ = absTol;
        relTol_ = relTol;
    }
    
    void daeInterface::setAlgebraic(const std::vector<bool> algebraic)
    {
        for (int i=0;i<NEQ_;i++)
        {
            NV_Ith_S(algebraicIDA_, i) = (algebraic[i] == true) ? 0.0 : 1.0;
            algebraic_[i] = algebraic[i];
        }
    }

    void daeInterface::setInitialConditions(double t0, std::vector<double> y0)
    {
        std::vector<double> dy0(NEQ_);

        int flag = eq_->getSystem(t0,y0,dy0);

        t0_ = t0;

        for (unsigned int i=0;i<NEQ_;i++)
        {
            NV_Ith_S(dy0IDA_, i) = dy0[i];
            NV_Ith_S( y0IDA_, i) =  y0[i];
        }
    }

    static int equations(double t, N_Vector y, N_Vector dy, N_Vector f, void *user_data)
    {
        ASALI::system *data;
        data = (ASALI::system*)user_data;

        unsigned int N = data->numberOfEquations();

        double *ydata  = N_VGetArrayPointer_Serial(y);
        double *dydata = N_VGetArrayPointer_Serial(dy);
        double *fdata  = N_VGetArrayPointer_Serial(f);

        std::vector<double> y_(N);
        std::vector<double> dy_(N);

        for (unsigned int i=0;i<N;i++)
        {
            y_[i] = ydata[i];
        }

        int flag = data->getSystem(t,y_,dy_);

        for (unsigned int i=0;i<N;i++)
        {
            fdata[i] = dy_[i];
        }

        for(unsigned int i=0;i<N;i++)
        {
            if (data->getAlgebraic()[i] == false)
            {
                fdata[i] -= dydata[i];
            }
        }

        return(flag);
    }

    int daeInterface::solve(const double tf, std::vector<double>& yf)
    {
        int flag;

        ida_mem_ = IDACreate();
        if (checkFlag((void *)ida_mem_, "IDACreate", 0)) exit(-1);

        flag = IDASetMaxNumSteps(ida_mem_, 5000000);
        if (checkFlag(&flag, "IDASetMaxNumSteps", 1)) exit(-1);

        flag = IDASetUserData(ida_mem_, eq_);
        if(checkFlag(&flag, "IDASetUserData", 1))exit(-1);

        flag = IDAInit(ida_mem_, equations, t0_, y0IDA_, dy0IDA_);
        if (checkFlag(&flag, "IDAInit", 1)) exit(-1);

        flag = IDASStolerances(ida_mem_, relTol_, absTol_);
        if (checkFlag(&flag, "IDASVtolerances", 1)) exit(-1);

        flag = IDASetId(ida_mem_, algebraicIDA_);
        if (checkFlag(&flag, "IDASetId", 1)) exit(-1);

        if (upperBand_ == 0 && lowerBand_ == 0)
        {
            flag = IDADense(ida_mem_, NEQ_);
            if (checkFlag(&flag, "IDADense", 1)) exit(-1);
        }
        else
        {
            flag = IDABand(ida_mem_, NEQ_, upperBand_, lowerBand_);
            if (checkFlag(&flag, "IDABand", 1)) exit(-1);
        }

        flag = IDASolve(ida_mem_,tf, &t0_, yIDA_, dyIDA_, IDA_NORMAL);

        yf.clear();
        yf.resize(NEQ_);
        double *sol = N_VGetArrayPointer_Serial(yIDA_);
        for (unsigned int i=0;i<NEQ_;i++)
        {
            yf[i] = sol[i];
        }
    }

    int daeInterface::checkFlag(void *flagvalue, const char *funcname, int opt)
    {
        /* 
         * Check function return value...
         *   opt == 0 means SUNDIALS function allocates memory so check if
         *            returned NULL pointer
         *   opt == 1 means SUNDIALS function returns a flag so check if
         *            flag >= 0
         *   opt == 2 means function allocates memory so check if returned
         *            NULL pointer 
         */

        int *errflag;

        /* Check if SUNDIALS function returned NULL pointer - no memory allocated */
        if (opt == 0 && flagvalue == NULL)
        {
            fprintf(stderr, "\nSUNDIALS_ERROR: %s() failed - returned NULL pointer\n\n", funcname);
            return(1);
        }
        else if (opt == 1)
        {
            /* Check if flag < 0 */
            errflag = (int *) flagvalue;
            if (*errflag < 0)
            {
                fprintf(stderr,"\nSUNDIALS_ERROR: %s() failed with flag = %d\n\n", funcname, *errflag);
                return(1); 
            }
        }
        else if (opt == 2 && flagvalue == NULL)
        {
            /* Check if function returned NULL pointer - no memory allocated */
            fprintf(stderr, "\nMEMORY_ERROR: %s() failed - returned NULL pointer\n\n", funcname);
            return(1);
        }

        return(0);
    }

    daeInterface::~daeInterface(void)
    {
        N_VDestroy_Serial(y0IDA_);
        N_VDestroy_Serial(dy0IDA_);
        N_VDestroy_Serial(yIDA_);
        N_VDestroy_Serial(dyIDA_);
        N_VDestroy_Serial(algebraicIDA_);
        IDAFree(&ida_mem_);
    }

}
