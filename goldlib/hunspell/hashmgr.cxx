#include "license.hun"
#include "license.mys"

#include <cstdlib>
#include <cstring>
#include <cctype>
#ifdef HAVE_FCNTL_H
    #include <fcntl.h>
#endif
#include <cstdio>

#include "hashmgr.hxx"
#include "csutil.hxx"

#if !defined(_MSC_VER)
    #include <unistd.h>
    using namespace std;
#endif

// build a hash table from a munched word list

HashMgr::HashMgr(const char * tpath, const char * apath)
{
    tablesize = 0;
    tableptr = NULL;
    flag_mode = FLAG_CHAR;
    complexprefixes = 0;
    utf8 = 0;
    numaliasf = 0;
    aliasf = NULL;
    numaliasm = 0;
    aliasm = NULL;
    load_config(apath);
    int ec = load_tables(tpath);
    if (ec)
    {
        /* error condition - what should we do here */
        fprintf(stderr,"Hash Manager Error : %d\n",ec);
        fflush(stderr);
        if (tableptr)
        {
            free(tableptr);
        }
        tablesize = 0;
    }
}


HashMgr::~HashMgr()
{
    if (tableptr)
    {
        // now pass through hash table freeing up everything
        // go through column by column of the table
        for (int i=0; i < tablesize; i++)
        {
            struct hentry * pt = &tableptr[i];
            struct hentry * nt = NULL;
            if (pt)
            {
                if (pt->astr && !aliasf) free(pt->astr);
                if (pt->word) free(pt->word);
                if (pt->description && !aliasm) free(pt->description);

                pt = pt->next;
            }
            while(pt)
            {
                nt = pt->next;
                if (pt->astr && !aliasf) free(pt->astr);
                if (pt->word) free(pt->word);
                if (pt->description && !aliasm) free(pt->description);
                free(pt);
                pt = nt;
            }
        }
        free(tableptr);
    }
    tablesize = 0;

    if (aliasf)
    {
        for (int j = 0; j < (numaliasf); j++) free(aliasf[j]);
        free(aliasf);
        aliasf = NULL;
        if (aliasflen)
        {
            free(aliasflen);
            aliasflen = NULL;
        }
    }
    if (aliasm)
    {
        for (int j = 0; j < (numaliasm); j++) free(aliasm[j]);
        free(aliasm);
        aliasm = NULL;
    }
}

// lookup a root word in the hashtable

struct hentry * HashMgr::lookup(const char *word) const
{
    struct hentry * dp;
    if (tableptr)
    {
        dp = &tableptr[hash(word)];
        if (dp->word == NULL) return NULL;
        for (  ;  dp != NULL;  dp = dp->next)
        {
            if (strcmp(word,dp->word) == 0) return dp;
        }
    }
    return NULL;
}

// add a word to the hash table (private)

int HashMgr::add_word(const char * word, int wl, unsigned short * aff, int al, const char * desc)
{
    char * st = mystrdup(word);
    if (wl && !st) return 1;
    if (complexprefixes)
    {
        if (utf8) reverseword_utf(st);
        else reverseword(st);
    }
    int i = hash(st);
    struct hentry * dp = &tableptr[i];
    if (dp->word == NULL)
    {
        dp->wlen = wl;
        dp->alen = al;
        dp->word = st;
        dp->astr = aff;
        dp->next = NULL;
        dp->next_homonym = NULL;
        if (aliasm)
        {
            dp->description = (desc) ? get_aliasm(atoi(desc)) : mystrdup(desc);
        }
        else
        {
            dp->description = mystrdup(desc);
            if (desc && !dp->description) return 1;
            if (dp->description && complexprefixes)
            {
                if (utf8) reverseword_utf(dp->description);
                else reverseword(dp->description);
            }
        }
    }
    else
    {
        struct hentry* hp = (struct hentry *) malloc (sizeof(struct hentry));
        if (!hp) return 1;
        hp->wlen = wl;
        hp->alen = al;
        hp->word = st;
        hp->astr = aff;
        hp->next = NULL;
        hp->next_homonym = NULL;
        if (aliasm)
        {
            hp->description = (desc) ? get_aliasm(atoi(desc)) : mystrdup(desc);
        }
        else
        {
            hp->description = mystrdup(desc);
            if (desc && !hp->description) return 1;
            if (dp->description && complexprefixes)
            {
                if (utf8) reverseword_utf(hp->description);
                else reverseword(hp->description);
            }
        }
        while (dp->next != NULL)
        {
            if ((!dp->next_homonym) && (strcmp(hp->word, dp->word) == 0)) dp->next_homonym = hp;
            dp=dp->next;
        }
        if ((!dp->next_homonym) && (strcmp(hp->word, dp->word) == 0)) dp->next_homonym = hp;
        dp->next = hp;
    }
    return 0;
}

// add a custom dic. word to the hash table (public)

int HashMgr::put_word(const char * word, int wl, char * aff)
{
    unsigned short * flags;
    int al = 0;
    if (aff)
    {
        al = decode_flags(&flags, aff);
        flag_qsort(flags, 0, al);
    }
    else
    {
        flags = NULL;
    }
    add_word(word, wl, flags, al, NULL);
    return 0;
}

int HashMgr::put_word_pattern(const char * word, int wl, const char * pattern)
{
    unsigned short * flags;
    struct hentry * dp = lookup(pattern);
    if (!dp || !dp->astr) return 1;
    flags = (unsigned short *) malloc (dp->alen * sizeof(short));
    memcpy((void *) flags, (void *) dp->astr, dp->alen * sizeof(short));
    add_word(word, wl, flags, dp->alen, NULL);
    return 0;
}

// walk the hash table entry by entry - null at end
struct hentry * HashMgr::walk_hashtable(int &col, struct hentry * hp) const
{
    //reset to start
    if ((col < 0) || (hp == NULL))
    {
        col = -1;
        hp = NULL;
    }

    if (hp && hp->next != NULL)
    {
        hp = hp->next;
    }
    else
    {
        col++;
        hp = (col < tablesize) ? &tableptr[col] : NULL;
        // search for next non-blank column entry
        while (hp && (hp->word == NULL))
        {
            col ++;
            hp = (col < tablesize) ? &tableptr[col] : NULL;
        }
        if (col < tablesize) return hp;
        hp = NULL;
        col = -1;
    }
    return hp;
}

// load a munched word list and build a hash table on the fly
int HashMgr::load_tables(const char * tpath)
{
    int wl, al;
    char * ap;
    char * dp;
    unsigned short * flags;

    // raw dictionary - munched file
    FILE * rawdict = fopen(tpath, "r");
    if (rawdict == NULL) return 1;

    // first read the first line of file to get hash table size */
    char ts[MAXDELEN];
    if (! fgets(ts, MAXDELEN-1,rawdict)) return 2;
    mychomp(ts);
    if ((*ts < '1') || (*ts > '9')) fprintf(stderr, "error - missing word count in dictionary file\n");
    tablesize = atoi(ts);
    if (!tablesize) return 4;
    tablesize = tablesize + 5 + USERWORD;
    if ((tablesize %2) == 0) tablesize++;

    // allocate the hash table
    tableptr = (struct hentry *) calloc(tablesize, sizeof(struct hentry));
    if (! tableptr) return 3;
    for (int i=0; i<tablesize; i++) tableptr[i].word = NULL;

    // loop through all words on much list and add to hash
    // table and create word and affix strings

    while (fgets(ts,MAXDELEN-1,rawdict))
    {
        mychomp(ts);
        // split each line into word and morphological description
        dp = strchr(ts,'\t');

        if (dp)
        {
            *dp = '\0';
            dp++;
        }
        else
        {
            dp = NULL;
        }

        // split each line into word and affix char strings
        // "\/" signs slash in words (not affix separator)
        // "/" at beginning of the line is word character (not affix separator)
        ap = ts;
        while ((ap = strchr(ap,'/')))
        {
            if (ap == ts)
            {
                ap++;
                continue;
            }
            else if (*(ap - 1) != '\\') break;
            // replace "\/" with "/"
            for (char * sp = ap - 1; *sp; *sp = *(sp + 1), sp++);

        }

        if (ap)
        {
            *ap = '\0';
            if (aliasf)
            {
                int index = atoi(ap + 1);
                al = get_aliasf(index, &flags);
                if (!al)
                {
                    fprintf(stderr, "error - bad flag vector alias: %s\n", ts);
                    *ap = '\0';
                }
            }
            else
            {
                al = decode_flags(&flags, ap + 1);
                flag_qsort(flags, 0, al);
            }
        }
        else
        {
            al = 0;
            ap = NULL;
            flags = NULL;
        }

        wl = strlen(ts);

        // add the word and its index
        if (add_word(ts,wl,flags,al,dp)) return 5;

    }

    fclose(rawdict);
    return 0;
}


// the hash function is a simple load and rotate
// algorithm borrowed

int HashMgr::hash(const char * word) const
{
    long  hv = 0;
    for (int i=0; i < 4  &&  *word != 0; i++)
        hv = (hv << 8) | (*word++);
    while (*word != 0)
    {
        ROTATE(hv,ROTATE_LEN);
        hv ^= (*word++);
    }
    return (unsigned long) hv % tablesize;
}

int HashMgr::decode_flags(unsigned short ** result, char * flags)
{
    int len;
    switch (flag_mode)
    {
    case FLAG_LONG:   // two-character flags (1x2yZz -> 1x 2y Zz)
    {
        len = strlen(flags);
        if (len%2 == 1) fprintf(stderr,"error: length of FLAG_LONG flagvector is odd: %s\n", flags);
        len = len/2;
        *result = (unsigned short *) malloc(len * sizeof(short));
        for (int i = 0; i < len; i++)
        {
            (*result)[i] = (((unsigned short) flags[i * 2]) << 8) + (unsigned short) flags[i * 2 + 1];
        }
        break;
    }
    case FLAG_NUM:   // decimal numbers separated by comma (4521,23,233 -> 4521 23 233)
    {
        len = 1;
        char * src = flags;
        unsigned short * dest;
        char * p;
        for (p = flags; *p; p++)
        {
            if (*p == ',') len++;
        }
        *result = (unsigned short *) malloc(len * sizeof(short));
        dest = *result;
        for (p = flags; *p; p++)
        {
            if (*p == ',')
            {
                *dest = (unsigned short) atoi(src);
                if (*dest == 0) fprintf(stderr, "error: 0 is wrong flag id\n");
                src = p + 1;
                dest++;
            }
        }
        *dest = (unsigned short) atoi(src);
        if (*dest == 0) fprintf(stderr, "error: 0 is wrong flag id\n");
        break;
    }
    case FLAG_UNI:   // UTF-8 characters
    {
        w_char w[MAXDELEN/2];
        len = u8_u16(w, MAXDELEN/2, flags);
        *result = (unsigned short *) malloc(len * sizeof(short));
        memcpy(*result, w, len * sizeof(short));
        break;
    }
    default:   // Ispell's one-character flags (erfg -> e r f g)
    {
        unsigned short * dest;
        len = strlen(flags);
        *result = (unsigned short *) malloc(len * sizeof(short));
        dest = *result;
        for (unsigned char * p = (unsigned char *) flags; *p; p++)
        {
            *dest = (unsigned short) *p;
            dest++;
        }
    }
    }
    return len;
}

unsigned short HashMgr::decode_flag(const char * f)
{
    unsigned short s = 0;
    switch (flag_mode)
    {
    case FLAG_LONG:
        s = ((unsigned short) f[0] << 8) + (unsigned short) f[1];
        break;
    case FLAG_NUM:
        s = (unsigned short) atoi(f);
        break;
    case FLAG_UNI:
        u8_u16((w_char *) &s, 1, f);
        break;
    default:
        s = (unsigned short) *((unsigned char *)f);
    }
    if (!s) fprintf(stderr, "error: 0 is wrong flag id\n");
    return s;
}

char * HashMgr::encode_flag(unsigned short f)
{
    unsigned char ch[10];
    if (f==0) return mystrdup("(NULL)");
    if (flag_mode == FLAG_LONG)
    {
        ch[0] = (unsigned char) (f >> 8);
        ch[1] = (unsigned char) (f - ((f >> 8) << 8));
        ch[2] = '\0';
    }
    else if (flag_mode == FLAG_NUM)
    {
        sprintf((char *) ch, "%d", f);
    }
    else if (flag_mode == FLAG_UNI)
    {
        u16_u8((char *) &ch, 10, (w_char *) &f, 1);
    }
    else
    {
        ch[0] = (unsigned char) (f);
        ch[1] = '\0';
    }
    return mystrdup((char *) ch);
}

// read in aff file and set flag mode
int  HashMgr::load_config(const char * affpath)
{

    // io buffers
    char line[MAXDELEN+1];

    // open the affix file
    FILE * afflst;
    afflst = fopen(affpath,"r");
    if (!afflst)
    {
        fprintf(stderr,"Error - could not open affix description file %s\n",affpath);
        return 1;
    }

    // read in each line ignoring any that do not
    // start with a known line type indicator

    while (fgets(line,MAXDELEN,afflst))
    {
        mychomp(line);

        /* parse in the try string */
        if ((strncmp(line,"FLAG",4) == 0) && isspace(line[4]))
        {
            if (flag_mode != FLAG_CHAR)
            {
                fprintf(stderr,"error: duplicate FLAG parameter\n");
            }
            if (strstr(line, "long")) flag_mode = FLAG_LONG;
            if (strstr(line, "num")) flag_mode = FLAG_NUM;
            if (strstr(line, "UTF-8")) flag_mode = FLAG_UNI;
            if (flag_mode == FLAG_CHAR)
            {
                fprintf(stderr,"error: FLAG need `num', `long' or `UTF-8' parameter: %s\n", line);
            }
        }
        if ((strncmp(line,"SET",3) == 0) && isspace(line[3]) && strstr(line, "UTF-8")) utf8 = 1;

        if ((strncmp(line,"AF",2) == 0) && isspace(line[2]))
        {
            if (parse_aliasf(line, afflst))
            {
                return 1;
            }
        }

        if ((strncmp(line,"AM",2) == 0) && isspace(line[2]))
        {
            if (parse_aliasm(line, afflst))
            {
                return 1;
            }
        }

        if (strncmp(line,"COMPLEXPREFIXES",15) == 0) complexprefixes = 1;
        if (((strncmp(line,"SFX",3) == 0) || (strncmp(line,"PFX",3) == 0)) && isspace(line[3])) break;
    }
    fclose(afflst);
    return 0;
}

/* parse in the ALIAS table */
int  HashMgr::parse_aliasf(char * line, FILE * af)
{
    if (numaliasf != 0)
    {
        fprintf(stderr,"error: duplicate AF (alias for flag vector) tables used\n");
        return 1;
    }
    char * tp = line;
    char * piece;
    int i = 0;
    int np = 0;
    while ((piece=mystrsep(&tp, 0)))
    {
        if (*piece != '\0')
        {
            switch(i)
            {
            case 0:
            {
                np++;
                break;
            }
            case 1:
            {
                numaliasf = atoi(piece);
                if (numaliasf < 1)
                {
                    numaliasf = 0;
                    aliasf = NULL;
                    aliasflen = NULL;
                    fprintf(stderr,"incorrect number of entries in AF table\n");
                    free(piece);
                    return 1;
                }
                aliasf = (unsigned short **) malloc(numaliasf * sizeof(unsigned short *));
                aliasflen = (unsigned short *) malloc(numaliasf * sizeof(short));
                if (!aliasf || !aliasflen)
                {
                    numaliasf = 0;
                    if (aliasf) free(aliasf);
                    if (aliasflen) free(aliasflen);
                    aliasf = NULL;
                    aliasflen = NULL;
                    return 1;
                }
                np++;
                break;
            }
            default:
                break;
            }
            i++;
        }
        free(piece);
    }
    if (np != 2)
    {
        numaliasf = 0;
        free(aliasf);
        free(aliasflen);
        aliasf = NULL;
        aliasflen = NULL;
        fprintf(stderr,"error: missing AF table information\n");
        return 1;
    }

    /* now parse the numaliasf lines to read in the remainder of the table */
    char * nl = line;
    for (int j=0; j < numaliasf; j++)
    {
        if (!fgets(nl,MAXDELEN,af)) return 1;
        mychomp(nl);
        tp = nl;
        i = 0;
        aliasf[j] = NULL;
        aliasflen[j] = 0;
        while ((piece=mystrsep(&tp, 0)))
        {
            if (*piece != '\0')
            {
                switch(i)
                {
                case 0:
                {
                    if (strncmp(piece,"AF",2) != 0)
                    {
                        numaliasf = 0;
                        free(aliasf);
                        free(aliasflen);
                        aliasf = NULL;
                        aliasflen = NULL;
                        fprintf(stderr,"error: AF table is corrupt\n");
                        free(piece);
                        return 1;
                    }
                    break;
                }
                case 1:
                {
                    aliasflen[j] = decode_flags(&(aliasf[j]), piece);
                    flag_qsort(aliasf[j], 0, aliasflen[j]);
                    break;
                }
                default:
                    break;
                }
                i++;
            }
            free(piece);
        }
        if (!aliasf[j])
        {
            free(aliasf);
            free(aliasflen);
            aliasf = NULL;
            aliasflen = NULL;
            numaliasf = 0;
            fprintf(stderr,"error: AF table is corrupt\n");
            return 1;
        }
    }
    return 0;
}

/* parse morph alias definitions */
int  HashMgr::parse_aliasm(char * line, FILE * af)
{
    if (numaliasm != 0)
    {
        fprintf(stderr,"error: duplicate AM (aliases for morphological descriptions) tables used\n");
        return 1;
    }
    char * tp = line;
    char * piece;
    int i = 0;
    int np = 0;
    while ((piece=mystrsep(&tp, 0)))
    {
        if (*piece != '\0')
        {
            switch(i)
            {
            case 0:
            {
                np++;
                break;
            }
            case 1:
            {
                numaliasm = atoi(piece);
                if (numaliasm < 1)
                {
                    fprintf(stderr,"incorrect number of entries in AM table\n");
                    free(piece);
                    return 1;
                }
                aliasm = (char **) malloc(numaliasm * sizeof(char *));
                if (!aliasm)
                {
                    numaliasm = 0;
                    return 1;
                }
                np++;
                break;
            }
            default:
                break;
            }
            i++;
        }
        free(piece);
    }
    if (np != 2)
    {
        numaliasm = 0;
        free(aliasm);
        aliasm = NULL;
        fprintf(stderr,"error: missing AM alias information\n");
        return 1;
    }

    /* now parse the numaliasm lines to read in the remainder of the table */
    char * nl = line;
    for (int j=0; j < numaliasm; j++)
    {
        if (!fgets(nl,MAXDELEN,af)) return 1;
        mychomp(nl);
        tp = nl;
        i = 0;
        aliasm[j] = NULL;
        while ((piece=mystrsep(&tp, 0)))
        {
            if (*piece != '\0')
            {
                switch(i)
                {
                case 0:
                {
                    if (strncmp(piece,"AM",2) != 0)
                    {
                        fprintf(stderr,"error: AM table is corrupt\n");
                        free(piece);
                        numaliasm = 0;
                        free(aliasm);
                        aliasm = NULL;
                        return 1;
                    }
                    break;
                }
                case 1:
                {
                    if (complexprefixes)
                    {
                        if (utf8) reverseword_utf(piece);
                        else reverseword(piece);
                    }
                    aliasm[j] = mystrdup(piece);
                    break;
                }
                default:
                    break;
                }
                i++;
            }
            free(piece);
        }
        if (!aliasm[j])
        {
            numaliasm = 0;
            free(aliasm);
            aliasm = NULL;
            fprintf(stderr,"error: map table is corrupt\n");
            return 1;
        }
    }
    return 0;
}

int HashMgr::is_aliasf()
{
    return (aliasf != NULL);
}

int HashMgr::is_aliasm()
{
    return (aliasm != NULL);
}

int HashMgr::get_aliasf(int index, unsigned short ** fvec)
{
    if ((index > 0) && (index <= numaliasf))
    {
        *fvec = aliasf[index - 1];
        return aliasflen[index - 1];
    }
    fprintf(stderr,"error: bad flag alias index: %d\n", index);
    fprintf(stderr,"hiba: %d\n", index);
    *fvec = NULL;
    return 0;
}

char * HashMgr::get_aliasm(int index)
{
    if ((index > 0) && (index <= numaliasm)) return aliasm[index - 1];
    fprintf(stderr,"error: bad morph. alias index: %d\n", index);
    return NULL;
}
