#-------------------------------------------------
#
# Project created by QtCreator 2021-01-12T15:26:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_play
TEMPLATE = app


SOURCES += main.cpp\
        qtmainwindow.cpp \
    usb/usb_util.cpp \
    log/log.cpp \
    finance/finance.cpp \
    nl_del/nl_del.cpp \
    config/appsettings.cpp \
    edit/text2htmldlg.cpp \
    account/AccountJsonData.cpp \
    account/Accounts.cpp \
    crypto/qaes.cpp

HEADERS  += qtmainwindow.h \
    usb/usb_util.h \
    log/log.h \
    finance/finance.h \
    nl_del/nl_del.h \
    config/appsettings.h \
    edit/text2htmldlg.h \
    account/AccountJsonData.h \
    account/Accounts.h \
    crypto/qaes.h \
    util/crypto/include/openssl/aes.h \
    util/crypto/include/openssl/asn1.h \
    util/crypto/include/openssl/asn1_mac.h \
    util/crypto/include/openssl/asn1err.h \
    util/crypto/include/openssl/asn1t.h \
    util/crypto/include/openssl/async.h \
    util/crypto/include/openssl/asyncerr.h \
    util/crypto/include/openssl/bio.h \
    util/crypto/include/openssl/bioerr.h \
    util/crypto/include/openssl/blowfish.h \
    util/crypto/include/openssl/bn.h \
    util/crypto/include/openssl/bnerr.h \
    util/crypto/include/openssl/buffer.h \
    util/crypto/include/openssl/buffererr.h \
    util/crypto/include/openssl/camellia.h \
    util/crypto/include/openssl/cast.h \
    util/crypto/include/openssl/cmac.h \
    util/crypto/include/openssl/cms.h \
    util/crypto/include/openssl/cmserr.h \
    util/crypto/include/openssl/comp.h \
    util/crypto/include/openssl/comperr.h \
    util/crypto/include/openssl/conf.h \
    util/crypto/include/openssl/conf_api.h \
    util/crypto/include/openssl/conferr.h \
    util/crypto/include/openssl/crypto.h \
    util/crypto/include/openssl/cryptoerr.h \
    util/crypto/include/openssl/ct.h \
    util/crypto/include/openssl/cterr.h \
    util/crypto/include/openssl/des.h \
    util/crypto/include/openssl/dh.h \
    util/crypto/include/openssl/dherr.h \
    util/crypto/include/openssl/dsa.h \
    util/crypto/include/openssl/dsaerr.h \
    util/crypto/include/openssl/dtls1.h \
    util/crypto/include/openssl/e_os2.h \
    util/crypto/include/openssl/ebcdic.h \
    util/crypto/include/openssl/ec.h \
    util/crypto/include/openssl/ecdh.h \
    util/crypto/include/openssl/ecdsa.h \
    util/crypto/include/openssl/ecerr.h \
    util/crypto/include/openssl/engine.h \
    util/crypto/include/openssl/engineerr.h \
    util/crypto/include/openssl/err.h \
    util/crypto/include/openssl/evp.h \
    util/crypto/include/openssl/evperr.h \
    util/crypto/include/openssl/hmac.h \
    util/crypto/include/openssl/idea.h \
    util/crypto/include/openssl/kdf.h \
    util/crypto/include/openssl/kdferr.h \
    util/crypto/include/openssl/lhash.h \
    util/crypto/include/openssl/md2.h \
    util/crypto/include/openssl/md4.h \
    util/crypto/include/openssl/md5.h \
    util/crypto/include/openssl/mdc2.h \
    util/crypto/include/openssl/modes.h \
    util/crypto/include/openssl/obj_mac.h \
    util/crypto/include/openssl/objects.h \
    util/crypto/include/openssl/objectserr.h \
    util/crypto/include/openssl/ocsp.h \
    util/crypto/include/openssl/ocsperr.h \
    util/crypto/include/openssl/opensslconf.h \
    util/crypto/include/openssl/opensslconf.h.in \
    util/crypto/include/openssl/opensslv.h \
    util/crypto/include/openssl/ossl_typ.h \
    util/crypto/include/openssl/pem.h \
    util/crypto/include/openssl/pem2.h \
    util/crypto/include/openssl/pemerr.h \
    util/crypto/include/openssl/pkcs7.h \
    util/crypto/include/openssl/pkcs7err.h \
    util/crypto/include/openssl/pkcs12.h \
    util/crypto/include/openssl/pkcs12err.h \
    util/crypto/include/openssl/rand.h \
    util/crypto/include/openssl/rand_drbg.h \
    util/crypto/include/openssl/randerr.h \
    util/crypto/include/openssl/rc2.h \
    util/crypto/include/openssl/rc4.h \
    util/crypto/include/openssl/rc5.h \
    util/crypto/include/openssl/ripemd.h \
    util/crypto/include/openssl/rsa.h \
    util/crypto/include/openssl/rsaerr.h \
    util/crypto/include/openssl/safestack.h \
    util/crypto/include/openssl/seed.h \
    util/crypto/include/openssl/sha.h \
    util/crypto/include/openssl/srp.h \
    util/crypto/include/openssl/srtp.h \
    util/crypto/include/openssl/ssl.h \
    util/crypto/include/openssl/ssl2.h \
    util/crypto/include/openssl/ssl3.h \
    util/crypto/include/openssl/sslerr.h \
    util/crypto/include/openssl/stack.h \
    util/crypto/include/openssl/store.h \
    util/crypto/include/openssl/storeerr.h \
    util/crypto/include/openssl/symhacks.h \
    util/crypto/include/openssl/tls1.h \
    util/crypto/include/openssl/ts.h \
    util/crypto/include/openssl/tserr.h \
    util/crypto/include/openssl/txt_db.h \
    util/crypto/include/openssl/ui.h \
    util/crypto/include/openssl/uierr.h \
    util/crypto/include/openssl/whrlpool.h \
    util/crypto/include/openssl/x509.h \
    util/crypto/include/openssl/x509_vfy.h \
    util/crypto/include/openssl/x509err.h \
    util/crypto/include/openssl/x509v3.h \
    util/crypto/include/openssl/x509v3err.h

FORMS    += qtmainwindow.ui \
    finance/finance.ui \
    nl_del/nl_del.ui \
    edit/text2htmldlg.ui \
    account/accounts.ui

win32: LIBS += -L$$PWD/libs/ -llibusb-1.0 \
    -llibcrypto-1_1 -llibssl-1_1

INCLUDEPATH += $$PWD/libs
INCLUDEPATH += $$PWD/util/crypto/include

DEPENDPATH += $$PWD/libs

RESOURCES += \
    resources.qrc
