// 
// *  This file was automatically generated by MoMEMta-MaGMEE,
// *  A MadGraph Matrix Element Exporter plugin for MoMEMta.
// *
// *  It is subject to MoMEMta-MaGMEE's license and copyright:
// *
// *  Copyright (C) 2016  Universite catholique de Louvain (UCL), Belgium
// *
// *  This program is free software: you can redistribute it and/or modify
// *  it under the terms of the GNU General Public License as published by
// *  the Free Software Foundation, either version 3 of the License, or
// *  (at your option) any later version.
// *
// *  This program is distributed in the hope that it will be useful,
// *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// *  GNU General Public License for more details.
// *
// *  You should have received a copy of the GNU General Public License
// *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 

#include <string> 
#include <utility> 
#include <vector> 
#include <map> 

#include <P1_Sigma_modTEFT_H_gg_hh.h> 
#include <HelAmps_modTEFT_H.h> 

#include <SLHAReader.h> 

namespace pp_hh_tree_MV_standalone_modTEFT_H 
{

//==========================================================================
// Class member functions for calculating the matrix elements for
// Process: g g > h h EFT<=1 QCD=0 EFT^2==2 @1

//--------------------------------------------------------------------------

// Initialize process.

P1_Sigma_modTEFT_H_gg_hh::P1_Sigma_modTEFT_H_gg_hh(const std::string&
    param_card)
{

  params.reset(new Parameters_modTEFT_H(SLHA::Reader(param_card))); 

  // Set external particle masses for this matrix element
  mME.push_back(std::ref(params->ZERO)); 
  mME.push_back(std::ref(params->ZERO)); 
  mME.push_back(std::ref(params->mdl_MH)); 
  mME.push_back(std::ref(params->mdl_MH)); 

  mapFinalStates[{25, 25}] = 
  {
    {
      &P1_Sigma_modTEFT_H_gg_hh::matrix_1_gg_hh, 
      false, 
      {
        std::make_pair(21, 21)
      }, 
      4, 
      512
    }
  }; 

}

//--------------------------------------------------------------------------
// Evaluate |M|^2, return a map of final states

std::map < std::pair < int, int > , double >
    P1_Sigma_modTEFT_H_gg_hh::compute(const std::pair < std::vector<double> ,
    std::vector<double> > &initialMomenta, const std::vector < std::pair < int,
    std::vector<double> > > &finalState)
{

  // Set initial particle momenta
  momenta[0] = (double * ) (&initialMomenta.first[0]); 
  momenta[1] = (double * ) (&initialMomenta.second[0]); 

  // Suppose final particles are passed in the "correct" order
  std::vector<int> selectedFinalState(4 - 2); 
  for (size_t index = 0; index < (4 - 2); index++ )
  {
    selectedFinalState[index] = finalState[index].first; 
    momenta[index + 2] = (double * ) (&finalState[index].second[0]); 
  }

  // Set the event specific parameters
  params->updateParameters(); 
  params->updateCouplings(); 

  // Initialise result object
  std::map < std::pair < int, int > , double > result; 

  // Define permutation
  int perm[4]; 
  for(int i = 0; i < 4; i++ )
  {
    perm[i] = i; 
  }

  for(auto &me: mapFinalStates[selectedFinalState])
  {

    double me_sum = 0; 
    double me_mirror_sum = 0; 

    for(int ihel = 0; ihel < 4; ihel++ )
    {

      if(me.goodHel[ihel])
      {

        double sum = 0.; 
        calculate_wavefunctions(perm, helicities[ihel]); 
        double meTemp = me.callback( * this); 
        sum += meTemp; 
        me_sum += meTemp/me.denominator; 

        if(me.hasMirrorProcess)
        {
          perm[0] = 1; 
          perm[1] = 0; 
          // Calculate wavefunctions
          calculate_wavefunctions(perm, helicities[ihel]); 
          // Mirror back
          perm[0] = 0; 
          perm[1] = 1; 
          meTemp = me.callback( * this); 
          sum += meTemp; 
          me_mirror_sum += meTemp/me.denominator; 
        }

        if( !sum)
          me.goodHel[ihel] = false; 
      }
    }

    for (auto const &initialState: me.initialStates)
    {
      result[initialState] = me_sum; 
      if (me.hasMirrorProcess)
        result[std::make_pair(initialState.second, initialState.first)] =
            me_mirror_sum;
    }
  }


  return result; 
}

//==========================================================================
// Private class member functions

//--------------------------------------------------------------------------
// Evaluate |M|^2 for each subprocess

void P1_Sigma_modTEFT_H_gg_hh::calculate_wavefunctions(const int perm[], const
    int hel[])
{
  // Calculate wavefunctions for all processes
  static std::complex<double> w[5][18]; 

  // Calculate all wavefunctions
  vxxxxx(&momenta[perm[0]][0], mME[0], hel[0], -1, w[0]); 
  vxxxxx(&momenta[perm[1]][0], mME[1], hel[1], -1, w[1]); 
  sxxxxx(&momenta[perm[2]][0], +1, w[2]); 
  sxxxxx(&momenta[perm[3]][0], +1, w[3]); 
  VVS20_3(w[0], w[1], params->GC_79, params->mdl_MH, params->mdl_WH, w[4]); 

  // Calculate all amplitudes
  // Amplitude(s) for diagram number 0
  VVSS26_0(w[0], w[1], w[2], w[3], params->GC_76, amp[0]); 
  SSS2_0(w[2], w[3], w[4], params->GC_45, amp[1]); 

}
double P1_Sigma_modTEFT_H_gg_hh::matrix_1_gg_hh() 
{

  static std::complex<double> ztemp; 
  static std::complex<double> jamp[1]; 
  // The color matrix
  static const double denom[1] = {1}; 
  static const double cf[1][1] = {{2}}; 

  // Calculate color flows
  jamp[0] = +2. * (+amp[0] + amp[1]); 

  // Sum and square the color flows to get the matrix element
  double matrix = 0; 
  for(int i = 0; i < 1; i++ )
  {
    ztemp = 0.; 
    for(int j = 0; j < 1; j++ )
      ztemp = ztemp + cf[i][j] * jamp[j]; 
    matrix = matrix + real(ztemp * conj(jamp[i]))/denom[i]; 
  }

  return matrix; 
}



}

