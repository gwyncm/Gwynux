#!/bin/sh

# Hacked by GCM to do a static install

tar zxf ../gwynux/basic/glibc*orig

# Copyright (C) 2002, 2005  Slackware Linux, Inc.
# Copyright 2005, 2006  Patrick J. Volkerding, Sebeka, Minnesota, USA
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# Swap glibc on the fly.
#
# If we're on a running system we have to handle this _very_ carefully.  :-)
# The tricks involved here get trickier every time...

# Install linuxthreads glibc libraries:
# no ldconfig?  Good, it's safe to just jam it on home (and make links below):
  ( cd lib/incoming
    for file in * ; do
      cp -a $file ..
    done
  )

# Now, get rid of the temporary directory:
rm -rf lib/incoming
# Done installing linuxthreads glibc libraries.

# Install NPTL glibc libraries:
# no ldconfig?  Good, it's safe to just jam it on home (and make links below):
  ( cd lib/tls/incoming
    for file in * ; do
      cp -a $file ..
    done
  )

# Now, get rid of the temporary directory:
rm -rf lib/tls/incoming
# Done installing NPTL glibc libraries.

# Handle config files:
config() {
  NEW="$1"
  OLD="$(dirname $NEW)/$(basename $NEW .new)"
  # If there's no config file by that name, mv it over:
  if [ ! -r $OLD ]; then
    mv $NEW $OLD
  elif [ "$(cat $OLD | md5sum)" = "$(cat $NEW | md5sum)" ]; then # toss the redundant copy
    rm $NEW
  fi
  # Otherwise, we leave the .new copy for the admin to consider...
}
config etc/profile.d/glibc.csh.new
config etc/profile.d/glibc.sh.new
# Clearly you already decided this issue.  :-)
rm -f etc/profile.d/glibc.csh.new
rm -f etc/profile.d/glibc.sh.new

# In case there's no ldconfig, make the links manually:
if [ true ]; then
# linuxthreads:
( cd lib ; rm -rf libnss_nis.so.2 )
( cd lib ; ln -sf libnss_nis-2.3.6.so libnss_nis.so.2 )
( cd lib ; rm -rf libm.so.6 )
( cd lib ; ln -sf libm-2.3.6.so libm.so.6 )
( cd lib ; rm -rf libnss_files.so.2 )
( cd lib ; ln -sf libnss_files-2.3.6.so libnss_files.so.2 )
( cd lib ; rm -rf libresolv.so.2 )
( cd lib ; ln -sf libresolv-2.3.6.so libresolv.so.2 )
( cd lib ; rm -rf libnsl.so.1 )
( cd lib ; ln -sf libnsl-2.3.6.so libnsl.so.1 )
( cd lib ; rm -rf libutil.so.1 )
( cd lib ; ln -sf libutil-2.3.6.so libutil.so.1 )
( cd lib ; rm -rf libnss_compat.so.2 )
( cd lib ; ln -sf libnss_compat-2.3.6.so libnss_compat.so.2 )
( cd lib ; rm -rf libthread_db.so.1 )
( cd lib ; ln -sf libthread_db-1.0.so libthread_db.so.1 )
( cd lib ; rm -rf libnss_hesiod.so.2 )
( cd lib ; ln -sf libnss_hesiod-2.3.6.so libnss_hesiod.so.2 )
( cd lib ; rm -rf libanl.so.1 )
( cd lib ; ln -sf libanl-2.3.6.so libanl.so.1 )
( cd lib ; rm -rf libcrypt.so.1 )
( cd lib ; ln -sf libcrypt-2.3.6.so libcrypt.so.1 )
( cd lib ; rm -rf libBrokenLocale.so.1 )
( cd lib ; ln -sf libBrokenLocale-2.3.6.so libBrokenLocale.so.1 )
( cd lib ; rm -rf ld-linux.so.2 )
( cd lib ; ln -sf ld-2.3.6.so ld-linux.so.2 )
( cd lib ; rm -rf libdl.so.2 )
( cd lib ; ln -sf libdl-2.3.6.so libdl.so.2 )
( cd lib ; rm -rf libnss_dns.so.2 )
( cd lib ; ln -sf libnss_dns-2.3.6.so libnss_dns.so.2 )
( cd lib ; rm -rf libpthread.so.0 )
( cd lib ; ln -sf libpthread-0.10.so libpthread.so.0 )
( cd lib ; rm -rf libnss_nisplus.so.2 )
( cd lib ; ln -sf libnss_nisplus-2.3.6.so libnss_nisplus.so.2 )
( cd lib ; rm -rf libc.so.6 )
( cd lib ; ln -sf libc-2.3.6.so libc.so.6 )
( cd lib ; rm -rf librt.so.1 )
( cd lib ; ln -sf librt-2.3.6.so librt.so.1 )
# NPTL:
( cd lib/tls ; rm -rf libnss_nis.so.2 )
( cd lib/tls ; ln -sf libnss_nis-2.3.6.so libnss_nis.so.2 )
( cd lib/tls ; rm -rf libm.so.6 )
( cd lib/tls ; ln -sf libm-2.3.6.so libm.so.6 )
( cd lib/tls ; rm -rf libnss_files.so.2 )
( cd lib/tls ; ln -sf libnss_files-2.3.6.so libnss_files.so.2 )
( cd lib/tls ; rm -rf libresolv.so.2 )
( cd lib/tls ; ln -sf libresolv-2.3.6.so libresolv.so.2 )
( cd lib/tls ; rm -rf libnsl.so.1 )
( cd lib/tls ; ln -sf libnsl-2.3.6.so libnsl.so.1 )
( cd lib/tls ; rm -rf libutil.so.1 )
( cd lib/tls ; ln -sf libutil-2.3.6.so libutil.so.1 )
( cd lib/tls ; rm -rf libnss_compat.so.2 )
( cd lib/tls ; ln -sf libnss_compat-2.3.6.so libnss_compat.so.2 )
( cd lib/tls ; rm -rf libthread_db.so.1 )
( cd lib/tls ; ln -sf libthread_db-1.0.so libthread_db.so.1 )
( cd lib/tls ; rm -rf libnss_hesiod.so.2 )
( cd lib/tls ; ln -sf libnss_hesiod-2.3.6.so libnss_hesiod.so.2 )
( cd lib/tls ; rm -rf libanl.so.1 )
( cd lib/tls ; ln -sf libanl-2.3.6.so libanl.so.1 )
( cd lib/tls ; rm -rf libcrypt.so.1 )
( cd lib/tls ; ln -sf libcrypt-2.3.6.so libcrypt.so.1 )
( cd lib/tls ; rm -rf libBrokenLocale.so.1 )
( cd lib/tls ; ln -sf libBrokenLocale-2.3.6.so libBrokenLocale.so.1 )
( cd lib/tls ; rm -rf ld-linux.so.2 )
( cd lib/tls ; ln -sf ld-2.3.6.so ld-linux.so.2 )
( cd lib/tls ; rm -rf libdl.so.2 )
( cd lib/tls ; ln -sf libdl-2.3.6.so libdl.so.2 )
( cd lib/tls ; rm -rf libnss_dns.so.2 )
( cd lib/tls ; ln -sf libnss_dns-2.3.6.so libnss_dns.so.2 )
( cd lib/tls ; rm -rf libpthread.so.0 )
( cd lib/tls ; ln -sf libpthread-2.3.6.so libpthread.so.0 )
( cd lib/tls ; rm -rf libnss_nisplus.so.2 )
( cd lib/tls ; ln -sf libnss_nisplus-2.3.6.so libnss_nisplus.so.2 )
( cd lib/tls ; rm -rf libc.so.6 )
( cd lib/tls ; ln -sf libc-2.3.6.so libc.so.6 )
( cd lib/tls ; rm -rf librt.so.1 )
( cd lib/tls ; ln -sf librt-2.3.6.so librt.so.1 )
fi
