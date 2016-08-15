#!/bin/sh
set -ex

CONFIGURE_FLAGS="--enable-static --enable-pic --disable-cli"

ARCHS="arm64 x86_64 i386 armv7 armv7s"
#ARCHS="i386"

# directories
SOURCE="curl-7.48.0"
FAT="libcurl-ios"

SCRATCH="scratch-libcurl"
# must be an absolute path
THIN=`pwd`/"thin-libcul"
OPTIONS=" \
	--enable-static \
	--disable-shared \
	--disable-debug \
	--disable-ftp \
	--disable-file \
	--disable-ldap \
	--disable-ldaps \
	--disable-rtsp \
	--disable-proxy \
	--disable-dict \
	--disable-telnet \
	--disable-tftp \
	--disable-imap \
	--disable-smtp \
	--disable-gopher \
	--disable-manual \
	--disable-ntlm-wb \
	--disable-tls-srp \
	--without-librtmp \
    	--enable-ipv6 \
	--with-darwinssl \
	"

GAS_PREPROCESSOR=/usr/local/bin/gas-preprocessor.pl

LIBCURL_URL="https://curl.haxx.se/download/curl-7.48.0.zip"
if [ ! -r $SOURCE ]
  then
    echo 'libcurl source not found. Trying to download...'
    curl ${LIBCURL_URL} | tar xj \
      || exit 1
fi

COMPILE="y"
LIPO="y"

if [ "$*" ]
then
	if [ "$*" = "lipo" ]
	then
		# skip compile
		COMPILE=
	else
		ARCHS="$*"
		if [ $# -eq 1 ]
		then
			# skip lipo
			LIPO=
		fi
	fi
fi

if [ "$COMPILE" ]
then
	CWD=`pwd`
	for ARCH in $ARCHS
	do
		echo "building $ARCH..."
		mkdir -p "$SCRATCH/$ARCH"
		cd "$SCRATCH/$ARCH"
		CFLAGS="-arch $ARCH"
                ASFLAGS=

		if [ "$ARCH" = "i386" -o "$ARCH" = "x86_64" ]
		then
		    PLATFORM="iPhoneSimulator"
		    CPU=
		    if [ "$ARCH" = "x86_64" ]
		    then
		    	CFLAGS="$CFLAGS -mios-simulator-version-min=7.0"
		    	HOST=
		    else
		    	CFLAGS="$CFLAGS -mios-simulator-version-min=5.0"
			HOST="--host=i386-apple-darwin"
		    fi
		else
		    PLATFORM="iPhoneOS"
		    if [ $ARCH = "arm64" ]
		    then
		        HOST="--host=aarch64-apple-darwin"
			XARCH="-arch aarch64"
		    else
		        HOST="--host=arm-apple-darwin"
			XARCH="-arch arm"
		    fi
                    CFLAGS="$CFLAGS -fembed-bitcode -mios-version-min=7.0"
                    ASFLAGS="$CFLAGS"
		fi

		XCRUN_SDK=`echo $PLATFORM | tr '[:upper:]' '[:lower:]'`
		CC="xcrun -sdk $XCRUN_SDK clang"
		if [ $PLATFORM = "iPhoneOS" ]
		then
		    export AS="gas-preprocessor.pl $XARCH -- $CC"
		else
		    export -n AS
		fi
		CXXFLAGS="$CFLAGS"
		LDFLAGS="$CFLAGS"

		# CFLAGS ios cflags ldflags cppflags
		export CFLAGS="${CFLAGS}"
		export CPPFLAGS="${CFLAGS}"
		export LDFLAGS="${LDFLAGS}"
		CC=$CC $CWD/$SOURCE/configure \
		    $CONFIGURE_FLAGS \
		    $HOST \
		    $OPTIONS \
		    --prefix="$THIN/$ARCH" || exit 1

		if [ ! -r extras ]
		then
			mkdir extras
		fi
		
		make install || exit 1
		cd $CWD
	done
fi

if [ "$LIPO" ]
then
	echo "building fat binaries..."
	mkdir -p $FAT/lib
	set - $ARCHS
	CWD=`pwd`
	cd $THIN/$1/lib
	for LIB in *.a
	do
		cd $CWD
		lipo -create `find $THIN -name $LIB` -output $FAT/lib/$LIB
	done

	cd $CWD
	cp -rf $THIN/$1/include $FAT
fi


