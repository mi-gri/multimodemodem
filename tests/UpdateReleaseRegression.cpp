#include "updates/UpdateController.h"
#include "updates/Ed25519Verify.h"
#include "settings/AppSettings.h"
#include "M3Version.h"
#include "M3BuildInfo.h"
#include <QApplication>
#include <QWidget>
#include <QMenu>
#include <QTimer>
#include <QEventLoop>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QSettings>
#include <QJsonArray>
#include <iostream>
#include <stdexcept>
static void check(bool ok,const char *what){if(!ok)throw std::runtime_error(what);}
static QJsonObject release(QString tag,bool pre=false,bool draft=false,QString body={}){
    return {{"tag_name",tag},{"name","An arbitrary edited title"},{"prerelease",pre},{"draft",draft},{"body",body}};
}
class SimulatedUpdate : public UpdateController {
public:
    QJsonArray pages[2]; QStringList requested; QJsonObject chosen;
    std::function<void()> finished;
    SimulatedUpdate(QWidget *w,QMenu *menu,std::function<bool()> idle):UpdateController(w,menu,idle){}
private:
    void fetch(const QUrl &url,qint64,std::function<void(QByteArray)> done) override {
        requested.append(url.toString());
        const int page = QUrlQuery(url).queryItemValue("page").toInt();
        ::check(page==1 || page==2,"unexpected pagination request");
        const auto bytes=QJsonDocument(pages[page-1]).toJson();
        QTimer::singleShot(0,this,[done,bytes]{done(bytes);});
    }
    void offer(const QJsonObject &release) override {chosen=release;if(finished)finished();}
};
int main(int argc,char **argv){
    QApplication app(argc,argv);
    auto newer=[](const QJsonObject&r,QString v="0.6.4",quint64 b=114){return UpdateController::newerPublishedVersion(r,v,b);};
    check(newer(release("build-0.6.4-115",true))=="0.6.4","newer prerelease build ignored");
    check(newer(release("build-0.6.4-115",false))=="0.6.4","unmarked build ignored");
    check(newer(release("build-0.6.4-114",true)).isEmpty(),"same build offered again");
    check(newer(release("build-0.6.4-113",false)).isEmpty(),"older build offered");
    check(newer(release("build-0.6.3-999",true)).isEmpty(),"public-version downgrade offered");
    check(newer(release("v0.6.5",true))=="0.6.5","higher prerelease version ignored");
    check(newer(release("v0.6.5",false,true)).isEmpty(),"draft offered");
    check(newer(release("v0.6.4",false,false,"Operator notes\n<!-- MultiModeModem build: 116 -->"))=="0.6.4","version-tag rebuild ignored");
    check(newer(release("v0.6.4",false,false,"<!-- MultiModeModem build: 114 -->")).isEmpty(),"same stamped build repeated");
    check(newer(release("v0.6.4",false,false,"<!-- MadModem build: 116 -->"))=="0.6.4","release stamped before the rename still read");   // m3-compat
    check(newer(release("build-0.6.4-115",true),"0.6.4",0)=="0.6.4","unidentified local build cannot acquire official identity");
    check(newer(release("v0.6.4"),"0.6.3",0)=="0.6.4","old client version bridge missing");
    check(newer(release("v0.6.4")).isEmpty(),"unknown same-version tag assumed newer");
    check(newer(release("main")).isEmpty(),"unversioned branch interpreted as release");
    check(newer(release("build-0.6.4-9999999999999999999999999")).isEmpty(),"overflow build accepted");
    check(newer(release("build-0.6.4-115",false,false,"<!-- MultiModeModem build: 116 -->")).isEmpty(),"contradictory identity accepted");
    check(UpdateController::releaseBuildNumber(release("build-0.6.4-115",true))==115,"build not extracted");
    auto oldLatest=release("v0.6.3");oldLatest.insert("make_latest",true);
    auto newest=release("build-0.6.4-116",true);newest.insert("make_latest",false);
    auto best=UpdateController::newestPublishedRelease(QJsonArray{newest,release("v9.0.0",false,true),oldLatest,release("build-0.6.4-115",true)});
    check(best.value("tag_name")=="build-0.6.4-116","Latest badge or API order won over newer prerelease");
    auto promoted=release("v0.6.4",false,false,"<!-- MultiModeModem build: 117 -->");
    best=UpdateController::newestPublishedRelease(QJsonArray{newest,promoted});
    check(best.value("tag_name")=="v0.6.4","stamped version release not ranked by build");
    QJsonArray firstPage;
    for(int i=0;i<100;++i)firstPage.append(release("v0.6.3"));
    auto page1=UpdateController::newestPublishedRelease(firstPage);
    auto page2=UpdateController::newestPublishedRelease(QJsonArray{newest});
    best=UpdateController::newestPublishedRelease(QJsonArray{page1,page2});
    check(best.value("tag_name")=="build-0.6.4-116","paged candidate merge lost the newest build");
    check(UpdateController::linuxPlatform("Ubuntu24.04","","ID=fedora\nVERSION_ID=43") == "Ubuntu24.04","bundle marker must win over host");
    check(UpdateController::linuxPlatform("","Build platform: Fedora43\n","ID=ubuntu\nVERSION_ID=24.04") == "Fedora43","legacy BUILD_INFO ignored");
    check(UpdateController::linuxPlatform("","","ID=linuxmint\nVERSION_ID=22\nUBUNTU_CODENAME=noble") == "Ubuntu24.04","Ubuntu derivative rejected");
    check(UpdateController::linuxPlatform("","","ID=ubuntu\nVERSION_ID=\"24.04\"") == "Ubuntu24.04","quoted Ubuntu version rejected");
    check(UpdateController::linuxPlatform("","","ID=fedora\nVERSION_ID='43'") == "Fedora43","quoted Fedora version rejected");
    check(UpdateController::linuxPlatform("","","ID=arch").isEmpty(),"unknown ABI guessed");
    for (const auto& mint : {QString("22"),QString("22.1"),QString("22.2"),QString("22.3")})
        check(UpdateController::linuxPlatform("","","ID=linuxmint\nVERSION_ID=\""+mint+"\"\nVERSION_CODENAME=zena\nID_LIKE=\"ubuntu debian\"\n")=="Ubuntu24.04","Mint 22 base without UBUNTU_CODENAME rejected");
    check(UpdateController::linuxPlatform("","","ID=pop\nID_LIKE=\"ubuntu debian\"\nVERSION_CODENAME=noble\r\n")=="Ubuntu24.04","Noble derivative rejected");
    for (const auto& host : {QString("ID=linuxmint\nVERSION_ID=21.3\nUBUNTU_CODENAME=jammy"),
                            QString("ID=linuxmint\nVERSION_ID=7\nID_LIKE=debian\nVERSION_CODENAME=gigi"),
                            QString("ID=linuxmint\nVERSION_ID=23\nID_LIKE=\"ubuntu debian\""),
                            QString("ID=debian\nVERSION_ID=13"),QString("ID=arch\nVERSION_CODENAME=noble")})
        check(UpdateController::linuxPlatform("","",host).isEmpty(),"incompatible or unknown host mapped to Ubuntu");
    check(UpdateController::linuxPlatform("Ubuntu24.04","","ID=linuxmint\nVERSION_ID=23")=="Ubuntu24.04","installed ABI identity lost to host version");
    const auto mintPlatform=UpdateController::linuxPlatform("","","ID=linuxmint\nVERSION_ID=22.3");
    const auto runNames=UpdateController::linuxAssetNames("0.6.4",true,mintPlatform,130);
    QJsonObject mintAssets{{"assets",QJsonArray{
        QJsonObject{{"name","MultiModeModem-0.6.4-Linux-Fedora43-64bit-Installer.run"}},
        QJsonObject{{"name","MultiModeModem-0.6.4-Linux-Ubuntu24.04-64bit-Package.deb"}},
        QJsonObject{{"name","MultiModeModem-0.6.4-130-Ubuntu24.04-amd64-Setup.run"}},
        QJsonObject{{"name","MultiModeModem-0.6.4-Linux-Ubuntu24.04-64bit-Installer.run"}}}}};
    check(UpdateController::selectAsset(mintAssets,runNames).value("name")==runNames.first(),"Mint did not prefer canonical Ubuntu RUN");
    auto mintEntries=mintAssets.value("assets").toArray();mintEntries.removeLast();mintAssets.insert("assets",mintEntries);
    check(UpdateController::selectAsset(mintAssets,runNames).value("name")==runNames.last(),"legacy same-build Ubuntu RUN ignored");
    check(UpdateController::selectAsset(mintAssets,UpdateController::linuxAssetNames("0.6.4",true,mintPlatform,131)).isEmpty(),"another build or native package selected as RUN");
    check(UpdateController::selectAsset(mintAssets,UpdateController::linuxAssetNames("0.6.4",false,mintPlatform,130)).isEmpty(),"portable changed into managed installation");
    check(UpdateController::linuxAssetNames("0.6.4",true,"Debian13",130).isEmpty(),"unsupported package ABI guessed");
    check(UpdateController::linuxAssetNames("0.6.4",true,"Fedora43",130).last()=="multimodemodem-0.6.4-130.fc43.x86_64-Setup.run","legacy Fedora RUN identity incorrect");
    std::cout<<"PASS Mint 22.x without Ubuntu codename, compatible Noble derivative, incompatible bases rejected, canonical/legacy RUN and channel separation\n";
    QJsonObject assets{{"assets",QJsonArray{QJsonObject{{"name","test.zip"}},QJsonObject{{"name","test.tar.gz"}},QJsonObject{{"name","wrong-platform.run"}}}}};
    check(UpdateController::selectAsset(assets,{"test.tar.gz","test.zip"}).value("name")=="test.tar.gz","asset preference follows API order");
    check(UpdateController::selectAsset(assets,{"absent.tar.gz","test.zip"}).value("name")=="test.zip","ZIP fallback ignored");
    check(UpdateController::selectAsset(assets,{"managed.run"}).isEmpty(),"portable selected for managed install");
    std::cout<<"PASS Linux bundle/legacy/host identity, Ubuntu derivatives, unknown ABI rejection, TAR preference and ZIP fallback\n";
    {   // Ed25519 against RFC 8032 section 7.1 tests 1 and 2.
        const auto h=[](const char *s){return QByteArray::fromHex(s);};
        const QByteArray pk1=h("d75a980182b10ab7d54bfed3c964073a0ee172f3daa62325af021a68f707511a");
        const QByteArray sig1=h("e5564300c360ac729086e2cc806e828a84877f1eb8e5d974d873e065224901555fb8821590a33bacc61e39701cf9b46bd25bf5f0595bbe24655141438e7a100b");
        const QByteArray pk2=h("3d4017c3e843895a92b70aa74d1b7ebc9c982ccf2ec4968cc0cd55f12af4660c");
        const QByteArray sig2=h("92a009a9f0d4cab8720e820b5f642540a2b27b5416503f8fb3762223ebdb69da085ac1e43e15996e458f3613d0f11d8c387b2eaeb4302aeeb00d291612bb0c00");
        check(M3Ed25519::verify(sig1,{},pk1) && M3Ed25519::verify(sig2,h("72"),pk2),"RFC 8032 signature rejected");
        check(!M3Ed25519::verify(sig2,h("73"),pk2) && !M3Ed25519::verify(sig2,h("72"),pk1),"wrong message or key accepted");
        // S + L verifies in plain TweetNaCl; RFC 8032 requires S < L.
        static const unsigned char order[32]={0xed,0xd3,0xf5,0x5c,0x1a,0x63,0x12,0x58,0xd6,0x9c,0xf7,0xa2,0xde,0xf9,0xde,0x14,
                                              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x10};
        QByteArray malleable=sig1;int carry=0;
        for(int i=0;i<32;++i){const int v=(unsigned char)malleable[32+i]+order[i]+carry;malleable[32+i]=char(v&255);carry=v>>8;}
        check(!M3Ed25519::verify(malleable,{},pk1),"non-canonical S accepted");
        check(!M3Ed25519::verify(sig1.left(63),{},pk1) && !M3Ed25519::verify(sig1,{},pk1.left(31)),"short signature or key accepted");
    }
    {   // Signed checksum list: fixture key, never a release key.
        const QByteArray key=QByteArray::fromHex("7b684f23c73b75b2783663b75b9e3958d02123acdb217e84f84d901f7da28001");
        const QByteArray sums=QByteArray(64,'a')+"  MultiModeModem-9.9.9-Windows-64bit-Installer-AutoCPU.exe\n"+
                              QByteArray(64,'b')+"  MultiModeModem-9.9.9-macOS-Intel-Portable.zip\n";
        const QByteArray signature=QByteArray::fromHex("8be3c6d410d2df6012b8905e039cf33a983a026ecad6899130074f36fbcae1fe9fdf56790ed24ce82b90cdc406936d6421bd5de60bac4e950f08c2eec5d3a805");
        QString reason;
        check(UpdateController::verifiedChecksum(sums,signature,"MultiModeModem-9.9.9-macOS-Intel-Portable.zip",{key},&reason)==QByteArray(64,'b'),"signed checksum not returned");
        check(UpdateController::verifiedChecksum(sums,signature,"absent.zip",{key},&reason).isEmpty() && !reason.isEmpty(),"missing package accepted");
        QByteArray forged=sums;forged[0]='c';
        check(UpdateController::verifiedChecksum(forged,signature,"MultiModeModem-9.9.9-Windows-64bit-Installer-AutoCPU.exe",{key}).isEmpty(),"altered checksum list accepted");
        check(UpdateController::verifiedChecksum(sums,signature,"MultiModeModem-9.9.9-macOS-Intel-Portable.zip",UpdateController::signingKeys()).isEmpty(),"fixture signature accepted by the release keys");
        check(UpdateController::verifiedChecksum(sums,{},"MultiModeModem-9.9.9-macOS-Intel-Portable.zip",{key}).isEmpty(),"unsigned checksum list accepted");
        check(!UpdateController::signingKeys().isEmpty(),"no release signing key compiled in");
        for(const auto &k:UpdateController::signingKeys())check(k.size()==32,"malformed release signing key");
    }
    std::cout<<"PASS Ed25519 RFC 8032 vectors, canonical S, signed checksum list accepted, forged/unsigned/foreign-key lists refused\n";
    QSettings settings(AppSettings::settingsFilePath(),QSettings::IniFormat);
    settings.setValue("Updates/checkAtStartup",false);settings.sync();
    QWidget window;QMenu menu;bool idle=false;
    SimulatedUpdate updater(&window,&menu,[&]{return idle;});
    updater.pages[0]=firstPage;
    const auto number=QStringLiteral(M3_BUILD_NUMBER).toULongLong()+1;
    const QString newTag="build-"+QStringLiteral(M3_VERSION_STRING)+"-"+QString::number(number);
    updater.pages[1]=QJsonArray{release(newTag,true)};
    updater.check();check(updater.requested.isEmpty(),"automatic check did not defer active operation");
    idle=true;QEventLoop loop;updater.finished=[&]{loop.quit();};
    QTimer::singleShot(3000,&loop,&QEventLoop::quit);updater.check(true);loop.exec();
    check(updater.requested.size()==2,"release pages were not retrieved");
    check(updater.chosen.value("tag_name")==newTag,"check did not offer the next-page prerelease");
    check(updater.requested[0].contains("/releases?") && !updater.requested[0].contains("/latest"),"checker still uses Latest endpoint");
    std::cout<<"PASS real checker paginates simulated responses, defers busy operation, and reaches the offer for a newer prerelease\n";
    std::cout<<"PASS published stable/prerelease selection, Latest independence, version/build ordering, repeat/downgrade/draft/invalid rejection, local bridge and paged merge\n";
}
