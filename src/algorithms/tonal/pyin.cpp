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

using namespace std;
using namespace essentia;
using namespace standard;


const char* Pyin::name = "Pyin";
const char* Pyin::description = DOC("This algorithm estimates the fundamental frequency from a given spectrum. It is an implementation of the Yin algorithm [1] for computations in the time domain.\n"
"\n"
"An exception is thrown if an empty signal is provided.\n"
"\n"
"Please note that if \"pitchConfidence\" is zero, \"pitch\" is undefined and should not be used for other algorithms.\n"
"Also note that a null \"pitch\" is never ouput by the algorithm and that \"pitchConfidence\" must always be checked out.\n"
"\n"
"References:\n"
"  [1] De Cheveigné, A., & Kawahara, H. \"YIN, a fundamental frequency estimator\n"
"  for speech and music. The Journal of the Acoustical Society of America,\n"
"  111(4), 1917-1930, 2002.\n\n"
"  [2] Pitch detection algorithm - Wikipedia, the free encyclopedia\n"
"  http://en.wikipedia.org/wiki/Pitch_detection_algorithm");

void Pyin::configure() {
  _frameSize = parameter("frameSize").toInt();
  _sampleRate = parameter("sampleRate").toReal();
  _interpolate = parameter("interpolate").toBool();

  _yin.resize(_frameSize/2+1);

  _tauMax = min(int(ceil(_sampleRate / parameter("minFrequency").toReal())), _frameSize/2);
  _tauMin = min(int(floor(_sampleRate / parameter("maxFrequency").toReal())), _frameSize/2);

  if (_tauMax <= _tauMin) {
    throw EssentiaException("Pyin: maxFrequency is lower than minFrequency, or they are too close, or they are out of the interval of detectable frequencies with respect to the specified frameSize.");
  }
}

void Pyin::compute() {
  const vector<Real>& signal = _signal.get();
  if (signal.empty()) {
    throw EssentiaException("Pyin: Cannot compute pitch detection on empty signal frame.");
  }
  std::vector<Real>& f0candidatesFreq = _f0candidatesFreq.get();
  f0candidatesFreq.resize(3);
  std::vector<Real>& f0candidatesProb = _f0candidatesProb.get();
  f0candidatesProb.resize(3);

    f0candidatesFreq[0] = 220.f;
    f0candidatesFreq[1] = 440.f;
    f0candidatesFreq[2] = 660.f;
    
    f0candidatesProb[0] = 0.2f;
    f0candidatesProb[1] = 0.3f;
    f0candidatesProb[2] = 0.5f;
}
