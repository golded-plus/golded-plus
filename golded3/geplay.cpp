
//  ------------------------------------------------------------------
//  GoldED+
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
//  ------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307 USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  Functions related to sound.
//  ------------------------------------------------------------------

#define GOLD_SOUNDSUPPORT 1
#include <golded.h>
#include <gtimall.h>
#ifdef GOLD_SOUNDSUPPORT
#include <gsndall.h>
#endif


//  ------------------------------------------------------------------

#ifdef GOLD_SOUNDSUPPORT
gsound* snd = NULL;
#endif


//  ------------------------------------------------------------------

#if defined(__MSDOS__)
#define Beep(A,B) { sound(A); usleep(B*CFG->beepfactor+1); }
#define Sleep(A)  { usleep(A*CFG->beepfactor+1); }
#define NoSound() { nosound(); }
#elif defined(__OS2__)
#define Beep(A,B) { DosBeep(A, B*CFG->beepfactor/1000+1); }
#define Sleep(A)  { usleep(A*CFG->beepfactor+1); }
#define NoSound() { }
#elif defined(__WIN32__)
#define Beep(A,B) { (Beep)(A, B*CFG->beepfactor/1000+1); }
#define Sleep(A)  { usleep(A*CFG->beepfactor+1); }
#define NoSound() { }
#else
#define Beep(A,B) { }
#define Sleep(A)  { }
#define NoSound() { }
#endif


//  ------------------------------------------------------------------

void ResetSound(void) {

  #ifdef GOLD_SOUNDSUPPORT
  if(snd->is_installed()) {
    while(snd->is_playing()) {
      if(kbxhit()) {
        if(kbxget() == Key_Esc) {
          snd->stop();
          break;
        }
      }
    }
  }
  delete snd;
  snd = NULL;
  #endif
}


//  ------------------------------------------------------------------

void InitSound() {

  // Find out if there are any sound files to be played
  #ifdef GOLD_SOUNDSUPPORT
  vector<GEvent>::iterator n;
  for(n = CFG->event.begin(); n != CFG->event.end(); n++) {
    if(n->play.type == PLAY_VOC) {
      snd = new gsound;
      if(snd->is_installed()) {
        if(not quiet)
          printf("\n* Soundcard support was successfully initialized.");
        atexit(ResetSound);
      }
      else {
        if(not quiet)
          printf("\n* Soundcard support could NOT be initialized!");
        ResetSound();
      }
      break;
    }
  }
  #endif
}


//  ------------------------------------------------------------------
//  Fill out a GPlay struct using a filename

void FileToGPlay(GPlay* playbuf, char* filename) {

  if(strchr(CleanFilename(filename), '.') != NULL)
    playbuf->type = PLAY_VOC;
  else {
    playbuf->type = PLAY_DOS;
    if(strieql(filename, "TheEnd"))
      playbuf->effectno = SND_THEEND;
    else if(strieql(filename, "GotIt"))
      playbuf->effectno = SND_GOTIT;
    else if(strieql(filename, "TooBad"))
      playbuf->effectno = SND_TOOBAD;
    else if(strieql(filename, "ToYou"))
      playbuf->effectno = SND_TOYOU;
    else if(strieql(filename, "SayBiBi"))
      playbuf->effectno = SND_SAYBIBI;
    else if(strieql(filename, "SOS"))
      playbuf->effectno = SND_S_O_S;
    else
      playbuf->effectno = 0xFFFF;
  }

  strcpy(playbuf->file, filename);
}


//  ------------------------------------------------------------------
//  Handle event

int HandleGEvent(uint event) {

  int retval = -1;     // Event could not be handled

  if(event == EVTT_BREAKLOOP) {
    #ifdef GOLD_SOUNDSUPPORT
    if(snd) {
      while(snd->is_playing()) {
        if(kbxhit()) {
          if(kbxget() == Key_Esc) {
            snd->stop();
            break;
          }
        }
      }
    }
    #endif
    return 0;
  }

  if(event == EVTT_STOPVOICE) {
    #ifdef GOLD_SOUNDSUPPORT
    if(snd)
      snd->stop();
    #endif
    return 0;
  }

  if(event == EVTT_REMOVEVOCBUF) {
    #ifdef GOLD_SOUNDSUPPORT
    if(snd) {
      if(not snd->is_playing())
        snd->unload();
    }
    #endif
    return 0;
  }

  if(CFG) {
    vector<GEvent>::iterator n;
    for(n = CFG->event.begin(); n != CFG->event.end(); n++) {
      if(event == n->type) {
        switch(n->command) {
          case EVTC_PLAY:
            n->play.Play();
            retval = 0;   // Event was found and handled
            break;
        }
        break;
      }
    }
  }
  else {
    MakeNoise(SND_S_O_S);
  }

  return retval;
}


//  ------------------------------------------------------------------
//  Make infernal noises

int MakeNoise(int type) {

  int n;

  if((CFG and CFG->switches.get(beepnoises)) and not disablesound) {

    switch(type) {

      case SND_SAYBIBI:
        Beep(440, 111);
	NoSound();
        break;

      case SND_TOYOU:
        for(n=100; n<800; n+=400)
          Beep(n, 18);
        for(; n>100; n-=400)
          Beep(n, 18);
	NoSound();
        break;

      case SND_THEEND:
        Beep(220, 111);
        Beep(110, 167);
	NoSound();
        break;

      case SND_GOTIT:
        Beep(110, 56);
        Beep(220, 56);
        Beep(110, 56);
        Beep(220, 111);
	NoSound();
        break;

      case SND_TOOBAD:
        Beep(440, 111);
        Beep(220, 111);
        Beep(110, 167);
	NoSound();
        break;

      case SND_S_O_S:
        Beep(1000, 56);
        Sleep(111);
        Beep(1000, 56);
        Sleep(111);
        Beep(1000, 56);
        Sleep(222);
        Beep(1000, 167);
        Sleep(167);
        Beep(1000, 167);
        Sleep(167);
        Beep(1000, 167);
        Sleep(222);
        Beep(1000, 56);
        Sleep(111);
        Beep(1000, 56);
        Sleep(111);
        Beep(1000, 56);
	NoSound();
        break;
    }
  }
  return(type);
}


//  ------------------------------------------------------------------
//  Play a file

int GPlay::Play() const {

  switch(type) {

    case PLAY_DOS:
      MakeNoise(effectno);
      break;

    case PLAY_VOC:
      if(not disablesound) {
        #ifdef GOLD_SOUNDSUPPORT
        if(snd) {
          snd->load(AddPath(CFG->soundpath, file));
          snd->play();
        }
        #endif
      }
      break;
  }

  return 0;
}


//  ------------------------------------------------------------------
//  Say a simple bibi

void SayBibi() {

  if(CFG and CFG->switches.get(beepnoises))
    HandleGEvent(EVTT_SAYBIBI);
}


//  ------------------------------------------------------------------

