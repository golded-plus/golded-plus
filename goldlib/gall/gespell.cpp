
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
//  SpellChecker functions.
//  ------------------------------------------------------------------

#if defined(__GOLD_SPELL__)

#if defined(_MSC_VER)
    /* C4786: 'identifier' : identifier was truncated to 'number'
          characters in the debug information
    */
  #pragma warning(disable: 4786)
#endif

#include <vector>

#include <gstrall.h>
#include <gespell.h>


//  ------------------------------------------------------------------

#if defined(__WIN32__)


//  ------------------------------------------------------------------

#define CHECK_ERROR(jump)     if (error != ERROR_SUCCESS) goto jump
#define CHECK_NULL(ptr, jump) if (ptr == NULL) goto jump
#define CHECK_SEC(jump)       if ((sec & 0xFF) != SC_SEC_NoErrors) goto jump


//  ------------------------------------------------------------------

const char SC_RKEY_Prooftools[]   = "Software\\Microsoft\\Shared Tools\\Proofing Tools";
const char SC_RKEY_Grammar[]      = "Grammar";
const char SC_RKEY_Spelling[]     = "Spelling";
const char SC_RKEY_Hyphenation[]  = "Hyphenation";
const char SC_RKEY_Thesaurus[]    = "Thesaurus";

//  Spell Check Command Code
const SCCC SC_SCCC_VerifyWord     = 1;
const SCCC SC_SCCC_VerifyBuffer   = 2;
const SCCC SC_SCCC_Suggest        = 3;
const SCCC SC_SCCC_SuggestMore    = 4;
const SCCC SC_SCCC_HyphInfo       = 5;
const SCCC SC_SCCC_Wildcard       = 6;
const SCCC SC_SCCC_Anagram        = 7;

//  Spell Check Return Status
const SCRS SC_SRCS_NoErrors                     = 0;
const SCRS SC_SRCS_UnknownInputWord             = 1;
const SCRS SC_SRCS_ReturningChangeAlways        = 2;
const SCRS SC_SRCS_ReturningChangeOnce          = 3;
const SCRS SC_SRCS_InvalidHyphenation           = 4;
const SCRS SC_SRCS_ErrorCapitalization          = 5;
const SCRS SC_SRCS_WordConsideredAbbreviation   = 6;
const SCRS SC_SRCS_HyphChangesSpelling          = 7;
const SCRS SC_SRCS_NoMoreSuggestions            = 8;
const SCRS SC_SRCS_MoreInfoThanBufferCouldHold  = 9;
const SCRS SC_SRCS_NoSentenceStartCap           = 10;
const SCRS SC_SRCS_RepeatWord                   = 11;
const SCRS SC_SRCS_ExtraSpaces                  = 12;
const SCRS SC_SRCS_MissingSpace                 = 13;
const SCRS SC_SRCS_InitialNumeral               = 14;

//  Major Error Codes (Low Byte of SEC)
const SEC  SC_SEC_NoErrors        = 0;
const SEC  SC_SEC_OOM             = 1;
const SEC  SC_SEC_ModuleError     = 2;
const SEC  SC_SEC_IOErrorMdr      = 3;
const SEC  SC_SEC_IOErrorUdr      = 4;

//  Minor Error Codes (High Byte of SEC)
const SEC  SC_SEC_ModuleAlreadyBusy       = 128 << 8;
const SEC  SC_SEC_InvalidID               = 129 << 8;
const SEC  SC_SEC_InvalidWsc              = 130 << 8;
const SEC  SC_SEC_InvalidMdr              = 131 << 8;
const SEC  SC_SEC_InvalidUdr              = 132 << 8;
const SEC  SC_SEC_InvalidSCC              = 133 << 8;
const SEC  SC_SEC_InvalidMainDict         = 134 << 8;
const SEC  SC_SEC_OperNotMatchedUserDict  = 135 << 8;
const SEC  SC_SEC_FileReadError           = 136 << 8;
const SEC  SC_SEC_FileWriteError          = 137 << 8;
const SEC  SC_SEC_FileCreateError         = 138 << 8;
const SEC  SC_SEC_FileShareError          = 139 << 8;
const SEC  SC_SEC_ModuleNotTerminated     = 140 << 8;
const SEC  SC_SEC_UserDictFull            = 141 << 8;
const SEC  SC_SEC_InvalidUdrEntry         = 142 << 8;
const SEC  SC_SEC_UdrEntryTooLong         = 143 << 8;
const SEC  SC_SEC_MdrCountExceeded        = 144 << 8;
const SEC  SC_SEC_UdrCountExceeded        = 145 << 8;
const SEC  SC_SEC_FileOpenError           = 146 << 8;
const SEC  SC_SEC_FileTooLargeError       = 147 << 8;
const SEC  SC_SEC_UdrReadOnly             = 148 << 8;

//  Spell Options bitfield definitions
const dword SC_SO_SuggestFromUserDict     = 0x0001; // Scan UDR as well as MDR
const dword SC_SO_IgnoreAllCaps           = 0x0002; // Ignore a word if all upppercase
const dword SC_SO_IgnoreMixedDigits       = 0x0004; // Ignore word if has any numbers in it
const dword SC_SO_IgnoreRomanNumerals     = 0x0008; // Ignore word composed of all roman numerals
const dword SC_SO_FindUncappedSentences   = 0x0010; // Flag sentences which don't start with a cap
const dword SC_SO_FindMissingSpaces       = 0x0020; // Find missing spaces between words/sentences
const dword SC_SO_FindRepeatWord          = 0x0040; // CSAPI to flag repeated words
const dword SC_SO_FindExtraSpaces         = 0x0080; // CSAPI to flag extra spaces between words
const dword SC_SO_FindSpacesBeforePunc    = 0x0100; // CSAPI to flag space preceeding certain punc
const dword SC_SO_FindSpacesAfterPunc     = 0x0200; // CSAPI to flag space after certain punc
const dword SC_SO_RateSuggestions         = 0x0400;
  // All suggestions returned should be given some scaled value 
  // corresponding to liklihood of being correct alternative.
  // Scale is 1..255, 255 most likely correction and 1 least likely
const dword SC_SO_FindInitialNumerals     = 0x0800; // Flag words starting with number(s)
const dword SC_SO_ReportUDHits            = 0x1000;
  // Report (via scrsNoErrorsUDHit) where user dict was used during verification
const dword SC_SO_QuickSuggest            = 0x2000; // Don't use typo suggest code (Soft-Art only)
const dword SC_SO_UseAllOpenUdr           = 0x4000;
  // Automatically use all udr's opened after this option is set,
  // or all opened udr's with mdr's opened after this option is set.
  // This option does not allow exclusion dicts to be edited. (HM only)
const dword SC_SO_SwapMdr                 = 0x8000;
  // Keep the most recent 2 mdr's around. swap between them instead of actually 
  // closing and reopening mdr's. (HM only)
const dword SC_SO_SglStepSugg             = 0x10000;
  // Break after each suggestion task for faster return of control to the
  // application. (HM only)

const dword SC_SO_LangMode                    = 0xF0000000; // Language Mode mask
// Hebrew Language Modes -- (CT only)
const dword SC_SO_HebrewFullScript            = 0x00000000;
const dword SC_SO_HebrewPartialScript         = 0x10000000;
const dword SC_SO_HebrewMixedScript           = 0x20000000;
const dword SC_SO_HebrewMixedAuthorizedScript = 0x30000000;
// French Language Modes -- (HM only)
const dword SC_SO_FrenchDialectDefault        = 0x00000000;
const dword SC_SO_FrenchUnaccentedUppercase   = 0x10000000;
const dword SC_SO_FrenchAccentedUppercase     = 0x20000000;
// Russian Language Modes -- (HM only)
const dword SC_SO_RussianDialectDefault       = 0x00000000;
const dword SC_SO_RussianIE                   = 0x10000000;
const dword SC_SO_RussianIO                   = 0x20000000;


//  ------------------------------------------------------------------

bool CSpellLang::Init(HKEY hKey, const char *name)
{
  bool result = false;
  int  error;
  HKEY hKeyLang;

  byte engine[_MAX_PATH];
  byte dictionary[_MAX_PATH];
  unsigned long esize = sizeof(engine);
  unsigned long dsize = sizeof(dictionary);

  mLIDC  = atoi(name);

  error = RegOpenKeyEx(hKey, name, 0, KEY_READ, &hKeyLang);
  CHECK_ERROR(cleanup0);

  error = RegQueryValueEx(hKeyLang, "Engine", NULL, NULL, engine, &esize);
  CHECK_ERROR(cleanup1);
  error = RegQueryValueEx(hKeyLang, "Dictionary", NULL, NULL, dictionary, &dsize);
  CHECK_ERROR(cleanup1);

  strcpy(mEngine, (char*)engine);
  strcpy(mDictionary, (char*)dictionary);

  result = true;

cleanup1:
  RegCloseKey(hKeyLang);
cleanup0:
  return result;
}


//  ------------------------------------------------------------------

bool CSpellLang::Load(const char *userdic)
{
  bool result = false;

  mLibrary = LoadLibrary(mEngine);
  CHECK_NULL(mLibrary, cleanup0);

  mSpellVer          = (SpellVer_fn         ) GetProcAddress(mLibrary, "SpellVer"         ); CHECK_NULL(mSpellVer, cleanup1);
  mSpellInit         = (SpellInit_fn        ) GetProcAddress(mLibrary, "SpellInit"        ); CHECK_NULL(mSpellInit, cleanup1);
  mSpellOptions      = (SpellOptions_fn     ) GetProcAddress(mLibrary, "SpellOptions"     ); CHECK_NULL(mSpellOptions, cleanup1);
  mSpellCheck        = (SpellCheck_fn       ) GetProcAddress(mLibrary, "SpellCheck"       ); CHECK_NULL(mSpellCheck, cleanup1);
  mSpellTerminate    = (SpellTerminate_fn   ) GetProcAddress(mLibrary, "SpellTerminate"   ); CHECK_NULL(mSpellTerminate, cleanup1);
  mSpellOpenMdr      = (SpellOpenMdr_fn     ) GetProcAddress(mLibrary, "SpellOpenMdr"     ); CHECK_NULL(mSpellOpenMdr, cleanup1);
  mSpellCloseMdr     = (SpellCloseMdr_fn    ) GetProcAddress(mLibrary, "SpellCloseMdr"    ); CHECK_NULL(mSpellCloseMdr, cleanup1);
  mSpellOpenUdr      = (SpellOpenUdr_fn     ) GetProcAddress(mLibrary, "SpellOpenUdr"     ); CHECK_NULL(mSpellOpenUdr, cleanup1);
  mSpellAddUdr       = (SpellAddUdr_fn      ) GetProcAddress(mLibrary, "SpellAddUdr"      ); CHECK_NULL(mSpellAddUdr, cleanup1);
  mSpellCloseUdr     = (SpellCloseUdr_fn    ) GetProcAddress(mLibrary, "SpellCloseUdr"    ); CHECK_NULL(mSpellCloseUdr, cleanup1);
  mSpellAddChangeUdr = (SpellAddChangeUdr_fn) GetProcAddress(mLibrary, "SpellAddChangeUdr"); CHECK_NULL(mSpellAddChangeUdr, cleanup1);
  mSpellDelUdr       = (SpellDelUdr_fn      ) GetProcAddress(mLibrary, "SpellDelUdr"      ); CHECK_NULL(mSpellDelUdr, cleanup1);
  mSpellClearUdr     = (SpellClearUdr_fn    ) GetProcAddress(mLibrary, "SpellClearUdr"    ); CHECK_NULL(mSpellClearUdr, cleanup1);
  mSpellGetSizeUdr   = (SpellGetSizeUdr_fn  ) GetProcAddress(mLibrary, "SpellGetSizeUdr"  ); CHECK_NULL(mSpellGetSizeUdr, cleanup1);
  mSpellGetListUdr   = (SpellGetListUdr_fn  ) GetProcAddress(mLibrary, "SpellGetListUdr"  ); CHECK_NULL(mSpellGetListUdr, cleanup1);
  mSpellVerifyMdr    = (SpellVerifyMdr_fn   ) GetProcAddress(mLibrary, "SpellVerifyMdr"   ); CHECK_NULL(mSpellVerifyMdr, cleanup1);

  memset(&mSIB, 0, sizeof(mSIB));
  memset(&mSRB, 0, sizeof(mSRB));
  memset(&mMDRS, 0, sizeof(mMDRS));
  mSLID = mUDR = 0;

  SEC sec;

//  word w1, w2, w3;
//  mSpellVer(&w1, &w2, &w3);

  WSC wsc;
  memset(&wsc, 0, sizeof(wsc));
  sec = mSpellInit(&mSLID, &wsc);
  CHECK_SEC(cleanup1);

  sec = mSpellOpenMdr(mSLID, mDictionary, 0, 0, 1, mLIDC, &mMDRS);
  CHECK_SEC(cleanup2);

  mSIB.cMdr = 1;
  mSIB.lrgMdr = &mMDRS.mdr;

  sec = mSpellOptions(mSLID, SC_SO_SuggestFromUserDict|SC_SO_IgnoreAllCaps|SC_SO_IgnoreMixedDigits|SC_SO_IgnoreRomanNumerals|SC_SO_RateSuggestions);
  CHECK_SEC(cleanup2);

  if (userdic)
  {
    BOOL ronly = FALSE;
    sec = mSpellOpenUdr(mSLID, (char*)userdic, TRUE, 0xfffe, &mUDR, &ronly);

    if ((sec & 0xFF) == SC_SEC_NoErrors)
    {
      mSIB.cUdr = 1;
      mSIB.lrgUdr = &mUDR;
    }
  }

  result = true;
  goto cleanup0;

cleanup2:
  mSpellTerminate(mSLID, TRUE);
cleanup1:
  FreeLibrary(mLibrary);
  mLibrary = NULL;
cleanup0:
  return result;
}


//  ------------------------------------------------------------------

void CSpellLang::UnLoad()
{
  if (!mLibrary) return;

  if (mSIB.cUdr) mSpellCloseUdr(mSLID, mUDR, TRUE);
  if (mSIB.cMdr) mSpellCloseMdr(mSLID, &mMDRS);

  FreeLibrary(mLibrary);
  mLibrary = NULL;
}


//  ------------------------------------------------------------------

bool CSpellLang::SpellCheck(const char *text)
{
  if (!IsLoaded()) return true;

  mSIB.wSpellState = 0;
  mSIB.lrgChr = (char*)text;
  mSIB.cChr = strlen(text);

  mSRB.cChr = sizeof(mSZ);
  mSRB.cbRate = sizeof(mRate);
  mSRB.lrgSZ = mSZ;
  mSRB.lrgbRate = mRate;

  SEC error = mSpellCheck(mSLID, SC_SCCC_VerifyBuffer, &mSIB, &mSRB);
  if (error & 0xFF) return false;

  return mSRB.scrs == SC_SRCS_NoErrors;
}


//  ------------------------------------------------------------------

bool CSpellLang::SpellSuggest(const char *text, bool more)
{
  if (!IsLoaded()) return false;

  mSIB.wSpellState = 0;
  mSIB.lrgChr = (char*)text;
  mSIB.cChr = strlen(text);

  mSRB.cChr = sizeof(mSZ);
  mSRB.cbRate = sizeof(mRate);
  mSRB.lrgSZ = mSZ;
  mSRB.lrgbRate = mRate;

  SEC error = mSpellCheck(mSLID, more ? SC_SCCC_SuggestMore : SC_SCCC_Suggest, &mSIB, &mSRB);
  if (error & 0xFF) return false;

  return mSRB.scrs == SC_SRCS_NoErrors;
}


//  ------------------------------------------------------------------

bool CSpellLang::AddWord(const char *text)
{
  if (!IsLoaded()) return false;
  SEC error = mSpellAddUdr(mSLID, mUDR, (char*)text);
  return (error & 0xFF) == 0;
}


//  ------------------------------------------------------------------

CSpellChecker::CSpellChecker()
{
  mInited = false;
  mLang = NULL;
  mText[0] = 0;
}

//  ------------------------------------------------------------------

bool CSpellChecker::Init()
{
  int  error;
  unsigned long index = 0;

  HKEY hKeyPTools;
  HKEY hKeySpelling;

  error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, SC_RKEY_Prooftools, 0, KEY_READ, &hKeyPTools);
  CHECK_ERROR(cleanup0);

  error = RegOpenKeyEx(hKeyPTools, SC_RKEY_Spelling, 0, KEY_READ, &hKeySpelling);
  CHECK_ERROR(cleanup1);

  do
  {
    char name[1024];
    unsigned long nsize = sizeof(name);

    error = RegEnumKeyEx(hKeySpelling, index, name, &nsize, NULL, NULL, NULL, NULL);
    if (error == ERROR_SUCCESS)
    {
      strcat(name, "\\Normal");
      AddLanguage(hKeySpelling, name);
      index++;
    }
  }
  while (error != ERROR_NO_MORE_ITEMS);

  RegCloseKey(hKeySpelling);
cleanup1:
  RegCloseKey(hKeyPTools);
cleanup0:
  return mInited = (mLangs.size() > 0);
}


//  ------------------------------------------------------------------

void CSpellChecker::Close()
{
  if (!IsInited()) return;
  mLangs.clear();
  mInited = false;
  mLang = NULL;
}


//  ------------------------------------------------------------------

bool CSpellChecker::Load(word lic, const char *userdic)
{
  if (!IsInited()) return false;

  std::vector<CSpellLang>::iterator it;
  for (it = mLangs.begin(); it != mLangs.end(); it++)
  {
    if ((it->GetLangCode() == lic) && it->Load(userdic))
    {
      UnLoad();
      mLang = it;
      break;
    }
  }

  return IsLoaded();
}


//  ------------------------------------------------------------------

void CSpellChecker::UnLoad()
{
  if (!IsLoaded()) return;
  mLang->UnLoad();
  mLang = NULL;
}


//  ------------------------------------------------------------------

bool CSpellChecker::Check(const char *text)
{
  if (!IsLoaded()) return true;

  OemToChar(text, mText);
  return mLang->SpellCheck(mText);
}


//  ------------------------------------------------------------------

CSpellSuggestV &CSpellChecker::Suggest()
{
  mSuggest.clear();
  if (!IsLoaded()) return mSuggest;
  if (!mLang->SpellSuggest(mText, false)) return mSuggest;

  bool flag = true;
  bool more = false;

  for (int idx = 0; idx < mLang->mSRB.cChrMac; idx++)
  {
    if (mLang->mSZ[idx] == 0) { idx++; flag = true; }

    if (flag && mLang->mSZ[idx])
    {
      flag = false;
      CharToOem(&mLang->mSZ[idx], &mLang->mSZ[idx]);
      mSuggest.push_back(std::pair<byte, std::string>(0, "  " + std::string(&mLang->mSZ[idx]) + char(' ')));
    }
    else if (!more && !mLang->mSZ[idx])
    {
      more = true;

      if (!mLang->SpellSuggest(mText, more = true)) 
        return mSuggest;
      else
      {
        flag = true;
        idx = -1;
      }
    }
  }

  return mSuggest;
}


//  ------------------------------------------------------------------

#undef CHECK_ERROR
#undef CHECK_NULL
#undef CHECK_SEC


//  ------------------------------------------------------------------

#endif  // #if defined(__WIN32__)


//  ------------------------------------------------------------------

#endif  // #if defined(__GOLD_SPELL__)

//  ------------------------------------------------------------------
