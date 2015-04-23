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

#ifndef ESSENTIA_PITCHTRACKER_H
#define ESSENTIA_PITCHTRACKER_H

#include "algorithmfactory.h"
#include "MonoPitch.h"

namespace essentia {
    namespace standard {
        
        class PitchTracker : public Algorithm {
            
        private:
            Input<std::vector<std::vector<Real> > > _f0candidatesFreq;
            Input<std::vector<std::vector<Real> > > _f0candidatesProb;
            Output<std::vector<Real> > _pitchTrack;
            
            std::vector<std::vector<std::pair<double, double> > > m_pitchProb;
            MonoPitch _pitchHmm;
            bool m_outputUnvoiced;
            
        public:
            PitchTracker(){
                declareInput(_f0candidatesFreq, "freqs", "the input F0 candidates frequencies");
                declareInput(_f0candidatesProb, "probs", "the input F0 candidates probabilities");
                declareOutput(_pitchTrack, "pitchTrack", "pitch track");
            }
            
            ~PitchTracker() {
            };
            
            void declareParameters() {
                declareParameter("outputUnvoiced", "output unvoiced frames as smoothed, or set to 0", "{false,true}", true);
            }
            
            void configure();
            void compute();
            
            static const char* name;
            static const char* description;
            
        }; // class PitchTracker
        
    } // namespace standard
} // namespace essentia


#include "streamingalgorithmwrapper.h"

namespace essentia {
    namespace streaming {
        
        class PitchTracker : public StreamingAlgorithmWrapper {
            
        protected:
            Sink<std::vector<std::vector<Real> > > _f0candidatesFreq;
            Sink<std::vector<std::vector<Real> > > _f0candidatesProb;
            Source<std::vector<Real> > _pitchTrack;
            
        public:
            PitchTracker() {
                declareAlgorithm("PitchTracker");
                declareInput(_f0candidatesFreq, TOKEN, "freqs");
                declareInput(_f0candidatesProb, TOKEN, "probs");
                declareOutput(_pitchTrack, TOKEN, "pitchTrack");
                declareParameter("outputUnvoiced", "output unvoiced frames as smoothed, or set to 0", "{false,true}", true);
            }
        };
        
    } // namespace streaming
} // namespace essentia

#endif // ESSENTIA_PITCHTRACKER_H
