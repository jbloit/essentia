/*
 * Copyright (C) 2006-2013  Music Technology Group - Universitat Pompeu Fabra
 *
 * This file is part of Essentia
 *
 * Essentia is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation (FSF), either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * version 3 along with this program.  If not, see http://www.gnu.org/licenses/
 */

#include "Pyin.h"
#include "essentiamath.h"
#include "vamp-sdk/FFT.h"
#include "Yin.h"

using namespace std;
using namespace essentia;
using namespace standard;


const char* Pyin::name = "Pyin";
const char* Pyin::description = DOC("This algorithm computes F0 candidates as for an audio frame. The result is givent as an array of F0 candidates and an array of their respective probabilities. It is a wrapper for stage 1 of the pyin algorithm. \n\n See PYIN : A FUNDAMENTAL FREQUENCY ESTIMATOR USING PROBABILISTIC THRESHOLD DISTRIBUTIONS, by Mauch Matthias and Dixon Simon");

void Pyin::configure() {
  _frameSize = parameter("frameSize").toInt();
  _sampleRate = parameter("sampleRate").toReal();
  _tuningFrequency = parameter("tuningFrequency").toReal();
  m_yin = new Yin(_frameSize, _sampleRate);
}

void Pyin::compute() {
  const vector<Real>& signal = _signal.get();
  if (signal.empty()) {
    throw EssentiaException("Pyin: Cannot compute pitch detection on empty signal frame.");
  }
  std::vector<Real>& f0candidatesFreq = _f0candidatesFreq.get();
  std::vector<Real>& f0candidatesProb = _f0candidatesProb.get();
  
    
    // rms
    float rms = 0;
    
    double *dInputBuffers = new double[int(signal.size())];
    for (int i = 0; i < int(signal.size()); ++i) {
        dInputBuffers[i] = signal[i];
        rms += signal[i] * signal[i];
    }
    rms /= signal.size();
    rms = sqrt(rms);
    bool isLowAmplitude = (rms < m_lowAmp);

    
    Yin::YinOutput yo = m_yin->processProbabilisticYin(dInputBuffers);
    delete [] dInputBuffers;
    
    // F0 candidates
    vector<pair<double, double> > tempPitchProb;
    for (int iCandidate = 0; iCandidate < yo.freqProb.size(); ++iCandidate)
    {
        double tempPitch = 12 * std::log(yo.freqProb[iCandidate].first/_tuningFrequency)/std::log(2.) + 69;
        if (!isLowAmplitude)
        {
            tempPitchProb.push_back(pair<double, double>
                                    (tempPitch, yo.freqProb[iCandidate].second));
        } else {
            float factor = ((rms+0.01*m_lowAmp)/(1.01*m_lowAmp));
            tempPitchProb.push_back(pair<double, double>
                                    (tempPitch, yo.freqProb[iCandidate].second*factor));
        }
    }
    
    // Prepare output vectors
    if (tempPitchProb.size() < 1) {
        throw EssentiaException("Pyin: no f0 candidates found.");
    }
    
    f0candidatesFreq.resize(tempPitchProb.size());
    f0candidatesProb.resize(tempPitchProb.size());

    for (int i = 0; i < tempPitchProb.size(); ++i)
    {
        f0candidatesFreq[i] = tempPitchProb[i].first;
        f0candidatesProb[i] = tempPitchProb[i].second;
    }

}
