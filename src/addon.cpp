#ifdef __GNUC__
#define __cdecl
#endif

#include <kodi/addon-instance/Visualization.h>

#include <vsx_version.h>
#include <vsx_platform.h>
#include <vsx_manager.h>
#include <vsx_gl_state.h>


#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

class CVisualizationVSXU
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstanceVisualization
{
public:
  CVisualizationVSXU() = default;

  virtual ~CVisualizationVSXU()
  {
    // stop vsxu nicely (unloads textures and frees memory)
    if (m_manager)
    {
      m_manager->stop();
      manager_destroy(m_manager);
    }

    m_manager = nullptr;
  }

  virtual bool Start(int channels, int samplesPerSec, int bitsPerSample,
                     std::string songName) override
  {
    std::string path = kodi::GetAddonPath("resources");

    // create a new manager
    m_manager = manager_factory();
    m_manager->set_option_preload_all(false);

    m_manager->init("/usr/share/vsxu", "media_player");
    m_manager->add_visual_path(path.c_str());
    m_presets = m_manager->get_visual_filenames();
    // strip off dir names - if there are duped presets this will misbehave.
    for (size_t i=0;i<m_presets.size();++i) {
      size_t sit = m_presets[i].rfind('/');
      size_t dit = m_presets[i].rfind('.');
      m_presets[i] = m_presets[i].substr(sit+1,dit-sit-1);
    }

    vsx_gl_state::get_instance()->viewport_set(0,0,Width(),Height());
  }

  virtual void Render() override
  {
    m_manager->render();
  }

  virtual void AudioData(const float* audioData, int audioDataLength,
                         float *freqData, int freqDataLength) override
  {
    m_manager->set_sound_wave(const_cast<float*>(audioData));
    m_manager->set_sound_freq(const_cast<float*>(freqData));
  }

  virtual void GetInfo(bool& wantsFreq, int& syncDelay) override
  {
    wantsFreq = true;
    syncDelay = 0;
  }

  virtual bool GetPresets(std::vector<std::string>& presets) override
  {
    presets = m_presets;
    return !presets.empty();
  }

  virtual int GetActivePreset()
  {
    std::string current = m_manager->get_meta_visual_filename();
    auto it = std::find_if(m_presets.begin(),
                           m_presets.end(),
                           [current](const std::string& a)
                           { return a.find(current) != std::string::npos;});

    return it - m_presets.begin();
  }

  virtual bool PrevPreset() override
  {
    m_manager->prev_visual();
    return true;
  }

  virtual bool NextPreset() override
  {
    m_manager->next_visual();
    return true;
  }

  virtual bool LoadPreset(int select) override
  {
    m_manager->pick_visual(select);
    return true;
  }

  virtual bool LockPreset(bool lockUnlock) override
  {
    m_manager->set_randomizer(lockUnlock);
    return true;
  }

  virtual bool RandomPreset() override
  {
    m_manager->pick_random_visual();
    return true;
  }

private:
  vsx_manager_abs* m_manager = nullptr;
  std::vector<std::string> m_presets;
};

ADDONCREATOR(CVisualizationVSXU) // Don't touch this!
