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
    class write 
    {
        public:

            write();

            void setWritePrecision(const unsigned int WP); 
            
            void setTimeFolder(const double folder, const std::string action);

            void setDimensions(const unsigned int Nx, const unsigned int Ny, const unsigned int Nz);

            void setBoundaries(const std::vector<unsigned int> boundaries);
            
            void setPoints(const std::vector<double> xMesh, const std::vector<double> yMesh, const std::vector<double> zMesh);

            void writeField(const std::string name, const std::vector<double> field);
            
            void writeField(const std::string name, const std::vector<std::vector<double> > field);
            
            void writeMeshPoints(const std::vector<double> x, const std::vector<double> y, const std::vector<double> z);
            
            void writeMeshBoundaries(const std::vector<std::pair<std::string,unsigned int> > b);
            
            void writeVTKfield(const std::string name, const std::vector<double> field);
            
            void writeVTKmesh();

        private:
        
            unsigned int WP_ = 6;

            unsigned int Nx_;
            unsigned int Ny_;
            unsigned int Nz_;
            unsigned int Ntot_;

            std::vector<double> xMesh_;
            std::vector<double> yMesh_;
            std::vector<double> zMesh_;
            
            std::vector<unsigned int> bMesh_;

            std::string timeFolder_;

    };
    
    write::write()
    {
    }
    
    void write::setWritePrecision(const unsigned int WP)
    {
        WP_ = WP;
    }

    void write::setDimensions(const unsigned int Nx, const unsigned int Ny, const unsigned int Nz)
    {
        Nx_   = Nx;
        Ny_   = Ny;
        Nz_   = Nz;
        Ntot_ = Nx_*Ny_*Nz_;
    }

    void write::setBoundaries(const std::vector<unsigned int> boundaries)
    {
        bMesh_.resize(boundaries.size());
        bMesh_ = boundaries;
    }

    void write::setTimeFolder(const double folder, const std::string action)
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

            if ( !boost::filesystem::exists(timeFolder_) && action == "make")
            {
                boost::filesystem::create_directory(timeFolder_);
            }
        }
    }

    void write::setPoints(const std::vector<double> xMesh, const std::vector<double> yMesh, const std::vector<double> zMesh)
    {
        xMesh_.resize(xMesh.size());
        yMesh_.resize(yMesh.size());
        zMesh_.resize(zMesh.size());

        xMesh_ = xMesh;
        yMesh_ = yMesh;
        zMesh_ = zMesh;
    }

    void write::writeField(const std::string name, const std::vector<double> field)
    {
        std::string fileName = timeFolder_ + "/" + name + ".asali";
        std::ofstream output;
        output.open(fileName,std::ios::out);
        output.setf(std::ios::scientific);
        output.precision(WP_);

        output << "#ASALI3D " << name << " file" << std::endl;
        output << "#output format" << std::endl;
        for(unsigned int i=0;i<field.size();i++)
        {
            output << field[i] << std::endl;
        }
        output.close();
    }

    void write::writeField(const std::string name, const std::vector<std::vector<double> > field)
    {
        std::string fileName = timeFolder_ + "/" + name + ".asali";
        std::ofstream output;
        output.open(fileName,std::ios::out);
        output.setf(std::ios::scientific);
        output.precision(WP_);

        output << "#ASALI3D " << name << ".x file" << std::endl;
        output << "#output format" << std::endl;
        for(unsigned int i=0;i<field.size();i++)
        {
            for (unsigned int j=0;j<3;j++)
            {
                output << field[i][j] << " ";
            }
            output << std::endl;
        }
        output.close();
    }

    void write::writeVTKmesh()
    {
        std::cout << "\nWriting mesh.vtk in mesh folder" << std::endl;
        std::ofstream output;
        output.open("mesh/mesh.vtk",std::ios::out);
        
        output << "# vtk DataFile Version 3.1" << std::endl;
        output << "This file has been generated by using ASALImeshToVTK" << std::endl;
        output << "ASCII" << std::endl;
        output << "DATASET UNSTRUCTURED_GRID" << std::endl;
        output << "POINTS " << Ntot_ << " FLOAT" << std::endl;
        
        for(unsigned int x=0;x<Nx_;x++)
        {
            for (unsigned int y=0;y<Ny_;y++)
            {
                for (unsigned int z=0;z<Nz_;z++)
                {
                    output << xMesh_[x] << " " << yMesh_[y] << " " << zMesh_[z] << std::endl;
                }
            }
        }
        
        output << "CELLS " << Ntot_ << " " << Ntot_*2 << std::endl;
        unsigned int counter = 0;
        for(unsigned int x=0;x<Nx_;x++)
        {
            for (unsigned int y=0;y<Ny_;y++)
            {
                for (unsigned int z=0;z<Nz_;z++)
                {
                    output << 1 << " " << counter++ << std::endl;
                }
            }
        }

        output << "CELL_TYPES " << Ntot_ << std::endl;
        for(unsigned int x=0;x<Nx_;x++)
        {
            for (unsigned int y=0;y<Ny_;y++)
            {
                for (unsigned int z=0;z<Nz_;z++)
                {
                    output << 1 << " ";
                }
                output << std::endl;
            }
        }
        
        output << "POINT_DATA " << Nx_*Ny_*Nz_ << " SCALARS boundaries INTEGER" << std::endl;
        output << "LOOKUP_TABLE default" << std::endl;
        for(unsigned int x=0;x<Nx_;x++)
        {
            for (unsigned int y=0;y<Ny_;y++)
            {
                for (unsigned int z=0;z<Nz_;z++)
                {
                    if ( xMesh_[x] == xMesh_[0] )
                    {
                        output << bMesh_[5] << std::endl;
                    }
                    else if ( xMesh_[x] == xMesh_[Nx_-1] )
                    {
                        output << bMesh_[4] << std::endl;
                    }
                    else if ( yMesh_[y] == yMesh_[0] )
                    {
                        output << bMesh_[1] << std::endl;
                    }
                    else if ( yMesh_[y] == yMesh_[Ny_-1] )
                    {
                        output << bMesh_[0] << std::endl;
                    }
                    else if ( zMesh_[z] == zMesh_[0] )
                    {
                        output << bMesh_[3] << std::endl;
                    }
                    else if ( zMesh_[z] == zMesh_[Nz_ - 1] )
                    {
                        output << bMesh_[2] << std::endl;
                    }
                    else
                    {
                        output << -1 << std::endl;
                    }
                }
            }
        }
        output.close();
    }

    void write::writeVTKfield(const std::string name, const std::vector<double> field)
    {
        std::cout << "\nWriting " << name << ".vtk in time " << timeFolder_ << std::endl;
        std::ofstream output;
        std::string folder = timeFolder_ + "/" + name + ".vtk";
        output.open(folder,std::ios::out);

        output << "# vtk DataFile Version 3.1" << std::endl;
        output << "This file has been generated by using ASALImeshToVTK" << std::endl;
        output << "ASCII" << std::endl;
        output << "DATASET UNSTRUCTURED_GRID" << std::endl;
        output << "POINTS " << Ntot_ << " FLOAT" << std::endl;
        
        for(unsigned int x=0;x<Nx_;x++)
        {
            for (unsigned int y=0;y<Ny_;y++)
            {
                for (unsigned int z=0;z<Nz_;z++)
                {
                    output << xMesh_[x] << " " << yMesh_[y] << " " << zMesh_[z] << std::endl;
                }
            }
        }
        
        output << "CELLS " << Ntot_ << " " << Ntot_*2 << std::endl;
        unsigned int counter = 0;
        for(unsigned int x=0;x<Nx_;x++)
        {
            for (unsigned int y=0;y<Ny_;y++)
            {
                for (unsigned int z=0;z<Nz_;z++)
                {
                    output << 1 << " " << counter++ << std::endl;
                }
            }
        }

        output << "CELL_TYPES " << Ntot_ << std::endl;
        for(unsigned int x=0;x<Nx_;x++)
        {
            for (unsigned int y=0;y<Ny_;y++)
            {
                for (unsigned int z=0;z<Nz_;z++)
                {
                    output << 1 << " ";
                }
                output << std::endl;
            }
        }

        output << "POINT_DATA " << Ntot_ << " SCALARS " << name << " FLOAT" << std::endl;
        output << "LOOKUP_TABLE default" << std::endl;
        counter = 0;
        for(unsigned int x=0;x<Nx_;x++)
        {
            for (unsigned int y=0;y<Ny_;y++)
            {
                for (unsigned int z=0;z<Nz_;z++)
                {
                    output << field[counter++] << std::endl;
                }
            }
        }
        output.close();
    }

    void write::writeMeshPoints(const std::vector<double> xMesh, const std::vector<double> yMesh, const std::vector<double> zMesh)
    {
        unsigned int Nx = xMesh.size();
        unsigned int Ny = yMesh.size();
        unsigned int Nz = zMesh.size();

        std::cout << "\nWriting points.x.asali in mesh folder" << std::endl;
        //- Write points.x
        {
            std::ofstream output;
            output.open("mesh/points.x.asali",std::ios::out);
            output.setf(std::ios::scientific);
            output.precision(WP_);

            output << "#ASALI3D mesh file" << std::endl;
            for(unsigned int x=0;x<Nx;x++)
            {
                output << xMesh[x] << std::endl;
            }
            output.close();
        }

        std::cout << "\nWriting points.y.asali in mesh folder" << std::endl;
        //- Write points.y
        {
            std::ofstream output;
            output.open("mesh/points.y.asali",std::ios::out);
            output.setf(std::ios::scientific);
            output.precision(WP_);

            output << "#ASALI3D mesh file" << std::endl;
            for(unsigned int y=0;y<Ny;y++)
            {
                output << yMesh[y] << std::endl;
            }
            output.close();
        }
        
        std::cout << "\nWriting points.z.asali in mesh folder" << std::endl;
        //- Write points.z
        {
            std::ofstream output;
            output.open("mesh/points.z.asali",std::ios::out);
            output.setf(std::ios::scientific);
            output.precision(WP_);

            output << "#ASALI3D mesh file" << std::endl;
            for(unsigned int z=0;z<Nz;z++)
            {
                output << zMesh[z] << std::endl;
            }
            output.close();
        }

        //- Write info
        {
            std::ofstream output;
            output.open("mesh/info.asali",std::ios::out);
            output << "#ASALI3D mesh file" << std::endl;
            output << Nx << "\t" << Ny << "\t" << Nz << std::endl;
            output.close();
        }
    }
    
    void write::writeMeshBoundaries(const std::vector<std::pair<std::string,unsigned int> > b)
    {
        std::cout << "\nWriting boundaries.asali in mesh folder" << std::endl;
        std::ofstream output;
        output.open("mesh/boundaries.asali",std::ios::out);
        output << "#ASALI3D mesh file - (up down front back left right)" << std::endl;
        for (unsigned int bi=0;bi<6;bi++)
        {
            output << bi << " " << b[bi].first << "\t" <<  b[bi].second << std::endl;
        }
        output.close();
    }
    
}
