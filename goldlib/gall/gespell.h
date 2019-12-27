
//  ------------------------------------------------------------------
//  The Goldware Library
//  Copyright (C) 1990-1999 Odinn Sorensen
//  Copyright (C) 1999-2000 Alexander S. Aganichev
//  Copyright (C) 2005-2006 Ianos Gnatius
//  Copyright (C) 2011 Max Vasilyev
//  Copyright (C) 2011 Stas Degteff
//  ------------------------------------------------------------------
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this program; if not, write to the Free
//  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
//  MA 02111-1307, USA
//  ------------------------------------------------------------------
//  $Id$
//  ------------------------------------------------------------------
//  SpellChecker Structures and Classes declarations.
//  ------------------------------------------------------------------

#ifndef __GESPELL_H__
#define __GESPELL_H__


//  ------------------------------------------------------------------

#if defined(GCFG_SPELL_INCLUDED)

#if defined (__WIN32__)
#include <windows.h>
#else
#include <sys/param.h>
#endif

#ifndef _MAX_PATH
#define _MAX_PATH 256
#endif

#ifndef PATH_MAX
#define PATH_MAX _MAX_PATH
#endif

const uint SCHECKET_TYPE_UNKNOWN = 0;
const uint SCHECKET_TYPE_MSSPELL = 1;
const uint SCHECKET_TYPE_MYSPELL = 2;


//  ------------------------------------------------------------------

typedef std::vector< std::pair<byte, std::string> > CSpellSuggestV;
struct Chs;

class CSpellLang
{
    friend class CSpellChecker;

protected:
    uint mSpellType;

    bool mIsMdrLoaded;
    bool mIsUdrLoaded;
    char mLangCode[100];

    char mEngine[PATH_MAX];
    char mDictionary[PATH_MAX];

    Chs  *mToDicTable;
    Chs  *mToLocTable;

public:
    CSpellLang()
    {
        mSpellType = SCHECKET_TYPE_UNKNOWN;
        mIsMdrLoaded = mIsUdrLoaded = false;
        mToDicTable = mToLocTable = NULL;
    }
    virtual ~CSpellLang() {};

    virtual bool Load(const char *codeset, const char *userdic) = 0;
    virtual void UnLoad() = 0;

    virtual void BuildRTable(const char *codeset) = 0;

    // Translate encoding:
    // flag=1: translate to charset of dictionnary, flag=0: translate to charset of locale
    void RecodeText(const char *srcText, char *dstText, bool flag);
    void RecodeText(const char *srcText, std::string &dstText, bool flag);

    virtual void BuildSuggest(const char *text, CSpellSuggestV &suggest) = 0;

    virtual bool SpellCheck(const char *text) = 0;
    virtual bool AddWord(const char *text) = 0;

    bool IsMdrLoaded()
    {
        return mIsMdrLoaded;
    }
    bool IsUdrLoaded()
    {
        return mIsUdrLoaded;
    }

    const char *GetLangCode()
    {
        return mLangCode;
    }
    uint GetSpellType()
    {
        return mSpellType;
    }
};


//  ------------------------------------------------------------------

#if !defined(GCFG_NO_MSSPELL)

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


//  ------------------------------------------------------------------

class CMSSpellLang: public CSpellLang
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

private:
    bool SpellSuggest(const char *text, bool more);

public:
    CMSSpellLang()
    {
        mLibrary = NULL;
        mSpellType = SCHECKET_TYPE_MSSPELL;
    }
    virtual ~CMSSpellLang()
    {
        UnLoad();
    }

    bool Init(HKEY hKey, const char *name);

    virtual bool Load(const char *codeset, const char *userdic);
    virtual void UnLoad();

    virtual void BuildRTable(const char *codeset);

    virtual void BuildSuggest(const char *text, CSpellSuggestV &suggest);

    virtual bool SpellCheck(const char *text);
    virtual bool AddWord(const char *text);
};

#endif  //#if !defined(GCFG_NO_MSSPELL)


#if !defined(GCFG_NO_MYSPELL)

class gdirentry;
class Hunspell;

class CMYSpellLang: public CSpellLang
{
    friend class CSpellChecker;

private:
    Hunspell *mMSpell;

public:
    CMYSpellLang()
    {
        mMSpell = NULL;
        mSpellType = SCHECKET_TYPE_MYSPELL;
    }
    virtual ~CMYSpellLang()
    {
        UnLoad();
    }

    bool Init(const gdirentry *entry);

    virtual bool Load(const char *codeset, const char *);
    virtual void UnLoad();

    virtual void BuildRTable(const char *codeset);

    virtual void BuildSuggest(const char *text, CSpellSuggestV &suggest);

    virtual bool SpellCheck(const char *text);
    virtual bool AddWord(const char *);
};

#endif  //#if !defined(GCFG_NO_MYSPELL)


typedef std::vector<CSpellLang*> CSpellLangV;

class CSpellChecker
{
private:
    bool mInited;
    std::string mText;

    char mXlatLocalset[256];

    CSpellLangV    mLangsLoaded;
    CSpellLangV    mLangs;
    CSpellSuggestV mSuggest;

public:
    CSpellChecker();
    ~CSpellChecker()
    {
        Close();
    }

    bool Init(const char *codeset, const char *dicPath);
    void Close();

    bool Load(const char *langId, const char *userDic);
    void UnLoad(const char *langId);
    void UnLoad();

    bool Check(const char *text);
    CSpellSuggestV &Suggest();

    bool Check(std::string &text)
    {
        return Check(text.c_str());
    }
    bool AddWord();

    CSpellSuggestV &GetSuggest()
    {
        return mSuggest;
    }
    CSpellLangV    &GetLangs()
    {
        return mLangs;
    }
    CSpellLangV    &GetLangsLoaded()
    {
        return mLangsLoaded;
    }
    const std::vector<const char*> GetLangCodes();

    bool IsUdrLoaded();

    bool IsInited()
    {
        return mInited;
    }
    bool IsLoaded()
    {
        return !mLangsLoaded.empty();
    }
    bool IsLoaded(const char *langId);
};


//  ------------------------------------------------------------------

#endif  //#if defined(GCFG_SPELL_INCLUDED)


//  ------------------------------------------------------------------

#endif  //#ifndef __GESPELL_H__


//  ------------------------------------------------------------------
