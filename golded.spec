%define reldate 20250401
%define reltype C
# may be one of: C (current), R (release), S (stable)

%global relnum 3

# for generic build; it will be overriden for some distributions
%global vendor_prefix %nil
%global vendor_suffix %nil

# for CentOS, Fedora and RHEL
%if "%_vendor" == "redhat"
    %global vendor_suffix %dist
%endif

# for ALT Linux
%if "%_vendor" == "alt"
    %global vendor_prefix %_vendor
%endif

Name: golded+
Version: 1.1.5.%{reldate}%{reltype}
Release: %{vendor_prefix}%relnum%{vendor_suffix}
Group: Applications/FTN
Summary: GoldEd - the FTN editor
URL: https://github.com/golded-plus/
License: GPL
Source: %{name}.tar.gz
#Patch0: golded-vendor.diff
Provides: golded
Requires: ncurses >= 5.4
BuildRequires: make
BuildRequires: gcc-c++
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
%make_build DEBUG=1

%install
mkdir -p %{buildroot}%{_bindir} \
	%{buildroot}%{_prefix}/share/man/man1 \
	%{buildroot}%{_prefix}/share/golded/charset
install -m 755 bin/* %{buildroot}%{_bindir}
install -m 644 docs/*.1 %{buildroot}%{_prefix}/share/man/man1
install -m 644 cfgs/charset/*{chs,esc} %{buildroot}%{_prefix}/share/golded/charset

%files
%defattr(-,root,root)
%{_bindir}/*
%{_prefix}/share/*
