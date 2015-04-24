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

#include "NoteTracker.h"
#include "essentiamath.h"


using namespace std;
using namespace essentia;
using namespace standard;

const char* NoteTracker::name = "NoteTracker";
const char* NoteTracker::description = DOC("This algorithm extracts notes out of a pitch track and an RMS track. It's a wrapper arount the MONO-NOTE code included with the pYin source. It works with an HMM whose parameters are hard-coded.");

void NoteTracker::configure() {
    m_outputUnvoiced = parameter("outputUnvoiced").toBool();
}

void NoteTracker::compute() {
    
    vector<Real> pitchTrack = _pitchTrack.get();
    vector<Real> rmsTrack = _rmsTrack.get();
    
    int numberOfFrames = pitchTrack.size();
    if (numberOfFrames < 1){
        throw EssentiaException("NoteTracker: Cannot compute note detection on empty pitch track.");
    }
    
    // output vectors
    std::vector<Real>& noteFrequencies = _noteFrequencies.get();
    std::vector<Real>& noteOnsets = _noteOnsets.get();
    std::vector<Real>& noteDurations = _noteDurations.get();
    

    // COMPUTATION HERE

    
    // prepare output
    
}
