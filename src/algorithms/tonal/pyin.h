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

namespace essentia {
namespace standard {

class Pyin : public Algorithm {

 private:
  Input<std::vector<Real> > _signal;
  Output<std::vector<Real> > _f0candidatesFreq;
  Output<std::vector<Real> > _f0candidatesProb;


  std::vector<Real> _yin;         // Yin function (cumulative mean normalized difference)
  std::vector<Real> _positions;   // Yin function peak positions
  std::vector<Real> _amplitudes;  // Yin function peak amplitudes

  int _frameSize;
  Real _sampleRate;               
  bool _interpolate;  // whether to use peak interpolation
  int _tauMin;
  int _tauMax;


 public:
  Pyin() {
    declareInput(_signal, "signal", "the input signal frame");
    declareOutput(_f0candidatesFreq, "f0candidatesFreq", "candidate F0 frequencies [Hz]");
    declareOutput(_f0candidatesProb, "f0candidatesProb", "candidate F0 probablities [0,1]");
  }

  ~Pyin() {
  };

  void declareParameters() {
    declareParameter("frameSize", "number of samples in the input frame", "[2,inf)", 2048);
    declareParameter("sampleRate", "sampling rate of the input spectrum [Hz]", "(0,inf)", 44100.);
    declareParameter("minFrequency", "the minimum allowed frequency [Hz]", "(0,inf)", 20.0);
    declareParameter("maxFrequency", "the maximum allowed frequency [Hz]", "(0,inf)", 22050.0);
    declareParameter("interpolate", "enable interpolation", "{true,false}", true);
//    declareParameter("tolerance", "tolerance for peak detection", "[0,1]", 0.15);
    // NOTE: default tolerance value is taken from aubio yin implementation
    // https://github.com/piem/aubio/blob/master/src/pitch/pitchyin.c
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
