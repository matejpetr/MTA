#include "Senzor_Encoder.hpp"

bool Rencoder::init() {
  if (!_enc) _enc = new ::Encoder(_pinA, _pinB);
  return true;
}

void Rencoder::reset() {
  if (_enc) { _enc->write(0); _position = 0; }
}


std::vector<KV> Rencoder::update() {
  std::vector<KV> kv;
  if (!_enc) return kv;

  long raw = _enc->read();
  // stejné škálování jako dřív: read()/2 * direction_factor
  _position = (raw / 2L) * _dir;

  String alarm = "OK";
  if (_position < _lLimit)      alarm = "LOW";
  else if (_position > _hLimit) alarm = "HIGH";

  kv.push_back({"position", String(_position)});
  kv.push_back({"alarm",    alarm});
  return kv;
}
