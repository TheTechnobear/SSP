#include "Matrix.h"

#include "Track.h"

Matrix::Matrix() {
}


static constexpr int checkMatrixBytes = 0x1FF4;
static constexpr int checkWireBytes = 0x1FF5;


void Matrix::getStateInformation(juce::MemoryOutputStream& outStream) {
    outStream.writeInt(checkMatrixBytes);
    outStream.writeInt(connections_.size());
    for (auto &w : connections_) {
        outStream.writeInt(checkWireBytes);
        outStream.writeInt(w.src_.modIdx_);
        outStream.writeInt(w.src_.chIdx_);
        outStream.writeInt(w.dest_.modIdx_);
        outStream.writeInt(w.dest_.chIdx_);
    }
}

void Matrix::setStateInformation(juce::MemoryInputStream& inStream) {
    int check = inStream.readInt();
    if (check != checkMatrixBytes) return;

    int size = inStream.readInt();
    for(int c=0;c<size;c++) {
        int check = inStream.readInt();
        if (check != checkWireBytes) { return; }
        int src = inStream.readInt();
        int srcCh = inStream.readInt();
        int dest = inStream.readInt();
        int destCh = inStream.readInt();
        connect(Jack(src,srcCh), Jack(dest,destCh));
    }
}
