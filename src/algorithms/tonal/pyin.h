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

#ifndef ESSENTIA_PYIN_H
#define ESSENTIA_PYIN_H

#include "algorithmfactory.h"

#include "Yin.h"

namespace essentia {
namespace standard {

class Pyin : public Algorithm {

 private:
  Input<std::vector<Real> > _signal;
  Output<std::vector<Real> > _f0candidatesFreq;
  Output<std::vector<Real> > _f0candidatesProb;

  int _frameSize;
  Real _sampleRate;
  Real _tuningFrequency;
  float m_lowAmp;
  Yin *m_yin;

    
 public:
    Pyin(){
    declareInput(_signal, "signal", "the input signal frame");
    declareOutput(_f0candidatesFreq, "f0candidatesFreq", "candidate F0 frequencies [midicent]");
    declareOutput(_f0candidatesProb, "f0candidatesProb", "candidate F0 probablities [0,1]");
  }

  ~Pyin() {
  };

  void declareParameters() {
    declareParameter("frameSize", "number of samples in the input frame", "[2,inf)", 2048);
    declareParameter("sampleRate", "sampling rate of the input spectrum [Hz]", "(0,inf)", 44100.);
    declareParameter("tuningFrequency", "tuning frequency for Hz to midicents conversion", "(400,500)", 440.);
  }

  void configure();
  void compute();

  static const char* name;
  static const char* description;

}; // class Pyin

} // namespace standard
} // namespace essentia


#include "streamingalgorithmwrapper.h"

namespace essentia {
namespace streaming {

class Pyin : public StreamingAlgorithmWrapper {

 protected:
  Sink<std::vector<Real> > _signal;
  Source<std::vector<Real> > _f0candidatesFreq;
  Source<std::vector<Real> > _f0candidatesProb;

 public:
  Pyin() {
    declareAlgorithm("Pyin");
    declareInput(_signal, TOKEN, "signal");
    declareOutput(_f0candidatesFreq, TOKEN, "f0candidatesFreq");
    declareOutput(_f0candidatesProb, TOKEN, "f0candidatesProb");
  }
};

} // namespace streaming
} // namespace essentia

#endif // ESSENTIA_PYIN_H
