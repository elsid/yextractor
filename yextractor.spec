%define _builddir .
%define _sourcedir .
%define _specdir .
%define _rpmdir .

Name: yextractor
Version: %{yandex_mail_version}
Release: %{yandex_mail_release}
Summary: Library to extract named parameters with verification from key-value container
License: Yandex License
Group: System Environment/Libraries
Packager: Roman Siromakha <elsid@yandex-team.ru>
Distribution: Red Hat Enterprise Linux

Requires: boost >= 1.53.0

BuildRequires: boost-devel >= 1.53.0

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

%description
Extractor

%package devel
Summary: Development environment for %{name}
Group: System Environment/Libraries

%description devel
Extractor

%build
cmake . -DCMAKE_INSTALL_PREFIX=/usr \
    -DCMAKE_BUILD_TYPE=Release
%{__make} %{?_smp_mflags}

%install
%{__rm} -rf %{buildroot}
%{__make} install DESTDIR=%{buildroot}
%{__mkdir_p} %{buildroot}%{_initrddir}

%clean
%{__rm} -rf $RPM_BUILD_ROOT

%files devel
%defattr(-,root,root)
%dir %{_includedir}/yamail
%{_includedir}/yamail/*.hpp
%{_includedir}/yamail/yextractor/*.hpp
%{_includedir}/yamail/yextractor/detail/*.hpp

%changelog
