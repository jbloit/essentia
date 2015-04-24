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

#ifndef ESSENTIA_NOTETRACKER_H
#define ESSENTIA_NOTETRACKER_H

#include "algorithmfactory.h"
#include "MonoPitch.h"

namespace essentia {
    namespace standard {
        
        class NoteTracker : public Algorithm {
            
        private:
            Input<std::vector<Real> > _pitchTrack;
            Input<std::vector<Real> > _rmsTrack;
            Output<std::vector<Real> > _noteFrequencies;
            Output<std::vector<Real> > _noteOnsets;
            Output<std::vector<Real> > _noteDurations;
            
            std::vector<std::vector<std::pair<double, double> > > m_pitchProb;
            MonoPitch _pitchHmm;
            bool m_outputUnvoiced;
            
        public:
            NoteTracker(){
                declareInput(_pitchTrack, "pitchTrack", "the input pitch track");
                declareInput(_rmsTrack, "rmsTrack", "the input rms track");
                declareOutput(_noteFrequencies, "noteFrequencies", "notes frequency array");
                declareOutput(_noteOnsets, "noteOnsets", "notes onset array");
                declareOutput(_noteDurations, "noteDurations", "notes duration array");
            }
            
            ~NoteTracker() {
            };
            
            void declareParameters() {
            }
            
            void configure();
            void compute();
            
            static const char* name;
            static const char* description;
            
        }; // class NoteTracker
        
    } // namespace standard
} // namespace essentia


#include "streamingalgorithmwrapper.h"

namespace essentia {
    namespace streaming {
        
        class NoteTracker : public StreamingAlgorithmWrapper {
            
        protected:
            Sink<std::vector<Real> > _pitchTrack;
            Sink<std::vector<Real> > _rmsTrack;

            Source<std::vector<Real> > _noteFrequencies;
            Source<std::vector<Real> > _noteOnsets;
            Source<std::vector<Real> > _noteDurations;
            
        public:
            NoteTracker() {
                declareAlgorithm("NoteTracker");
                declareInput(_pitchTrack, TOKEN, "pitchTrack");
                declareInput(_rmsTrack, TOKEN, "rmsTrack");
                declareOutput(_noteFrequencies, TOKEN, "noteFrequencies");
                declareOutput(_noteOnsets, TOKEN, "noteOnsets");
                declareOutput(_noteDurations, TOKEN, "noteDurations");

            }
        };
        
    } // namespace streaming
} // namespace essentia

#endif // ESSENTIA_NOTETRACKER_H
