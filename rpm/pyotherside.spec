Summary: Asynchronous Python 3 Bindings for Qt 5
Name: pyotherside-qml-plugin-python3-qt5
Version: 1.5.9
Release: 1
Source: %{name}-%{version}.tar.gz
URL: http://thp.io/2011/pyotherside/
License: ISC
BuildRequires: python3-devel
BuildRequires: qt5-qmake
BuildRequires: pkgconfig(Qt5Gui)
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: pkgconfig(Qt5Quick)
BuildRequires: pkgconfig(Qt5Test)
BuildRequires: pkgconfig(Qt5Svg)
Requires: python3-base
Requires: python3-sqlite

%description
A QML Plugin that provides access to a Python 3 interpreter from QML.

%prep
%setup -q

%build
%qmake5
make %{?_smp_mflags}

%install
make INSTALL_ROOT=%{buildroot} install

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%defattr(-,root,root)
%license LICENSE
%doc README
%{_libdir}/qt5/qml/io/thp/pyotherside/*
