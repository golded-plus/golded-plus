
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

#if defined(_MSC_VER)
    /* C4786: 'identifier' : identifier was truncated to 'number'
          characters in the debug information
    */
  #pragma warning(disable: 4786)
#endif

#include <vector>

#include <gdirposx.h>
#include <gstrall.h>
#if !defined(GCFG_NO_MYSPELL)
#include <hunspell.hxx>
#endif
#include <gespell.h>

typedef char XlatName[17];
typedef byte ChsTab[4];

struct Chs
{
  long   id;
  int    version;
  int    level;
  XlatName imp;   // From Charset
  XlatName exp;   // To Charset
  ChsTab t[256];  // The Translation Table
};

int   LoadCharset(const char* imp, const char* exp, int query = 0);
char* XlatStr(char* dest, const char* src, int level, Chs* chrtbl, int qpencoded=false, bool i51=false);

extern Chs* CharTable;


//  ------------------------------------------------------------------

#if defined(GCFG_SPELL_INCLUDED)


//  ------------------------------------------------------------------

#if !defined(GCFG_NO_MSSPELL)


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

bool CMSSpellLang::Init(HKEY hKey, const char *name)
{
  bool result = false;
  int  error;
  HKEY hKeyLang;

  byte engine[_MAX_PATH];
  byte dictionary[_MAX_PATH];
  unsigned long esize = sizeof(engine);
  unsigned long dsize = sizeof(dictionary);

  mLIDC  = atoi(name);
  strcpy(mLangCode, name);

  char name2[1024];
  strcpy(name2, name);
  strcat(name2, "\\Normal");

  error = RegOpenKeyEx(hKey, name2, 0, KEY_READ, &hKeyLang);
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

bool CMSSpellLang::Load(const char *codeset, const char *userdic)
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
  if (result)
  {
    BuildRTable(codeset);
    mIsMdrLoaded = (mSIB.cMdr != 0);
    mIsUdrLoaded = (mSIB.cUdr != 0);
  }
  return result;
}


//  ------------------------------------------------------------------

void CMSSpellLang::UnLoad()
{
  if (!mLibrary) return;

  if (mSIB.cUdr) mSpellCloseUdr(mSLID, mUDR, TRUE);
  if (mSIB.cMdr) mSpellCloseMdr(mSLID, &mMDRS);
  if (mToDicTable) delete mToDicTable;
  if (mToLocTable) delete mToLocTable;
  mToDicTable = mToLocTable = NULL;

  FreeLibrary(mLibrary);
  mLibrary = NULL;
}


//  ------------------------------------------------------------------

void CMSSpellLang::BuildRTable(const char *codeset)
{
  char codeset2[20];
  strcpy(codeset2, "CP");
  GetLocaleInfo(mLIDC, LOCALE_IDEFAULTANSICODEPAGE, &codeset2[2], sizeof(codeset2)-2);

  LoadCharset(codeset, codeset2);
  mToDicTable = new Chs;
  memset(mToDicTable, 0, sizeof(Chs));
  if (CharTable ) *mToDicTable = *CharTable;

  LoadCharset(codeset2, codeset);
  mToLocTable = new Chs;
  memset(mToLocTable, 0, sizeof(Chs));
  if (CharTable ) *mToLocTable = *CharTable;
}


//  ------------------------------------------------------------------

void CMSSpellLang::BuildSuggest(const char *text, CSpellSuggestV &suggest)
{
  if (!SpellSuggest(text, false)) return;

  bool flag = true;
  bool more = false;

  for (int idx = 0; idx < mSRB.cChrMac; idx++)
  {
    if (mSZ[idx] == 0) { idx++; flag = true; }

    if (flag && mSZ[idx])
    {
      flag = false;
      RecodeText(&mSZ[idx], &mSZ[idx], false);
      suggest.push_back(std::pair<byte, std::string>(0, "  " + std::string(&mSZ[idx]) + char(' ')));
    }
    else if (!more && !mSZ[idx])
    {
      more = true;

      if (!SpellSuggest(text, more = true))
        return;
      else
      {
        flag = true;
        idx = -1;
      }
    }
  }
}


//  ------------------------------------------------------------------

bool CMSSpellLang::SpellCheck(const char *text)
{
  if (!IsMdrLoaded()) return true;

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

bool CMSSpellLang::SpellSuggest(const char *text, bool more)
{
  if (!IsMdrLoaded()) return false;

  mSIB.wSpellState = 0;
  mSIB.lrgChr = (char*)text;
  mSIB.cChr = strlen(text);

  memset(mSZ, 0, sizeof(mSZ));

  mSRB.cChr = sizeof(mSZ);
  mSRB.cbRate = sizeof(mRate);
  mSRB.lrgSZ = mSZ;
  mSRB.lrgbRate = mRate;

  SEC error = mSpellCheck(mSLID, more ? SC_SCCC_SuggestMore : SC_SCCC_Suggest, &mSIB, &mSRB);
  if (error & 0xFF) return false;

  return mSRB.scrs == SC_SRCS_NoErrors;
}


//  ------------------------------------------------------------------

bool CMSSpellLang::AddWord(const char *text)
{
  if (!IsMdrLoaded()) return false;
  SEC error = mSpellAddUdr(mSLID, mUDR, (char*)text);
  return (error & 0xFF) == 0;
}


//  ------------------------------------------------------------------

#endif  //#if !defined(GCFG_NO_MSSPELL)


//  ------------------------------------------------------------------

#if !defined(GCFG_NO_MYSPELL)


//  ------------------------------------------------------------------

bool CMYSpellLang::Init(const gdirentry *entry)
{
  gposixdir dir(entry->dirname);

  std::string affname = entry->name.substr(0, entry->name.length()-4);
  strcpy(mLangCode, affname.c_str());

  const gdirentry *entry2 = dir.nextentry((affname+".aff").c_str(), true);
  if (entry2)
  {
    strcpy(mEngine, entry2->dirname);
    strcat(mEngine, "/");
    strcat(mEngine, entry2->name.c_str());
    strcpy(mDictionary, entry->dirname);
    strcat(mDictionary, "/");
    strcat(mDictionary, entry->name.c_str());

    return true;
  }

  return false;
}


//  ------------------------------------------------------------------

bool CMYSpellLang::Load(const char *codeset, const char *)
{
  mMSpell = new Hunspell(mEngine, mDictionary);

  if (mMSpell)
  {
    BuildRTable(codeset);
    return (mIsMdrLoaded = true);
  }

  return false;
}


//  ------------------------------------------------------------------

void CMYSpellLang::UnLoad()
{
  if (!mMSpell) return;
  delete mMSpell;
  mMSpell = NULL;
  if (mToDicTable) delete mToDicTable;
  if (mToLocTable) delete mToLocTable;
  mToDicTable = mToLocTable = NULL;
}


//  ------------------------------------------------------------------

void CMYSpellLang::BuildRTable(const char *codeset)
{
  LoadCharset(codeset, mMSpell->get_dic_encoding());
  mToDicTable = new Chs;
  memset(mToDicTable, 0, sizeof(Chs));
  if (CharTable ) *mToDicTable = *CharTable;

  LoadCharset(mMSpell->get_dic_encoding(), codeset);
  mToLocTable = new Chs;
  memset(mToLocTable, 0, sizeof(Chs));
  if (CharTable ) *mToLocTable = *CharTable;
}


//  ------------------------------------------------------------------

void CMYSpellLang::BuildSuggest(const char *text, CSpellSuggestV &suggest)
{
  char ** wlst = NULL;
  int ns = mMSpell->suggest(&wlst, text);

  for (int i=0; i < ns; i++)
  {
    char buff[1024];
    RecodeText(wlst[i], buff, false);
    suggest.push_back(std::pair<byte, std::string>(0, "  " + std::string(buff) + char(' ')));
    free(wlst[i]);
  }

  free(wlst);
}


//  ------------------------------------------------------------------

bool CMYSpellLang::SpellCheck(const char *text)
{
  if (!IsMdrLoaded()) return true;

  if (mMSpell->spell(text))
    return true;

  return false;
}


//  ------------------------------------------------------------------

bool CMYSpellLang::AddWord(const char *text)
{
  return false;
}


//  ------------------------------------------------------------------

#endif  //#if !defined(GCFG_NO_MYSPELL)


//  ------------------------------------------------------------------

void CSpellLang::RecodeText(const char *srcText, char *dstText, bool flag)
{
  if (flag)
    XlatStr(dstText, srcText, mToDicTable->level, mToDicTable);
  else
    XlatStr(dstText, srcText, mToLocTable->level, mToLocTable);
}


//  ------------------------------------------------------------------

CSpellChecker::CSpellChecker()
{
  mInited = false;
  mLang = NULL;
  mText[0] = 0;
}


//  ------------------------------------------------------------------

bool CSpellChecker::Init(const char *codeset, const char *dicPath)
{
#if !defined(GCFG_NO_MSSPELL)

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
      CMSSpellLang *lang = new CMSSpellLang;
      if (lang->Init(hKeySpelling, name))
        mLangs.push_back(lang);
      else
        delete lang;

      index++;
    }
  }
  while (error != ERROR_NO_MORE_ITEMS);

  RegCloseKey(hKeySpelling);
cleanup1:
  RegCloseKey(hKeyPTools);
cleanup0:

#endif  //#if !defined(GCFG_NO_MSSPELL)

#if !defined(GCFG_NO_MYSPELL)

  gposixdir d(dicPath);
  const gdirentry *de;

  while ((de = d.nextentry("*.dic", true)) != NULL)
  {
    CMYSpellLang *lang = new CMYSpellLang;
    if (lang->Init(de))
      mLangs.push_back(lang);
    else
      delete lang;
  }

#endif  //#if !defined(GCFG_NO_MSSPELL)

  strcpy(mXlatLocalset, codeset);

  return mInited = (mLangs.size() > 0);
}


//  ------------------------------------------------------------------

void CSpellChecker::Close()
{
  if (!IsInited()) return;
  UnLoad();
  mLangs.clear();
  mInited = false;
}


//  ------------------------------------------------------------------

bool CSpellChecker::Load(const char *langId, const char *userDic)
{

  if (!IsInited()) return false;
  if (IsLoaded() && streql(mLang->GetLangCode(), langId)) return true;

  CSpellLangV::iterator it;
  for (it = mLangs.begin(); it != mLangs.end(); it++)
  {
    if (streql((*it)->GetLangCode(), langId) && (*it)->Load(mXlatLocalset, userDic))
    {
      UnLoad();
      mLang = *it;
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

  mLang->RecodeText(text, mText, true);
  return mLang->SpellCheck(mText);
}


//  ------------------------------------------------------------------

CSpellSuggestV &CSpellChecker::Suggest()
{
  mSuggest.clear();
  if (!IsLoaded()) return mSuggest;

  mLang->BuildSuggest(mText, mSuggest);

  return mSuggest;
}


//  ------------------------------------------------------------------

#if !defined(GCFG_NO_MSSPELL)
#undef CHECK_ERROR
#undef CHECK_NULL
#undef CHECK_SEC
#endif


//  ------------------------------------------------------------------

#endif  //#if defined(GCFG_SPELL_INCLUDED)

//  ------------------------------------------------------------------
