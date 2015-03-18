
#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

#include <fmod.hpp>

#define MAX_SOUND_CHANNELS       200

#define INVALID_SOUND_INDEX      0
#define INVALID_SOUND_CHANNEL    -1

using namespace Ogre;


typedef enum
   {
   SOUND_TYPE_INVALID = 0,
   SOUND_TYPE_3D_SOUND,
   SOUND_TYPE_3D_SOUND_LOOPED,
   SOUND_TYPE_2D_SOUND,
   SOUND_TYPE_2D_SOUND_LOOPED,
   } SOUND_TYPE;


class SoundInstance
   {
public:
   void           Clear(void);
   String         fileName;
   Archive *      fileArchive;
   DataStreamPtr  streamPtr;
   SOUND_TYPE     soundType;
   FMOD::Sound *  fmodSound;
   };


class ChannelInstance
   {
public:
   void              Clear(void);
   Ogre::SceneNode * sceneNode;
   Ogre::Vector3     prevPosition;
   };




class SoundManager : public Ogre::Singleton<SoundManager>
   {
public:
                        SoundManager();
   virtual             ~SoundManager();
   void                 Initialize(void);
   void                 StopAllSounds(void);
   void                 FrameStarted(Ogre::SceneNode *listenerNode, Ogre::Real timeElapsed);

   int                  CreateSound(String &fileName);         // single-shot 3D sound.  returns soundIndex
   int                  CreateStream(String &fileName);        // single-shot 2D stream.  returns soundIndex
   int                  CreateLoopedSound(String &fileName);   // looping 3D sound.  returns soundIndex
   int                  CreateLoopedStream(String &fileName);  // looping 2D stream.  returns soundIndex

   int                  CreateSound(String &fileName, SOUND_TYPE soundType);

   void                 PlaySound(int soundIndex, SceneNode *soundNode, int *channelIndex,float volume=1.0f);
   void                 StopSound(int *channelIndex);
   int                  FindSound(String &fileName, SOUND_TYPE soundType);    // returns soundIndex;
   /**--------------------得到声道------------------------
   @ channelIndex 声道索引,INVALID_SOUND_CHANNEL或NULL为无效
   @ soundChannel 声道,可以为NULL
   -------------------------------------------------------*/
   bool                 isPlaying(int* channelIndex, FMOD::Channel* soundChannel); //判断声道是否正在播放
   /**--------------------得到音量------------------------
   @ volume       表示得到的音量（得到的值在0.0f~1.0f之间)
   @ channelIndex 通道索引,INVALID_SOUND_CHANNEL或NULL为无效
   -------------------------------------------------------*/
   bool                 getVolume(float* volume, int *channelIndex);   //得到音量
   /**--------------------设置音量------------------------
   @ volume       表示音量，值在0.0f~1.0f之间
   @ channelIndex 通道索引,INVALID_SOUND_CHANNEL或NULL为无效
   -------------------------------------------------------*/
   bool                 setVolume(float volume, int *channelIndex);   //设置音量
   /*---------------------暂停声音------------------------
   @ channelIndex 通道索引，INVALID_SOUND_CHANNEL或NULL为无效
   -------------------------------------------------------*/
   bool                 pauseSound(int *channelIndex);        //暂停声音
   /*---------------------继续声音------------------------
   @ channelIndex 通道索引，INVALID_SOUND_CHANNEL或NULL为无效
   -------------------------------------------------------*/
   bool                 continueSound(int *channelIndex);        //继续声音
   void                 Set3DMinMaxDistance(int channelIndex, float minDistance, float maxDistance);

   float                GetSoundLength(int soundIndex);        // returns seconds

   static SoundManager& getSingleton(void);
   static SoundManager* getSingletonPtr(void);

private:
   typedef std::vector<SoundInstance *>   SoundInstanceVector;
   typedef SoundInstanceVector::iterator  SoundInstanceVectorItr;

   int                  nextSoundInstanceIndex;
   FMOD::System *       system;
   Ogre::Vector3        prevListenerPosition;
   SoundInstanceVector *soundInstanceVector;
   ChannelInstance      channelArray[MAX_SOUND_CHANNELS];

   void                 IncrementNextSoundInstanceIndex(void);

   static FMOD_RESULT F_CALLBACK  fmodFileOpenCallback(const char *fileName, int unicode, unsigned int *filesize, void **handle, void **userdata);
   static FMOD_RESULT F_CALLBACK  fmodFileCloseCallback(void *handle, void *userdata);
   static FMOD_RESULT F_CALLBACK  fmodFileReadCallback(void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread, void *userdata);
   static FMOD_RESULT F_CALLBACK  fmodFileSeekCallback(void *handle, unsigned int pos, void *userdata);
   };

#endif // __SOUNDMANAGER_H__
