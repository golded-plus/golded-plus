using namespace std;

#include <cctype>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>

typedef struct {
  char name[256];
  int replacement[3];
} unicodechar;

typedef int charset[256];

charset from_charset, to_charset;
unicodechar unicodedata[65536], alt_unicodedata[65536];

static int atox(const char *str)
{
  unsigned int result = 0;
  int i = 0;

  while(isxdigit(str[i]))
    {
       result <<= 4;
       result |= isdigit(str[i]) ? (str[i] - '0') : (toupper(str[i]) - 'A' + 10);
       i++;
    }
  return (int)result;
}

static void load_charset(const char *fn, charset &recode)
{
  int chr;
  for(chr = 0; chr < 128; chr++)
    recode[chr] = chr;
  for(chr = 128; chr < 256; chr++)
    recode[chr] = 0;

  ifstream file(fn);
  if(file)
    {
      char str[1024];
      while(!file.eof())
        {
          str[0] = '\0';
          file.getline(str, sizeof(str));
          if((str[0] == '\0') || (str[0] == '#'))
            continue;
          if(str[0] == '0')
            chr = atox(str + 2);
          else if(str[0] == '=')
            chr = atox(str + 1);
          else
            {
              cerr << "error in input file (string: \"" << str << '\"' << endl;
              continue;
            }
          char *ustr = strstr(str, "U+");
          if(ustr == NULL)
            ustr = strstr(str, "u+");
          if(ustr == NULL)
            {
              cerr << "error in input file (string: \"" << str << '\"' << endl;
              continue;
            }
          recode[chr] = atox(ustr + 2);
        }
    }
  else
    {
      cerr << "error opening file " << fn << endl;
    }
}

static int lookup_char(int unicode_char)
{
  int i;
  bool found = false;
  for(i = 0; i < 256; i++)
    {
      if(to_charset[i] == unicode_char)
        {
          found = true;
          break;
        }
    }
  if(!found)
    {
      i = -1;
    }
  return i;
}

static int lookup_char_by_name(const char *unicode_name)
{
  int i;
  bool found = false;
  for(i = 0; i < 256; i++)
    {
      if(strcmp(unicodedata[to_charset[i]].name, unicode_name) == 0)
        {
          found = true;
          break;
        }
    }
  if(!found)
    {
      i = -1;
    }
  return i;
}

static char *mystrtok(char *str, char *delim)
{
  static char *last = NULL;

  if(str == NULL)
    str = last;

  if(str == NULL)
    return str;

  last = strpbrk(str, delim);
  if(last != NULL)
    {
      *last = '\0';
      last++;
    }
  return str;
}

static void load_unicodedata(const char *fn, unicodechar *udata)
{
  memset(udata, 0, sizeof(unicodechar) * 65536);

  ifstream file(fn);
  if(file)
    {
      int chr;
      char str[1024];
      while(!file.eof())
        {
          str[0] = '\0';
          file.getline(str, sizeof(str));
          if((str[0] == '\0') || (str[0] == '#'))
            continue;
          char *tok = mystrtok(str, ";");
          if(tok == NULL)
            continue;
          chr = atox(tok);
          if(chr > 65536)
            continue;
          tok = mystrtok(NULL, ";");
          if(tok == NULL)
            continue;
          strncpy(udata[chr].name, tok, sizeof(udata[chr].name));
          udata[chr].name[sizeof(udata[chr].name) - 1] = '\0';
          tok = mystrtok(NULL, ";");
          if(tok == NULL)
            continue;
          tok = mystrtok(NULL, ";");
          if(tok == NULL)
            continue;
          tok = mystrtok(NULL, ";");
          if(tok == NULL)
            continue;
          tok = mystrtok(NULL, ";");
          if(tok == NULL)
            continue;
          if((tok[0] != '\0') && !isxdigit(tok[0]))
            {
              tok = strchr(tok, '>');
              if(tok == NULL)
                {
                  continue;
                }
              while((tok[0] != '\0') && !isxdigit(tok[0]))
                {
                  tok++;
                }
            }
          if(tok[0] != '\0')
            {
              udata[chr].replacement[0] = atox(tok);
              while(isxdigit(tok[0]))
                {
                  tok++;
                }
            }
          while((tok[0] != '\0') && !isxdigit(tok[0]))
            {
              tok++;
            }
          if(tok[0] != '\0')
            {
              udata[chr].replacement[1] = atox(tok);
              while(isxdigit(tok[0]))
                {
                  tok++;
                }
            }
          while((tok[0] != '\0') && !isxdigit(tok[0]))
            {
              tok++;
            }
          if(tok[0] != '\0')
            {
              udata[chr].replacement[2] = atox(tok);
            }
        }
    }
  else
    {
      cerr << "error opening file " << fn << endl;
    }
}

static void generate_table(const char *cp1, const char *cp2)
{
  char incp[256], outcp[256], fn[sizeof(incp) + sizeof(outcp) + 4];
  const char *slash = strrchr(cp1, '/');
  const char *backslash = strrchr(cp1, '\\');
  if((slash != NULL) && (backslash != NULL))
    {
      if(slash < backslash)
        slash = backslash;
    }
  else if(slash == NULL)
    slash = backslash;
  strncpy(incp, (slash == NULL) ? cp1 : (slash + 1), sizeof(incp));
  incp[sizeof(incp) - 1] = '\0';
  char *dot = strchr(incp, '.');
  if(dot != NULL)
    dot[0] = '\0';

  slash = strrchr(cp2, '/');
  backslash = strrchr(cp2, '\\');
  if((slash != NULL) && (backslash != NULL))
    {
      if(slash < backslash)
        slash = backslash;
    }
  else if(slash == NULL)
    slash = backslash;
  strncpy(outcp, (slash == NULL) ? cp2 : (slash + 1), sizeof(outcp));
  outcp[sizeof(outcp) - 1] = '\0';
  dot = strchr(outcp, '.');
  if(dot != NULL)
    dot[0] = '\0';

  strcpy(fn, incp);
  strcat(fn, "_");
  strcat(fn, outcp);
  strcat(fn, ".chs");

  ofstream file(fn);
  if(file)
    {
      int chr = 0;

      file << ";" << endl
        << "; This file is a charset conversion module in text form." << endl
        << ";" << endl
        << "; Automatically generated." << endl
        << ";" << endl
        << "0" << endl
        << "0" << endl
        << ";" << endl;

      if(memcmp(from_charset, to_charset, 128 * sizeof(int)) == 0)
        chr = 128;

      file << ((chr == 128) ? 2 : 1) << endl
        << incp << "\t; from charset" << endl
        << outcp << "\t; to charset" << endl
        << ";" << endl;

      for(; chr < 256; chr++)
        {
          int i;

          if((chr != 0) && (from_charset[chr] == 0))
            {
              file << "\\0 ?" << "\t; not defined" << endl;
              continue;
            }
          // try exact match first
          i = lookup_char(from_charset[chr]);
          if(i != -1)
            {
              file << "\\0 \\x" << hex << i << "\t; "
                << unicodedata[from_charset[chr]].name << endl;
              continue;
            }
          // if not found exact match try to compose
          int replacement_char1 = unicodedata[from_charset[chr]].replacement[0];
          if(replacement_char1 != 0)
            {
              i = lookup_char(replacement_char1);
              if(i != -1)
                {
                  int replacement_char2 = unicodedata[from_charset[chr]].replacement[1];
                  if(replacement_char2 == 0)
                    {
                      file << "\\0 \\x" << hex << i << "\t; "
                        << unicodedata[replacement_char1].name << endl;
                      continue;
                    }
                  int j = lookup_char(replacement_char2);
                  if(j != -1)
                    {
                      int replacement_char3 = unicodedata[from_charset[chr]].replacement[2];
                      if(replacement_char3 == 0)
                        {
                          file << "\\x" << hex << i << " \\x" << hex << j << "\t; "
                            << unicodedata[replacement_char1].name << " + "
                            << unicodedata[replacement_char2].name << endl;
                          continue;
                        }
                      int k = lookup_char(replacement_char3);
                      if(k != -1)
                        {
                          file << "\\x" << hex << i << " \\x" << hex << j << " \\x" << hex << k << "\t; "
                            << unicodedata[replacement_char1].name << " + "
                            << unicodedata[replacement_char2].name << " + "
                            << unicodedata[replacement_char3].name << endl;
                          continue;
                        }
                    }
                }
            }
          // if unable to compose through the standart data try our own
          replacement_char1 = alt_unicodedata[from_charset[chr]].replacement[0];
          if(replacement_char1 != 0)
            {
              i = lookup_char(replacement_char1);
              if(i != -1)
                {
                  int replacement_char2 = alt_unicodedata[from_charset[chr]].replacement[1];
                  if(replacement_char2 == 0)
                    {
                      file << "\\0 \\x" << hex << i << "\t; "
                        << unicodedata[replacement_char1].name << endl;
                      continue;
                    }
                  int j = lookup_char(replacement_char2);
                  if(j != -1)
                    {
                      int replacement_char3 = alt_unicodedata[from_charset[chr]].replacement[2];
                      if(replacement_char3 == 0)
                        {
                          file << "\\x" << hex << i << " \\x" << hex << j << "\t; "
                            << unicodedata[replacement_char1].name << " + "
                            << unicodedata[replacement_char2].name << endl;
                          continue;
                        }
                      int k = lookup_char(replacement_char3);
                      if(k != -1)
                        {
                          file << "\\x" << hex << i << " \\x" << hex << j << " \\x" << hex << k << "\t; "
                            << unicodedata[replacement_char1].name << " + "
                            << unicodedata[replacement_char2].name << " + "
                            << unicodedata[replacement_char3].name << endl;
                          continue;
                        }
                    }
                }
            }
          // if not found exact match and unable to compose try to remove
          // macrons, circumflexes, etc.
          const char *letter;
          const char *name = unicodedata[from_charset[chr]].name;
          if((letter = strstr(name, "LETTER ")) != NULL)
            {
              letter += 7;
              const char *letterend = strchr(letter, ' ');
              char lettercopy[256];
              int letterlen = ((letterend == NULL) ? strlen(name) : (letterend - name)) + 1;
              if(letterlen > sizeof(lettercopy))
                letterlen = sizeof(lettercopy);
              strncpy(lettercopy, unicodedata[from_charset[chr]].name, letterlen);
              lettercopy[letterlen - 1] = '\0';
              i = lookup_char_by_name(lettercopy);
              if(i != -1)
                {
                  file << "\\0 \\x" << hex << i << "\t; " << lettercopy;
                  if(letterend)
                    file << " [" << (letterend + 1) << "]";
                  file << endl;
                  continue;
                }
            }
          file << "\\0 ?" << "\t; (" << unicodedata[from_charset[chr]].name << ")" << endl;
        }

      file << "END" << endl;
    }
  else
    {
      cerr << "error opening file " << fn << endl;
    }
}

int main(int ac, char *av[])
{
  if(ac != 3)
    {
      cout << "usage: " << av[0] << " from_charset to_charset" << endl;
      exit(-1);
    }
  load_charset(av[1], from_charset);
  load_charset(av[2], to_charset);
  load_unicodedata("UnicodeData.txt", unicodedata);
  load_unicodedata("AltUnicodeData.txt", alt_unicodedata);
  generate_table(av[1], av[2]);
  return 0;
}
