/*
 * JS8 frame unpacking from DecodedText of JS8Call 2.2.0, GPL-3.0-or-later.
 * MultiModeModem: reduced to the JS8 constructor and the tryUnpack* chain.
 */
#include "DecodedFrame.h"
#include "Varicode.h"

#include <QRegularExpression>

namespace js8 {

DecodedFrame::DecodedFrame(QString const &js8callmessage, int bits, int submode)
    : frameType_(Varicode::FrameUnknown),
      message_(js8callmessage),
      bits_(bits),
      submode_(submode),
      frame_(js8callmessage)
{
    static const QRegularExpression standard(QStringLiteral("^(CQ|DE|QRZ)\\s"));
    is_standard_ = standard.match(message_).hasMatch();
    tryUnpack();
}

bool DecodedFrame::tryUnpack(){
    if(is_standard_){
        message_ = message_.append(" ");
        return false;
    }

    bool unpacked = false;
    if(!unpacked){
        unpacked = tryUnpackFastData();
    }

     if(!unpacked){
        unpacked = tryUnpackData();
     }

    if(!unpacked){
        unpacked = tryUnpackHeartbeat();
    }

    if(!unpacked){
        unpacked = tryUnpackCompound();
    }

    if(!unpacked){
        unpacked = tryUnpackDirected();
    }

    return unpacked;
}

bool DecodedFrame::tryUnpackHeartbeat(){
    QString m = message().trimmed();

    // directed calls will always be 12+ chars and contain no spaces.
    if(m.length() < 12 || m.contains(' ')){
      return false;
    }

    if((bits_ & Varicode::JS8CallData) == Varicode::JS8CallData){
            return false;
    }

    bool isAlt = false;
    quint8 type = Varicode::FrameUnknown;
    quint8 bits3 = 0;
    QStringList parts = Varicode::unpackHeartbeatMessage(m, &type, &isAlt, &bits3);

    if(parts.isEmpty() || parts.length() < 2){
        return false;
    }

    // Heartbeat Alt Type
    // ---------------
    // 1      0   HB
    // 1      1   CQ
    isHeartbeat_ = true;
    isAlt_ = isAlt;
    extra_ = parts.length() < 3 ? "" : parts.at(2);

    QStringList cmp;
    if(!parts.at(0).isEmpty()){
        cmp.append(parts.at(0));
    }
    if(!parts.at(1).isEmpty()){
        cmp.append(parts.at(1));
    }
    compound_ = cmp.join("/");

    if(isAlt){
        auto sbits3 = Varicode::cqString(bits3);
        message_ = QString("%1: @ALLCALL %2 %3 ").arg(compound_).arg(sbits3).arg(extra_);
        frameType_ = type;
    } else {
        auto sbits3 = Varicode::hbString(bits3);
        if(sbits3 == "HB"){
            message_ = QString("%1: @HB HEARTBEAT %2 ").arg(compound_).arg(extra_);
            frameType_ = type;
        } else {
            message_ = QString("%1: @HB %2 %3 ").arg(compound_).arg(sbits3).arg(extra_);
            frameType_ = type;
        }
    }

    return true;
}

bool DecodedFrame::tryUnpackCompound(){
    auto m = message().trimmed();
    // directed calls will always be 12+ chars and contain no spaces.
    if(m.length() < 12 || m.contains(' ')){
        return false;
    }

    quint8 type = Varicode::FrameUnknown;
    quint8 bits3 = 0;
    auto parts = Varicode::unpackCompoundMessage(m, &type, &bits3);
    if(parts.isEmpty() || parts.length() < 2){
        return false;
    }

    if((bits_ & Varicode::JS8CallData) == Varicode::JS8CallData){
        return false;
    }

    QStringList cmp;
    if(!parts.at(0).isEmpty()){
        cmp.append(parts.at(0));
    }
    if(!parts.at(1).isEmpty()){
        cmp.append(parts.at(1));
    }
    compound_ = cmp.join("/");
    extra_ = parts.length() < 3 ? "" : parts.mid(2).join(" ");

    if(type == Varicode::FrameCompound){
        message_ = QString("%1: ").arg(compound_);
    } else if(type == Varicode::FrameCompoundDirected){
        message_ = QString("%1%2 ").arg(compound_).arg(extra_);
        directed_ = QStringList{ "<....>", compound_ } + parts.mid(2);
    }

    frameType_ = type;
    return true;
}

bool DecodedFrame::tryUnpackDirected(){
    QString m = message().trimmed();

    // directed calls will always be 12+ chars and contain no spaces.
    if(m.length() < 12 || m.contains(' ')){
      return false;
    }

    if((bits_ & Varicode::JS8CallData) == Varicode::JS8CallData){
        return false;
    }

    quint8 type = Varicode::FrameUnknown;
    QStringList parts = Varicode::unpackDirectedMessage(m, &type);

    if(parts.isEmpty()){
      return false;
    }

    if(parts.length() == 3){
      // replace it with the correct unpacked (directed)
      message_ = QString("%1: %2%3 ").arg(parts.at(0), parts.at(1), parts.at(2));
    } else if(parts.length() == 4){
      // replace it with the correct unpacked (directed numeric)
      message_ = QString("%1: %2%3 %4 ").arg(parts.at(0), parts.at(1), parts.at(2), parts.at(3));
    } else {
      // replace it with the correct unpacked (freetext)
      message_ = QString(parts.join(""));
    }

    directed_ = parts;
    frameType_ = type;
    return true;
}

bool DecodedFrame::tryUnpackData(){
    QString m = message().trimmed();

    // data frames calls will always be 12+ chars and contain no spaces.
    if(m.length() < 12 || m.contains(' ')){
        return false;
    }

    if((bits_ & Varicode::JS8CallData) == Varicode::JS8CallData){
        return false;
    }

    QString data = Varicode::unpackDataMessage(m);

    if(data.isEmpty()){
      return false;
    }

    message_ = data;
    frameType_ = Varicode::FrameData;
    return true;
}

bool DecodedFrame::tryUnpackFastData(){
    QString m = message().trimmed();

    // data frames calls will always be 12+ chars and contain no spaces.
    if(m.length() < 12 || m.contains(' ')){
        return false;
    }

    if((bits_ & Varicode::JS8CallData) != Varicode::JS8CallData){
        return false;
    }

    QString data = Varicode::unpackFastDataMessage(m);

    if(data.isEmpty()){
      return false;
    }

    message_ = data;
    frameType_ = Varicode::FrameData;
    return true;
}

} // namespace js8
