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

#include "pitchtracker.h"
#include "essentiamath.h"


using namespace std;
using namespace essentia;
using namespace standard;

const char* PitchTracker::name = "PitchTracker";
const char* PitchTracker::description = DOC("This algorithm creates a pitch track from a set of F0 candidates arrays, using an HMM. It is a wrapper for stage 2 of the pyin algorithm. \n\n See PYIN : A FUNDAMENTAL FREQUENCY ESTIMATOR USING PROBABILISTIC THRESHOLD DISTRIBUTIONS, by Mauch Matthias and Dixon Simon");

void PitchTracker::configure() {
    m_outputUnvoiced = parameter("outputUnvoiced").toBool();
}

void PitchTracker::compute() {
    
    vector<vector<Real> > f0candidatesFreq = _f0candidatesFreq.get();
    vector<vector<Real> > f0candidatesProb = _f0candidatesProb.get();
    
    int numberOfFrames = f0candidatesFreq.size();
    if (numberOfFrames < 1){
        throw EssentiaException("PitchTracker: Cannot compute pitch detection on empty pitch candidates matrix.");
    }
    
    // output vector
    std::vector<Real>& pitchTrack = _pitchTrack.get();
    
    // Reshape input vectors to comply with the input format needed by the HMM.
    // TODO: it's probably a lot more efficient not to reshape, and to adapt the HMM decoding flow instead.
    // but right now we're only evaluating the overall method viability. Let's optimize only if we indeed get good results.
    vector<pair<double, double> > tempPitchProb;
    for (int i = 0; i < numberOfFrames; i++) {
        int numberOfCandidates = f0candidatesFreq[i].size();
        tempPitchProb.clear();
        for (int j = 0; j < numberOfCandidates; j++){
            tempPitchProb.push_back(pair<double, double> (f0candidatesFreq[i][j], f0candidatesProb[i][j]));
        }
        m_pitchProb.push_back(tempPitchProb);
    }
    
    // do the actual pitch tracking
    vector<float> mpOut = _pitchHmm.process(m_pitchProb);
    
    // prepare output
    pitchTrack.clear();
    pitchTrack.resize(mpOut.size());
    for (int iFrame = 0; iFrame < mpOut.size(); ++iFrame){
        if (mpOut[iFrame] < 0 && (!m_outputUnvoiced)) continue;
        if (m_outputUnvoiced)
        {
            pitchTrack[iFrame]=(fabs(mpOut[iFrame]));
        } else {
            pitchTrack[iFrame]=(mpOut[iFrame]);
        }
    }
    
}
