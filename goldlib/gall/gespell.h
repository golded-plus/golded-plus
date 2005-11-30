
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
//  SpellChecker Structures and Classes.
//  ------------------------------------------------------------------

#ifndef __GESPELL_H__
#define __GESPELL_H__


//  ------------------------------------------------------------------

#if defined(__GOLD_SPELL__)


//  ------------------------------------------------------------------

#if defined(__WIN32__)


//  ------------------------------------------------------------------

typedef uint32_t MDR;   // Main Dictionary Reference
typedef uint32_t UDR;   // User Dictionary Reference
typedef uint16_t SEC;   // Spell Error Code
typedef uint16_t LIDC;  // Language Identifier Code
typedef uint16_t SCCC;  // Spell Check Command Code
typedef uint32_t SLID;  // Spell Id type
typedef uint16_t SCIS;  // Spell Check Input Status
typedef uint16_t SCRS;  // Spell Check Return Status

typedef struct WizSpecChars
{
  byte bIgnore;
  byte bHyphenHard;
  byte bHyphenSoft;
  byte bHyphenNonBreaking;
  byte bEmDash;
  byte bEnDash;
  byte bEllipsis;
  byte rgLineBreak[2];
  byte rgParaBreak[2];
} WSC;

typedef struct SpellInputBuffer
{
  word cChr;        // Total characters in buffer area
  word cMdr;        // Count of MDR's specified in lrgMdr
  word cUdr;        // Count of UDR's specified in lrgUdr
  word wSpellState; // State relative to previous SpellCheck call
  char *lrgChr;     // Pointer to text to be spell checked
  MDR  *lrgMdr;     // List of main dicts to use when spelling the buffer
  UDR  *lrgUdr;     // List of user dicts to use when spelling the buffer
} SIB;

typedef struct SpellReturnBuffer
{
  // These fields are set by the SpellCheck() function
  word iChrErr;   // Position in the SIB
  word cChrErr;   // Length of error "word" in SIB
  SCRS scrs;      // Spell check return status
  word cSZ;       // Count of sz's put in buffer
  word cChrMac;   // Current total of chars in buffer

  // These fields MUST be set by the app, NULL pointers are invalid
  word cChr;      // Total space in lrgSZ
  char *lrgSZ;    // Ptr to alternatives format: word\0word\0...word\0\0
  byte *lrgbRate; // Ptr to Rating value for each suggest returned
  word cbRate;    // Number of elements in lrgbRate
} SRB;

typedef struct mdrs
{
  MDR  mdr;
  LIDC lidc;
  UDR  udr;
} MDRS;

typedef SEC (*SpellVer_fn         ) (word*, word*, word*);
typedef SEC (*SpellInit_fn        ) (SLID*, WSC*);
typedef SEC (*SpellOptions_fn     ) (SLID, dword);
typedef SEC (*SpellCheck_fn       ) (SLID, SCCC, SIB*, SRB*);
typedef SEC (*SpellTerminate_fn   ) (SLID, BOOL);
typedef SEC (*SpellOpenMdr_fn     ) (SLID, char*, char*, BOOL, BOOL, LIDC, MDRS*);
typedef SEC (*SpellCloseMdr_fn    ) (SLID, MDRS*);
typedef SEC (*SpellOpenUdr_fn     ) (SLID, char*, BOOL, word, UDR*, BOOL*);
typedef SEC (*SpellAddUdr_fn      ) (SLID, UDR, char*);
typedef SEC (*SpellCloseUdr_fn    ) (SLID, UDR, BOOL);
typedef SEC (*SpellAddChangeUdr_fn) (SLID, UDR, char*, char*);
typedef SEC (*SpellDelUdr_fn      ) (SLID, UDR, char*);
typedef SEC (*SpellClearUdr_fn    ) (SLID, UDR);
typedef SEC (*SpellGetSizeUdr_fn  ) (SLID, UDR, word*);
typedef SEC (*SpellGetListUdr_fn  ) (SLID, UDR, word, SRB*);
typedef SEC (*SpellVerifyMdr_fn   ) (char*, LIDC, LIDC*);

class CSpellLang
{
  friend class CSpellChecker;

private:
  LIDC mLIDC;
  SLID mSLID;
  MDRS mMDRS;
  UDR  mUDR;
  SIB  mSIB;
  SRB  mSRB;

  char mSZ[1024];
  byte mRate[1024];

  char mEngine[_MAX_PATH];
  char mDictionary[_MAX_PATH];

  HINSTANCE mLibrary;

  SpellVer_fn           mSpellVer;
  SpellInit_fn          mSpellInit;
  SpellOptions_fn       mSpellOptions;
  SpellCheck_fn         mSpellCheck;
  SpellTerminate_fn     mSpellTerminate;
  SpellOpenMdr_fn       mSpellOpenMdr;
  SpellCloseMdr_fn      mSpellCloseMdr;
  SpellOpenUdr_fn       mSpellOpenUdr;
  SpellAddUdr_fn        mSpellAddUdr;
  SpellCloseUdr_fn      mSpellCloseUdr;
  SpellAddChangeUdr_fn  mSpellAddChangeUdr;
  SpellDelUdr_fn        mSpellDelUdr;
  SpellClearUdr_fn      mSpellClearUdr;
  SpellGetSizeUdr_fn    mSpellGetSizeUdr;
  SpellGetListUdr_fn    mSpellGetListUdr;
  SpellVerifyMdr_fn     mSpellVerifyMdr;

public:
  CSpellLang() { mLibrary = NULL; }
  ~CSpellLang() { Close(); }

  bool Init(HKEY hKey, const char *name);
  void Close() { UnLoad(); }

  bool Load(const char *userdic);
  void UnLoad();

  bool SpellCheck(const char *text);
  bool SpellSuggest(const char *text, bool more);
  bool AddWord(const char *text);

  LIDC GetLangCode() { return mLIDC; }

  bool IsLoaded() { return mLibrary != NULL; }
};


typedef std::vector< std::pair<byte, std::string> > CSpellSuggestV;
typedef std::vector<CSpellLang> CSpellLangV;

class CSpellChecker
{
private:
  bool mInited;
  char mText[1024];

  CSpellLang     *mLang;
  CSpellLangV    mLangs;
  CSpellSuggestV mSuggest;

private:
  void AddLanguage(HKEY hKey, const char *name)
  {
    CSpellLang lang;

    if (lang.Init(hKey, name))
      mLangs.push_back(lang);
  }

public:
  CSpellChecker();
  ~CSpellChecker() { Close(); }

  bool Init();
  void Close();

  bool Load(LIDC lidc, const char *userdic = NULL);
  void UnLoad();

  bool Check(const char *text);
  CSpellSuggestV &Suggest();

  bool Check(std::string &text) { return Check(text.c_str()); }
  bool AddWord() { return IsLoaded() ?  mLang->AddWord(mText) : false; };

  CSpellSuggestV &GetSuggest() { return mSuggest; }
  CSpellLangV    &GetLangs()   { return mLangs; }
  LIDC GetLangCode() { return IsLoaded() ? mLang->GetLangCode() : 0xFFFF; }

  bool IsInited() { return mInited; }
  bool IsLoaded() { return mLang != NULL; }
};


//  ------------------------------------------------------------------

#endif  //#if defined(__WIN32__)


//  ------------------------------------------------------------------

#endif  //#if defined(__GOLD_SPELL__)


//  ------------------------------------------------------------------

#endif  //#ifndef __GESPELL_H__


//  ------------------------------------------------------------------
