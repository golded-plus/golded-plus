//////////////////////////////////////////////////////////////////////
//
//  GoldED+ 1.1.5 * Swedish Language Definition File
//
//  Before you start redefining the language, I suggest you print this
//  file and keep it close for reference. Redefining can be a slow and
//  difficult process.
//
//  The format of lines in this language file is:
//
//      <stringid> <"string">
//
//  Each stringid is prefixed with a category, to allow easier
//  identification for your own redefinitions. Not case sensitive.
//
//  ID Categories:
//  --------------
//  ST_     Status Line     Variable length.
//  WT_     Window Title    Variable length.
//  MI_     Menu Item       Fixed length, first character QuickChar.
//  IL_     Info Line       Variable length.
//  WL_     Window Line     Usually fixed length.
//  HD_     Header Line     Usually fixed length.
//  MS_     Message         Variable length.
//  ER_     Error Message   Variable length.
//
//  "Fixed length" means that all the strings must the same length.
//  The actual length is normally only limited by the screen size. In
//  case of menus, the window width is always based on the length of
//  the *first* item string.
//
//  A "QuickChar" is the highlighted direct-select character in the
//  menu items. The QuickChar is case sensitive, and must match a
//  character in the string.
//
//  Strings should always be enclosed in quotes ("" or ''). The
//  exception is if the string does not need leading/trailing spaces.
//
//  Some strings contains escape sequences or format specifiers as
//  used in the "C" language. Certain tokens from the templates can
//  also be used in some special strings.
//
//  Escape sequences: (Case sensitive!)
//  -----------------------------------
//  \n  - Line Feed (LF) (translates to CR+LF in files).
//  \r  - Carriage Return (CR).
//
//  The escape sequences are normally used in lines for files or
//  standard (DOS) output messages.
//
//  Format specifiers: (Case sensitive!)
//  ------------------------------------
//  %c  - Character.
//  %s  - String.
//  %i  - Integer.
//  %u  - Unsigned Integer.
//  %%  - A literal percent (%) character.
//
//  If you change a line with format specifiers, please be careful to
//  have them in the same quantity and order as in the original
//  example. Failure to do this can cause unpredictable results..
//
//////////////////////////////////////////////////////////////////////


----------------------------------------------------------------------
-- WEEKDAY AND MONTH NAMES

// Weekdays, short
MS_SWSUN             "s�n"
MS_SWMON             "m�n"
MS_SWTUE             "tis"
MS_SWWED             "ons"
MS_SWTHU             "tor"
MS_SWFRI             "fre"
MS_SWSAT             "l�r"

// Weekdays, long
MS_LWSUNDAY          "s�ndag"
MS_LWMONDAY          "m�ndag"
MS_LWTUESDAY         "tisdag"
MS_LWWEDNESDAY       "onsdag"
MS_LWTHURSDAY        "torsdag"
MS_LWFRIDAY          "fredag"
MS_LWSATURDAY        "l�rdag"

// Months, short
MS_SMJAN             "jan"
MS_SMFEB             "feb"
MS_SMMAR             "mar"
MS_SMAPR             "apr"
MS_SMMAY             "maj"
MS_SMJUN             "jun"
MS_SMJUL             "jul"
MS_SMAUG             "aug"
MS_SMSEP             "sep"
MS_SMOCT             "okt"
MS_SMNOV             "nov"
MS_SMDEC             "dec"

// Months, long
MS_LMJANUARY         "januari"
MS_LMFEBRUARY        "februari"
MS_LMMARCH           "mars"
MS_LMAPRIL           "april"
MS_LMMAY             "maj"
MS_LMJUNE            "juni"
MS_LMJULY            "juli"
MS_LMAUGUST          "augusti"
MS_LMSEPTEMBER       "september"
MS_LMOCTOBER         "oktober"
MS_LMNOVEMBER        "november"
MS_LMDECEMBER        "december"

MS_UPTIME             "%days day(s) %hours:%minutes:%seconds.%useconds"

----------------------------------------------------------------------
-- MISCELLANEOUS LANGUAGE KEYWORDS

ST_IMPORTFILE        "Importera fil"
WT_IMPORTWHICHFILE   " Importera vilken fil? "
WT_IMPORTPICK        " Importera "
WT_IMPORTTXT         " Importera "
MI_IMPORTTXTTEXT     "t Fil som text   "
MI_IMPORTTXTQUOTE    "c Fil som citat  "
MI_IMPORTTXTUUE      "U Bin�r som UUE  "
MI_IMPORTTXTMIME     "M Bin�r som MIME "
MI_IMPORTTXTCLIP     "k Fr�n klippbord "
MI_IMPORTTXTXLAT     "t Anv�nd tknset.."
MI_IMPORTTXTQUIT     "i Inget / Avbryt "
ST_IMPORTSTATUS      "Importerar fr�n %s"
ST_EXPORTFILE        "Exportera fil"
WT_EXPORTWHATFILE    " Exportera till vilken? "
ST_EXPORTSTATUS      "Exporterar till %s"
ST_EDITSTATUS        "Redigerar %i,%i (%02u). %s"
;ST_EDITSTATUS        "Redigerar %i,%i (%03u). %s"
;ST_EDITSTATUS        "Redigerar %i,%i (%03o). %s"
ST_SELECTFILES       "V�lj filer"
WL_SELECTEDFILES     "Valda filer         "
WL_SELECTEDBYTES     "      byte          "
WL_TOTALFILES        "Total filer         "
WL_TOTALBYTES        "      byte          "
WL_SCANNINGDIRECTORY "              Avs�ker katalog               "
WL_NOFILESFOUND      "            * INGA FILER FUNNA *            "
ST_FILESPRESSKEY     "Tryck tangent f�r att forts�tta"
WT_AREA              "M�te"
WT_DESCRIPTION       "Beskrivning"
WT_MSGS              "Texter"
WT_LAST              "Sista"
WT_ECHOID            "EchoID"
ST_UNREAD            "ol�st"
WT_SCANAREAS         " Avs�k m�ten "
MI_SCANALL           "A Avs�k alla     "
MI_SCANMARKED        "m Avs�k markerade"
MI_SCANCURRENT       "n Avs�k nuvarande"
MI_SCANMATCHING      "t Avs�k matchande"
MI_SCANUNSCANNED     "o Avs�k oavs�kta "
MI_NOSCAN            "E Avs�k ej / ESC "
WT_HEATAREAS         " Heat Areas "
MI_HEATALL           "A Heat All      "
MI_HEATMARKED        "M Heat Marked   "
MI_HEATCURRENT       "C Heat Current  "
MI_NOHEAT            "N No Heat / ESC "
WT_ZAPAREAS          " Zap Areas "
MI_ZAPALL            "A Zap All      "
MI_ZAPMARKED         "M Zap Marked   "
MI_ZAPCURRENT        "C Zap Current  "
MI_NOZAP             "N No Zap / ESC "
MS_DOS_SHELL         "GoldED-skal. Tryck EXIT f�r att �terv�nda."
IL_SCANNINGAREA      " Avs�ker m�te:"
IL_SEARCHINGFOR      " S�ker "
ST_READINGMSG        "L�ser text %u av %u"
ER_OUTOFMEM          "Slut p� minne!"
MS_HEXDUMPHEAD       "Hexdump av texthuvud:"
MS_HEXDUMPTEXT       "Hexdump av textkropp:"
ST_RENUMBERING       "Omnumrerar"
ST_LOCKED            " (locked)"
ST_RENUMBERED        "Meddelanden omnumrerade - tryck tangent"
WL_BLANKMSG          ""
WL_WAIT              " V�nta"
IL_GENHEXDUMP        " Genererar hexdump - v�nta "
ST_PROCESSCC         "Hanterar kopior"
ST_STATUSCC          "CC: %s of %s"
ST_DESTINATIONCC     "V�lj kopiemottagare"
MS_LISTCC            "%s  %s"
WT_DELORIG           " Ta bort original? "
MI_DELORIGYES        "J        Ja, tack.       "
MI_DELORIGNO         "N          Nej!          "
WT_DROPMSG           " Kasta text? "
MI_DROPMSGYES        "J        Ja, tack.       "
MI_DROPMSGNO         "N          Nej!          "
WT_ZONEGATE          " S�nd via ZoneGate? "
MI_ZONEGATEYES       "J        Ja, tack.       "
MI_ZONEGATENO        "N          Nej!          "
ST_QUOTEPCT          "Din text inneh�ller %i%% citat%s"
WT_SAVEMSG           " Spara dessa %i rader? "
MI_YESGREAT          "J Ja, f�r tusan         "
MI_KICKIT            "N Nej, kasta dem  / ESC "
MI_CONTINUE          "F Forts�tt skriva       "
MI_ROT13             "R ROT13-kodning         "
MI_ATTRS             "a �ndra attribut        "
MI_ORIGS             "O �ndra Origin          "
MI_VIEW              "V Visa texten           "
ST_SELECTDESTNODE    "V�lj mottagaradress"
WT_ATTACHFILES       " Bifoga filer "
WT_UPDREQFILES       " Uppdatera f�fr�gningsfil "
ST_EDITHEADER        "Redigera data i huvudet"
WT_EDITING           " Redigera "
MI_INTERNALED        "I Intern          "
MI_EXTERNALED        "E Extern          "
MI_SAVEMESSAGE       "S Spara text      "
MI_ATTRO             "A Attribut        "
MI_TEMPLATE          "M Mall            "
MI_ORIGIN            "O Originrader     "
MI_QUITMESSAGE       "E Avsluta   / ESC "
ST_TEMPLATES         " Mallar "
WT_CHANGETEMPLATES   "Byt standardmall"
WT_CARBONCOPY        " Kopia "
MI_CCPROCESS         "H Hantera CC   "
MI_CCIGNORE          "S Skippa  CC   "
MI_CCATTRIBS         "a �ndra attrib "
MI_CCLISTFMT         "L Listformat   "
WT_CCLIST            " CC-lista "
MI_CCLISTKEEP        "B Beh�ll     "
MI_CCLISTNAMES       "E Endast namn"
MI_CCLISTVISIBLE     "S Synlig     "
MI_CCLISTHIDDEN      "D Dold       "
MI_CCLISTREMOVE      "T Ta bort    "
WT_ATTRTITLE         " Attribut "
MI_ATTR01            " Privat              <Alt-P>   Archive/Sent         <Alt-B> "
MI_ATTR02            " Crash               <Alt-C>   Direct               <Alt-D> "
MI_ATTR03            " Mottagen            <Alt-R>   Zonegate             <Alt-G> "
MI_ATTR04            " S�nd                <Alt-S>   Hub/Host-Route       <Alt-V> "
MI_ATTR05            " Fil bifogad         <Alt-A>   Immediate            <Alt-I> "
MI_ATTR06            " Transit             <Alt-J>   Xmail                <Alt-X> "
MI_ATTR07            " Orphan              <Alt-O>   Erase File/Sent      <Alt-E> "
MI_ATTR08            " Kill/Sent           <Alt-K>   Trunc File/Sent      <Alt-T> "
MI_ATTR09            " Local               <Alt-W>   Locked               <Alt-L> "
MI_ATTR10            " Hold                <Alt-H>   Confirm Rcpt Request <Alt-Y> "
MI_ATTR11            " File Request        <Alt-F>   FTS1 Reserved        <Alt-1> "
MI_ATTR12            " Return Rcpt Request <Alt-M>   QBBS Reserved, Net   <Alt-2> "
MI_ATTR13            " Return Rcpt         <Alt-N>   QBBS Reserved, Echo  <Alt-3> "
MI_ATTR14            " Audit Request       <Alt-Q>   Squish Scanned       <Alt-4> "
MI_ATTR15            " File Update Request <Alt-U>   Rensa attribut       <Alt-Z> "
HD_FROM              " Fr�n : "
HD_TO                " Till : "
HD_SUBJ              " �mne : "
HD_FILE              " Fil  : "
MS_EMPTYMSG          "\r"
MS_AUTOATTACHMSG     "\r"
MS_AUTOREQUESTMSG    "\r"
MS_AUTOUPDREQMSG     "\r"
WT_FILEATTACH        " Bifoga fil "
WT_FILEREQUEST       " Efterfr�ga fil "
WT_FILEUPDREQ        " Efterfr�ga uppdatering "
ST_FILEATTACHING     "Bifogar [%i/%i] till %u:%u/%u.%u"
ST_FILEREQUESTING    "Efterfr�gar [%i/%i] fr�n %u:%u/%u.%u"
ST_FILEUPDREQING     "Efterfr�gar uppdatering [%i/%i] fr�n %u:%u/%u.%u"
ST_READMARKED        "L�smarkerad - Text %u av %u (%u kvar)"
ST_READALL           "L�s alla - Text %u av %u (%u kvar)"
ST_NOQRENUM          "M�tet kan inte omnumreras"
MS_HIDINGTWIT        "Denna text �r filtrerad - Tryck <Home> f�r att l�sa."
; Alternative from Mik
; MS_HIDINGTWIT        "Detta �r ett Twittmeddelande - Tryck <Home> f�r att l�sa."
HD_VIA               "via"
WT_CHANGEATTRS       "�ndra textattribut"
WT_HEADERTEXT        " Ange s�kstr�ng (huvud+text) "
WT_HEADERONLY        " Ange s�kstr�ng (endast huvud) "
WT_NEWAREA           ">>V�lj nytt m�te: "
WT_REPLYAREA         ">>Svara i m�te: "
WT_COPYAREA          ">>Kopia till m�te: "
WT_MOVEAREA          ">>Flytta till m�te: "
WT_FORWARDAREA       ">>Vidarebefordra till m�te: "
WT_FREQAREA          ">>Efterfr�ga fil i m�te: "
WT_FREQMENUTITLE     "Efterfr�gbara filer"
ST_FREQSTAT          "V�lj filer att efterfr�ga"
IL_FREQINFONOFILES   " Inga filer hittades i texten! "
WT_COPY              " Kopiera "
WT_MOVE              " Flytta "
WT_COPYING           " Kopierar "
WT_MOVING            " Flyttar "
ST_COPYINGMSG        "Kopierar text %u av %u till %s"
ST_MOVINGMSG         "Flyttar text %u av %u till %s"
WT_DELETE            " Ta bort "
WT_DELETING          " Tar bort "
ST_DELETINGMSG       "Tar bort text %u av %u"
WT_WRITE             " Skriv "
WT_WRITEMSGS         " Skriv text(er) till fil "
WT_WRITING           " Skriver "
ST_WRITINGMSG        "Skriver text %u av %u"
WT_WRITINGFILE       " Skriver text(er) till filen %s "
WT_WRITINGPRN        " Skriver ut text(er) "
IL_READONLYWARN      " M�tet �r SKRIVSKYDDAT! "
WT_ISREADONLY        " Skriva �nd�? "
MI_READONLYYES       "J        Ja, tack.       "
MI_READONLYNO        "N          Nej!          "
IL_CHANGEWARN        " Meddelandet �r INTE fr�n dig! "
WT_CHANGE            " �ndra meddelandet? "
MI_CHANGEYES         "J        Ja, tack.       "
MI_CHANGENO          "N          Nej!          "
WT_DELETETHIS        " Radera meddelandet? "
MI_DELETEYES         "J        Ja, tack.       "
MI_DELETENO          "N          Nej!          "
MI_DELETENOASK       "F       Fr�ga inte..     "
WT_GOTONEXT          " G� till n�sta m�te? "
MI_GOTONEXTYES       "J        Ja, tack.       "
MI_GOTONEXTNO        "N          Nej!          "
MI_GOTONEXTNEW       "o Ja, n�sta med ol�sta i "
WT_FORWARD           " Anv�nda FWD-m�rkning? "
MI_FORWARDYES        "J        Ja, tack.       "
MI_FORWARDNO         "N          Nej!          "
WT_MSG               "Text"
WT_MSGREAL           "Text#"
WT_FROML             "Fr�n"
WT_TOL               "Till"
WT_SUBJL             "�mne"
ST_MSGLISTER         "Listare - text %u av %u (%u kvar)"
ST_COPYMOVEFORWARD   "Kopiera, flytta eller vidarebefordra aktuell text"
WT_SELECTACTION      " V�lj "
MI_FORWARDMESSAGE    "V Vidarebef.  "
MI_MOVEMESSAGE       "F Flytta      "
MI_COPYMESSAGE       "K Kopiera     "
MI_QUITCMF           "E Avbryt/ ESC "
ST_ARE               "meddelanden har"
ST_IS                "meddelande har"
ST_MARKED            "markerats"
MI_MARKEDMSG         "M Markerade   "
MI_CURRENTMSG        "A Aktuell text"
MI_QUITMSGS          "E Avbryt/ ESC "
ST_WRITEMSGSTO       "Skriv text(er) till fil eller skrivare"
WT_WRITETO           " Skriv till: "
MI_DISKFILE          "D Fil p� disk  "
MI_PRINTER           "S Skrivare     "
MI_QUITWRITE         "E Avbryt / ESC "
ST_MARKINGOPTIONS    "Markera texter"
WT_MARKWHAT          " Markera vad? "
MI_YOURMAIL          "D Din personliga post    "
MI_FROMTOSUBJ        "H Huvud (fr�n/till/�mne) "
MI_TEXTHDR           "T Text och huvud         "
MI_THREAD            "K Kommentarstr�d         "
MI_NEWMSGS           "N Nya texter   >aktuellt "
MI_OLDMSGS           "G Gamla texter <aktuellt "
MI_ALLMSGS           "A Alla texter            "
MI_UNMARK            "v Avmarkera alla texter  "
MI_RANGE             "F F�ljd (bokm�rke-nuv.)  "
MI_MARKSTOGGLE       "m V�nd alla markeringar  "
MI_QUITMARKS         "E Avbryt           / ESC "
WT_ENTERMARKSTRING   " Ange markeringsstr�ng "
ST_SEARCHINGMSG      "S�ker text %u av %u. Markerade: %u"
WT_USERLISTNAME      " Filnamn f�r anv�ndarlista? "
IL_GENUSERLIST       " Genererar anv�ndarlista "
WT_FILEEXISTS        " Fil finns redan! "
MI_APPEND            "L L�gg till i slutet      "
MI_OVERWRITE         "S Skriv �ver filen        "
MI_QUITEXIST         "E Ange nytt filnamn / ESC "
IL_WARNUNSENT        " VARNING! Meddelandet �r INTE s�nt! "
IL_WARNLOCKED        " VARNING! Meddelandet �r l�st! "
ST_CHANGEORIGIN      "�ndra standard-Origin"
WT_ORIGINS           " Origin "
ST_CHANGEUSERNAME    "�ndra standardnamn"
WT_USERNAMES         " Namn "
ST_CHANGEAKA         "�ndra standardadress"
WT_AKAS              " Adresser "
WT_LOOKUP            " Sl� upp: %s "
WL_PHONE             "Telefon"
WT_QUITGOLDED        " Avsluta GoldED? "
MI_QUITYES           "J        Ja, tack.       "
MI_QUITNO            "N          Nej!          "
MS_EDITCMD           "Redigeringsprogram: %s"
IL_NOORIGDEFINED     " Inga Originrader definierade. Tryck tangent "
IL_NOUSERDEFINED     " Inga namn definierade. Tryck tangent "
IL_NOAKADEFINED      " Inga adresser definierade. Tryck tangent "
IL_NOTPLDEFINED      " Inga mallar definierade. Tryck tangent "
IL_NOTHREADLIST      " Inget tr�d tillg�nglig. Tryck tangent "
MS_SKIPPINGTWIT      "Hoppar �ver filtrerade texter..."
//
MS_SKIPPINGDELETED   "Hoppar �ver d�das texter..."
MS_KILLINGTWIT       "D�dar filtrerade texter..."
IL_WRITINGCFG        " Skriver inst�llningar "
IL_COULDNOTOPEN      " Kunde inte �ppna %s "
MS_PROMPT            "Ange kommandot \"exit\" f�r att �terg�t till GoldED.\r\n"
IL_UNFINISHEDMSG     " Avbruten text funnen! "
ST_LOADUNFINISHED    "Tryck tangent f�r att ladda eller <ESC> f�r att ignorera"
IL_READINGECHOLIST   "L�ser m�teslista"
IL_READINGADDRMACROS "L�ser adressmakron"
IL_CHECKINGNODELISTS "Kollar nodlistor"
ST_CROSSPOSTING      "Korspostar i %s"
IL_TWITBLANKED       " Filtrerade texter blankas ut "
IL_TWITSKIPPED       " Filtrerade texter �verhoppas "
IL_TWITIGNORESKIP    " Filtrerade texter ignoreras och �verhoppas "
IL_TWITDISPLAYED     " Filtrerade texter visas "
IL_TWITKILLED        " Filtrerade texter d�das "
IL_STYLECODESNO      " Stilkoder avst�ngda "
IL_STYLECODESYES     " Stilkoder p�slagna "
IL_STYLECODESHIDE    " Stilkoder p�slagna och dolda "
ST_GENCFMRECEIPT     "Skapar leveranskvitto"
WT_NEW               "Nya"
ST_LOOKUPINFO        "Information fr�n nodlista"
MS_DATETIMEFMT       "%y-%m-%d %H.%M.%S"
MS_DATEFMT           "%Y-%m-%d"
MS_TIMEFMT           "%H.%M"
MS_CROSSPOSTEDIN     "* Korspostat i %s"
ST_STATUSLINEHELP    "F1 Hj�lp"
HD_OF                "av"
MS_ROBOTMSG          "\r--- @longpid @version\r"
ST_STATUSLINETIMEFMT "%H.%M.%S"
WT_REPLIES           " Svar "
ST_SELECTREPLY       "V�lj svaret du vill se"
IL_WAITOREXIT        " At this point you must either wait or exit GoldED entirely - Sorry! "
WT_REALLYEXIT        " Verkligen avsluta? "
MI_TAGLINES          "t �ndra tagline "
MI_HEADEREDIT        "h �ndra huvud"
MI_SCANGROUP         "g Avs�k grupp"
WT_QWKPACKET         " QWK-paket "
MI_SCANQWK           "Q QWK-paket "
WT_SOUPPACKET        " SOUP-paket "
MI_SCANSOUP          "S SOUP-paket "
MI_SCANIMPORT        "I Import"
MI_SCANEXPORT        "E Export"
WT_SCANPM            " Scan PM "
MI_TAGLINE           "g Taglines"
WT_CONFIRM           " Skicka kvitto enligt f�rfr�gan? "
MI_CONFIRMYES        "J Ja, skicka ett kvitto   "
MI_CONFIRMNO         "N Nej, ignorera f�rfr�gan "
IL_CONFIRMINFO       " S�ndaren av meddelandet �nskar bevis p� mottagning (CFM) "
ST_MSG               "text"
ST_MSGS              "texter"
ST_PERSONAL          "personliga"
WT_GRP               "Grp"
IL_FOUNDPERSONAL     " Found %u personal mail%s in %u area%s "
IL_NOPERSONAL        " Inga personliga brev funna "
ST_ESCORCONTINUE     "ESC avbryter. Andra tangenter: f�rs�ker forts�tta"
MS_SPELLCHECKER      "Stavningskollr: %s"
WT_INS               " Ins "
WT_DRAWSL            " DL1 "
WT_DRAWDL            " DL2 "
MI_FILELISTFROM      "Fillista fr�n %s"
ST_INITIALIZING      "Initierar ..."
ST_CHECKING          "Checking"
ST_LOCKSHARECAP      "%s Lock/Share Capability"
WT_ATTRTURNOFF       " Tryck Alt-F1 f�r att st�nga av detta f�nster "
WT_TAGLINES          " Taglines "
ST_CHANGETAGLINE     "�ndra standardtagline"
IL_NOTAGLINE         " Inga taglines definierade. Tryck tangent "
WT_CHARSETS          " Teckenupps�ttningar "
ST_CHANGEXLATIMP     "�ndra standardteckenupps�ttning f�r import"
IL_NOXLATIMPORT      " Inga XLATIMPORT definierade. Tryck tangent "
WT_N_A               "saknas"
WT_WRITTEN           "Skrivet"
WT_ARRIVED           "Levererat"
WT_RECEIVED          "Mottaget"
IL_NONODELIST        " Kan inte komma �t nodlista eller indexfiler "
IL_NODELISTMISSING   "Nodlista saknas:"
IL_NODELISTOUTDATED  "Nodlista gammal:"
MS_REPLYLINKER       "Kommentarsl�nkare: %s"
WT_ENTERMSGNO        " Ange textnummer "
IL_WAITUUDECODING    " V�nta - avkodar aktuell text "
IL_COMPLETEDUUDECODE " UU-avkodning av %s klar "
IL_NOTUUENCODED      " Hittade inget att avkoda i textn "
IL_UUEINVALIDPATH    " Ogiltig s�kv�g f�r uu-avkodning - fil ej skriven "
; Alternative from Mik
; IL_UUEINVALIDPATH    " Ogilltig s�kv�g f�r uuavkodning - filen sparades ej "
IL_TMPINVALIDPATH    " Ogilltig s�kv�g f�r TMPPATH "
IL_ERRORINSOUP       " Fel i SOUP-fil %s "
IL_WARNALREADYSENT   " VARNING: Meddelandet �r redan s�nt! "
IL_WAITLOCKED        " V�nta: %s �r l�st "
ST_RETRYORESC        "%s retry (%lu). Press ESC to exit GoldED."
ST_RETRYLOCK         "L�st"
ST_RETRYOPEN         "�ppen"
WT_TOUCHSEMAPHORE    " Enter name of semaphore file to touch "
MI_WRITETOHDRNO      "N Anv huvud: NEJ  "
MI_WRITETOHDRYES     "J Anv huvud: JA   "
MI_WRITETOHDRONLY    "O Anv huvud: ONLY "
MI_CLIPBOARD         "K Klippbord "
WT_SELECTMARKS       " V�lj markering "
WT_SELECTMARKSEDIT   " Redigera markeringsbeskrivning "
IL_DROPMARKSINFO     " %s meddelanden markerade "
WT_DROPMARKS         " Drop Msg Marks "
MI_DROPALL           "A Drop All       "
MI_DROPMARKED        "M Drop Marked    "
MI_DROPCURRENT       "C Drop Current   "
MI_NODROP            "N No Drop / ESC  "
WT_CATCHAREAS        " Catch-Up Areas "
MI_CATCHALL          "A Catch All      "
MI_CATCHMARKED       "M Catch Marked   "
MI_CATCHCURRENT      "C Catch Current  "
MI_NOCATCH           "N No Catch / ESC "

----------------------------------------------------------------------

IL_DECODE            "Avkoda"
IL_NOMOREMATCHES     " Inga fler tr�ffar funna "
IL_PATHREPORT        " Genererar PATH-rapport "
IL_PEEKINFONOURLS    " Kunde inte hitta n�gra gilltiga URL:er i detta meddelande! "
;IL_PEEKINFONOHANDLER " URL handler are not defined in config file! "
IL_PREPARING         "F�rbereder"
IL_USERWAIT          " V�nta - Bl�ddrar i adressboken.. "
MI_ALSELECTIONS1     "Markera m�ten med ny post"
MI_ALSELECTIONS2     "(oanv�nd)"
MI_CHARSETAUTO       " Automatisk "
MI_ENTERCMDLINE      " Ange kommandorad "
MI_XCIGNORE          "I Ignorera XC:s "
MI_XCLISTFMT         "L Listformat "
MI_XCLISTLINE        "d Rad      "
MI_XCLISTRAW         "S Spara    "
MI_XCLISTREMOVE      "R Radera   "
MI_XCLISTVERBOSE     "U Utf�rlig "
MI_XCPROCESS         "P Processa XC:s "
MS_CCTO              "* Carbonkopia till %s"
MS_ORIGINALLYIN      "* Ursprungligen postat i %s"
ST_EXTERNUTIL        "V�lj externt program att exekvera"
ST_PEEKURL           "V�lj URL"
ST_USERSTATUSLINE    "GoldEDs adressbok - %d av %d (%d kvar)"
WT_ADDRESSBOOK       " Adressbok "
WT_ADVANCEDMARKING   "Avancerade M�rkningar"
WT_ADVANCEDSEARCH    "Avancerade S�kningar"
WT_CROSSPOST         " Korsposta "
WT_EXECCMDLINE       " Ange kommandorad att exekvera "
WT_EXTERNUTIL        " Externt program "
WT_HEADEREDITHELP_1  " Shift-F10: Nodlista "
WT_HEADEREDITHELP_2  " F10: Adressbok "
WT_PEEKURL           " Tillg�ngliga URLer "
WT_THREADLISTTITLE   " Meddelandetr�dvy"
WT_USERHEADERAKA     "Adress"
WT_USERHEADERNAME    "Namn"
WT_USERHEADERORG     "Oranisation"
WT_XCLIST            " XC Lista "
