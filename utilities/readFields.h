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
#include <iomanip>
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
    class readFields 
    {
        public:

            readFields();

            void setDimensions(const int Nx, const int Ny, const int Nz);

            void setBoundaries(const std::vector<std::string> boundaries);
            
            void setPoints(const std::vector<double> xMesh, const std::vector<double> yMesh, const std::vector<double> zMesh);
            
            void setWritePrecision(const int WP); 
            
            void setTimeFolder(const double folder);

            std::vector<std::vector<double> > readVectorField(std::string name);

            std::vector<double>               readScalarField(std::string name);

            std::vector<double>               magnitudine(std::vector<std::vector<double> > v);

        private:
        
            int Nx_;
            int Ny_;
            int Nz_;
            int Ntot_;
            int WP_;

            std::vector<double> xMesh_;
            std::vector<double> yMesh_;
            std::vector<double> zMesh_;

            std::vector<std::string> boundaries_;
            std::vector<std::string> dict_;

            ASALI::readFromFile fieldFile_;
            
            std::vector<std::vector<double> > fieldVector_;
            
            std::vector<double>               fieldScalar_;
            
            std::string                       timeFolder_;
            
            
            double                            magVector(const std::vector<double> v);
    };
    
    readFields::readFields()
    {
    }

    double readFields::magVector(const std::vector<double> v)
    {
        double sum = 0.;

        for (unsigned int i=0;i<v.size();i++)
        {
            sum = sum + v[i]*v[i];
        }

        sum = std::sqrt(sum);

        return sum;
    }

    void readFields::setWritePrecision(const int WP)
    {
        WP_ = WP;
    }

    void readFields::setDimensions(const int Nx, const int Ny, const int Nz)
    {
        Nx_   = Nx;
        Ny_   = Ny;
        Nz_   = Nz;
        Ntot_ = Nx_*Ny_*Nz_;
    }

    void readFields::setBoundaries(const std::vector<std::string> boundaries)
    {
        boundaries_.resize(boundaries.size());
        boundaries_ = boundaries;
    }
    
    std::vector<double> readFields::magnitudine(std::vector<std::vector<double> > v)
    {
        std::vector<double> mag(v.size());
        for (unsigned int i=0;i<v.size();i++)
        {
            mag[i] = magVector(v[i]);
        }
        return mag;
    }
    
    void readFields::setTimeFolder(const double folder)
    {
        if ( folder == 0. )
        {
            timeFolder_ = boost::lexical_cast<std::string>(folder);
        }
        else
        {
            std::stringstream stream;
            stream << std::fixed << std::setprecision(WP_) << folder;

            timeFolder_ = stream.str();

            if ( !boost::filesystem::exists(timeFolder_) )
            {
                boost::filesystem::create_directory(timeFolder_);
            }
        }
    }

    void readFields::setPoints(const std::vector<double> xMesh, const std::vector<double> yMesh, const std::vector<double> zMesh)
    {
        xMesh_.resize(xMesh.size());
        yMesh_.resize(yMesh.size());
        zMesh_.resize(zMesh.size());

        xMesh_ = xMesh;
        yMesh_ = yMesh;
        zMesh_ = zMesh;
    }

    std::vector<std::vector<double> > readFields::readVectorField(std::string name)
    {
        std::vector<std::vector<double> > field3D(Ntot_);
        std::cout << "\nReading " << name << ".asali from time " << timeFolder_ << std::endl;
        std::string fileName = timeFolder_ + "/" + name + ".asali";
        fieldFile_.checkFile(fileName);
        fieldFile_.setFile(fileName);

        if ( timeFolder_ == "0" )
        {
            if ( fieldFile_.searchForString("#output format") == false )
            {
                dict_.clear();
                fieldVector_.clear();

                fieldFile_.checkFile(fileName);
                fieldFile_.setFile(fileName);

                dict_ = fieldFile_.lookUpDictionary("Boundaries");
                fieldVector_.resize(7);

                fieldVector_[6] = fieldFile_.lookUp3DVector("Internal values");

                std::string type;

                for (int i=0;i<6;i++)
                {
                    type = fieldFile_.lookUpString(boundaries_[i]);
                    if ( type == "value" )
                    {
                        fieldVector_[i] = fieldFile_.lookUp3DVector(boundaries_[i]);
                    }
                    else
                    {
                        fieldVector_[i].resize(3);
                        for (int j=0;j<3;j++)
                        {
                            fieldVector_[i][j] = fieldVector_[6][j];
                        }
                    }
                }

                int counter = 0;
                for(int x=0;x<Nx_;x++)
                {
                    for (int y=0;y<Ny_;y++)
                    {
                        for (int z=0;z<Nz_;z++)
                        {
                            field3D[counter].resize(3);
                            if ( xMesh_[x] == xMesh_[0] )
                            {
                                field3D[counter] = fieldVector_[5];
                            }
                            else if ( xMesh_[x] == xMesh_[Nx_-1] )
                            {
                                field3D[counter] = fieldVector_[4];
                            }
                            else if ( yMesh_[y] == yMesh_[0] )
                            {
                                field3D[counter] = fieldVector_[1];
                            }
                            else if ( yMesh_[y] == yMesh_[Ny_-1] )
                            {
                                field3D[counter] = fieldVector_[0];
                            }
                            else if ( zMesh_[z] == zMesh_[0] )
                            {
                                field3D[counter] = fieldVector_[3];
                            }
                            else if ( zMesh_[z] == zMesh_[Nz_ - 1] )
                            {
                                field3D[counter] = fieldVector_[2];
                            }
                            else
                            {
                                field3D[counter] = fieldVector_[6];
                            }
                            counter++;
                        }
                    }
                }

                return field3D;
            }
            else
            {
                field3D = fieldFile_.readMatrixFile(fileName,Ntot_);
                return field3D;
            }
        }
        else
        {
            field3D = fieldFile_.readMatrixFile(fileName,Ntot_);
            return field3D;
        }
    }

    std::vector<double> readFields::readScalarField(std::string name)
    {
        std::vector<double> field1D(Ntot_);
        std::cout << "\nReading " << name << ".asali from time " << timeFolder_ << std::endl;
        std::string fileName = timeFolder_ + "/" + name + ".asali";
        fieldFile_.checkFile(fileName);
        fieldFile_.setFile(fileName);

        if ( timeFolder_ == "0" )
        {
            if ( fieldFile_.searchForString("#output format") == false )
            {
                dict_.clear();
                fieldScalar_.clear();

                dict_ = fieldFile_.lookUpDictionary("Boundaries");

                fieldScalar_.resize(7);
                fieldScalar_[6] = fieldFile_.lookUpScalar("Internal values");

                std::string type;

                for (int i=0;i<6;i++)
                {
                    type = fieldFile_.lookUpString(boundaries_[i]);
                    if ( type == "value" )
                    {
                        fieldScalar_[i] = fieldFile_.lookUpScalar(boundaries_[i]);
                    }
                    else
                    {
                        fieldScalar_[i] = fieldScalar_[6];
                    }
                }

                int counter = 0;
                for(int x=0;x<Nx_;x++)
                {
                    for (int y=0;y<Ny_;y++)
                    {
                        for (int z=0;z<Nz_;z++)
                        {
                            if ( xMesh_[x] == xMesh_[0] )
                            {
                                field1D[counter] = fieldScalar_[5];
                            }
                            else if ( xMesh_[x] == xMesh_[Nx_-1] )
                            {
                                field1D[counter] = fieldScalar_[4];
                            }
                            else if ( yMesh_[y] == yMesh_[0] )
                            {
                                field1D[counter] = fieldScalar_[1];
                            }
                            else if ( yMesh_[y] == yMesh_[Ny_-1] )
                            {
                                field1D[counter] = fieldScalar_[0];
                            }
                            else if ( zMesh_[z] == zMesh_[0] )
                            {
                                field1D[counter] = fieldScalar_[3];
                            }
                            else if ( zMesh_[z] == zMesh_[Nz_ - 1] )
                            {
                                field1D[counter] = fieldScalar_[2];
                            }
                            else
                            {
                                field1D[counter] = fieldScalar_[6];
                            }
                            counter++;
                        }
                    }
                }

                return field1D;
            }
            else
            {
                field1D = fieldFile_.readVectorFile(fileName,Ntot_);
                return field1D;
            }
        }
        else
        {
            field1D = fieldFile_.readVectorFile(fileName,Ntot_);
            return field1D;
        }
    }

}
