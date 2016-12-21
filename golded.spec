%define reldate 20161221
%define reltype C
# may be one of: C (current), R (release), S (stable)

Name: golded+
Version: 1.1.5.%{reldate}%{reltype}
Release: 1
Group: Applications/FTN
Summary: GoldEd - the FTN editor
URL: http://golded-plus.sf.net
License: GPL
Source: %{name}.tar.gz
#Patch0: golded-vendor.diff
Provides: golded
Requires: ncurses >= 5.4
BuildRequires: ncurses-devel >= 5.4
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%description
GoldEd is The FTN Editor.

%prep
%setup -q -n %{name}
#%patch0 -p1

cat << __MYGOLDED_H_EOF__ > golded3/mygolded.h
#undef __GVER_VENDOR_NAME__
#define __GVER_VENDOR_NAME__ "GoldEd+ team"
#undef __GVER_VENDOR_FIDO__
#define __GVER_VENDOR_FIDO__ ""
#undef __GVER_VENDOR_EMAIL__
#define __GVER_VENDOR_EMAIL__ ""

#undef __GVER_RELEASENAME__
#define __GVER_RELEASENAME__   "Public build for Linux by "__GVER_VENDOR_NAME__

#undef __GVER_PRENAME__
#define __GVER_PRENAME__       ""
#undef __GVER_POSTNAME__
#define __GVER_POSTNAME__      "+"
#undef __GVER_PRESHORTNAME__
#define __GVER_PRESHORTNAME__  ""
#undef __GVER_POSTSHORTNAME__
#define __GVER_POSTSHORTNAME__ "+"

#undef __GVER_PREVERSION__
#define __GVER_PREVERSION__    ""
#undef __GVER_POSTVERSION__
#define __GVER_POSTVERSION__   ""
__GVER__(1,1,5)
__MYGOLDED_H_EOF__

%build
make
make strip

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}%{_bindir} \
	%{buildroot}%{_prefix}/share/man/man1 \
	%{buildroot}%{_prefix}/share/golded/charset
#rm -rf bin/CVS
install -m 755 bin/* %{buildroot}%{_bindir}
install -m 644 docs/*.1 %{buildroot}%{_prefix}/share/man/man1
install -m 644 cfgs/charset/*{chs,esc} %{buildroot}%{_prefix}/share/golded/charset

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%{_bindir}/*
%{_prefix}/share/*
