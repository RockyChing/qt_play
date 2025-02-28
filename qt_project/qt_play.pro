#-------------------------------------------------
#
# Project created by QtCreator 2021-01-12T15:26:03
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_play
TEMPLATE = app


SOURCES += main.cpp\
        qtmainwindow.cpp \
    usb/usb_util.cpp \
    edit/nl_del.cpp \
    config/appsettings.cpp \
    crypto/qaes.cpp \
    account/AccountJsonData.cpp \
    account/AccountView.cpp \
    pog/PoGTableInfo.cpp \
    pog/PoGTableView.cpp \
    picmerge/PicMergeView.cpp \
    picmerge/PicMerge.cpp \
    log/LogViewer.cpp \
    log/LogServer.cpp \
    log/LogClient.cpp \
    log/Log.cpp \
    http/HttpClient.cpp \
    http/HttpClientView.cpp \
    reminder/ReminderDlg.cpp \
    edit/filenewdlg.cpp \
    edit/filerenamedlg.cpp \
    utils/msgboxutil.cpp \
    edit/filesyncdlg.cpp \
    utils/fileutil.cpp \
    epub/Text2HtmlDlg.cpp \
    epub/htmlrefgendlg.cpp \
    epub/htmlidgendlg.cpp \
    knote/knotemgr.cpp \
    knote/knoteitem.cpp \
    epub/htmlimgrenamedlg.cpp \
    epub/pdfnewlineremove.cpp \
    epub/pdfimgcrop.cpp \
    tools/imagewbrevert.cpp \
    utils/toast.cpp \
    edit/filepickdlg.cpp \
    finance/returncal.cpp \
    finance/tradearithmeticsequence.cpp \
    finance/tradegrid.cpp

HEADERS  += qtmainwindow.h \
    usb/usb_util.h \
    edit/nl_del.h \
    config/appsettings.h \
    crypto/qaes.h \
    include/openssl/aes.h \
    include/openssl/asn1.h \
    include/openssl/asn1_mac.h \
    include/openssl/asn1err.h \
    include/openssl/asn1t.h \
    include/openssl/async.h \
    include/openssl/asyncerr.h \
    include/openssl/bio.h \
    include/openssl/bioerr.h \
    include/openssl/blowfish.h \
    include/openssl/bn.h \
    include/openssl/bnerr.h \
    include/openssl/buffer.h \
    include/openssl/buffererr.h \
    include/openssl/camellia.h \
    include/openssl/cast.h \
    include/openssl/cmac.h \
    include/openssl/cms.h \
    include/openssl/cmserr.h \
    include/openssl/comp.h \
    include/openssl/comperr.h \
    include/openssl/conf.h \
    include/openssl/conf_api.h \
    include/openssl/conferr.h \
    include/openssl/crypto.h \
    include/openssl/cryptoerr.h \
    include/openssl/ct.h \
    include/openssl/cterr.h \
    include/openssl/des.h \
    include/openssl/dh.h \
    include/openssl/dherr.h \
    include/openssl/dsa.h \
    include/openssl/dsaerr.h \
    include/openssl/dtls1.h \
    include/openssl/e_os2.h \
    include/openssl/ebcdic.h \
    include/openssl/ec.h \
    include/openssl/ecdh.h \
    include/openssl/ecdsa.h \
    include/openssl/ecerr.h \
    include/openssl/engine.h \
    include/openssl/engineerr.h \
    include/openssl/err.h \
    include/openssl/evp.h \
    include/openssl/evperr.h \
    include/openssl/hmac.h \
    include/openssl/idea.h \
    include/openssl/kdf.h \
    include/openssl/kdferr.h \
    include/openssl/lhash.h \
    include/openssl/md2.h \
    include/openssl/md4.h \
    include/openssl/md5.h \
    include/openssl/mdc2.h \
    include/openssl/modes.h \
    include/openssl/obj_mac.h \
    include/openssl/objects.h \
    include/openssl/objectserr.h \
    include/openssl/ocsp.h \
    include/openssl/ocsperr.h \
    include/openssl/opensslconf.h \
    include/openssl/opensslconf.h.in \
    include/openssl/opensslv.h \
    include/openssl/ossl_typ.h \
    include/openssl/pem.h \
    include/openssl/pem2.h \
    include/openssl/pemerr.h \
    include/openssl/pkcs7.h \
    include/openssl/pkcs7err.h \
    include/openssl/pkcs12.h \
    include/openssl/pkcs12err.h \
    include/openssl/rand.h \
    include/openssl/rand_drbg.h \
    include/openssl/randerr.h \
    include/openssl/rc2.h \
    include/openssl/rc4.h \
    include/openssl/rc5.h \
    include/openssl/ripemd.h \
    include/openssl/rsa.h \
    include/openssl/rsaerr.h \
    include/openssl/safestack.h \
    include/openssl/seed.h \
    include/openssl/sha.h \
    include/openssl/srp.h \
    include/openssl/srtp.h \
    include/openssl/ssl.h \
    include/openssl/ssl2.h \
    include/openssl/ssl3.h \
    include/openssl/sslerr.h \
    include/openssl/stack.h \
    include/openssl/store.h \
    include/openssl/storeerr.h \
    include/openssl/symhacks.h \
    include/openssl/tls1.h \
    include/openssl/ts.h \
    include/openssl/tserr.h \
    include/openssl/txt_db.h \
    include/openssl/ui.h \
    include/openssl/uierr.h \
    include/openssl/whrlpool.h \
    include/openssl/x509.h \
    include/openssl/x509_vfy.h \
    include/openssl/x509err.h \
    include/openssl/x509v3.h \
    include/openssl/x509v3err.h \
    account/AccountJsonData.h \
    account/AccountView.h \
    pog/PoGTableInfo.h \
    pog/PoGTableView.h \
    picmerge/PicMergeView.h \
    picmerge/PicInfo.h \
    picmerge/PicMerge.h \
    pog/PoGItem.h \
    log/LogViewer.h \
    log/LogServer.h \
    log/LogClient.h \
    log/Log.h \
    http/HttpClient.h \
    http/HttpClientView.h \
    reminder/ReminderDlg.h \
    edit/filenewdlg.h \
    edit/filerenamedlg.h \
    utils/msgboxutil.h \
    edit/filesyncdlg.h \
    utils/fileutil.h \
    epub/Text2HtmlDlg.h \
    epub/htmlrefgendlg.h \
    epub/htmlidgendlg.h \
    knote/knotemgr.h \
    knote/knoteitem.h \
    epub/htmlimgrenamedlg.h \
    epub/pdfnewlineremove.h \
    epub/pdfimgcrop.h \
    tools/imagewbrevert.h \
    utils/toast.h \
    edit/filepickdlg.h \
    finance/returncal.h \
    finance/tradearithmeticsequence.h \
    finance/tradegrid.h

FORMS    += qtmainwindow.ui \
    edit/nl_del.ui \
    account/accountview.ui \
    pog/pogtableview.ui \
    picmerge/picmergeview.ui \
    log/logviewer.ui \
    http/httpclientview.ui \
    reminder/reminderdlg.ui \
    edit/filenewdlg.ui \
    edit/filerenamedlg.ui \
    edit/filesyncdlg.ui \
    epub/text2htmldlg.ui \
    epub/htmlidgendlg.ui \
    epub/htmlrefgendlg.ui \
    knote/knotemgr.ui \
    epub/htmlimgrenamedlg.ui \
    epub/pdfnewlineremove.ui \
    epub/pdfimgcrop.ui \
    tools/imagewbrevert.ui \
    utils/toast.ui \
    edit/filepickdlg.ui \
    finance/returncal.ui \
    finance/tradearithmeticsequence.ui \
    finance/tradegrid.ui

win32: LIBS += -L$$PWD/libs/ -llibusb-1.0 \
    -llibcrypto-1_1 -llibssl-1_1

INCLUDEPATH += $$PWD/libs
INCLUDEPATH += $$PWD/include

DEPENDPATH += $$PWD/libs

RESOURCES += \
    resources.qrc
