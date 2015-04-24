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
#include "MonoNote.h"

using namespace std;
using namespace essentia;
using namespace standard;

const char* NoteTracker::name = "NoteTracker";
const char* NoteTracker::description = DOC("This algorithm extracts notes out of a pitch track and an RMS track. It's a wrapper arount the MONO-NOTE code included with the pYin source. It works with an HMM whose parameters are hard-coded.");

void NoteTracker::configure() {

    _pruneThresh = 0.1f;
    _hopSize = parameter("hopSize").toInt();
    _sampleRate = parameter("sampleRate").toReal();
    _onsetSensitivity = 0.7f;
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
    
    // MONO-NOTE STUFF
    // NB: We're replacing mpOut by the pitchTrack values. It shouldn't make a difference.
    // We may re-examine if outputs look bad.

    MonoNote _monoNote;
    std::vector<std::vector<std::pair<double, double> > > smoothedPitch;
    for (int iFrame = 0; iFrame < pitchTrack.size(); ++iFrame) {
        std::vector<std::pair<double, double> > temp;
        
        if (pitchTrack[iFrame] > 0)
        {
            double tempPitch = 12 * std::log(pitchTrack[iFrame]/440)/std::log(2.) + 69;
            temp.push_back(std::pair<double,double>(tempPitch, .9));
        }
        smoothedPitch.push_back(temp);
    }
    // vector<MonoNote::FrameOutput> _monoNoteOut = _monoNote.process(m_pitchProb);
    vector<MonoNote::FrameOutput> _monoNoteOut = _monoNote.process(smoothedPitch);
    
    // Note feature

    int onsetFrame = 0;
    bool isVoiced = 0;
    bool oldIsVoiced = 0;
    int nFrame = pitchTrack.size();
    
    float minNoteFrames = (_sampleRate * _pruneThresh) / _hopSize;
    
    std::vector<float> notePitchTrack; // collects pitches for one note at a time
    for (int iFrame = 0; iFrame < nFrame; ++iFrame)
    {
        isVoiced = _monoNoteOut[iFrame].noteState < 3
        && smoothedPitch[iFrame].size() > 0
        && (iFrame >= nFrame-2
            || ((rmsTrack[iFrame]/rmsTrack[iFrame+2]) > _onsetSensitivity));
        // std::cerr << rmsTrack[iFrame]/rmsTrack[iFrame-1] << " " << isVoiced << std::endl;
        if (isVoiced && iFrame != nFrame-1)
        {
            if (oldIsVoiced == 0) // beginning of a note
            {
                onsetFrame = iFrame;
            }
            float pitch = smoothedPitch[iFrame][0].first;
            notePitchTrack.push_back(pitch); // add to the note's pitch track
        } else { // not currently voiced
            if (oldIsVoiced == 1) // end of note
            {
                // std::cerr << notePitchTrack.size() << " " << minNoteFrames << std::endl;
                if (notePitchTrack.size() >= minNoteFrames)
                {
                    std::sort(notePitchTrack.begin(), notePitchTrack.end());
                    float medianPitch = notePitchTrack[notePitchTrack.size()/2];
                    float medianFreq = std::pow(2,(medianPitch - 69) / 12) * 440;
                    noteFrequencies.push_back(medianFreq);
                    noteOnsets.push_back(onsetFrame);
                    noteDurations.push_back(iFrame - onsetFrame);
                }
                notePitchTrack.clear();
            }
        }
        oldIsVoiced = isVoiced;
    }
    
}
