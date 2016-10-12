#Compulsory libraries
export EIGEN=$HOME/NumericalLibraries/Eigen/eigen-3.2.4
export BOOST=$HOME/NumericalLibraries/Boost/boost_1_57_0/build
export RAPIDXML=$HOME/NumericalLibraries/Rapidxml/rapidxml-1.13 
export OPENSMOKE=$HOME/NumericalLibraries/OpenSMOKEpp/source
export CANTERA=$HOME/NumericalLibraries/cantera/include/cantera
export SUNDIALS=$HOME/NumericalLibraries/Sundials/sundials-2.7.0/build

#External support options for MKL
export SUPPORT='-DOPENSMOKE_USE_MKL=0'
export ASALI_SUPPORT='-DASALI_USE_BZZ=0'

#MLK library options
export MKL_INCLUDE=$HOME
export MKL_LIBS_PATH=$HOME
export MKL_LIBS=

#BzzMath options
export BZZ_INCLUDE=$HOME
export BZZ_LIBS_PATH=$HOME
export BZZ_LIBS=
