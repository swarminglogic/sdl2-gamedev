#ifndef AUDIO_AUDIOPLAYBACK_H
#define AUDIO_AUDIOPLAYBACK_H

#include <audio/MusicPlayer.h>
#include <util/Log.h>


/**
 * AudioPlayback class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class AudioPlayback
{
public:
  static MusicPlayer& music();

  // TODO swarminglogic, 2014-02-08:
  // static SoundManager& sound();

private:
  AudioPlayback();
  virtual ~AudioPlayback();

  Log log_;

  // NonCopyable
  AudioPlayback(const AudioPlayback& c);
  AudioPlayback& operator=(const AudioPlayback& c);
};

#endif
