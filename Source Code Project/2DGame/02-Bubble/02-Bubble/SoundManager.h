#ifndef _SOUND_MANAGER_INCLUDE
#define _SOUND_MANAGER_INCLUDE

#include <irrKlang.h>
using namespace irrklang;
class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	static SoundManager& instance() {
		static SoundManager soundManager;
		return soundManager;
	}
	
	void init();
	ISoundEngine* getSoundEngine();
private:
	ISoundEngine* engine;

};
#endif

