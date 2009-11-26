#! /bin/sh
#
# $Id$
# Shell script to creating terminfo file for Golded+ in the openbsd console
# Author: Dmitri Zubko 2:465/312
#
set -e
INFODIR=$HOME/.terminfo
CAPFILE=$HOME/.termcap
/bin/cat << THERE

  Create new terminfo/termcap entries called
    openbsd and openbsd-m  (and vt220)
  by making a new terminfo file and terminfo entries in
  my $INFODIR directory
  and by creating my own termcap file as $CAPFILE
-----------------------------------------------------
  To use the new terminfo/termcap entries,
  put something like this in your .profile

     export TERM=openbsd

  Or if you prefer monochrome:

     export TERM=openbsd-m

THERE
/bin/mkdir -p $INFODIR
cd $INFODIR
/bin/cat /dev/null      > terminfo
/usr/bin/infocmp vt220 >> terminfo
/bin/cat << HERE       >> terminfo
# This is a newer version of vt220 for OpenBSD that
# fixes the Function keys and the Home and End keys.
openbsd-m|OpenBSD mono from vt220,
  kend=\E[8~, kf1=\E[11~, kf10=\E[21~, kf11=\E22~,
  kf12=\E23~, kf2=\E[12~, kf3=\E[13~, kf4=\E[14~, kf5=\E[15~,
  kf6=\E[17~, kf7=\E[18~, kf8=\E[19~, kf9=\E[20~,
  khome=\E[7~, use=vt220,

# A copy of wsvt25 for OpenBSD in color with
# Function keys and Home/End keys fixed
openbsd|OpenBSD color 25 line vt220 mode,
  bce, msgr,
  colors#8, cols#80, lines#25, ncv#3, pairs#64,
  kend=\E[8~, kf1=\E[11~, kf10=\E[21~, kf11=\E22~,
  kf12=\E23~, kf2=\E[12~, kf3=\E[13~, kf4=\E[14~, kf5=\E[15~,
  kf6=\E[17~, kf7=\E[18~, kf8=\E[19~, kf9=\E[20~,
  khome=\E[7~, op=\E[m, setab=\E[4%p1%dm, setaf=\E[3%p1%dm,
  use=vt220,
HERE
/usr/bin/tic -o$INFODIR -s terminfo
/usr/bin/infotocap         terminfo > $CAPFILE
/bin/ls -aldF $HOME/.term*
exit 
