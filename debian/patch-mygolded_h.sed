# SED commands to create the Debianized version of golded3/mygolded.h
s/^#define __GVER_VENDOR_NAME__.*$/#define __GVER_VENDOR_NAME__ "Debian Project"/
s/^#define __GVER_VENDOR_FIDO__.*$/#define __GVER_VENDOR_FIDO__ ""/
s/^#define __GVER_VENDOR_EMAIL__.*$/#define __GVER_VENDOR_EMAIL__ "submit@bugs.debian.org"/
s/^#define __GVER_RELEASENAME__.*$/#define __GVER_RELEASENAME__   __GVER_VENDOR_NAME__" Edition"/
